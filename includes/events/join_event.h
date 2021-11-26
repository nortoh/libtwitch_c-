#include <string>
#include "event.h"
#include "../user.h"
#include "../channel.h"

using namespace std;

#ifndef __JOIN_EVENT_H__
#define __JOIN_EVENT_H__
class JoinEvent : public Event {
    public:
        JoinEvent(Channel, User);
        void print() override;
        Channel channel();
        User user();
    private:
        Channel _channel;
        User _user;
};
#endif