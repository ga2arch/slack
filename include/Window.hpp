#ifndef Window_hpp
#define Window_hpp

#include <curses.h>
#include <iostream>

class Window {

public:
    Window(int y, int x, int start_y, int start_x, const std::string &title);

    ~Window();

    void resize_win(int y, int x, int start_y, int start_x);

protected:
    WINDOW* win;

    std::string title;

    void draw_borders();

};

#endif