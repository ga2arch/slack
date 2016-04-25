#pragma once

#ifndef _XOPEN_SOURCE_EXTENDED
    #define _XOPEN_SOURCE_EXTENDED
#endif
#include <curses.h>
#include <string>

class Session;

class Window {

public:
    Window(int y, int x, int start_y, int start_x, const std::string &title);

    ~Window();
    
    int get_real_rows();
    int get_real_cols();
    
    virtual int wait(Session& sess) { return 0; };

protected:
    WINDOW* win;
    int rows, cols;

    std::string title;

    void draw_borders();

};
