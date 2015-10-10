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

    chat->print_starting_message();
    c = roster->wait();
    if (c != 27) {
        chat->chat_context_switch(get_session());
    }
    while (c != 27) {
        c = input->wait();
        if (c == 9) {
            c = roster->wait();
            if (c != 27) {
                chat->chat_context_switch(get_session());
            }
        }
    }
}

Session& SlackUI::get_session() {
    return sessions[roster->get_active_channel()];
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
    curs_set(0);
}

void SlackUI::add_message(const RosterItem& item,
                          const std::string& content) {
    sessions[item.channel].messages.emplace_back(item, content);
}