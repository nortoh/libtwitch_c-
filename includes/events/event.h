#include <string>

using namespace std;

#ifndef __EVENT_H__
#define __EVENT_H__
class Event {
    public:
        enum EventType {
            SOCKET_CONNECTED,
            CHAT_CONNECTED,
            READ_MOTD,
            JOIN,
            PART,
            PRIVMSG,
            PING
        };
        Event(EventType);
        EventType type();
        virtual void print();
    private:
        string s;
        EventType _type;
};

#endif