//
// Created by liulei on 23-4-26.
//

#ifndef MYSERVER_TIMER_H
#define MYSERVER_TIMER_H

#include <iostream>
#include <sys/epoll.h>
#include <chrono>
#include <functional>
#include <set>
#include <memory>
#include <numeric>
//命令行编译cd build  cmake ..

using namespace std;
using namespace std::chrono;

struct TimerNodeBase{
    time_t expire; //到达时间
    int64_t tid;   //用来唯一标识定时器
};
//c++14 find可以使用set中数据结构中的变量作为key
/*
 * 1、回调方法，定时器时间到达后执行
 * */
struct TimerNode : public TimerNodeBase{
    using Callback = std::function<void(const TimerNode &node)>;
    Callback func;
    time_t msec; //定时时间，毫秒
    bool isCircle = false;
};

bool operator < (const TimerNodeBase &nl,const TimerNodeBase &nr);

class Timer{
public:
    //获取当前时间的接口
    static time_t GetTick();
    static int64_t GenID();
    //添加定时器 函数模板实现接受任意回调函数
    TimerNodeBase AddTimer(time_t msec,bool isCircle,const TimerNode::Callback& func);
    //删除定时器 根据tid
    bool DelTimer(TimerNodeBase &node);
    //检查定时器
    bool CheckTimer();
    //找到当前最小的节点还要多久触发
    time_t TimeToSleep();
private:
    static int64_t gid;
    std::set<TimerNode,less<>> timermap;
};



#endif //MYSERVER_TIMER_H
