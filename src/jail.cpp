#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::hex;
using std::dec;

#include <iomanip>
using std::setw;
using std::setfill;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "argparser.hpp"
#include "fileio.hpp"
#include "config.hpp"

#include "buffer.hpp"
#include "view.hpp"
#include "controller.hpp"

void parseCommand(string command);
void printHelp();
void printVersion();
void dump(vector<string> fileNames);
void view(vector<string> fileNames);
void edit(vector<string> fileNames);

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    ArgParser mArgParser;
    mArgParser.setHelp();
    mArgParser.setHelpHeader("jail [switches] <file>\n");
    mArgParser.setHelpFooter(
            "\nYou may also wish to check the README for more information\n");

    mArgParser.add(ArgParser::SwitchName("--version", ""), &printVersion,
            "Print the version of jail which is running");
    mArgParser.add(ArgParser::SwitchName("--verbose", "-v"),
            &config::setVerbose, "Set the output to be verbose");
    mArgParser.add(ArgParser::SwitchName("--quiet", "-q"), &config::setQuiet,
            "Set the output to be quiet");

    mArgParser.add(ArgParser::SwitchName("--parse-command", ""), &parseCommand,
            "Attempt to say what jail would do with a command");
    mArgParser.add(ArgParser::SwitchName("--dump", ""), &config::setDump,
            "Dump the contents of a file to the screen");
    mArgParser.add(ArgParser::SwitchName("--hex", ""), &config::setHexMode,
            "When dumping, output in hexadecimal");
    mArgParser.add(ArgParser::SwitchName("--hex-dump", ""),
            &config::setHexDump, "Combination of --hex and --dump");
    mArgParser.add(ArgParser::SwitchName("--width", "-w"), &config::setWidth,
            "Sets the maximum width in columns of the hex dump output");
    mArgParser.add(ArgParser::SwitchName("", "-R"), &config::setReadOnly,
            (string)"Puts jail into read-only mode, so that you cannot " +
            (string)"overwrite the backing file");

    mArgParser.parseArguments(argc, argv);
    mArgParser.runCommands();

    if(config::isVerbose()) {
        cout << "Invoked as: " << mArgParser.getInvocationName() << "\n";

        vector<string> extraArguments = mArgParser.getExtraArguments();
        if(extraArguments.size() > 0) {
            cout << "These arguments were left over after switch parsing: \n";
            for(vector<string>::iterator i = extraArguments.begin();
                    i != extraArguments.end(); ++i)
                cout << ": " + (*i) << "\n";
        }
        cout << endl;
    }

    if(config::isVerbose())
        cout << "We're being verbose!" << endl;

    if(config::isDump())
        dump(mArgParser.getExtraArguments());
    else {
        // TODO: this may not be the only other option. Currently, the output
        // looks alright though. Must fix! /TODO
        if(config::isReadOnly())
            view(mArgParser.getExtraArguments());
        else
            edit(mArgParser.getExtraArguments());
    }

    return 0;
}

void parseCommand(string command) {
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
}

void printVersion() {
    cout << "version: " << config::getVersion() << endl;
}

void dump(vector<string> fileNames) {
    if(fileNames.empty())
        return;

    string fileName = fileNames.back();
    FileIO file(fileName);
    file.open();
    int length = file.read();

    if(length < 0) {
        cerr << "There was a problem reading the file \""
            << fileName << "\" for dumping." << endl;
        return;
    }

    if(config::isVerbose())
        cout << "File \"" << fileName << "\" is " << length << " bytes long.\n";

    if(!config::isHexMode()) {
        cout << file.getData();
    } else {
        char *data = file.getData();
        int stop = (config::getWidth() + 1) / 3;
        cout << hex;
        for(int i = 0; i < length; ++i) {
            cout << setw(2) << setfill('0') << (unsigned int)data[i] << ' ';
            if(((i+1) % stop) == 0)
                cout << '\n';
        }
        cout << dec;
    }
    cout << endl;

    return;
}

void view(vector<string> fileNames) {
    if(fileNames.empty())
        return;

    string fileName = fileNames.back();
    FileIO file(fileName);

    Buffer mBuffer(&file, true);
    Window mWindow;
    View mView(&mWindow, &mBuffer);
    Controller mController(&mView);
    mController();
}

void edit(vector<string> fileNames) {
    if(fileNames.empty())
        return;

    string fileName = fileNames.back();
    FileIO file(fileName);

    Buffer mBuffer(&file);
    Window mWindow;
    View mView(&mWindow, &mBuffer);
    Controller mController(&mView);
    mController();

}

// vim:ts=4 et sw=4 sts=4
