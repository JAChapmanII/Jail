#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

void parseCommand(string command);

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    bool isParseCommand = false;
    string pCommand = "";

    for(int i = 0; i < argc; ++i) {
        if((string)argv[i] == (string)"--parse-command") {
            isParseCommand = true;
            if(i < argc - 1)
                // set pCommand to be the next argument, and then skip it
                pCommand = argv[i++ + 1];
        }
    }

    if( isParseCommand )
        parseCommand(pCommand);

    return 0;
}

void parseCommand(string command) {
    cout << "Looks like a fine command to me!" << endl;
    cout << command << endl;
    return;
}

// vim:ts=4 et sw=4 sts=4
