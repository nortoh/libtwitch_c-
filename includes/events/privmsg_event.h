#include <string>
#include "event.h"
#include "../message.h"
#include "../tag.h"
#include "../user.h"

using namespace std;

#ifndef __PRIVMSG_EVENT_H__
#define __PRIVMSG_EVENT_H__
class PrivmsgEvent : public Event {
    public:
        PrivmsgEvent(Channel, User, Tag, Message);
        void print() override;
        Channel channel();
        User user();
        Tag tag();
        Message message();
    private:
        Channel _channel;
        User _user;
        Tag _tag;
        Message _message;
};
#endif