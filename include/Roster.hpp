#ifndef Roster_hpp
#define Roster_hpp

#include <iostream>
#include <vector>

#include "Window.hpp"

class Roster: public Window {

public:
    Roster(int y, int x, int start_y, int start_x) :  Window(y, x, start_y, start_x) {};
    void draw();
    void add_user(const std::string& user);

private:
    std::vector<std::string> users = {};

};

#endif