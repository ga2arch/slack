#ifndef Roster_hpp
#define Roster_hpp

#include <iostream>
#include <map>

#include "Window.hpp"

struct RosterItem {
    std::string id;
    std::string name;

    RosterItem() {}

    RosterItem(const std::string& i,
               const std::string& n): id(i), name(n) {}
};

class Roster: public Window {

public:
    Roster(int y, int x, int start_y, int start_x, const std::string &title) :  Window(y, x, start_y, start_x, title) {};
    void draw();
    void add_item(const std::string& id, const std::string& name);

    RosterItem get_item(const std::string& id);

private:
    std::map<std::string, RosterItem> roster;

};

#endif