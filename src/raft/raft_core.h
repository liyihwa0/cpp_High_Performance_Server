
#include "../locker/locker.h"
#include "../smart_ptr.h"
#include "./raft_core_proto.pb.h"
#include "src/util/timer.h"
#include "../fiber/channel.h"
#include "../net/rpc/rpc.h"
namespace wa{
namespace raft{
    const Int HeartBeatTimeout=25;  //25毫秒
    const Int ElectionTimeLowerBound=300;
    const Int ElectionTimeUpperBound=500;
    const Int ApplyInterval=10;      //10毫秒

    enum VoteState{
        Killed=0,
        Voted = 1,   //本轮已经投过票了
        Expire = 2,  //投票（消息、竞选者）过期
        Normal = 3
    };
    // 当节点认同日志后,就会给server发送ApplyMessage
    struct ApplyMessage{
        Int logIndex;
        String command;
    };

    // raft中用于远程调用其他节点服务的类
    class RaftRpcServiceCaller {
    private:
        rpc::RpcMethodCaller<AppendEntriesArgs,AppendEntriesReply> appendEntriesCaller_;
        rpc::RpcMethodCaller<RequestVoteArgs,RequestVoteReply> requestVoteCaller_;
    public:
        rpc::RpcCode AppendEntries(AppendEntriesArgs *args,AppendEntriesReply *response){
            return appendEntriesCaller_.call(args,response);
        }
        rpc::RpcCode RequestVote(RequestVoteArgs *args,RequestVoteReply *response){
            return requestVoteCaller_.call(args,response);
        }
    };


    class Raft : public rpc::RpcService{
    private:
        SpinLock lock_;
        std::vector<SP<RaftRpcServiceCaller>> peers_;

        // 在上层调用了 addLog后,会在channels中创建一个对应管道
        struct LogChannelNode{
            Channel<Bool> ch;
            Int term;
            LogChannelNode(Int term): ch(1),term(term){
            }
        };
        std::unordered_map<Int,SP<LogChannelNode>> logChannels_;
        ID selfId_;
        Int term_;
        ID votedTo_;
        std::vector<LogEntry> logs_;  //// 日志条目数组，包含了状态机要执行的指令集，以及收到领导时的任期号

        // 这两个状态所有结点都在维护，易失
        Int commitIndex_;   // 记录已经提交的index
        Int lastAppliedIndex_;  // 已经汇报给状态机（上层应用）的log 的index
        SP<Channel<UP<ApplyMessage>>> applyChannel_;    // 应用的管道,当日志被提交后,下层raft会通过此管道向上层状态机发送提交的日志

        std::vector<Int> nextIndex_;    // 记录了peer下一条日志的index,只有作为leader时,这个值才有意义
        std::vector<Int> matchedIndex_; // 记录了peer已经确认匹配了的日志的index,只有作为leader时,这个值才有意义
        enum Status { Follower, Candidate, Leader };
        Status status_;


        // 选举超时
        Timer lastResetElectionTime_;

        // 心跳超时，用于leader
        Timer lastHeartBeatTime_;




        static Timer randomElectionTime();


        void electionTask();
        void applierTask();
        void leaderHearBeatTask();


        void doElection();
        void doHeartBeat();
        std::vector<UP<ApplyMessage>>  getApplyLogs();
        void getPrevLogInfo(Int nodeId, Int &preIndex, Int &preTerm);

        void persist();
        Bool upToDate(Int index, Int term);
        Int getLastLogIndex();
        Int getLogTermFromLogIndex(Int index);
        void getLastLogIndexAndTerm(Int *lastLogIndex, Int *lastLogTerm);

        void callRequestVote(Int nodeId, UP<RequestVoteArgs> args, SP<Int> votedNum);
        void callAppendEntries(Int rpcNodeId, UP<AppendEntriesArgs> args, SP<Int> appendNums);


        void readPersist(std::string data);
        std::string persistData();

    public:
        // 从文件中序列化
        static UP<Raft> makeFromFile(const String& path){
            //todo
            return UP<Raft>();
        }
        Raft(std::vector<SP<RaftRpcServiceCaller>> peers,
             Int selfId,
             SP<Channel<UP<ApplyMessage>>> applyChannel){
            peers_=std::move(peers);
            selfId_=selfId;
            applyChannel_=std::move(applyChannel);

            {
                auto sl = AsScopedLock(lock_);
                term_ = 0;
                status_ = Follower;
                commitIndex_ = 0;
                lastAppliedIndex_=0;
                logs_.clear();

                matchedIndex_.reserve(peers_.size());
                nextIndex_.reserve(peers_.size());
                for (int i = 0; i < peers_.size(); i++) {
                    matchedIndex_.push_back(0);
                    nextIndex_.push_back(0);
                }
                votedTo_ = -1;    // 表示没有给任何人投票
                lastResetElectionTime_ = Timer::Now();
                lastHeartBeatTime_ = Timer::Now();

                //todo
                readPersist("todo");
            }
            // 启动三个定时任务
            //1.作为leader的定时心跳任务
            Fiber::AddTask([this]{
                this->leaderHearBeatTask();
            });

            //2.作为follower的选举任务,如果长期没收到leader的心跳后,就会触发该任务
            Fiber::AddTask([this]{
                this->electionTask();
            });

            //3.作为leader时需要将已经提交但没有应用的任务应用到上层
            Fiber::AddTask([this]{
                this->applierTask();
            });
        }

        Raft();


        Bool addLog(String command);

        void appendEntriesRpcMethod(AppendEntriesArgs *args, AppendEntriesReply *reply);
        void requestVoteRpcMethod(RequestVoteArgs *args, RequestVoteReply *reply);
    };
}

}
