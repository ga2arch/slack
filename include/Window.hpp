#pragma once

#ifndef _XOPEN_SOURCE_EXTENDED
    #define _XOPEN_SOURCE_EXTENDED
#endif
#include <curses.h>
#include <string>

class Window {

public:
    Window(int y, int x, int start_y, int start_x, const std::string &title);

    ~Window();

protected:
    WINDOW* win;

    std::string title;

    void draw_borders();

};
