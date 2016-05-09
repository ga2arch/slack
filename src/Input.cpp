#include "Input.hpp"

Input::Input(int y, int x,
             int start_y, int start_x,
             const std::string &title,
             SlackClient *cl): Window(y, x, start_y, start_x, title), client(cl) {

    noecho();
    keypad(win, TRUE);
}

int Input::wait(Session &sess) {
    const int KEY_ESC = 27;
    const int KEY_BS = 127;
    const int KEY_TAB = 9;
    const int KEY_ENT = 10;
    wint_t c;
    MEVENT event;

#if NCURSES_MOUSE_VERSION > 1
    mousemask(BUTTON3_PRESSED | BUTTON4_PRESSED | BUTTON5_PRESSED, NULL);
#else
    mousemask(BUTTON3_PRESSED, NULL);
#endif
    
    int& col = sess.col;
    std::wstring& input_str = sess.input_str;
        
    // check if we reached maximum string length
    if (input_str.length() == 1000) {
        return 0;
    }

    wget_wch(win, &c);
    switch (c) {
    case KEY_ESC: // ESC to quit;
    case KEY_TAB: // tab to switch to roster selection mode
    case KEY_UP:   // go back in chat history
    case KEY_DOWN:  // go forward in chat history
    case KEY_MOUSE:
        if (getmouse(&event) == OK) {
            if (event.bstate & BUTTON3_PRESSED) {
                c = KEY_TAB;
            }
#if NCURSES_MOUSE_VERSION > 1
            /* scroll up and down events associated with mouse wheel */
            else if (event.bstate & BUTTON4_PRESSED) {
                c = KEY_UP;
            } else if (event.bstate & BUTTON5_PRESSED) {
                c = KEY_DOWN;
            }
#endif
        }

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
    case KEY_ENT:
        col = 0;
        client->send_message(input_str);
        input_str.clear();
        fixed_print_input(input_str, 1);
        break;
    
    default:
        if (iswprint(c)) {
            input_str.insert(col, 1, c);
            col++;
            fixed_print_input(input_str, col);
        }
        break;
    }
    return 0;
}

void Input::fixed_print_input(const std::wstring& str, const int &column) {
    int col = 1, line = 1;
    // only reprints visible part of string (ie, at maximum (cols)*2 chars)
    int i = column - column % (cols);
        
    if (column >= cols) {
        i -= cols;
    }
    wclear(win);
    while (i < str.length()) {
        mvwprintw(win, line, col, "%lc", str[i]);
        i++;
        col++;
        if (col == cols + 1) {
            col = 1;
            line++;
        }
    }
    draw_borders();
    highlight(column);
}

void Input::input_context_switch(const Session& sess) {
    curs_set(2);
    fixed_print_input(sess.input_str,  sess.col);
}

void Input::highlight(int col) {
    int row = 1;
    if (col >= cols) {
        row = 2;
    }
    wmove(win, row, col % cols + 1);
    wrefresh(win);
}

void Input::clear() {
    curs_set(0);
    wclear(win);
    draw_borders();
}
