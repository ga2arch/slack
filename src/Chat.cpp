#include "Chat.hpp"

void Chat::draw(Session& current_session) {
    if (current_session.messages.empty()) {
        return;
    }

    int line = current_session.chat_line;
    int size = current_session.messages.size();
    if (current_session.scroll_lines > 0) {
        wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wscrl(win, current_session.scroll_lines);
        draw_borders();
        current_session.scroll_lines = 0;
    }
    
    for (int i = 0; (i <= current_session.scrolled_back); i++) {
        auto content = current_session.messages.at(size - 1 - i).content;
        for (auto it = content.rbegin(); it != content.rend(); ++it, line--) {
            mvwprintw(win, line  - current_session.delta, 1, "%s", (*it).c_str());
        }
    }
    
    current_session.scrolled_back = 0;
    wrefresh(win);
}

void Chat::chat_context_switch(Session& current_session) {
    wclear(win);

    int line = current_session.chat_line;
    int i = current_session.messages.size() - 1;

    for (; (i >= 0) && (current_session.chat_line - line < LINES - 6) ; i--) {
        auto m = current_session.messages[i].content;
        for (auto it = m.rbegin(); it != m.rend(); ++it, line--) {
            mvwprintw(win, line  - current_session.delta, 1, "%s", (*it).c_str());
        }
    }
    current_session.scrolled_back = 0;
    current_session.scroll_lines = 0;
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

void Chat::scroll_back(Session& current_session) {
    if (current_session.delta - current_session.scrolled_back <= 0) {
        return;
    }
    
    int i = 0;
    
    current_session.scrolled_back++;
    auto content = current_session.messages.at(current_session.delta - current_session.scrolled_back).content;
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wscrl(win, - (content.size()));
    current_session.scroll_lines += content.size();
    
    for (auto it = content.rbegin(); it != content.rend(); ++it, i++) {
        mvwprintw(win, content.size()  - i, 1, "%s", (*it).c_str());
    }
    
    draw_borders();
    
}