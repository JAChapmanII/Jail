#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "argparser.hpp"

int parseCommand(string command);

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    ArgParser mArgParser;
    mArgParser.add(ArgParser::SwitchName("--parse-command", ""), &parseCommand);
    mArgParser.parseArguments(argc, argv);
    mArgParser.runCommands();

    cout << "We were invoked as: " << mArgParser.getInvocationName() << "\n";

    vector<string> extraArguments = mArgParser.getExtraArguments();
    if(extraArguments.size() > 0) {
        cout << "These arguments were left over after switch parsing: \n";
        for(vector<string>::iterator i = extraArguments.begin();
                i != extraArguments.end(); ++i)
            cout << ": " + (*i) << "\n";
    }

    cout << endl;

    return 0;
}

int parseCommand(string command) {
    cout << command;
    switch(command[0]) {
        case ':':
            cout << " looks like an ex command to me!";
            break;
        case '/':
            cout << " looks like a search to me!";
            break;
        default:
            cout << " looks like a fine command to me!";
    }
    cout << endl;
    return 0;
}

// vim:ts=4 et sw=4 sts=4
