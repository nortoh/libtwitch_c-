#include <iostream>
#include <string>
#include <list>
#include "../includes/tag.h"
#include "../includes/utils.h"

Tag::Tag(string tag) {
    if(tag.empty()) return;
    this->tag_string = tag.substr(1, tag.length()); // Remove the @
    dissect();
}

string Tag::get(string key) {
    return this->pairs.find(key)->second;
}

void Tag::dissect() {
    list<string> parts = Utils::split(this->tag_string, ";");

    while(!parts.empty()) {
        list<string> elements = Utils::split(parts.front(), "=");

        string key;
        string value;
        int count = 0;
        while(!elements.empty()) {
            switch(count) {
                // Key
                case 0:
                    key = elements.front();
                    break;
                // Value
                case 1:
                    value = elements.front();
                    break;
            }
            elements.pop_front();
            count++;
        }
        if(value.empty() || value == "") value = "0";
                
        this->pairs.insert(pair<string, string> (key, value));
        parts.pop_front();
    }
}

string Tag::str() {
    return this->tag_string;
}