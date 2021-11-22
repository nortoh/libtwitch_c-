#include <string>
#include <iostream>
#include "../includes/channel.h"

using namespace std;

Channel::Channel(string name) {
    this->channel_name = name;
}

void Channel::set_name(string name) {
    this->channel_name = name;
}

string Channel::name() {
    return this->channel_name;
}