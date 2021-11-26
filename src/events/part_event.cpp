#include <iostream>
#include <string>
#include "../../includes/events/part_event.h"
#include "../../includes/channel.h"
#include "../../includes/user.h"

using namespace std;

PartEvent::PartEvent(Channel channel, User user) : Event(EventType::PART) {
    this->_channel = channel;
    this->_user = user;
}

Channel PartEvent::channel() {
    return this->_channel;
}

User PartEvent::user() {
    return this->_user;
}