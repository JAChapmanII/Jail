#include "controller.hpp"
using std::string;

#include <sstream>
using std::stringstream;

#include <unistd.h>

#include "keymap.hpp"

Controller::Controller(View *iView) :
        view(iView),
        window(iView->getWindow()),
        cursor(iView->getCursor()),
        done(false) {
}

void Controller::run() {
    keymap::keymap_controller = this;
    keymap::keymap_view = this->view;
    this->view->view();

    this->cursor->move(0, 0);
    string line(this->window->getWidth(), ' ');
    string command;
    while(!this->done) {
        int i = this->window->getKey();

        keymap::push_execute(i);

        if(!this->view->checkSanity())
            this->view->repaint();

        stringstream ss; ss << this->getModeline() << " -- :" << command;
        this->writeModeline(ss.str());
    }
}

void Controller::stop() {
    this->done = true;
}

void Controller::write() {
    if(this->view->getBuffer()->isReadOnly()) {
        stringstream ss; ss << this->getModeline();
        ss << " -- Buffer is read only";
        this->writeModeline(ss.str());
    } else {
        int saved = this->view->getBuffer()->save();
        stringstream ss; ss << this->getModeline();
        ss << " -- ";
        if(saved < 0)
            ss << "Failed to save.";
        else
            ss << "Saved " << saved << " bytes to file";
        this->writeModeline(ss.str());
    }
    sleep(1);
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
