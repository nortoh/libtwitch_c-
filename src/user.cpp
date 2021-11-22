#include <string>
#include <iostream>
#include "../includes/user.h"

using namespace std;

User::User(string username, Channel channel){
    this->username = username;
    this->channel = channel;
}

void User::set_name(string name) {
    this->username = name;
}

void User::set_channel(Channel channel) {
    this->channel = channel;
}

Channel User::get_channel() {
    return this->channel;
}

string User::get_name() {
    return this->username;
}