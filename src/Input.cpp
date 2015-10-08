#include "Input.hpp"

int Input::wait() {
    int c;
    
    mvwprintw(win, 1, 1, ">> ");
    wmove(win, 1, input_str.length() + 4);
    echo();
    
    do {
        c = wgetch(win);
        switch (c) {
                //             case 9: // tab to go to roster win
                //                 roster->loop();
                //                 change_chat(); // here messages[i] will switch to new chatted person's one.
                //                 break;
            case KEY_RESIZE:
            case 27: // ESC or resize event;
                return c;
                
            default:
                if (isprint(c)) {
                    input_str.push_back(c);
                    if ((input_str.length() + 1) % (COLS - 26) == 0) {
                        wmove(win, 2, 1); // prototype. I'll have got to check when line is > 2 and scroll the content just like I do in chat.cpp
                    }
                }
                break;
        }
    } while (c != 10 && input_str.length() < 4000);

    client->send_message(input_str);

    input_str.clear();
    
    wclear(win);
    draw_borders();
    noecho();
    return 0;
}

void Input::resize_win(int y, int x, int start_y, int start_x) {
    Window::resize_win(y, x, start_y, start_x);
    
    mvwprintw(win, 1, 4, input_str.c_str());
}