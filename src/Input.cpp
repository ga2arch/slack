#include "Input.hpp"

Input::Input(int y, int x,
             int start_y, int start_x,
             const std::string &title,
             SlackClient *cl): Window(y, x, start_y, start_x, title), client(cl) {

    noecho();
    keypad(win, TRUE);
}

int Input::wait(std::wstring& input_str, int& col) {
    const int KEY_ESC = 27;
    const int KEY_BS = 127;
    const int KEY_TAB = 9;
    wint_t c;

    highlight(col);
    do {
        wget_wch(win, &c);
        switch (c) {
        case KEY_ESC: // ESC to quit;
        case KEY_TAB: // tab to switch to roster selection mode
        case KEY_UP:   // go back in chat history
        case KEY_DOWN:  // go forward in chat history
            return c;
            
        case KEY_BS:
            if (!input_str.empty() && col > 0) {
                col--;
                input_str.erase(col, 1);
                fixed_print_input(input_str, col);
            }
            break;
        case KEY_DC:
            if (!input_str.empty() && col < input_str.length()) {
                input_str.erase(col, 1);
                fixed_print_input(input_str, col);
            }
            break;
            
        case KEY_RIGHT:
            if (col < input_str.length()) {
                col++;
                highlight(col);
            }
            break;
        case KEY_LEFT:
            if (col > 0) {
                col--;
                highlight(col);
            }
            break;
        case 10: //enter
            break;
        
        default:
            if (iswprint(c)) {
                input_str.insert(col, 1, c);
                col++;
                fixed_print_input(input_str, col);
            }
            break;
        }
    } while (c != 10 && input_str.length() < 1000);

    col = 0;
    client->send_message(input_str);
    input_str.clear();
    wclear(win);
    draw_borders();
    return 0;
}

void Input::fixed_print_input(const std::wstring& str, const int &column) {
    int col = 1, line = 1;
    int i = 0;
    
    wclear(win);
    while (i < str.length()) {
        mvwprintw(win, line, col, "%lc", str[i]);
        i++;
        col++;
        if (col == COLS - 23) {
            col = 1;
            if (line == 2) {
                wscrl(win, 1);
            } else {
                line++;
            }
        }
    }
    draw_borders();
    highlight(column);
}

void Input::input_context_switch(const Session& sess) {
    fixed_print_input(sess.input_str,  sess.col);
}

void Input::highlight(int col) {
    int row = 1;
    if (col >= COLS - 24) {
        row = 2;
    }
    wmove(win, row, col % (COLS - 24) + 1);
    wrefresh(win);
}
