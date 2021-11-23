#ifndef __TSOCKET_H__
#define __TSOCKET_H__

enum IRC_TYPE {
    CAP,
    JOIN,
    PART,
    PING,
    PRIVMSG,
    CLEARCHAT,
    ROOMSTATE,
    USERNOTICE,
    CTCP,
    CLEARMSG,
    NOTICE,
    USERSTATE,
    HOSTTARGET,
    WHISPER,
    RECONNECT,
    UNKNOWN
};
extern void start_connection();
extern IRC_TYPE irc_2_type(string line);
void receive_chunk(int* more, string* buffer, size_t* size);
#endif