#include <string>
#include "event.h"
#include "../user.h"
#include "../channel.h"

using namespace std;

#ifndef __PART_EVENT_H__
#define __PART_EVENT_H__
class PartEvent : public Event {
    public:
        PartEvent(Channel, User);
        void print() override;
        Channel channel();
        User user();
    private:
        Channel _channel;
        User _user;
};
#endif