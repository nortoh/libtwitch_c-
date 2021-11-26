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
extern IRC_TYPE irc_2_type(string);
extern int send_raw_str(string);
void receive_chunk(int*, string*, size_t*);
#endif