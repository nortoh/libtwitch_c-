#include <string>
#include <list>

using namespace std;

#ifndef __BOT_H__
#define __BOT_H__
class Bot {
    public:
        Bot(string username);
        void join_channel(string);
        void part_channel(string);
        bool in_channel(string);
        void send_message(string, string);
        list<string> channels_in();
        string name();
        void start_bot();
    private:
        string _username;
        list<string> _channels;
};
#endif