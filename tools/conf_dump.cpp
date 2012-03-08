#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include "config.hpp"

int main(int argc, char **argv) {
    if(argc > 2) {
        cerr << "Usage: " << argv[0] << " [in conf]" << endl;
        return 1;
    }

    config::init();

    if(argc > 1) {
        if(((string)"--help" == argv[1]) || ((string)"-h" == argv[1])) {
            cerr << "Usage: " << argv[0] << " [in conf]" << endl;
            return 0;
        } else {
            int ret = config::load(argv[1]);
            if(ret == 0 || ret == -1) {
                return 2;
            }
            cout << "config::load: " << ret << endl;
        }
    }

    for(auto i = config::begin(); i != config::end(); ++i) {
        cout << i->first << endl;
        for(auto j = i->second.begin(); j != i->second.end(); ++j)
            cout << "\t" << j->first << " = " << j->second << "" << endl;
        cout << endl;
    }

    return 0;
}

// vim:ts=4 et sw=4 sts=4
