#include "Chat.hpp"

void Chat::draw(Session& sess, int num_lines, int start, int scroll) {
    // if current session is empty
    if (sess.messages.size() == 0) {
        print_starting_message("This is the beginning of your chat.");
        return;
    }
    
    start = std::min(start, (int)sess.messages.size() - 1);
    
    if (scroll) {
        wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
        wscrl(win, scroll);
    }
    
    for (int i = 0; i < abs(num_lines) && i < sess.messages.size(); i++, start++) {
        int dim = sess.delta - sess.scrolled_back + start;
        bool sender = sess.messages.at(dim).sender;
        auto m = sess.messages.at(dim).content;
        if (sender) {
            wattron(win, A_BOLD);
        }
        mvwprintw(win, start + 1, 1, m.c_str());
        wattroff(win, A_BOLD);
    }
    draw_borders();
}

void Chat::chat_context_switch(Session &sess) {
    wclear(win);
    draw(sess, get_real_rows(), 0, 0);
}

void Chat::draw_next(Session &sess) {
    int scroll = 0;
    
    if (sess.delta) {
        scroll = 1;
    }
    
    draw(sess, 1, get_real_rows() - 1, scroll);
}

void Chat::print_starting_message(const std::string& mesg) {
    const std::string hello_mesg = "Welcome to slack++";
    
    wclear(win);
    wattron(win, A_BOLD);
    mvwprintw(win, rows / 2 - 1, (cols - hello_mesg.length()) / 2, "%.*s", cols, hello_mesg.c_str());
    int starting_col = cols - mesg.length();
    if (starting_col < 0) {
        starting_col = 0;
    }
    mvwprintw(win, rows / 2 + 1, starting_col / 2, "%.*s", cols,  mesg.c_str());
    wattroff(win, A_BOLD);
    draw_borders();
}

int Chat::scroll_back(Session& sess) {
    if (sess.delta == sess.scrolled_back) {
        return -1;
    }
    
    sess.scrolled_back++;
    draw(sess, 1, 0, -1);
    return 0;
}

int Chat::scroll_forward(Session& sess) {
    if (sess.scrolled_back == 0) {
        return -1;
    }
    
    sess.scrolled_back--;
    draw(sess, 1, get_real_rows() - 1, 1);
    
    return sess.scrolled_back;
}

void Chat::set_title(std::string s) {
    title = s;
}
