#include "controller.hpp"

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

Controller::Controller(View *iView) :
        view(iView),
        window(iView->getWindow()),
        cursor(iView->getCursor()),
        state(State::Command) {
}

void Controller::run() {
    this->view->view();

    this->cursor->move(0, 0);
    string line(this->window->getWidth(), ' ');
    bool done = false;
    while(!done) {
        int i = this->window->getKey();
        switch(this->state) {
            case State::Command:
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

                    case 'i':
                        this->state = State::Insert;
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

                    case 'q':
                        done = true;
                        break;

                    default:
                        break;
                }
                break;
            case State::Insert:
                switch(i) {
                    case Key::Down:
                        this->cursor->down();
                        break;

                    case Key::Up:
                        this->cursor->up();
                        break;

                    case Key::Left:
                        this->cursor->left();
                        break;

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
                        this->state = State::Command;
                        break;

                    case Key::Backspace:
                        this->cursor->backspace();
                        this->view->repaint();
                        break;

                    default:
                        if((i >= ' ' && i <= 126) || (i == '\n')) {
                            this->cursor->insert((char)i);
                            this->view->repaint();
                        }
                        break;
                }
                break;
            default:
                done = true;
                break;
        }
        if(!this->view->checkSanity())
            this->view->repaint();

        stringstream ss; ss << ": k" << i
            << " " << "(" << this->cursor->getCol()
            << ", " << this->cursor->getRow() << ") -- %"
            << this->cursor->getRow() * 100 /
                // TODO ugly hack to get number of lines
                this->view->getBuffer()->getData().size() << " ";
        ss << " " << ((this->state == State::Insert) ? "Insert" : "Command");
        this->window->write(this->window->getHeight() - 1, ss.str());
        this->window->update(this->cursor->getCol() - this->view->getStartX(),
                this->cursor->getRow() - this->view->getStartY());
    }
}

void Controller::operator()() {
    this->run();
}

// vim:ts=4 et sw=4 sts=4
