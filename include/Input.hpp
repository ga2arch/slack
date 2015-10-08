#ifndef Input_hpp
#define Input_hpp

#include <iostream>
#include <vector>
#include <string>
#include "Window.hpp"

class SlackClient;

class Input: public Window {

public:
    Input(int y, int x, int start_y, int start_x, const std::string &title, SlackClient *cl) : Window(y, x, start_y, start_x, title), client(cl) {
        scrollok(win, TRUE);
        idlok(win, TRUE);
        keypad(win, TRUE);
};

    int wait();

    void resize_win(int y, int x, int start_y, int start_x);

private:
    SlackClient* client;

    std::string input_str;

    int curr_pos = 0;

};

#include "SlackClient.hpp"

#endif