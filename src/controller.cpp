#include "controller.hpp"
using std::string;

#include <sstream>
using std::stringstream;

#include <unistd.h>

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
    string command;
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

                    case ':':
                        command = this->getCommand();
                        if(command == "w") {
                            int saved = this->view->getBuffer()->save();
                            stringstream ss; ss << this->getModeline();
                            ss << " -- ";
                            if(saved < 0)
                                ss << "Failed to save.";
                            else
                                ss << "Saved " << saved << " bytes to file";
                            string m = ss.str();
                            this->window->write(this->window->getHeight() - 1, m);
                            this->window->update(
                                    this->cursor->getCol() - this->view->getStartX(),
                                    this->cursor->getRow() - this->view->getStartY());
                            sleep(1);
                        }
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

        this->window->write(this->window->getHeight() - 1, this->getModeline());
        this->window->update(this->cursor->getCol() - this->view->getStartX(),
                this->cursor->getRow() - this->view->getStartY());
    }
}

string Controller::getCommand() {
    string ret;
    this->window->write(this->window->getHeight() - 1, "Command: ");
    this->window->update(this->cursor->getCol() - this->view->getStartX(),
            this->cursor->getRow() - this->view->getStartY());
    while(true) {
        int i = this->window->getKey();
        switch(i) {
            case Key::Escape:
                return "";
            case '\n':
                return ret;
            default:
                if(i >= ' ' && i <= 126)
                    ret += (char)i;
        }
        this->window->write(this->window->getHeight() - 1, ("Command: " + ret));
        this->window->update(this->cursor->getCol() - this->view->getStartX(),
                this->cursor->getRow() - this->view->getStartY());
    }
}

string Controller::getModeline() {
    stringstream ss; 
    ss << ": (" << this->cursor->getCol()
        << ", " << this->cursor->getRow() << ") -- %"
        << this->cursor->getRow() * 100 /
            // TODO ugly hack to get number of lines
            this->view->getBuffer()->getData().size() << " ";
    ss << " " << ((this->state == State::Insert) ? "Insert" : "Command");
    return ss.str();
}

void Controller::operator()() {
    this->run();
}

// vim:ts=4 et sw=4 sts=4
