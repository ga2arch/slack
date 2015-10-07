#include "Window.hpp"

Window::Window(int y, int x, int start_y, int start_x) {
    win = newwin(y, x, start_y, start_x);
    draw_borders();
};

Window::~Window() {
    delwin(win);
}

void Window::draw_borders() {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
}