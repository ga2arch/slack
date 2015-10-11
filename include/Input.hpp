#ifndef Input_hpp
#define Input_hpp

#include <string>
#include "Window.hpp"

class SlackClient;

class Input: public Window {

public:
    Input(int y, int x,
          int start_y, int start_x,
          const std::string &title,
          SlackClient *cl);

    int wait();

private:
    SlackClient* client;

    std::string input_str;

    int line = 1;
    int col = 1;

};

#include "SlackClient.hpp"

#endif