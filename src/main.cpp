//
// Created by liulei on 23-4-25.
//
#include <iostream>
#include <sys/epoll.h>
#include "User.h"
#include "timer.h"
#include "hightimer.h"
//命令行编译cd build  cmake ..


int main(){
    int epfd = epoll_create(1);
    epoll_event ev[64] = {0};
    unique_ptr<Timer> timer = make_unique<Timer>(); //c++14
    int x=0;
    /* mutable两种用法
     * 1、若想在const成员函数中对一个私有成员变量进行更改，那么将函数后面添加mutable
     * 2、当使用lambda表达式时，可以使用引用捕获或值捕获，值捕获时若想改变传入的值，就可以声明为mutable，但是不改变其本体
     * 注意：如果lambda表达式以传值的方式，传入x那么在表达式体内就不能对x进行更改。如果以传址的方式进行传入，那么x的数值会改变
     * */
    timer->AddTimer(1000,true,[=](const TimerNode &node) mutable {
        x = node.tid + 100;
        cout << "timer:" << node.tid <<"," << node.msec << " ms timeout,x= " << x << endl;
    });
    timer->AddTimer(2000, false,[=](const TimerNode &node){
        cout << "timer:" << node.tid <<"," << node.msec << " ms timeout,x= " << x << endl;
    });
    timer->AddTimer(2100, false,[&](const TimerNode &node) {
        x++;
        cout << "timer:" << node.tid <<"," << node.msec << " ms timeout,x= " << x << endl;
    });
    timer->AddTimer(2400,false, [&](const TimerNode &node) {
        cout << "timer:" << node.tid <<"," << node.msec << " ms timeout,x= " << x << endl;
    });
    auto node5 = timer->AddTimer(2400, false,[&](const TimerNode &node) {
        cout << "timer:" << node.tid <<"," << node.msec << " ms timeout,x= " << x << endl;
    });
    timer->DelTimer(node5);

    while(true){
        /*
         * -1 永久阻塞
         * 0 没有事件不等待，有事件拷贝到ev数组
         * >0 阻塞等待 t ms
         * timeout 最近触发的定时任务离当前的时间
         * */
        int n = epoll_wait(epfd,ev,64,timer->TimeToSleep());
        for(int i=0;i<n;i++)
        {
            /**/
            cout << "n:" << n << endl;
        }
        /* 处理定时事件 */
        while(timer->CheckTimer());
    }

}