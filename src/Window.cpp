#include "Window.hpp"

Window::Window(int y, int x, int start_y, int start_x, const std::string &title) {
    win = newwin(y, x, start_y, start_x);
    this->title = title;
    draw_borders();
};

Window::~Window() {
    delwin(win);
}

void Window::draw_borders() {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    mvwprintw(win, 0, 0, title.c_str());
    wrefresh(win);
}