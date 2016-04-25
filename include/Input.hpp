#pragma once

#include <string>
#include <cwctype>

#include "Window.hpp"

class SlackClient;

class Input: public Window {

public:
    Input(int y, int x,
          int start_y, int start_x,
          const std::string &title,
          SlackClient *cl);

    int wait(Session &sess);

    void input_context_switch(const Session& current_session);
    
    void highlight(int col);
    
    void clear();

private:
    void fixed_print_input(const std::wstring& str, const int &column);
    
    SlackClient* client;

};

#include "SlackClient.hpp"
