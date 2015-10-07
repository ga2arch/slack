#include "MyWindow.hpp"

MyWindow::MyWindow(int y, int x, int start_y, int start_x) {
    win = newwin(y, x, start_y, start_x);
    draw_borders();
};

MyWindow::~MyWindow() {
    delwin(win);
}

void MyWindow::draw_borders() {
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
}