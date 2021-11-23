#include <string>
#include "channel.h"
#include "user.h"
#include "tag.h"

using namespace std;

#ifndef __MESSAGE_H__
#define __MESSAGE_H__
class Message {
    public:
        Message(Channel, User, Tag, string);
        void print();
        Channel channel();
        User user();
        Tag tag();
        string message();
    private:
        Channel _channel;
        User _user;
        Tag _tag;
        string _message;
};

#endif