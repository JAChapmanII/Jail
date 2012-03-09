#include "keymap.hpp"
using std::string;

#include <map>
using std::map;

#include <functional>
using std::function;

#include <vector>
using std::vector;

#include <iostream>
using std::cerr;
using std::endl;

#include "util.hpp"
using util::split;
using util::join;
using util::contains;
using util::trim;
using util::startsWith;

#include "datamap.hpp"

// TODO: this doesn't work with multiple controllers/views, or if it does it
// TODO: probably isn't thread safe...
Controller *keymap::keymap_controller = NULL;
View *keymap::keymap_view = NULL;

static DataMap keymap_map;
static map<string, function<void(Controller &, View &)>> function_map;
static map<string, string> customFunction_map;
static vector<string> modes;
static string mode;
static string command;
static bool keymap_inited = false;

// function_map lamda prototype:
//    function_map[""] = [](Controller &controller, View &view) {
//        });

void keymap::init() {
    if(keymap_inited)
        return;

    keymap_inited = true;
#include "default_keymap.cpp"

    function_map["quit"] = [](Controller &controller, View &view) {
            controller.stop();
        };
    function_map["write"] = [](Controller &controller, View &view) {
            controller.write();
        };
    function_map["view-pageup"] = [](Controller &controller, View &view) {
            Cursor *cursor = view.getCursor();
            Window *window = view.getWindow();
            cursor->move(
                    cursor->getRow() - window->getHeight() + 1,
                    cursor->getCol());
        };
    function_map["view-pagedown"] = [](Controller &controller, View &view) {
            Cursor *cursor = view.getCursor();
            Window *window = view.getWindow();
            cursor->move(
                    cursor->getRow() + window->getHeight() - 1,
                    cursor->getCol());
        };
    function_map["view-beggining"] = [](Controller &controller, View &view) {
            view.getCursor()->toBeginningOfBuffer();
        };
    function_map["view-end"] = [](Controller &controller, View &view) {
            view.getCursor()->toEndOfBuffer();
        };
    function_map["delete-line"] = [](Controller &controller, View &view) {
            view.getCursor()->deleteLine();
            view.checkSanity();
            view.repaint();
        };
    function_map["delete-character"] = [](Controller &controller, View &view) {
            view.getCursor()->erase();
            view.repaint();
        };
    function_map["cursor-beginning"] = [](Controller &controller, View &view) {
            view.getCursor()->toBeginningOfLine();
        };
    function_map["cursor-end"] = [](Controller &controller, View &view) {
            view.getCursor()->toEndOfLine();
        };
    function_map["cursor-up"] = [](Controller &controller, View &view) {
            view.getCursor()->up();
        };
    function_map["cursor-down"] = [](Controller &controller, View &view) {
            view.getCursor()->down();
        };
    function_map["cursor-left"] = [](Controller &controller, View &view) {
            view.getCursor()->left();
        };
    function_map["cursor-right"] = [](Controller &controller, View &view) {
            view.getCursor()->right();
        };
    function_map["named-command"] = [](Controller &controller, View &view) {
            execute(controller.getCommand());
        };
    function_map["command-clear"] = [](Controller &controller, View &view) {
            command.clear();
        };
}

//# mode-<one of the core.modes>
//# backspace

void keymap::load(std::string file) {
    keymap_map.load(file);

    modes.clear();
    if(keymap_map.has("core.modes")) {
        vector<string> moreModes = split(keymap_map.get("core.modes"), ",");
        for(auto nmode : moreModes)
            if(!nmode.empty())
                modes.push_back(nmode);
    }
    modes.push_back("");
    mode = modes[0] + "-mode";
    cerr << "mode count: " << modes.size() << " -- " << mode << endl;

    customFunction_map.clear();
    if(keymap_map.hasScope("functions")) {
        for(auto func : keymap_map["functions"])
            customFunction_map[func.first] = func.second;
    }
}

static bool tryExecute(string command) {
    if(!keymap::keymap_controller || !keymap::keymap_view) {
        cerr << "keymap::execute: controller or view undefined" << endl;
        return false;
    }

    string function = "";
    if(keymap_map.has(mode, command))
        function = keymap_map.get(mode, command);
    else if(keymap_map.has("global", command))
        function = keymap_map.get("global", command);
    else
        return false;

    vector<string> sfuncs = split(function);
    for(auto sfunc : sfuncs) {
        sfunc = trim(sfunc);
        if(startsWith(sfunc, "mode:")) {
            mode = sfunc.substr(5);
            continue;
        }
        if(startsWith(sfunc, "insert:")) {
            command += sfunc.substr(7);
            continue;
        }
        if(!keymap::execute(sfunc))
            break;
    }

    return true;
}

bool keymap::execute(string function) {
    if(!keymap_controller || !keymap_view) {
        cerr << "keymap::execute: controller or view undefined" << endl;
        return false;
    }
    if(contains(customFunction_map, function)) {
        //cerr << function << " is custom function" << endl;
        return true;
    }
    if(contains(function_map, function)) {
        function_map[function](*keymap_controller, *keymap_view);
        return true;
    }
    cerr << "keymap::execute: " << function << " is not a function" << endl;
    return false;
}

void keymap::push_execute(int keycode) {
    command += (char)keycode;
    for(int i = 1; i < command.length(); ++i) {
        string subcommand = command.substr(0, i);
        if(tryExecute(subcommand)) {
            command = command.substr(i);
            i = 0;
        }
    }
    if(keymap_map.has(mode, "default")) {
        string defaction = keymap_map.get(mode, "default");
        if(defaction == "none") {
            // TODO: don't hard-code this number
            if(keycode == 127) {
                if(command.length() == 1)
                    command.clear();
                else
                    command.resize(command.size() - 2);
            }
            return;
        }
        if(defaction == "insert") {
            for(int keyc : command)
                keymap_view->getCursor()->insert((char)keyc);
            keymap_view->repaint();
            return;
        }
    }
    return;
}

// vim:ts=4 et sw=4 sts=4
