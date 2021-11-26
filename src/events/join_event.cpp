#include <iostream>
#include <string>
#include "../../includes/events/join_event.h"
#include "../../includes/channel.h"
#include "../../includes/user.h"

using namespace std;

JoinEvent::JoinEvent(Channel channel, User user) : Event(EventType::JOIN) {
    this->_channel = channel;
    this->_user = user;
}

Channel JoinEvent::channel() {
    return this->_channel;
}

User JoinEvent::user() {
    return this->_user;
}