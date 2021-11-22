#include <string>
using namespace std;

#ifndef __CHANNEL_H__
#define __CHANNEL_H__
class Channel {
    public:
        string name();
        void set_name(string name);
    private:
        string channel_name;
};
#endif