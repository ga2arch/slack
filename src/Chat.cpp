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

void Chat::resize_win(int y, int x, int start_y, int start_x) {
    Window::resize_win(y, x, start_y, start_x);

    if (messages.size() == 0) {
        return;
    }

    if (chat_line < LINES - 7) {
        delta = 0;
    } else {
        delta = chat_line - (LINES - 7);
    }

    int j, line = 0;
    for (int i = delta; line < chat_line; i++) {
        j = 0;
        do {
            mvwprintw(win, i - delta + 1 + (j % (COLS - 24)), 1, "%s", messages[i].substr(j, COLS - 24).c_str());
            j += COLS - 24;
            line++;
        } while (j < messages[i].size());
    }
    wrefresh(win);
}