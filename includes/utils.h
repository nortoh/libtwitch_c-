#include <string>
#include <list>

using namespace std;

#ifndef __UTILS_H__
#define __UTILS_H__

class Utils {
    public:
        static list<string> split(string input, string delim);
        static string clear_newlines(string input);
        static string clear_whitespaces(string);
};

#endif