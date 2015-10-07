#include "MyWindow.hpp"

MyWindow::MyWindow(int y, int, int start_y, int start_x) {
    newwin(y, x, start_y, start_x);
    draw_borders();
};

MyWindow::~MyWindow() {
    delwin(win);
}

MyWindow::void draw_borders() {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
}