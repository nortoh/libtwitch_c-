#include <iostream>
#include <string>
#include "../../includes/events/event.h"
#include "../../includes/events/privmsg_event.h"
#include "../../includes/channel.h"
#include "../../includes/tag.h"
#include "../../includes/user.h"
#include "../../includes/message.h"

using namespace std;

PrivmsgEvent::PrivmsgEvent(Channel channel, User user, Tag tag, Message message) : Event(EventType::PRIVMSG) {
    this->_channel = channel;
    this->_user = user;
    this->_tag = tag;
    this->_message = message;
}

Channel PrivmsgEvent::channel() {
    return this->_channel;
}

User PrivmsgEvent::user() {
    return this->_user;
}

Tag PrivmsgEvent::tag() {
    return this->_tag;
}

Message PrivmsgEvent::message() {
    return this->_message;
}

void PrivmsgEvent::print() {
    cout << "[" << this->channel().name() << "/" << this->user().name() << "]: " << this->message().message() << "\n";
}