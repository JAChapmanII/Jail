#include "view.hpp"

View::View(Buffer *buf) : // TODO: once again, styling on init lists
        buffer(buf) {
}

void View::view() const {
}

void View::operator()() const {
    this->view();
}

// vim:ts=4 et sw=4 sts=4
