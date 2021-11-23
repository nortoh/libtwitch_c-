#include <iostream>
#include "../includes/channel.h"
#include "../includes/user.h"
#include "../includes/tsocket.h"
#include "../includes/config.h"
#include "../includes/bot.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Driver application for twitch bot\n"; 
    
    Configuration config;
    cout << "The username is " << Configuration::get("username") << "\n";

    Bot bot (Configuration::get("username"));    
    bot.start_bot();
    
    return 0;
}
