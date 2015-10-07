#ifndef Chat_hpp
#define Chat_hpp

#include "MyWindow.hpp"

class Chat: public MyWindow {

public:
    Chat(int y, int x, int start_y, int start_x) : MyWindow(y, x, start_y, start_x) {};

    void draw();
    void add_message(const std::string& message);

private:
    std::vector<std::string> messages = {};

};

#endif