#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>
using std::ofstream;

#include <string>
using std::string;

#include "config.hpp"

string escape(string str) {
    string result;
    for(char c : str) {
        switch(c) {
            case '"':
                result += "\\\"";
                break;
            default:
                result += c;
                break;
        }
    }
    return result;
}

int main(int argc, char **argv) {
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " <in conf> <out file>" << endl;
        return 1;
    }

    config::init();
    int ret = config::map.load(argv[1]);
    if(ret == 0 || ret == -1) {
        return 2;
    }
    if(argc > 3)
        cout << "config::load: " << ret << endl;

    ofstream out(argv[2]);

    for(auto i = config::map.begin(); i != config::map.end(); ++i) {
        for(auto j = i->second.begin(); j != i->second.end(); ++j) {
            if(argc > 3)
                cout << "config::map[\"" << escape(i->first) << "\"]"
                    << "[\"" << escape(j->first)
                    << "\"] = \"" << escape(j->second) << "\";" << endl;
            out << "config::map[\"" << escape(i->first) << "\"]"
                << "[\"" << escape(j->first)
                << "\"] = \"" << escape(j->second) << "\";" << endl;
        }
    }

    out.close();
    return 0;
}

// vim:ts=4 et sw=4 sts=4
