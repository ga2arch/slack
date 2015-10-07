#ifndef Input_hpp
#define Input_hpp

#include "MyWindow.hpp"
#include "SlackClient.hpp"

class Input: public MyWindow {

public:
    Input(int y, int x, int start_y, int start_x, SlackClient *cl);

    void wait();

private:
    SlackClient* client;

};

#endif