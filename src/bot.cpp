#include <iostream>
#include <string>
#include "../includes/bot.h"
#include "../includes/tsocket.h"


Bot::Bot(string username) {

}

void Bot::start_bot() {
    start_connection();
}

string Bot::name() {
    return this->_username;
}

void Bot::join_channel(string channel) {
    if(channel.find('#') == string::npos) return;
    
}
