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
    getmaxyx(stdscr, max_y, max_x);

    roster = std::make_unique<Roster>(max_y, 22, 0, 0);
    chat   = std::make_unique<Chat>(max_y-4, max_x-22, 0, 22);
    input  = std::make_unique<Input>(4, max_x-22, max_y-4, 22, client);

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