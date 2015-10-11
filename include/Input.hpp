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

    int wait(std::string& input_str, int line, int col);

    void change_session(const Session& session);

private:
    SlackClient* client;

};

#include "SlackClient.hpp"

#endif