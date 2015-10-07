#ifndef Window_hpp
#define Window_hpp

#include <curses.h>

class Window {

public:
    Window(int y, int x, int start_y, int start_x);

    ~Window();

protected:
    WINDOW* win;

    void draw_borders();

};

#endif