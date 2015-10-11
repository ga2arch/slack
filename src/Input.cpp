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

int Input::wait(std::string& input_str, int& line, int& col) {
    const int KEY_ESC = 27;
    const int KEY_BS = 127;
    const int KEY_TAB = 9;
    int c;

    wmove(win, line, col);
    do {
        c = wgetch(win);
        switch (c) {
            case KEY_ESC: // ESC or tab event;
            case KEY_TAB:
                return c;

            case KEY_BS:
                if (!input_str.empty()) {
                    input_str.pop_back();
                    col--;
                    if (col == 0) {
                        col = COLS - 24;
                        if (line == 1) {
                            wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
                            wscrl(win, -1);
                            mvwprintw(win, line, 1, "%s", input_str.c_str());
                            draw_borders();
                        } else {
                            line--;
                        }
                    }
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

void Input::input_context_switch(const Session& current_session) {
    wclear(win);
    int line = current_session.line;
    int col = (current_session.col - 1);
    int i = current_session.input_str.length() - 1;
    if (col == 0) {
        col = COLS - 24;
        line--;
    }

    do {
        mvwprintw(win, line, col, "%c", current_session.input_str[i]);
        i--;
        col--;
        if (col == 0) {
            col = COLS - 24;
            line--;
        }
    } while (i >= 0 && line >= 0);
    draw_borders();
}