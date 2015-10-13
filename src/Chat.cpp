#include "Chat.hpp"

void Chat::draw(Session& current_session) {
    if (current_session.messages.empty()) {
        return;
    }

    int line = current_session.chat_line;
    auto content = current_session.messages.back().content;
    
    if (current_session.delta > 0) {
        wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        int size = content.size();
        while (size > 0) {
            wscrl(win, 1);
            size--;
        }
        draw_borders();
    }

    for (auto it = content.cbegin(); it != content.cend(); ++it, line--) {
        mvwprintw(win, line  - current_session.delta, 1, "%s", (*it).c_str());
    }

    wrefresh(win);
}

void Chat::chat_context_switch(const Session& current_session) {
    wclear(win);

    int line = current_session.chat_line;
    int i = current_session.messages.size() - 1;

    for (; i >= current_session.delta; i--) {
        auto m = current_session.messages[i].content;
        for (auto it = m.rbegin(); it != m.rend(); ++it, line--) {
            mvwprintw(win, line  - current_session.delta, 1, "%s", (*it).c_str());
        }
    }
    draw_borders();
}

void Chat::print_starting_message(const std::string& mesg) {
    const std::string hello_mesg = "Welcome to slack++";
    
    wclear(win);
    wattron(win, A_BOLD);
    mvwprintw(win, (LINES - 6) / 2 - 1, (COLS - 24 - hello_mesg.length()) / 2, hello_mesg.c_str());
    mvwprintw(win, (LINES - 6) / 2 + 1, (COLS - 24 - mesg.length()) / 2, mesg.c_str());
    wattroff(win, A_BOLD);
    draw_borders();
}