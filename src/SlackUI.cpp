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

    for (;;) {
        input->wait();
    }
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