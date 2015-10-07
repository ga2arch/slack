#ifndef Roster_hpp
#define Roster_hpp

#include "MyWindow.hpp"

class Roster: public MyWindow {

public:
    Roster(int y, int x, int start_y, int start_x) :  MyWindow(y, x, start_y, start_x) {};
    void draw();
    void add_user(const std::string& user);

private:
    std::vector<std::string> users = {};

};

#endif