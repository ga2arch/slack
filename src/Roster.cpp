#include "Roster.hpp"

void Roster::draw() {
    for (int i=0; i < users.size(); i++) {
        mvwprintw(win, i+2, 1, "%.*s", 20, users[i].c_str());
    }

    wrefresh(win);
}

void Roster::add_user(const std::string& user) {
    users.emplace_back(user);
}