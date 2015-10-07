#include "Input.hpp"

Input::Input(int y, int x, int start_y, start_x, SlackClient *cl) {
    newwin(y, x, start_y, start_x);
    draw_borders();
    client = cl;
};

void Input::wait() {
    char str[10000];

    mvwprintw(win, 1, 1, ">> ");
    echo();
    wgetstr(win, str);

    cl->send_message(str);

    wclear(win);
    draw_border(win);
    noecho();
}