#include <string>
#include "channel.h"

using namespace std;

#ifndef __USER_H__
#define __USER_H__
class User {
    public:
        User(string, Channel);
        string get_name();
        Channel get_channel();
        void set_name(string name);
        void set_channel(Channel channel);
    private:
        string username;
        Channel channel;
};
#endif