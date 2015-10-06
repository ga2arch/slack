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

    create_roster();
    create_chat();
    create_input();

    draw_roster();
    draw_chat();

    for (;;) {
        wait_input();
    }
}

void SlackUI::add_message(const std::string& message) {
    messages.emplace_back(message);
    draw_chat();
}

void SlackUI::add_user(const std::string& user) {
    users.emplace_back(user);
    draw_roster();
}

SlackUI::~SlackUI() {
    delwin(roster);
    delwin(chat);
    delwin(input);
    delwin(stdscr);

    endwin();
}

void SlackUI::setup_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

void SlackUI::create_roster() {
    roster = newwin(max_y, 22, 0, 0);
    draw_border(roster);
}

void SlackUI::create_chat() {
    chat = newwin(max_y-4, max_x-22, 0, 22);
    draw_border(chat);
}

void SlackUI::create_input() {
    input = newwin(4, max_x-22, max_y-4, 22);
    draw_border(input);
}

void SlackUI::draw_border(WINDOW *win) {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
}

void SlackUI::draw_roster() {
    draw_lock.lock();

    for (int i=0; i < users.size(); i++) {
        mvwprintw(roster, i+2, 1, "%.*s", 20, users[i].c_str());
    }

    wrefresh(roster);

    draw_lock.unlock();
}

void SlackUI::draw_chat() {
    draw_lock.lock();

    for (int i=0; i < messages.size(); i++) {
        mvwprintw(chat, i+2, 1, "%s", messages[i].c_str());
    }

    wrefresh(chat);

    draw_lock.unlock();
}

void SlackUI::wait_input() {
    char str[10000];

    wmove(input, 1, 2);
    echo();
    wgetstr(input, str);
    wclear(input);
    draw_border(input);
    noecho();
    add_message(str);
    client->send_message(str);
}