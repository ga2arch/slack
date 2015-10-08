#include "Chat.hpp"

void Chat::draw() {
    int j = 0;
    int i = messages.size() - 1;

    if (i == -1) {
        return;
    }
    do {
        mvwprintw(win, chat_line + 1 - delta, 1, "%s", messages[i].substr(j, COLS - 24).c_str());
        chat_line++;
        if (chat_line > LINES - 7) {
            delta++;
            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
            wscrl(win, 1);
            draw_borders();
        }
        j += COLS - 24;
    } while (j < messages[i].size());

    wrefresh(win);
}

void Chat::add_message(const std::string& message) {
    messages.emplace_back(message);
    draw();
}
