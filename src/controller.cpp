#include "controller.hpp"
using std::string;

#include <sstream>
using std::stringstream;

#include "keymap.hpp"

Controller::Controller(View *iView) :
        view(iView),
        window(iView->getWindow()),
        cursor(iView->getCursor()),
        done(false),
        message(""),
        messageLeft(0) {
}

void Controller::run() {
    keymap::keymap_controller = this;
    keymap::keymap_view = this->view;
    this->view->view();

    this->cursor->move(0, 0);
    string line(this->window->getWidth(), ' ');
    while(!this->done) {
        if(!this->view->checkSanity())
            this->view->repaint();

        this->writeModeline(this->getModeline());
        if(this->messageLeft)
            this->messageLeft--;

        int i = this->window->getKey();
        keymap::push_execute(i);

    }
}

void Controller::stop() {
    this->done = true;
}

void Controller::write() {
    if(this->view->getBuffer()->isReadOnly()) {
        this->message = "Buffer is read only";
    } else {
        int saved = this->view->getBuffer()->save();
        if(saved < 0)
            this->message = "Failed to save.";
        else {
            stringstream ss;
            ss << saved;
            this->message = "Saved " + ss.str() + " bytes to file";
        }
    }
    this->messageLeft = 30;
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
            case Key::Backspace:
                if(ret.empty())
                    return "";
                else
                    ret.resize(ret.length() - 1);
                break;
            case '\n':
                return ret;
            default:
                if(i >= ' ' && i <= 126)
                    ret += (char)i;
        }
        this->writeModeline("Command: " + ret);
    }
}

string Controller::getModeline() {
    double ruler = 100;
    if(this->view->getBuffer()->getSize() > 0)
        ruler = (this->cursor->getRow() + 1) * 100 / this->view->getBuffer()->getSize();
    stringstream ss; 
    ss << ": (" << this->cursor->getCol()
        << ", " << this->cursor->getRow() << ") -- %" << ruler << " ";
    ss << " " << keymap::getMode();
    ss << " :" << keymap::asString(keymap::getCommand());
    if((this->messageLeft > 0) && !this->message.empty())
        ss << " -- " << this->message;
    return ss.str();
}

void Controller::writeModeline(string mline) {
    this->window->write(this->window->getHeight() - 1, mline);
    this->window->update(this->cursor->getCol() - this->view->getStartX(), 
            this->cursor->getRow() - this->view->getStartY());
}

void Controller::operator()() {
    this->run();
}

// vim:ts=4 et sw=4 sts=4
