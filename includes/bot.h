#include <string>

using namespace std;

#ifndef __BOT_H__
#define __BOT_H__
class Bot {
    public:
        Bot(string username);
        void join_channel(string);
        void part_channel(string);
        string name();
        void start_bot();
    private:
        string _username;
};
#endif