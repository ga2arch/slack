#include "Input.hpp"

Input::Input(int y, int x,
             int start_y, int start_x,
             const std::string &title,
             SlackClient *cl): Window(y, x, start_y, start_x, title), client(cl) {

    scrollok(win, TRUE);
    idlok(win, TRUE);
    noecho();
    keypad(win, TRUE);
}

int Input::wait() {
    const int KEY_ESC = 27;
    const int KEY_BS = 127;

    int c;

    mvwprintw(win, 1, 1, ">> ");
    wmove(win, 1, input_str.length() + 4);

    do {
        draw();

        c = wgetch(win);
        switch (c) {
            case KEY_RESIZE:
            case KEY_ESC: // ESC or resize event;
                return c;

            case KEY_BS:
                if (input_str.length() > 0) input_str.pop_back();
                break;

            default:
                if (isprint(c)) {
                    input_str.push_back(c);

                    if ((input_str.length() + 1) % (COLS - 26) == 0) { // we just support 2 lines input for now. Properly add a scroll function here.
                        wmove(win, 2, 1);
                    }
                }
                break;
        }
    } while (c != 10 && input_str.length() < 4000);

    client->send_message(input_str);

    input_str.clear();

    wclear(win);
    draw_borders();
    return 0;
}

void Input::draw() {
    wclear(win);
    draw_borders();

    mvwprintw(win, 1, 1, (">> " + input_str).c_str());
}

void Input::resize_win(int y, int x, int start_y, int start_x) {
    Window::resize_win(y, x, start_y, start_x);

    mvwprintw(win, 1, 4, input_str.c_str());
}