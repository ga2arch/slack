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
    Roster(int y, int x, int start_y, int start_x, const std::string &title) :  Window(y, x, start_y, start_x, title), active(0) {
        noecho();
        keypad(win, TRUE);
    };

    int wait();
    void draw();

    void add_user(const std::string& id,
                  const std::string& name,
                  const std::string& channel);

    void add_group(const std::string& channel,
                   const std::string& name);

    RosterItem get_user(const std::string& id);
    RosterItem get_group(const std::string& id);

    void remove_user(const std::string& id);

    std::string get_active_channel();

    void change_status(const std::string& status, const RosterItem& user);

private:
    int active;

    std::map<std::string, RosterItem> users;
    std::map<std::string, RosterItem> groups;

};

#endif