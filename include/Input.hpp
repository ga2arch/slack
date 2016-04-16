#ifndef Input_hpp
#define Input_hpp

#include <string>
#include "Window.hpp"
#include "Session.hpp"

class SlackClient;

class Input: public Window {

public:
    Input(int y, int x,
          int start_y, int start_x,
          const std::string &title,
          SlackClient *cl);

    int wait(std::wstring& input_str, int& col);

    void input_context_switch(const Session& current_session);
    
    void highlight(int col);

private:
    void fixed_print_input(const std::wstring& str, const int &column);
    
    SlackClient* client;

};

#include "SlackClient.hpp"

#endif
