#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <string>
#include "controller.hpp"
#include "view.hpp"

namespace keymap {
    extern Controller *keymap_controller;
    extern View *keymap_view;

    void init();
    void load(std::string file);
    bool execute(std::string function);
    void push_execute(int keycode);
}

#endif // KEYMAP_HPP
// vim:ts=4 et sw=4 sts=4
