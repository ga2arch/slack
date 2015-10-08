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
    int c;

    setup_ncurses();

    roster = std::make_unique<Roster>(LINES, 22, 0, 0, "Roster");
    chat   = std::make_unique<Chat>(LINES-4, COLS-22, 0, 22, "Chat");
    input  = std::make_unique<Input>(4, COLS-22, LINES-4, 22, "InputBox", client);

    do {
        c = input->wait();
        if (c == KEY_RESIZE) {
            resize();
        }
        if (c == 9) {
            roster->wait();
            chat->draw_all(roster->get_messages());
        }
    } while (c != 27);
}

SlackUI::~SlackUI() {
    endwin();
    delwin(stdscr);
}

void SlackUI::setup_ncurses() {
    initscr();
    cbreak();
    noecho();
    ESCDELAY = 25;
}

void SlackUI::resize() {
    endwin();
    roster->resize_win(LINES, 22, 0, 0);
    chat->resize_win(LINES-4, COLS-22, 0, 22, roster->get_messages());
    input->resize_win(4, COLS-22, LINES-4, 22);
}