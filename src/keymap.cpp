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
#include "window.hpp"

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
    function_map["view-beginning"] = [](Controller &controller, View &view) {
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
    function_map["command-backspace"] = [](Controller &controller, View &view) {
            // TODO: don't hard code?
            int l = mapkey(127).length();
            if(command.length() <= l)
                command.clear();
            else
                command.resize(command.size() - l - 1);
            // TODO: this should actually backspace the lats token, such as
            // TODO: <space> or l
        };
    function_map["backspace"] = [](Controller &controller, View &view) {
            view.getCursor()->backspace();
            view.repaint();
        };
}

//# mode-<one of the core.modes>
//# backspace

// TODO: normalize key-names <C-f>, <^f> -> <C-f>, etc.
void keymap::load(std::string file) {
    keymap_map.load(file);

    modes.clear();
    if(keymap_map.has("core.modes")) {
        vector<string> moreModes = split(keymap_map.get("core.modes"), ",");
        for(auto nmode : moreModes) {
            nmode = trim(nmode);
            if(!nmode.empty())
                modes.push_back(nmode);
        }
    }
    modes.push_back("");
    mode = modes[0] + "-mode";
    cerr << "mode count: " << modes.size() << " -- " << mode << endl;

    for(auto m : modes) {
        cerr << "mode: " << m << endl;
        for(auto e : keymap_map[m + "-mode"])
            cerr << e.first << " -> " << e.second << endl;
    }

    customFunction_map.clear();
    if(keymap_map.hasScope("functions")) {
        for(auto func : keymap_map["functions"])
            customFunction_map[func.first] = func.second;
    }
}

// TODO: onlyGlobal? Really >_>
static bool tryExecute(string command, bool onlyGlobal = false) {
    if(!keymap::keymap_controller || !keymap::keymap_view) {
        cerr << "keymap::execute: controller or view undefined" << endl;
        return false;
    }

    string function = "";
    if(!onlyGlobal && keymap_map.has(mode, command))
        function = keymap_map.get(mode, command);
    else if(keymap_map.has("global", command))
        function = keymap_map.get("global", command);
    else
        return false;

    vector<string> sfuncs = split(function);
    for(auto sfunc : sfuncs) {
        sfunc = trim(sfunc);
        if(!keymap::execute(sfunc))
            break;
    }

    return true;
}

bool keymap::execute(string function) {
    if(function.empty())
        return true;
    if(startsWith(function, "mode:")) {
        mode = function.substr(5) + "-mode";
        return true;
    }
    if(startsWith(function, "insert:")) {
        vector<int> codes = mapkeys(function.substr(7));
        for(int code : codes)
            command += mapkey(code);
        return true;
    }

    if(!keymap_controller || !keymap_view) {
        cerr << "keymap::execute: controller or view undefined" << endl;
        return false;
    }
    if(contains(customFunction_map, function)) {
        vector<string> sfuncs = split(customFunction_map[function]);
        for(auto sfunc : sfuncs) {
            sfunc = trim(sfunc);
            if(!keymap::execute(sfunc))
                break;
        }
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
    command += mapkey(keycode);
    // TODO: this needs to go by tokens
    // TODO: we'll probably store an vector<int> and convert as needed
    for(int i = 1; i <= command.length(); ++i) {
        string subcommand = command.substr(0, i);
        if(tryExecute(subcommand)) {
            if(command.empty())
                break;
            command = command.substr(i);
            i = 1;
        }
    }
    if(keymap_map.has(mode, "default")) {
        string defaction = keymap_map.get(mode, "default");
        if(defaction == "none") {
            ;// continue
        }
        if(defaction == "insert") {
            for(int keyc : command)
                keymap_view->getCursor()->insert((char)keyc);
            keymap_view->repaint();
            command.clear();
            return;
        }
    }
    // TODO: proper way to do this?
    if(keymap_map.has("global.default") &&
            keymap_map.get("global.default") == "check-last") {
        string last = mapkey(keycode);
        if(tryExecute(last, true)) {
            // TODO: this really makes things clash :)
            if(command.length() > last.length())
                command = command.substr(0, command.length() - last.length());
        }
    }
    return;
}

string keymap::getMode() {
    return mode;
}
string keymap::getCommand() {
    return command;
}

// TODO: better way to handle this?
int keymap::mapkey(string key) {
    if(key == (string)"<escape>")
        return Key::Escape;
    if(key == (string)"<space>")
        return Key::Space;
    if(key == (string)"<bspace>")
        return Key::Backspace;
    if(key == (string)"<enter>")
        return Key::Enter;
    if(key == (string)"<C-b>")
        return Key::CtrlB;
    if(key == (string)"<C-f>")
        return Key::CtrlF;
    if(key == (string)"<pageup>")
        return Key::PageUp;
    if(key == (string)"<pagedown>")
        return Key::PageDown;

    if(key == (string)"<left>")
        return Key::Left;
    if(key == (string)"<right>")
        return Key::Right;
    if(key == (string)"<up>")
        return Key::Up;
    if(key == (string)"<down>")
        return Key::Down;

    return key[0];
}
string keymap::mapkey(int key) {
    switch((unsigned)key) {
        case Key::CtrlB:
            return "<C-b>";
        case Key::CtrlF:
            return "<C-f>";
        case Key::Escape:
            return "<escape>";
        case Key::Backspace:
            return "<bspace>";
        case Key::PageDown:
            return "<pagedown>";
        case Key::PageUp:
            return "<pageup>";

        case Key::Left:
            return "<left>";
        case Key::Right:
            return "<right>";
        case Key::Up:
            return "<up>";
        case Key::Down:
            return "<down>";
        //case '\n':
            //return "<enter>";
    }
    return (string)"" + (char)key;
}

vector<int> keymap::mapkeys(string keys) {
    vector<int> res;
    size_t s = keys.find("<");
    size_t e = keys.find(">");
    while((s != string::npos) && (e != string::npos)) {
        if(s > 0) {
            vector<int> startmap = mapkeys(keys.substr(0, s));
            res.insert(res.end(), startmap.begin(), startmap.end());
        }
        string end = keys.substr(e + 1);
        res.push_back(mapkey(keys.substr(s, e - s + 1)));
        keys = end;

        // refind < and >
        s = keys.find("<");
        e = keys.find(">");
    }
    for(char c : keys)
        res.push_back(c);
    return res;
}

// vim:ts=4 et sw=4 sts=4
