#ifndef Window_hpp
#define Window_hpp

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

#endif