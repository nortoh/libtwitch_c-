#include <iostream>
#include "../includes/channel.h"
#include "../includes/user.h"
#include "../includes/tsocket.h"
#include "../includes/config.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Driver application for twitch bot\n"; 
    
    Configuration config;
    
    Channel test_channel ("#test");
    User test_user ("nortoh", test_channel);
    
    cout << "User " << test_user.get_name() << " is in " << test_user.get_channel().name() << "\n";

    cout << "The username is " << Configuration::get("oauth_key") << "\n";
    
    start_connection();    
    return 0;
}
