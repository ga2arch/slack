#include "Chat.hpp"

void Chat::draw(Session& current_session) {
    int j = 0;
    int i = current_session.messages.size() - 1;

    if (current_session.messages.empty()) {
        return;
    }

    do {
        mvwprintw(win, current_session.chat_line + 1 - current_session.delta, 1,
                  "%s", current_session.messages[i].content.substr(j, COLS - 24).c_str());
        current_session.chat_line++;

        if ( current_session.chat_line > LINES - 7) {
            current_session.delta++;
            wborder(win, ' ', ' ', ' ',
                    ' ', ' ', ' ', ' ', ' ');
            wscrl(win, 1);
            draw_borders();
        }

        j += COLS - 24;
    } while (j <  current_session.messages[i].content.size());

    wrefresh(win);
}

void Chat::draw_all(const Session& current_session) {
    wclear(win);

    for (int i = current_session.delta; i < current_session.messages.size(); i++) {
        mvwprintw(win, i + 1 - current_session.delta, 1, "%s", current_session.messages[i].content.c_str());
    }
    draw_borders();
}

void Chat::resize_win(int y, int x,
                      int start_y, int start_x,
                      Session& current_session) {
    Window::resize_win(y, x, start_y, start_x);


    if (current_session.messages.empty()) {
        return;
    }

    if (current_session.chat_line < LINES - 7) {
        current_session.delta = 0;
    } else {
        current_session.delta = current_session.chat_line - (LINES - 7);
    }

    int j, line = 0;
    for (int i = current_session.delta; line < current_session.chat_line; i++) {
        j = 0;
        do {
            mvwprintw(win, i - current_session.delta + 1 + (j % (COLS - 24)), 1, "%s", current_session.messages[i].content.substr(j, COLS - 24).c_str());
            j += COLS - 24;
            line++;
        } while (j < current_session.messages[i].content.size());
    }
    wrefresh(win);
}

void Chat::print_starting_message() {
    const std::string hello_mesg = "Welcome to slack++";
    const std::string start_mesg = "Please select an user to chat with using arrows (up/down) and enter.";

    wattron(win, A_BOLD);
    mvwprintw(win, (LINES - 6) / 2 - 1, (COLS - 24 - hello_mesg.length()) / 2, hello_mesg.c_str());
    mvwprintw(win, (LINES - 6) / 2 + 1, (COLS - 24 - start_mesg.length()) / 2, start_mesg.c_str());
    wrefresh(win);
    wattroff(win, A_BOLD);

}