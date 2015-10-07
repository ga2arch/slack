#ifndef Chat_hpp
#define Chat_hpp

#include <iostream>
#include <vector>
#include "Window.hpp"

class Chat: public Window {

public:
    Chat(int y, int x, int start_y, int start_x) : Window(y, x, start_y, start_x) {wrefresh(win);};

    void draw();
    void add_message(const std::string& message);

private:
    std::vector<std::string> messages;

    int chat_line = 0;

};

#endif