#include <string>
#include <iostream>
#include "../includes/channel.h"

using namespace std;

void Channel::set_name(string name) {
    channel_name = name;
}

string Channel::name() {
    return channel_name;
}