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
#include "../includes/tag.h"
#include "../includes/channel.h"
#include "../includes/message.h"
/* Events*/
#include "../includes/events/privmsg_event.h"

using namespace std;

static int sock = 0;
static struct sockaddr_in serv_addr;
static size_t bytes_recv = 0;
static char recv_buffer[512] = {0}; // 512B

static string message_buffer ("");
static size_t message_size (0);

static bool connected = 0;
static bool motd = 0;

static Channel channel;
static User user;
static Message message;
static Tag tag;

int send_raw_str(string raw) {
    if(!sock) return -1;
    cout << "< " << raw.c_str();
    return send(sock, raw.c_str(), raw.length(), 0);
}

IRC_TYPE irc_2_type(string line) {
    bool has_cap = line.find("CAP") != std::string::npos;
    bool has_join = line.find("JOIN") != std::string::npos;
    bool has_part = line.find("PART") != std::string::npos;
    bool has_ping = line.find("PING") != std::string::npos;
    bool has_privmsg = line.find("PRIVMSG") != std::string::npos;
    bool has_usernotice = line.find("USERNOTICE") != std::string::npos;
    bool has_clearchat = line.find("CLEARCHAT") != std::string::npos;
    bool has_clearmsg = line.find("CLEARMSG") != std::string::npos;
    bool has_notice = line.find("NOTICE") != std::string::npos;
    bool has_roomstate = line.find("ROOMSTATE") != std::string::npos;

    if(has_privmsg) return IRC_TYPE::PRIVMSG;
    if(has_join && !has_privmsg) return IRC_TYPE::JOIN;
    if(has_part && !has_privmsg) return IRC_TYPE::PART;
    if(has_ping && !has_privmsg) return IRC_TYPE::PING;
    if(has_cap && !has_privmsg) return IRC_TYPE::CAP;
    if(has_usernotice && !has_privmsg) return IRC_TYPE::USERNOTICE;
    if(has_clearchat && !has_privmsg) return IRC_TYPE::CLEARCHAT;
    if(has_clearmsg && !has_privmsg) return IRC_TYPE::CLEARMSG;
    if(has_notice && !has_privmsg) return IRC_TYPE::NOTICE;

    return IRC_TYPE::UNKNOWN;
}

bool received_id(string line, int id) {
    list<string> parts = Utils::split(line, " ");

    int count = 0;
    int got = 0;
    while(!parts.empty()) {
        if(count != 1) {
            count++;
            parts.pop_front();
            continue;
        }
        got = stoi(parts.front());
        break;
    }
    parts.clear();

    return got == id;
}

void handle_privmsg(string line) {
    list<string> parts = Utils::split(line, " ");

    string _tag = "";
    string _user = "";
    string _channel = "";

    int building = 0;
    int count = 0;
    while(!parts.empty()) {
        switch(count) {
            case 0:
                // Tag
                _tag = parts.front();
                break;
            case 1:
                // Username
                _user = parts.front().substr(1, parts.front().find("!") - 1);
                break;
            case 2:
                // IRC Type
                break;
            case 3:
                // Channel name
                _channel = parts.front();
                break;
            default:

                string part = parts.front();

                if(!building) {
                    building = 1;

                    message_buffer += (part).append(" ");
                    message_size += message_buffer.length();
                } else {
                    message_buffer += (part).append(" ");
                    message_size += message_buffer.length();
                }
                break;
        }

        // Pop the data we just used and increase count
        parts.pop_front();
        count++;
    }
    message_buffer[message_size - 1] = '\0';
    
    channel = Channel(_channel);
    user = User(_user, channel);
    tag = Tag(_tag);
    message = Message(channel, user, tag, message_buffer.substr(1, message_buffer.length() - 2));
    PrivmsgEvent privmsg_event (channel, user, tag, message);

    privmsg_event.print();

    message_buffer.clear();
    parts.clear();
}

void handle_ping(string line) {
    send_raw_str("PONG :tmi.twitch.tv\r\n");
}

void handle_join(string line) {
    list<string> parts = Utils::split(line, " ");

    string _user;
    string _channel;

    int count = 0;
    while(!parts.empty()) {
        switch(count) {
            case 0:
                _user = parts.front();
                break;
            case 2:
                _channel = parts.front();
                break;
        }
        count++;
        parts.pop_front();
    }

    parts.clear();
    cout << "JOIN " << _user << "/" << _channel << "\n";
 }

void handle_part(string line) {
    list<string> parts = Utils::split(line, " ");

    string _user;
    string _channel;

    int count = 0;
    while(!parts.empty()) {
        switch(count) {
            case 0:
                _user = parts.front();
                break;
            case 2:
                _channel = parts.front();
                break;
        }
        count++;
        parts.pop_front();
    }

    parts.clear();
    cout << "PART " << _user << "/" << _channel << "\n";
}

void handle_cap(string line) {

}

void handle_notice(string line) {

}

void handle_line(string line) {
    if(!connected) {
        connected = received_id(line, 1);
        if(connected) {
        } else {
            return;
        }
    }

    if(!motd) {
        motd = received_id(line, 376);
        if(motd) {
            send_raw_str("JOIN #xqcow\r\n");
            send_raw_str("JOIN #ludwig\r\n");
            send_raw_str("JOIN #hasanabi\r\n");
        }
        return;
    }

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
        case IRC_TYPE::NOTICE:
            handle_notice(line);
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

void caps_and_auth() {
    // Do some authentication
    send_raw_str(string("PASS ").append(Configuration::get("oauth_key").append("\r\n")));
    send_raw_str(string("USER ").append(Configuration::get("username").append("\r\n")));
    send_raw_str(string("NICK ").append(Configuration::get("username").append("\r\n")));

    // Caps
    send_raw_str("CAP REQ :twitch.tv/commands\r\n");
    send_raw_str("CAP REQ :twitch.tv/tags\r\n");
    send_raw_str("CAP REQ :twitch.tv/membership\r\n");
}

void start_connection() {
    // Clear garbage out of recv buffer
    memset(recv_buffer, 0, sizeof(recv_buffer));

    cout << "Attempting to connect to Twitch...\n";

    int connection = conn("44.226.36.141", 6667);

    // Sleep a second to give time for the server to process our connection
    sleep(1);
    if(connection < 0) {
        cout << "Error connecting to twitch\n";
    }

    caps_and_auth();

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
            buffer[buffer_size - 2] = '\0';
            buffer[buffer_size - 1] = '\0';

            list<string> line = Utils::split(string(buffer), "\n");
            while(!line.empty()) {
                handle_line(line.front());
                line.pop_front();
            }

            // Clear
            buffer.clear();
            buffer_size = 0;
            memset(recv_buffer, 0, sizeof(recv_buffer));
        }
    }
    
    close(connection);
    cout << "Closed socket\n";
}