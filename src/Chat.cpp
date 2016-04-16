#include "Chat.hpp"

void Chat::draw(Session& sess, int num_lines) {
    // if current session is empty
    if (sess.messages.front().content == "\0") {
        print_starting_message("Nothing to show here.");
        return;
    }
        
    int line = sess.chat_line - sess.delta;
    int size = sess.last_mess - sess.scrolled_back;
    
    // if we're changing session (thus num_lines == LINES -6)
    // or this is our first printed message in this chat (to remove "nothig to show here" mesg)
    if (num_lines == LINES - 6 || sess.chat_line == num_lines) {
        wclear(win);
    } else  if (sess.delta) {
        wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wscrl(win, num_lines);
    }
    // i will hold the number of lines to be printed
    int i = abs(num_lines);
    // we're scrolling back
    if (num_lines < 0) {
        num_lines = i * line;
    }
    while (i > 0) {
        // dim needed because messages is a circular array of 1000 elements
        int dim = (size - num_lines + 1000) % 1000;
        bool sender = sess.messages.at(dim).sender;
        auto m = sess.messages.at(dim).content;
        if (sender) {
            wattron(win, A_BOLD);
        }
        mvwprintw(win, line - num_lines + 1, 1, m.c_str());
        wattroff(win, A_BOLD);
        i--;
        num_lines--;
    }
    draw_borders();
}

void Chat::print_starting_message(const std::string& mesg) {
    const std::string hello_mesg = "Welcome to slack++";
    
    wclear(win);
    wattron(win, A_BOLD);
    mvwprintw(win, (LINES - 6) / 2 - 1, (COLS - 24 - hello_mesg.length()) / 2, "%.*s", COLS - 24, hello_mesg.c_str());
    mvwprintw(win, (LINES - 6) / 2 + 1, (COLS - 24 - mesg.length()) / 2, "%.*s", COLS - 24,  mesg.c_str());
    wattroff(win, A_BOLD);
    draw_borders();
}

void Chat::scroll_back(Session& sess) {
    if (sess.scrolled_back == sess.delta) {
        return;
    }
    
    sess.scrolled_back++;
    draw(sess, -1);
}

int Chat::scroll_forward(Session& sess) {
    if (sess.scrolled_back == 0) {
        return -1;
    }

    sess.scrolled_back--;
    draw(sess, 1);
    
    return sess.scrolled_back;
}
