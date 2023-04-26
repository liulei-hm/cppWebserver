//
// Created by liulei on 23-4-25.
//

#ifndef MYSERVER_USER_H
#define MYSERVER_USER_H
#include <string>
#include <utility>
class User{
private:
    int age;
    std::string name;
public:
    //尽量使用列表初始化构造函数
    User(std::string name_,int age_):name(std::move(name_)),age(age_){}; //这里使用move性能更好
    std::string get_name();
};
#endif //MYSERVER_USER_H
