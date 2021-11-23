#ifndef __TSOCKET_H__
#define __TSOCKET_H__

enum IRC_TYPE {
    PRIVMSG,
    JOIN,
    PART,
    PING,
    CAP,

    UNKNOWN
};
extern void start_connection();
extern IRC_TYPE irc_2_type(string line);
void receive_chunk(int* more, string* buffer, size_t* size);
#endif