#ifndef WASIGNAL_H
#define WASIGNAL_H
#include"global_def.h"
#include<signal.h>
#include<cstring>


void addsig(int sig,SignalHandler sighandler){
    SignalAction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler= sighandler;
    sigfillset(&sa.sa_mask);
    sigaction(sig,&sa,nullptr);
}

#endif 
