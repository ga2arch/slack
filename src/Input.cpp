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
    const int KEY_TAB = 9;
    int c;

    wmove(win, line, col);
    do {
        c = wgetch(win);
        switch (c) {
            case KEY_ESC: // ESC or resize event;
            case KEY_TAB:
                return c;

            case KEY_BS:
                if (input_str.length() > 0) {
                    input_str.pop_back();
                    col--;
                    mvwprintw(win, line, col, " ");
                    wmove(win, line, col);
                }
                break;

            default:
                if (isprint(c)) {
                    input_str.push_back(c);

                    mvwprintw(win, line, col, "%c", c);
                    if (col ==  COLS - 24) {
                        col = 1;
                        if (line == 2) {
                            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
                            wscrl(win, 1);
                            draw_borders();
                        } else {
                            line++;
                        }
                    } else {
                        col++;
                    }

                }
                break;
        }
    } while (c != 10 && input_str.length() < 4000);

    col = 1;
    line = 1;

    client->send_message(input_str);

    input_str.clear();

    wclear(win);
    draw_borders();
    return 0;
}