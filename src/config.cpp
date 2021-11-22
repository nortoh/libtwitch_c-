#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include <list>
#include "../includes/config.h"
#include "../includes/utils.h"

using namespace std;

static map<string, string> options_map;

Configuration::Configuration() {
    cout << "Reading configuration\n";
    read_configuration();
}

void Configuration::build_config_pair(string line) {
    list<string> options = Utils::split(line, "=");

    string key;
    string value;
    int count = 0;

    while(!options.empty()) {
        switch(count) {
            case 0:
                key = options.front();
                break;
            case 1:
                value = options.front();
                break;
        }
        options.pop_front();
        count++;
    }    
    options_map.insert(pair<string, string>(key, value));
}


string Configuration::get(string key) {;
    return options_map.find(key)->second;
}

void Configuration::read_configuration() {
    ifstream config_file("bot.config");
    string line;

    while(getline(config_file, line)) {
        build_config_pair(line);
    }

    map<string, string>::iterator it = options_map.begin();

    for(it=options_map.begin(); it != options_map.end(); ++it) {
        cout << "Loaded (" << it->first << ", " << it->second << ")\n";
    }
}