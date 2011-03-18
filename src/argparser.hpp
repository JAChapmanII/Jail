#include <string>
#include <vector>
#include <map>

/**
 * Parse arguments given to the program in standard form
 */
class ArgParser {
    public:
        /// Type for a callback function invoked on different switches
        typedef int (*CallbackFunction)(std::string);

        /// Typedef for the map of functions to run after parsing
        typedef std::map<CallbackFunction, std::string> toCallMap;

        /// Construct an empty ArgParser
        ArgParser();

        /// Construct an ArgParser with an initial switch
        ArgParser(std::string longCommand, std::string shortCommand,
            bool hasArg, CallbackFunction function);

        /// Add another switch to look for
        void add(std::string longCommand, std::string shortCommand,
            bool hasArg, ArgParser::CallbackFunction function);
        /// Do not look for the specified switch any longer
        void remove(std::string commandName);
        /// Add a function to run directly into the call map
        void queue(ArgParser::CallbackFunction function, std::string argument);

        /// Parse the arguments given the the main function
        int parseArguments(int argc, char **argv);
        /// Run the commands this has found need to be executed
        void runCommands();

    protected:
        /// vector of long forms of switches
        std::vector<std::string> longCommands;
        /// vector of short form of switches
        std::vector<std::string> shortCommands;
        /// vector telling if a function to run takes an argument
        std::vector<bool> hasArgs;
        /// vector of callback functions
        std::vector<ArgParser::CallbackFunction> callbackFunctions;

        /// map of functions to call on runCommands invocation
        toCallMap toCall;
};

// vim:ts=4 et sw=4 sts=4
