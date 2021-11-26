#include <iostream>
#include <string>
#include <list>
#include <algorithm>
#include "../includes/bot.h"
#include "../includes/tsocket.h"


Bot::Bot(string username) {
    this->_username = username;
}

void Bot::start_bot() {
    start_connection();
}

string Bot::name() {
    return this->_username;
}

list<string> Bot::channels_in() {
    return this->_channels; 
}

bool Bot::in_channel(string channel) {
    list<string>::iterator it;
    it = find(_channels.begin(), _channels.end(), channel);
    return it != _channels.end();
}

void Bot::join_channel(string channel) {
    if(channel.find('#') == string::npos) return;
    if(in_channel(channel)) return;
    _channels.push_back(channel);
    send_raw_str(string("JOIN ").append(channel).append("\r\n"));
}

void Bot::part_channel(string channel) {
    if(channel.find('#') == string::npos) return;
    if(!in_channel(channel)) return;
    send_raw_str(string("PART ").append(channel).append("\r\n"));
}

void Bot::send_message(string channel, string message) {
    if(channel.find('#') == string::npos) return;
    if(!in_channel(channel)) return;
    send_raw_str(string("PRIVMSG ").append(channel).append(" :").append(message).append("\r\n"));
}
