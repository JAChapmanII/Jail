#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <string>
#include "controller.hpp"
#include "view.hpp"

namespace keymap {
    Controller *keymap_controller;
    View *keymap_view;

    void init();
    void load(std::string file);
    void push_execute(int keycode);
}

#endif // KEYMAP_HPP
// vim:ts=4 et sw=4 sts=4
