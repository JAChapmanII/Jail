#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <string>
#include <vector>
#include "controller.hpp"
#include "view.hpp"

// TODO: turn this into a real class so we can have more than one?
namespace keymap {
    extern Controller *keymap_controller;
    extern View *keymap_view;

    void init();
    void load(std::string file);
    bool execute(std::string function);
    void push_execute(int keycode);

    std::string getMode();
    std::string getCommand();

    int mapkey(std::string key);
    std::string mapkey(int key);

    std::vector<int> mapkeys(std::string keys);
}

#endif // KEYMAP_HPP
// vim:ts=4 et sw=4 sts=4
