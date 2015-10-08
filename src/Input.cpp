#include "Input.hpp"

int Input::wait() {
    int c;
    
    mvwprintw(win, 1, 1, ">> ");
    wmove(win, 1, curr_pos + 4);
    echo();
    do {
        c = wgetch(win);
        if (isprint(c)) {
            input_str[curr_pos] = c;
            curr_pos++;
            if ((curr_pos + 1) % (COLS - 26) == 0) {
                wmove(win, 2, 1); // prototype. I'll have got to check when line is > 2 and scroll the content just like I do in chat.cpp
            }
        } else {
            switch (c) {
                    //             case 9: // tab to go to roster win
                    //                 roster->loop();
                    //                 change_chat(); // here messages[i] will switch to new chatted person's one.
                    //                 break;
                case KEY_RESIZE: case 27: // ESC or resize event;
                    return c;
            }
        }
    } while ((c != 10) && (curr_pos < 4000));
    
    client->send_message(input_str);
    
    memset(input_str, 0, strlen(input_str));
    curr_pos = 0;
    
    wclear(win);
    draw_borders();
    noecho();
    return 0;
}

void Input::resize_win(int y, int x, int start_y, int start_x) {
    Window::resize_win(y, x, start_y, start_x);
    
    mvwprintw(win, 1, 4, input_str);
}