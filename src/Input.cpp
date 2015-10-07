#include "Input.hpp"

void Input::wait() {
    char str[10000];

    mvwprintw(win, 1, 1, ">> ");
    echo();
    wgetstr(win, str);

    client->send_message(str);

    wclear(win);
    draw_borders();
    noecho();
}