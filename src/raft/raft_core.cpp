#include <utility>
#include <random>

#include "./raft_core.h"
#include "src/logger/logger.h"

namespace wa{
    namespace raft {
        // appendEntriesRpcMethod rpc
        void Raft::appendEntriesRpcMethod(AppendEntriesArgs *args, AppendEntriesReply *reply) {
            // raft作为follower时,需要对外提供appendEntries rpc
            auto sl= AsScopedLock(lock_);
            if (args->term() < term_) {
                reply->set_success(FALSE);
                reply->set_term(term_);
                //todo -100
                reply->set_updatenextindex(-100);  // 论文中：让Leader可以及时更新自己
                LOG_DEBUG(gl)<<selfId_<<" refuse "<<args->leaderid()<<" appendEntriesRpcMethod, cause of term";
                return;  // 注意从过期的领导人收到消息不要重设定时器
            }

            if (args->term() > term_) {
                status_ = Follower;
                term_ = args->term();
                votedTo_ = -1;  // 这里设置成-1有意义，如果突然宕机然后上线理论上是可以投票的
            }

            status_ = Follower;  // 这里是有必要的，因为如果candidate收到同一个term的leader的AE，需要变成follower
            lastResetElectionTime_=Timer::Now();

            //  不能无脑的从prevlogIndex开始阶段日志，因为rpc可能会延迟，导致发过来的log是很久之前的
            //  那么就比较日志,日志有3种情况
            if (args->prevlogindex() > getLastLogIndex()) {
                reply->set_success(FALSE);
                reply->set_term(term_);
                reply->set_updatenextindex(getLastLogIndex() + 1);
                return;
            }else if(args->prevlogterm()== getLogTermFromLogIndex(args->prevlogindex())){

                for (int i = 0; i < args->entries_size(); i++) {
                    auto& log = args->entries(i);
                    if (log.logindex() > getLastLogIndex()) {
                        //超过就直接添加日志
                        logs_.push_back(log);
                    } else if (logs_[log.logindex()].logterm() != log.logterm()){
                        //不匹配就更新
                        logs_[log.logindex()] = log;
                    }
                }


                if (args->leadercommit() > commitIndex_) {
                    commitIndex_ = args->leadercommit();
                }

                reply->set_success(TRUE);
                reply->set_term(term_);
                reply->set_updatenextindex(getLastLogIndex()+1);
                return;
            } else {

                // 沿着leader发来的 log,找到第一个不匹配的
                Bool match=TRUE;
                for (int i = 0; i < args->entries_size(); i++) {
                    auto& log = args->entries(i);
                    if (log.logindex() > getLastLogIndex()) {
                        logs_.push_back(log); //超过就直接添加日志
                        continue;
                    }

                    if (logs_[log.logindex()].logterm() != log.logterm()){
                        //不匹配就从此处开始更新
                        match=FALSE;
                    }

                    if(!match){
                        logs_[log.logindex()] = log;
                    }
                }

                // 无论match与否,都需要以args中的最后一条日志的下一个日志作为下次发来的日志的index
                reply->set_updatenextindex(args->prevlogindex()+args->entries_size());
                reply->set_success(TRUE);
                reply->set_term(term_);
                return;
            }

        }
        // requestVoteRpcMethod rpc
        void Raft::requestVoteRpcMethod(RequestVoteArgs *args, RequestVoteReply *reply) {
            auto sl= AsScopedLock(lock_);

            if (args->term() < term_) {
                reply->set_term(term_);
                reply->set_votestate(VoteState::Expire);
                reply->set_votegranted(FALSE);
                return;
            }
            // fig2:右下角，如果任何时候rpc请求或者响应的term大于自己的term，更新term，并变成follower
            if (args->term() > term_) {
                //        DPrintf("[	    func-RequestVote-rf(%v)		] : 变成follower且更新term
                //        因为candidate{%v}的term{%v}> rf{%v}.term{%v}\n ", rf.me, args.CandidateId, args.Term, rf.me,
                //        rf.currentTerm)
                status_ = Follower;
                term_ = args->term();
                votedTo_ = -1;

                //	重置定时器：收到leader的ae，开始选举，透出票
                //这时候更新了term之后，votedFor也要置为-1
            }

            //只有没投票，且candidate的日志的新的程度 ≥ 接受者的日志新的程度 才会授票
            if (!upToDate(args->lastlogindex(), args->lastlogterm())) {
                reply->set_term(term_);
                reply->set_votestate(VoteState::Expire);
                reply->set_votegranted(FALSE);
                return;
            }

            // 投过票,并且没有投给该候选人
            if (votedTo_ != -1 && votedTo_ != args->candidateid()) {
                reply->set_term(term_);
                reply->set_votestate(VoteState::Voted);
                reply->set_votegranted(FALSE);
                return;
            } else {
                votedTo_ = args->candidateid();
                lastResetElectionTime_ =Timer::Now();  //认为必须要在投出票的时候才重置定时器，
                reply->set_term(term_);
                reply->set_votestate(VoteState::Normal);
                reply->set_votegranted(TRUE);

                return;
            }
        }

        // 用于将已经认同的日志应用给上层复制状态机
        void Raft::applierTask() {
            while (true) {
                lock_.lock();
                auto applyMsgs = getApplyLogs();
                lock_.unlock();

                // 将所有已提交,但未应用的msg发送给上层
                for (auto& message : applyMsgs) {
                    applyChannel_->write(message);
                    lock_.lock();
                    auto fd=logChannels_.find(message->logIndex);
                    if(fd!=logChannels_.end()){
                        fd->second->ch.write(TRUE); //告知上层,已经应用了,这个channel不会阻塞住
                        fd->second->ch.close();
                        logChannels_.erase(fd);
                    }
                    lock_.unlock();
                }

                Fiber::AwaitTime(Timer(ApplyInterval));
            }
        }


        void Raft::doElection() {
            auto sl= AsScopedLock(lock_);

            if(status_==Leader) return;
            LOG_DEBUG(gl)<<"follower "<<selfId_<<" do election...";

            //当选举的时候定时器超时就必须重新选举，不然没有选票就会一直卡主
            //重竞选超时，term也会增加的
            status_=Candidate;
            ///开始新一轮的选举
            term_ += 1;
            votedTo_=selfId_; //即是自己给自己投，也避免candidate给同辈的candidate投
            persist();
            SP<Int> votedNum(new Int(1));
            lastResetElectionTime_=Timer::Now();//	重新设置定时器


            Int lastLogIndex = -1, lastLogTerm = -1;
            getLastLogIndexAndTerm(&lastLogIndex, &lastLogTerm);  //获取最后一个log的term和下标
            //	 调用其他节点的RequestVote RPC
            for (int i = 0; i < peers_.size(); i++) {
                if (i == selfId_) {
                    continue;
                }

                Fiber::AddTask([this,lastLogIndex,lastLogTerm,i,votedNum]{
                    UP<RequestVoteArgs> requestVoteArgs(new RequestVoteArgs);
                    requestVoteArgs->set_term(term_);
                    requestVoteArgs->set_candidateid(selfId_);
                    requestVoteArgs->set_lastlogindex(lastLogIndex);
                    requestVoteArgs->set_lastlogterm(lastLogTerm);
                    this->callRequestVote(i, requestVoteArgs, votedNum);
                });
            }

        }

        void Raft::doHeartBeat() {
            if(status_!=Leader) return;

            LOG_DEBUG(gl)<<"leader "<<selfId_<<" heart beat...";
            auto sl= AsScopedLock(lock_);

            auto appendNums = SP<Int>(new Int(1));  //正确返回的节点的数量

            //对Follower发送心跳
            for (int i = 0; i < peers_.size(); i++) {
                if (i == selfId_) {
                    continue;
                }

                Fiber::AddTask([this,i, appendNums]{
                    //构造发送值
                    Int preLogIndex = -1;
                    Int prevLogTerm = -1;
                    getPrevLogInfo(i, preLogIndex, prevLogTerm);
                    UP<AppendEntriesArgs> appendEntriesArgs = UP<AppendEntriesArgs>(new AppendEntriesArgs);
                    appendEntriesArgs->set_term(term_);
                    appendEntriesArgs->set_leaderid(selfId_);
                    appendEntriesArgs->set_prevlogindex(preLogIndex);
                    appendEntriesArgs->set_prevlogterm(prevLogTerm);
                    appendEntriesArgs->clear_entries();
                    appendEntriesArgs->set_leadercommit(commitIndex_);

                    for(Int j=preLogIndex+1;j<logs_.size();++j){
                        LogEntry* sendEntryPtr = appendEntriesArgs->add_entries();
                        *sendEntryPtr = logs_[j];  //=是可以点进去的，可以点进去看下protobuf如何重写这个的
                    }


                    callAppendEntries(i, appendEntriesArgs, appendNums);
                });
            }
            lastHeartBeatTime_ = Timer::Now();

        }

        void Raft::electionTask() {
            while (true) {

                while (status_ == Leader) {
                    Fiber::AwaitTime(Timer(HeartBeatTimeout));
                }

                // 当前是Leader,需要向follower发送
                Timer wakeupTime,suitableSleepTime;
                {
                    auto sl= AsScopedLock(lock_);
                    wakeupTime = Timer::Now();
                    suitableSleepTime =  randomElectionTime() + lastResetElectionTime_ - wakeupTime;
                }


                if(suitableSleepTime.getMS()>1){
                    Fiber::AwaitTime(Timer(suitableSleepTime));
                    auto now=Timer::Now();
                    LOG_DEBUG(gl)<<selfId_<<" wakeup after "<< (now-wakeupTime).getMS()<<" ms sleep";
                }


                doElection();
            }
        }


        Timer Raft::randomElectionTime() {
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<Int> dist(ElectionTimeLowerBound, ElectionTimeUpperBound);
            return Timer(dist(rng));
        }

        std::vector<UP<ApplyMessage>>  Raft::getApplyLogs() {
            std::vector<UP<ApplyMessage>> messages;

            while(lastAppliedIndex_<commitIndex_){
                UP<ApplyMessage> applyMessage(new ApplyMessage());
                applyMessage->command=logs_[lastAppliedIndex_].command();
                applyMessage->logIndex=lastAppliedIndex_;
                lastAppliedIndex_+=1;
                messages.push_back(std::move(applyMessage));
            }
            return messages;
        }

        void Raft::getPrevLogInfo(Int nodeId, Int &preIndex, Int &preTerm) {

            auto nextIndex = nextIndex_[nodeId]; // 下一个该发送给node的日志的index
            preIndex = nextIndex - 1;
            preTerm = logs_[preIndex].logterm();
        }

        void Raft::leaderHearBeatTask() {
            while (true) {
                while (status_ != Leader) {
                    Fiber::AwaitTime(Timer(HeartBeatTimeout));
                }

                // 当前是Leader,需要向follower发送
                Timer wakeupTime,suitableSleepTime;
                {
                    auto sl= AsScopedLock(lock_);
                    wakeupTime = Timer::Now();
                    suitableSleepTime =  Timer(HeartBeatTimeout) + lastHeartBeatTime_ - wakeupTime; //睡醒时间恰好是下一次发送心跳的时间
                }

                if(suitableSleepTime.getMS()>1){

                    Fiber::AwaitTime(Timer(suitableSleepTime));
                    auto now=Timer::Now();
                    LOG_DEBUG(gl)<<selfId_<<" wakeup after "<< (now-wakeupTime).getMS()<<" ms sleep";
                }


                doHeartBeat();
            }
        }

        //todo
        void Raft::persist() {
            LOG_ERROR(gl)<<"todo";
        }


        Bool Raft::upToDate(Int index, Int term) {
            Int lastIndex = -1;
            Int lastTerm = -1;
            getLastLogIndexAndTerm(&lastIndex, &lastTerm);
            return term > lastTerm || (term == lastTerm && index >= lastIndex);
        }

        Int Raft::getLastLogIndex() {
            Int lastLogIndex = -1;
            Int _ = -1;
            getLastLogIndexAndTerm(&lastLogIndex, &_);
            return lastLogIndex;
        }

        void Raft::getLastLogIndexAndTerm(Int *lastLogIndex, Int *lastLogTerm) {
            *lastLogIndex = logs_.back().logindex();
            *lastLogTerm = logs_.back().logterm();

        }




        // 向指定server发送reqeust vote rpc
        void Raft::callRequestVote(Int nodeId, UP<RequestVoteArgs> args, SP<Int> votedNum) {
            RequestVoteReply reply;
            LOG_DEBUG(gl) << selfId_ << " request " << nodeId << " for vote";
            rpc::RpcCode code = peers_[nodeId]->RequestVote(args.get(), &reply);
            if(!rpc::rpcSuccess(code)){
                return ;   // 因为某种原因发送失败了
            }

            auto sl= AsScopedLock(lock_);
            if (reply.term() > term_) {
                status_ = Follower;  //三变：身份，term，和投票
                term_ = reply.term();
                votedTo_ = -1;
                persist();
                return ;
            } else if (reply.term() < term_) { //无需理会过去的信息
                return ;
            }

            if (!reply.votegranted()) {
                return ;
            }

            *votedNum = *votedNum + 1;
            if (*votedNum >= peers_.size() / 2 + 1) {
                //变成leader
                *votedNum = 0;
                if (status_ == Leader) {
                    LOG_ERROR(gl)<<selfId_<<" 同一个term当两次领导";
                    return ;
                }
                //	第一次变成leader，初始化状态和nextIndex、matchIndex
                status_ = Leader;

                Int lastLogIndex = getLastLogIndex();
                for (int i = 0; i < nextIndex_.size(); i++) {
                    nextIndex_[i] = lastLogIndex + 1;  //有效下标从1开始，因此要+1
                    matchedIndex_[i] = 0;                //每换一个领导都是从0开始，见fig2
                }

                //  立马执行一次doHeartBeat
                Fiber::AddTask([this]{
                   this->doHeartBeat();
                });


                persist();
            }
            return ;
        }

        void Raft::callAppendEntries(Int rpcNodeId, UP<AppendEntriesArgs> args, SP<Int> appendNums) {
            AppendEntriesReply reply;

            auto code = peers_[rpcNodeId]->AppendEntries(args.get(), &reply);

            if (!rpc::rpcSuccess(code)) {
                LOG_WARN(gl) << "callAppendEntries from " << selfId_ << " to " << rpcNodeId << " failed";
                return ;
            }

            LOG_WARN(gl) << "callAppendEntries from " << selfId_ << " to " << rpcNodeId << " success";
            auto sl= AsScopedLock(lock_);

            //对reply进行处理
            if (reply.term() > term_) {
                status_ = Follower;
                term_ = reply.term();
                votedTo_ = -1;
                return ;
            } else if (reply.term() < term_) {
                return; //过期的消息,不必理会
            }

            //如果不是leader，那么就不要对返回的情况进行处理了
            if (status_ != Leader) {
                return;
            }
            // term相等
            if (!reply.success()) {
                LOG_INFO(gl) << selfId_ << " 与 " << rpcNodeId << " 日志不匹配,需要回退nextIndex";
                nextIndex_[rpcNodeId] = reply.updatenextindex();
            } else {
                *appendNums = *appendNums + 1;
                nextIndex_[rpcNodeId]=reply.updatenextindex();

                //commit
                if (*appendNums >= 1 + peers_.size() / 2) {
                    *appendNums = 0;
                    if (args->entries_size() > 0 && args->entries(args->entries_size() - 1).logterm() == term_) {
                        commitIndex_ = std::max(commitIndex_,(Int) args->prevlogindex() + args->entries_size());
                    }
                }
            }
        }

        void Raft::readPersist(std::string data) {
            LOG_ERROR(gl)<<"todo";
        }

        std::string Raft::persistData() {
            LOG_ERROR(gl)<<"todo";
            return std::string();
        }

        // 专门用于执行上层任务的方法
        // 上层在有新业务时,需要让下层的raft集群认同, 因此有了addLog方法
        Bool Raft::addLog(String content) {
            lock_.lock();
            if (status_ != Leader) {
                lock_.unlock();
                return FALSE;
            }

            LogEntry newLogEntry;
            newLogEntry.set_command(content);
            newLogEntry.set_logterm(term_);
            Int idx=getLastLogIndex()+1;
            newLogEntry.set_logindex(idx);
            logs_.emplace_back(newLogEntry);


            auto fd=logChannels_.find(idx);
            if(fd!=logChannels_.end()){
                if(fd->second->term<term_){
                    fd->second->ch.close();
                    logChannels_.erase(fd);
                }else{
                    // fd->second->term>=term_
                    return FALSE;
                }
            }
            // 创建一个channel并读取
            SP<LogChannelNode> node(new LogChannelNode(term_));
            logChannels_.insert(std::make_pair(idx, node));

            Bool result=FALSE;
            lock_.unlock();
            if(!node->ch.read(result)){ // channel被关闭了,没有读成功
                return FALSE;
            }
            if(!result){
                return FALSE;
            }

            lock_.lock();
            persist();
            lock_.unlock();
            return TRUE;
        }





        Int Raft::getLogTermFromLogIndex(Int index) {
            return logs_[index].logterm();
        }



    }
}