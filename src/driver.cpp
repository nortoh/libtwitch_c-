#include <iostream>
#include "../includes/channel.h"

using namespace std;

int main(int argc, char** argv) {
    cout << "Driver application for twitch bot\n"; 
    
    Channel test_channel;
    test_channel.set_name("#test");

    cout << "The channel is called " << test_channel.name() << "\n";

    return 0;
}
