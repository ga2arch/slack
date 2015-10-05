//
//  SlackUI.cpp
//  slack++
//
//  Created by Gabriele Carrettoni on 04/10/15.
//
//

#include "SlackUI.hpp"

void SlackUI::show() {
    setup_ncurses();
    getmaxyx(stdscr, max_y, max_x);

    create_roster();
    create_chat();
    create_input();

    for (;;) {
        draw_roster();
        draw_chat();
        draw_input();

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
}

void SlackUI::create_chat() {
    chat = newwin(max_y-4, max_x-22, 0, 22);
}

void SlackUI::create_input() {
    input = newwin(4, max_x-22, max_y-4, 22);
}

void SlackUI::draw_roster() {
    draw_lock.lock();

    wclear(roster);
    wrefresh(roster);

    auto x = 2;

    for (int i=0; i < users.size(); i++) {
        mvwprintw(roster, i+2, x, "%.*s", 20, users[i].c_str());
    }

    wborder(roster, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(roster);

    draw_lock.unlock();
}

void SlackUI::draw_chat() {
    draw_lock.lock();

    wclear(chat);
    wrefresh(chat);

    auto x = 3;

    for (int i=0; i < messages.size(); i++) {
        mvwprintw(roster, i+2, x, "%s", messages[i].c_str());
    }

    wborder(chat, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(chat);

    draw_lock.unlock();
}

void SlackUI::draw_input() {
    draw_lock.lock();

    wclear(input);
    wrefresh(input);

    wborder(input, 0, 0, 0, 0, 0, 0, 0, 0);
    wmove(input, 1, 2);

    echo();
    wrefresh(input);

    draw_lock.unlock();
}

void SlackUI::wait_input() {
    char str[10000];
    wgetstr(input, str);

    messages.emplace_back(str);
}
