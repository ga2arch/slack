#ifndef Roster_hpp
#define Roster_hpp

#include <iostream>
#include <map>
#include <vector>

#include "Window.hpp"

struct RosterItem {
    std::string id;
    std::string name;
    std::string channel;

    RosterItem() {}

    RosterItem(const std::string& i,
               const std::string& n,
               const std::string& c): id(i), name(n), channel(c) {}
};

class Roster: public Window {

public:
    Roster(int y, int x, int start_y, int start_x, const std::string &title) :  Window(y, x, start_y, start_x, title) {
        noecho();
        keypad(win, TRUE);
    };

    void draw();
    void add_item(const std::string& id,
                  const std::string& name,
                  const std::string& channel);
    void resize_win(int y, int x, int start_y, int start_x);

    RosterItem get_item(const std::string& id);

    const std::string get_active_channel();

    const int get_active();

    void wait();

private:
    std::map<std::string, RosterItem> roster;

    std::vector<std::string> roster_channels;

    int active = 0;

};

#endif