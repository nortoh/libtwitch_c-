#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../includes/utils.h"
#include "../includes/config.h"

using namespace std;

static int sock = 0;
static struct sockaddr_in serv_addr;

int send_raw(char* raw) {
    if(!sock) return -1;
    cout << "< " << raw;
    return send(sock, raw, strlen(raw), 0);
}

int conn(string host, int port) {

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    cout << "Created socket\n";
    
    if(inet_pton(AF_INET, host.c_str(), &serv_addr.sin_addr) <= 0) {
        cout << "Failed to convert IP to binary form\n";
        return -1;
    }

    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection failed\n";
        return -1;
    }

    cout << "Connected!\n";

    return 1;
}

void start_connection() {

    cout << "Attempting to connect to Twitch...\n";

    int connection = conn("44.226.36.141", 6667);
    if(connection < 0) {
        cout << "Error connecting to twitch\n";
    }

    // Do some authentication
    char password[64];
    char username[64];
    char nickname[64];
    sprintf(password, "PASS %s\r\n", Configuration::get("oauth_key").c_str());
    sprintf(username, "USER %s\r\n", Configuration::get("username").c_str());
    sprintf(nickname, "NICK %s\r\n", Configuration::get("username").c_str());

    send_raw(password);
    send_raw(username);
    send_raw(nickname);

    size_t bytes_recv;
    char recv_buffer[512];

    while(connection > 0) {
        if((bytes_recv = recv(sock, (void *)recv_buffer, sizeof(recv_buffer), 0)) > 0) {
            cout << "> " << recv_buffer << "\n";
        }
        
        break;
    }

    cout << "Closing socket\n";

    close(connection);
}
