//
//  SlackUI.cpp
//  slack++
//
//  Created by Gabriele Carrettoni on 04/10/15.
//
//

#include "SlackUI.hpp"

void SlackUI::set_client(SlackClient* client) {
    this->client = client;
}

void SlackUI::show() {
    setup_ncurses();

    roster = std::make_unique<Roster>(LINES, 22, 0, 0);
    chat   = std::make_unique<Chat>(LINES-4, COLS-22, 0, 22);
    input  = std::make_unique<Input>(4, COLS-22, LINES-4, 22, client);

    roster->draw();
    chat->draw();

    for (;;) {
        input->wait();
    }
}

void SlackUI::add_message(const std::string& message) {
    chat->add_message(message);

    draw_lock.lock();
    chat->draw();
    draw_lock.unlock();
}

void SlackUI::add_user(const std::string& user) {
    roster->add_user(user);
}

void SlackUI::refresh_user() {
    draw_lock.lock();
    roster->draw();
    draw_lock.unlock();
}

SlackUI::~SlackUI() {
    delwin(stdscr);
    endwin();
}

void SlackUI::setup_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
}