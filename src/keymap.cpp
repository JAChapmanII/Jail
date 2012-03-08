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

#include "datamap.hpp"

static DataMap keymap_map;
static map<string, function<void(Controller &, View &)>> function_map;
static vector<string> modes;
static string mode;
static vector<int> command;
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
            controller.getCommand();
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
    modes.push_back("");
    if(keymap_map.has("core.modes")) {
        vector<string> moreModes = split(keymap_map.get("core.modes"), ",");
        for(auto nmode : moreModes)
            if(!nmode.empty())
                modes.push_back(nmode);
    }
    cerr << "mode count: " << modes.size() << endl;
}

static void execute(vector<int> com) {
    cerr << "executing: " << join(com) << endl;
}

void keymap::push_execute(int keycode) {
    command.push_back(keycode);
    // TODO: convert command into string?
    /*
    if(keymap_map.has(mode, command)) {
        execute(mode + "." + command);
        command = "";
        return;
    }
    if(keymap_map.has("global", command)) {
        execute("global." + command);
        command = "";
        return;
    }
    */
    if(keymap_map.has(mode, "default")) {
        string defaction = keymap_map.get(mode, "default");
        if(defaction == "none") {
            // TODO: don't hard-code this number
            if(keycode == 127)
                command.resize(command.size() - 1);
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
