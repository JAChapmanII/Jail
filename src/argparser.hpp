#include <string>
#include <vector>
#include <map>

/**
 * Parse arguments given to the program in standard form
 */
// TODO: template-ize this thing?
class ArgParser {
    public:
        /// Type for a void callback function invoked on different switches
        typedef int (*VoidCallbackFunction)();
        /// Typedef for the map of (void) functions to run after parsing
        typedef std::map<VoidCallbackFunction, std::string> VoidCallMap;

        /// Type for a string callback function invoked on different switches
        typedef int (*StringCallbackFunction)(std::string);
        /// Typedef for the map of (string) functions to run after parsing
        typedef std::map<StringCallbackFunction, std::string> StringCallMap;

        /// A long/short switch name pair
        typedef std::pair<std::string, std::string> SwitchName;

        /// Construct an empty ArgParser
        ArgParser();

        /// Construct an ArgParser with an initial (string) switch
        ArgParser(SwitchName commandName, StringCallbackFunction function);
        /// Construct an ArgParser with an initial (void) switch
        ArgParser(SwitchName commandName, VoidCallbackFunction function);

        /// Add another (string) switch to look for
        void add(SwitchName commandName, StringCallbackFunction function);
        /// Add another (void) switch to look for
        void add(SwitchName commandName, VoidCallbackFunction function);

        /// Do not look for the specified switch any longer
        void remove(SwitchName commandName);
        /// Remove a switch, looking in only one map
        void remove(SwitchName commandName, bool takesArgument);

        /// Add a (string) function to run directly into the call map
        void queue(StringCallbackFunction function, std::string argument);
        /// Add a (void) function to run directly into the call map
        void queue(VoidCallbackFunction function);

        /// Parse the arguments given the the main function
        int parseArguments(int argc, char **argv);

        /// Run the commands this has found need to be executed
        void runCommands();
        /// Run the commands which take an argument
        void runStringCommands();
        /// Run the commands which take no arguments
        void runVoidCommands();

    protected:
        /// Map of names to functions that don't take an argument
        std::map<SwitchName, VoidCallbackFunction> voidCallbackMap;
        /// Map of names to functions that take a string
        std::map<SwitchName, StringCallbackFunction> stringCallbackMap;

        /// map of (void) functions to call on runCommands invocation
        VoidCallMap voidCalls;
        /// map of (string) functions to call on runCommands invocation
        StringCallMap stringCalls;
};

// vim:ts=4 et sw=4 sts=4
