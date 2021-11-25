#include <string>
#include "../includes/channel.h"
#include "../includes/user.h"
#include "../includes/message.h"

Message::Message(Channel channel, User user, Tag tag, string message) {
    this->_channel = channel;
    this->_user = user;
    this->_tag = tag;
    this->_message = message;
}

Channel Message::channel() {
    return this->_channel;
}

User Message::user() {
    return this->_user;
}

Tag Message::tag() {
    return this->_tag;
}

string Message::message() {
    return this->_message;
}