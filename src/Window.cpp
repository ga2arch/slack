#include "Window.hpp"

Window::Window(int y, int x, int start_y, int start_x, const std::string &title) {
    win = newwin(y, x, start_y, start_x);
    this->title = title;
    draw_borders();
    scrollok(win, TRUE);
    idlok(win, TRUE);
};

Window::~Window() {
    delwin(win);
}

void Window::draw_borders() {
    int row, col;
    
    getmaxyx(win, row, col);
    
    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
    wattron(win, A_BOLD);
    mvwprintw(win, 0, 0, "%.*s", col, title.c_str());
    wattroff(win, A_BOLD);
    wrefresh(win);
}
