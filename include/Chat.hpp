#ifndef Chat_hpp
#define Chat_hpp

#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include "Window.hpp"
#include "Session.hpp"

class Chat: public Window {

public:
    Chat(int y, int x, int start_y, int start_x, const std::string &title) : Window(y, x, start_y, start_x, title) {};

    void draw(Session& current_session, int num_lines);

    void print_starting_message(const std::string& mesg);
    void scroll_back(Session& current_session);
    int scroll_forward(Session& current_session);

};

#endif
