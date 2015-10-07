#include "Chat.hpp"

void Chat::draw() {
    int j = 0;
    int i = messages.size() - 1;

    do {
        mvwprintw(win, chat_line + 2, 1, "%s", messages[i].substr(j, COLS - 24).c_str());
        chat_line++;
        j += COLS - 24;
    } while (j < messages[i].size());

    wrefresh(win);
}

void Chat::add_message(const std::string& message) {
    messages.emplace_back(message);
    draw();
}
