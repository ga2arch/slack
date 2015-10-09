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

    RosterItem& operator=(const RosterItem& other) {
        id = other.id;
        name = other.name;
        channel = other.channel;

        return *this;
    }
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
    int wait();
    std::string get_active_channel();

    int active = 0;
    int users_cont = 0;
    int groups_cont = 0;

private:
    std::map<std::string, RosterItem> roster;

};

#endif