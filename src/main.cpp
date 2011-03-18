#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "argparser.hpp"

void parseCommand(string command);

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    ArgParser mArgParser;
    mArgParser.add("--parse-command", "", true, &parseCommand);
    mArgParser.parseArguments(argc, argv);
    mArgParser.runCommands();

    return 0;
}

void parseCommand(string command) {
    cout << "Looks like a fine command to me!" << endl;
    cout << command << endl;
    return;
}

// vim:ts=4 et sw=4 sts=4
