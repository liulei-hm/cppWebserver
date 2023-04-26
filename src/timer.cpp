//
// Created by liulei on 23-4-26.
//
#include "timer.h"
bool operator < (const TimerNodeBase &nl,const TimerNodeBase &nr){
    if(nl.expire < nr.expire) //按超时时间排序
        return true;
    else if(nl.expire > nr.expire)
        return false;
    else if(nl.expire == nr.expire)
    {
        return nl.tid<nr.tid; //超时时间相同，此时id更小的排在前面
    }
    return false;
}
int64_t Timer::gid = 0;

time_t Timer::GetTick() {
    auto  sc = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
    auto temp = chrono::duration_cast<milliseconds>(sc.time_since_epoch());
    return temp.count();
}

int64_t Timer::GenID() {
    return ++gid;
}

TimerNodeBase Timer::AddTimer(time_t msec, bool isCircle, const TimerNode::Callback &func) {
    TimerNode tnode;
    tnode.msec = msec;
    tnode.expire = GetTick() + msec;
    tnode.func = func;
    tnode.isCircle = isCircle;
    tnode.tid = GenID();
    // 加入set中
    timermap.insert(tnode);
    return static_cast<TimerNodeBase>(tnode);
}

bool Timer::DelTimer(TimerNodeBase &node) {
    auto iter = timermap.find(node);
    if(iter!=timermap.end())
    {
        timermap.erase(iter);
        return true;
    }
    return false;
}

bool Timer::CheckTimer() {
    // 先找到最小的节点
    auto iter = timermap.begin();
    if(iter != timermap.end() && iter->expire <= GetTick())
    {
        iter->func(*iter);
        if(iter->isCircle)
        {
            AddTimer(iter->msec,iter->isCircle,iter->func);
        }
        timermap.erase(iter);

        return true;
    }
    return false;
}

time_t Timer::TimeToSleep() {
    auto iter = timermap.begin();
    if(iter == timermap.end())
    {
        return -1;
    }
    time_t dis = iter->expire - GetTick();
    return dis>0 ? dis:0;
}