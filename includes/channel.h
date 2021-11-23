#include <string>
using namespace std;

#ifndef __CHANNEL_H__
#define __CHANNEL_H__
class Channel {
    public:
        Channel() {};
        Channel(string);
        string name();
        void set_name(string);
    private:
        string channel_name;
};
#endif