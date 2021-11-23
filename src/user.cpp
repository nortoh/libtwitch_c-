#include <string>
#include <iostream>
#include "../includes/user.h"

using namespace std;

User::User(string username, Channel channel){
    this->username = username;
    this->chan = channel;
}

void User::set_name(string name) {
    this->username = name;
}

void User::set_channel(Channel channel) {
    this->chan = channel;
}

Channel User::channel() {
    return this->chan;
}

string User::name() {
    return this->username;
}