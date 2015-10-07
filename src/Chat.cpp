#include "Chat.hpp"

void Chat::draw() {
    for (int i=0; i < messages.size(); i++) {
        mvwprintw(win, i+2, 1, "%s", messages[i].c_str());
    }

    wrefresh(win);
}

void Chat::add_message(const std::string& message) {
    messages.emplace_back(message);
}
