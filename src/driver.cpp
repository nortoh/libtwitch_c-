#include <iostream>
#include "../includes/channel.h"
#include "../includes/user.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Driver application for twitch bot\n"; 
    
    Channel test_channel ("#test");
    User test_user ("nortoh", test_channel);
    
    cout << "User " << test_user.get_name() << " is in " << test_user.get_channel().name() << "\n";

    return 0;
}
