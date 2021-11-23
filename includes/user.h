#include <string>
#include "channel.h"

using namespace std;

#ifndef __USER_H__
#define __USER_H__



class User {
    public:
        User() {};
        User(string, Channel);
        string name();
        Channel channel();
        void set_name(string name);
        void set_channel(Channel channel);
    private:
        string username;
        Channel chan;
};

#endif