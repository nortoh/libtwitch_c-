#include <iostream>
#include <string>
#include <cstring>
#include <list>
#include "../includes/utils.h"

using namespace std;

list<string> Utils::split(string input, string delim) {
    list<string> result;
    char input_chars[input.length() + 1];
    char delim_chars[delim.length() + 1];
    input.copy(input_chars, sizeof(input_chars), 0);
    delim.copy(delim_chars, sizeof(delim_chars), 0);

    char* save;
    char* line;

    line = strtok_r(input_chars, delim.c_str(), &save);
    while(line != 0) {
        result.push_back(line);
        line = strtok_r(0, delim.c_str(), &save);
    }
    return result;
}

string Utils::clear_newlines(string input) {
    size_t pos = 0;
    while((pos = input.find("\n", pos)) != string::npos) {
        input.erase(pos);
    }

    pos = 0;
    while((pos = input.find("\r", pos)) != string::npos) {
        input.erase(pos);
    }
    return input;
}