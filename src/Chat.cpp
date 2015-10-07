#include "Chat.hpp"

void Chat::draw() {
    int j, l;

    for (int i=0, l = 0; i < messages.size(); i++) {
        j = 0;
        do {
            mvwprintw(win, l + 2, 1, "%s", messages[i].substr(j, COLS - 24).c_str());
            l++;
            j += COLS - 24;
        } while (j < messages[i].size());
    }

    wrefresh(win);
}

void Chat::add_message(const std::string& message) {
    messages.emplace_back(message);
}
