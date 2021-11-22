#include <iostream>
#include <string>
#include <list>
#include "../includes/utils.h"

using namespace std;

list<string> Utils::split(string input, string delim) {
    size_t start = 0, end, delim_len = delim.length();
    list<string> result;
    string token;
    while((end = input.find(delim, start)) != string::npos) {
        token = input.substr(start, end - start);
        start = end + delim_len;
        result.push_back(token);
    }
    result.push_back(input.substr(start));
    return result;
}

string Utils::clear_newlines(string input) {
    size_t pos = 0;
    while((pos = input.find("\n", pos)) != string::npos) {
        input.erase(pos);
    }
    return input;
}