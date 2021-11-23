#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../includes/utils.h"
#include "../includes/config.h"
#include "../includes/tsocket.h"
#include "../includes/user.h"

using namespace std;

static int sock = 0;
static struct sockaddr_in serv_addr;
static size_t bytes_recv = 0;
static char recv_buffer[512] = {0}; // 512B
// static char full_buffer[1024 * 5] = {0}; // 5KB

// static char message_buffer[1024] = {0}; // 3KB
// static size_t message_buffer_size = 0;
static string message ("");
static size_t message_size (0);

int send_raw(char* raw) {
    if(!sock) return -1;
    cout << "< " << raw;
    return send(sock, raw, strlen(raw), 0);
}

int send_raw_str(string raw) {
    if(!sock) return -1;
    cout << "< " << raw.c_str();
    return send(sock, raw.c_str(), raw.length(), 0);
}

IRC_TYPE irc_2_type(string line) {
    bool has_privmsg = line.find("PRIVMSG") != std::string::npos;
    bool has_join = line.find("JOIN") != std::string::npos;
    bool has_part = line.find("PART") != std::string::npos;
    bool has_ping = line.find("PING") != std::string::npos;
    bool has_cap = line.find("CAP") != std::string::npos;

    if(has_privmsg) return IRC_TYPE::PRIVMSG;
    if(has_join && !has_privmsg) return IRC_TYPE::JOIN;
    if(has_part && !has_privmsg) return IRC_TYPE::PART;
    if(has_ping && !has_privmsg) return IRC_TYPE::PING;
    if(has_cap && !has_privmsg) return IRC_TYPE::CAP;
    return IRC_TYPE::UNKNOWN;
}

void handle_privmsg(string line) {
    list<string> parts = Utils::split(line, " ");

    string tag = "";
    string username = "";
    string chan_name = "";

    int building = 0;
    int count = 0;
    while(!parts.empty()) {
        switch(count) {
            case 0:
                // Tag
                tag = parts.front();
                break;
            case 1:
                // Username
                username = parts.front().substr(1, parts.front().find("!") - 1);
                break;
            case 2:
                // IRC Type
                break;
            case 3:
                // Channel name
                chan_name = parts.front();
                break;
            default:

                string part = parts.front();

                if(!building) {
                    building = 1;

                    message += (part).append(" ");
                    message_size += message.length();
                } else {
                    message += (part).append(" ");
                    message_size += message.length();
                }
                break;
        }

        // Pop the data we just used and increase count
        parts.pop_front();
        count++;
    }
    parts.clear();
    
    message[message_size - 1] = '\0';

    cout << "Tag: " << tag << "\n";
    cout << "Username: " << username.substr(0, username.find("!") - 1) << "\n";
    cout << "Message: " << message.substr(1, message.length() - 2) << "\n";

    Channel channel (chan_name);
    User user (username, channel);
    cout << "Channel: " << channel.name() << "\n";
    cout << "User: " << user.name() << "\n";

    message.clear();
}

void handle_ping(string line) {
    send_raw_str("PONG :tmi.twitch.tv\r\n");
}

void handle_join(string line) {

}

void handle_part(string line) {

}

void handle_cap(string line) {

}

void handle_line(string line) {
    cout << "> " << line << "\n";

    IRC_TYPE type = irc_2_type(line);
    switch(type) {
        case IRC_TYPE::PRIVMSG:
            handle_privmsg(line);
            break;
        case IRC_TYPE::CAP:
            handle_cap(line);
            break;
        case IRC_TYPE::JOIN:
            handle_join(line);
            break;
        case IRC_TYPE::PART:
            handle_part(line);
            break;
        case IRC_TYPE::PING:
            handle_ping(line);
            break;
    }
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

void receive_chunk(int* more, string* buffer, size_t* size) {
    memset(recv_buffer, 0, sizeof(recv_buffer));

    if((bytes_recv = recv(sock, recv_buffer, sizeof(recv_buffer), 0)) > 0) {
        string converted_buffer = string(recv_buffer);
        bool newline = converted_buffer[bytes_recv - 1] == '\n';
        bool carriage = converted_buffer[bytes_recv - 2] == '\r';

        (*buffer) += converted_buffer;
        (*size) += converted_buffer.length();
        if(newline && carriage) {
            *more = 0;  
        }
    }
}

void start_connection() {

    cout << "Attempting to connect to Twitch...\n";

    int connection = conn("44.226.36.141", 6667);

    sleep(1);
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
    send_raw_str("CAP REQ :twitch.tv/commands\r\n");
    send_raw_str("CAP REQ :twitch.tv/tags\r\n");
    send_raw_str("CAP REQ :twitch.tv/membership\r\n");

    send_raw_str("JOIN #xqcow\r\n");
    send_raw_str("JOIN #ludwig\r\n");
    send_raw_str("JOIN #hasanabi\r\n");

    int more = 0;

    string buffer ("");
    size_t buffer_size (0);

    while(connection > 0) {
        if((bytes_recv = recv(sock, recv_buffer, sizeof(recv_buffer), 0)) > 0) {
            string converted_buffer (recv_buffer);
            bool newline = converted_buffer[bytes_recv - 1] == '\n';
            bool carriage = converted_buffer[bytes_recv - 2] == '\r';
            
            // No new line or carriage found, keep grabbing more data
            if(!(newline && carriage)) {
                buffer += converted_buffer;
                buffer_size += converted_buffer.length();
                more = 1;
                
                while(more) {
                    receive_chunk(&more, &buffer, &buffer_size);
                }
            } else {
                buffer += converted_buffer;
                buffer_size += converted_buffer.length();
            }

            cout << "============================================" << buffer_size << "============================================\n";
            buffer[buffer_size - 2] = '\0';
            buffer[buffer_size - 1] = '\0';

            list<string> line = Utils::split(string(buffer), "\n");

            while(!line.empty()) {
                handle_line(line.front());
                line.pop_front();
            }

            buffer.clear();
            buffer_size = 0;
            memset(recv_buffer, 0, sizeof(recv_buffer));
        }
    }
    
    close(connection);
    cout << "Closed socket\n";
}
