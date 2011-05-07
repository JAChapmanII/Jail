#include "controller.hpp"

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

Controller::Controller(View *iView) :
        view(iView),
        window(iView->getWindow()),
        cursor(iView->getCursor()) {
}

void Controller::run() {
    this->view->view();

    this->cursor->move(0, 0);
    string line(this->window->getWidth(), ' ');
    bool done = false;
    while(!done) {
        int i = this->window->getKey();
        switch(i) {
            case 'j':
            case Key::Down:
                this->cursor->down();
                break;

            case 'k':
            case Key::Up:
                this->cursor->up();
                break;

            case 'h':
            case Key::Left:
                this->cursor->left();
                break;

            case 'l':
            case Key::Right:
                this->cursor->right();
                break;

            case Key::PageUp:
            case Key::CtrlB:
                this->cursor->move(
                        this->cursor->getRow() - this->window->getHeight() + 1,
                        this->cursor->getCol());
                break;

            case Key::PageDown:
            case Key::CtrlF:
                this->cursor->move(
                        this->cursor->getRow() + this->window->getHeight() - 1,
                        this->cursor->getCol());
                break;

            case Key::Escape:
                done = true;
                break;

            default:
                break;
        }
        if(!this->view->checkSanity())
            this->view->repaint();

        stringstream ss; ss << ": k" << i;
        this->window->write(this->window->getHeight() - 1, ss.str());
        this->window->update(this->cursor->getCol() - this->view->getStartX(),
                this->cursor->getRow() - this->view->getStartY());
    }
}

void Controller::operator()() {
    this->run();
}

// vim:ts=4 et sw=4 sts=4
