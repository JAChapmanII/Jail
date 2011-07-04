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
                if(!command.empty()) {
                    switch(i) {
                        case Key::Escape:
                            command = "";
                            break;

                        case Key::Backspace:
                            command.resize(command.length() - 1);
                            break;

                        default:
                            if(i >= ' ' && i <= '~')
                                command += (char)i;
                            if(i == '\n')
                                command = "";
                            // execute command if it is one
                            if(command == (string)"dd") {
                                this->cursor->deleteLine();
                                this->view->checkSanity();
                                this->view->repaint();
                                command = "";
                                break;
                            }
                            if(command == (string)"gg") {
                                this->cursor->toBeginningOfBuffer();
                                command = "";
                                break;
                            }
                            if(command == (string)"ZZ") {
                                if(this->view->getBuffer()->isReadOnly()) {
                                    stringstream ss; ss << this->getModeline();
                                    ss << " -- Buffer is read only";
                                    this->window->write(
                                            this->window->getHeight() - 1, ss.str());
                                    this->window->update(
                                            this->cursor->getCol() - this->view->getStartX(),
                                            this->cursor->getRow() - this->view->getStartY());
                                    sleep(1);
                                    break;
                                }
                                int saved = this->view->getBuffer()->save();
                                if(saved < 0) {
                                    stringstream ss; ss << this->getModeline();
                                    ss << " -- Writing the buffer failed";
                                    this->window->write(
                                            this->window->getHeight() - 1, ss.str());
                                    this->window->update(
                                            this->cursor->getCol() - this->view->getStartX(),
                                            this->cursor->getRow() - this->view->getStartY());
                                    sleep(1);
                                    break;
                                }
                                done = true;
                            }
                            break;
                    }
                    break;
                }
                switch(i) {
                    case '0':
                        this->cursor->toBeginningOfLine();
                        break;

                    case '$':
                        this->cursor->toEndOfLine();
                        break;

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

                    case 'G':
                        this->cursor->toEndOfBuffer();
                        break;

                    case 'I':
                        this->cursor->toBeginningOfLine();
                    case 'i':
                        this->state = State::Insert;
                        break;

                    case 'A':
                        this->cursor->toEndOfLine();
                    case 'a':
                        this->cursor->right();
                        this->state = State::Insert;
                        break;

                    case 'o':
                        this->cursor->toEndOfLine();
                        this->cursor->insert('\n');
                        this->state = State::Insert;
                        this->view->repaint();
                        break;

                    case 'O':
                        this->cursor->toBeginningOfLine();
                        this->cursor->insert('\n');
                        this->cursor->up();
                        this->state = State::Insert;
                        this->view->repaint();
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

                    case 'x':
                        this->cursor->right();
                        this->cursor->backspace();
                        this->view->repaint();
                        break;

                    case 'q':
                        done = true;
                        break;

                    case ':':
                    case ';':
                        command = this->getCommand();
                        if(command == "w") {
                            if(this->view->getBuffer()->isReadOnly()) {
                                stringstream ss; ss << this->getModeline();
                                ss << " -- Buffer is read only";
                                this->window->write(
                                        this->window->getHeight() - 1, ss.str());
                                this->window->update(
                                        this->cursor->getCol() - this->view->getStartX(),
                                        this->cursor->getRow() - this->view->getStartY());
                                sleep(1);
                                break;
                            }
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
                        if(i >= ' ' && i <= '~')
                            command += (char)i;
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

        stringstream ss; ss << this->getModeline() << " -- :" << command;
        this->window->write(this->window->getHeight() - 1, ss.str());
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
