#pragma once

#include <string>
#include <iostream>
#include <cstdio>
#include <memory>
#include "Window.hpp"
#include "Session.hpp"

class Chat: public Window {

public:
    Chat(int y, int x, int start_y, int start_x, const std::string &title) : Window(y, x, start_y, start_x, title) {};

    void chat_context_switch(Session &sess);
    void draw_next(Session &sess);
    void set_title(std::string s);

    void print_starting_message(const std::string& mesg);
    int scroll_back(Session& current_session);
    int scroll_forward(Session& current_session);

private:
    void draw(Session& current_session, int num_lines, int start, int scroll);
};
