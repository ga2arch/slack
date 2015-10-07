#ifndef Input_hpp
#define Input_hpp

#include <iostream>
#include <vector>
#include "Window.hpp"

class SlackClient;

class Input: public Window {

public:
    Input(int y, int x, int start_y, int start_x, SlackClient *cl) : Window(y, x, start_y, start_x), client(cl) {};

    void wait();

private:
    SlackClient* client;

};

#include "SlackClient.hpp"

#endif