#ifndef MyWindow_hpp
#define MyWindow_hpp

#include <curses.h>

class MyWindow {

public:
    MyWindow(int y, int x, int start_y, int start_x);

    ~MyWindow();

protected:
    WINDOW* win;

    void draw_borders();

};

#endif