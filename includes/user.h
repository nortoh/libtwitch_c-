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
        void set_name(string);
        void set_channel(Channel);
    private:
        string username;
        Channel chan;
};

#endif