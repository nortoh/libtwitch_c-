#include <string>
#include <fstream>
#include <vector>

using namespace std;

#ifndef __CONFIG_H__
#define __CONFIG_H__

class Configuration {
    public:
        Configuration();
        static string get(string key);
    private:
        void read_configuration();
        void build_config_pair(string line);
};

#endif