#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../includes/utils.h"
#include "../includes/config.h"

using namespace std;

static int sock = 0;
static struct sockaddr_in serv_addr;
static size_t bytes_recv = 0, full_buffer_size = 0;
static char recv_buffer[512]; // 512B
static char full_buffer[1024 * 5]; // 5KB


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

void receive_full_chunk(int* more) {
    
    // Clear the recv_buffer before reading in more
    memset(recv_buffer, 0, sizeof(recv_buffer));

    if((bytes_recv = recv(sock, recv_buffer, sizeof(recv_buffer), 0)) > 0) {
        int newline_flag = recv_buffer[bytes_recv - 1] == '\n';
        int carriage_flag = recv_buffer[bytes_recv - 2] == '\r';

        memmove(full_buffer + full_buffer_size, recv_buffer, bytes_recv * sizeof(char));
        full_buffer_size += bytes_recv;

        if(newline_flag && carriage_flag) {
            *more = 0;
        }
    }
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

    // Caps
    send_raw("CAP REQ :twitch.tv/commands\r\n");
    send_raw("CAP REQ :twitch.tv/tags\r\n");
    send_raw("CAP REQ :twitch.tv/membership\r\n");

    send_raw("JOIN #xqcow\r\n");

    int more_flag = 0;

    while(connection > 0) {
        if((bytes_recv = recv(sock, (void *)recv_buffer, sizeof(recv_buffer), 0)) > 0) {

            if(bytes_recv < 0) {
                cout << "Not receiving any data\n";
                break;
            }

            // Check if our buffer ends with /r/n
            int newline_flag = recv_buffer[bytes_recv - 1] == '\n';
            int carriage_flag = recv_buffer[bytes_recv - 2] == '\r';

            // If it does not, dump the recv buffer into the full_buffer
            // and continue loading more data until /r/n
            if(!(newline_flag && carriage_flag)) {
                memmove(full_buffer, recv_buffer, bytes_recv * sizeof(char));
                full_buffer_size += bytes_recv;
                more_flag = 1;

                while(more_flag) {
                    receive_full_chunk(&more_flag);
                }
            } else {
                // We have /r/n, copy the recv_buffer to the full_buffer
                memmove(full_buffer, recv_buffer, bytes_recv * sizeof(char));
            }

            // Add null termination 
            full_buffer[full_buffer_size - 1] = '\0';
            full_buffer_size = 0;

            list<string> line = Utils::split(string(full_buffer), "\r\n");

            while(!line.empty()) {
                cout << "> " << line.front() << "\n";
                line.pop_front();
            }
            cout << "----------------------------------------------------------\n";
            
            // Clear memory when we are done
            memset(full_buffer, 0, sizeof(full_buffer));
            memset(recv_buffer, 0, sizeof(recv_buffer));
        }
    }

    cout << "Closing socket\n";

    close(connection);
}
