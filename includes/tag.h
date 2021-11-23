#include <string>
#include <map>
using namespace std;

#ifndef __TAG_H__
#define __TAG_H__
class Tag {
    public:
        Tag(string);
        string get(string key);
        string str();
    private:
        string tag_string;
        void dissect();
        map<string, string> pairs;
};

#endif