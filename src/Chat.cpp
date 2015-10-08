#include "Chat.hpp"

void Chat::draw(const std::vector<Message>& messages) {
    int j = 0;
    int i = messages.size() - 1;

    if (messages.empty()) {
        return;
    }
    
    do {
        mvwprintw(win, chat_line + 1 - delta, 1,
                  "%s", messages[i].content.substr(j, COLS - 24).c_str());
        chat_line++;
        
        if (chat_line > LINES - 7) {
            delta++;
            wborder(win, ' ', ' ', ' ',
                    ' ', ' ', ' ', ' ', ' ');
            wscrl(win, 1);
            draw_borders();
        }
        
        j += COLS - 24;
    } while (j < messages[i].content.size());

    wrefresh(win);
}

void Chat::draw_all(const std::vector<Message>& messages) {
    wclear(win);
    for (int i = 0; (i < messages.size()) && (i < LINES - 6); i++) {
        mvwprintw(win, i + 1, 1, "%s", messages[i].content.c_str());
    }
    draw_borders();
}

void Chat::resize_win(int y, int x,
                      int start_y, int start_x,
                      const std::vector<Message>& messages) {
    Window::resize_win(y, x, start_y, start_x);

    if (messages.empty()) {
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
            mvwprintw(win, i - delta + 1 + (j % (COLS - 24)), 1, "%s", messages[i].content.substr(j, COLS - 24).c_str());
            j += COLS - 24;
            line++;
        } while (j < messages[i].content.size());
    }
    wrefresh(win);
}