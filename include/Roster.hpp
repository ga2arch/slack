#pragma once

#include <string>
#include <map>
#include <vector>

#include "Window.hpp"

/* ROSTER WIN STRUCTURE:
 *
 * |*| |U: |name....|+9|
 *
 * first col: selected or not
 * second col: space
 * third col: type (User, Group, Channel)
 * forth col: name
 * fifth col: unread messages counter
 * If user is muted, it will be printed without bold
 */

struct RosterItem {
    std::string id;
    std::string name;
    std::string channel;
    std::string status;
    int unread_counter;
    bool muted;
    char type;

    RosterItem() {}

    RosterItem(const std::string& i,
               const std::string& n,
               const std::string& c,
               const std::string& s,
               const char t): id(i), name(n), channel(c), status(s), type(t),
               unread_counter(0), muted(false) {}

    RosterItem& operator=(const RosterItem& other) {
        id = other.id;
        name = other.name;
        channel = other.channel;
        status = other.status;
        unread_counter = other.unread_counter;
        muted = other.muted;
        type = other.type;

        return *this;
    }
};

class Roster: public Window {

public:
    Roster(int y, int x, int start_y, int start_x, const std::string &title) :  Window(y, x, start_y, start_x, title) {
        noecho();
        keypad(win, TRUE);
    };

    int wait(Session &sess);
    void draw();
    
    int get_active();
    void set_active(int x);
    void set_current_active();

    void add_user(const std::string& id,
                  const std::string& name,
                  const std::string& channel,
                  const std::string& status);

    void add_group(const std::string& channel,
                   const std::string& name);
    
    void add_channel(const std::string& channel,
                   const std::string& name);

    RosterItem get_user(const std::string& id);
    
    std::string get_active_type();

    void remove_user(const std::string& id);

    std::string get_active_channel();

    void change_status(const std::string& status, const RosterItem& user);
    void highlight_user(const std::string &channel);
    void remove_highlight();
    
    std::string get_active_name();

private:
    void mute_current(int&);
    void mute_all();
    RosterItem& get_roster(int);
    void draw_user(int i, RosterItem &user);
    void scroll_down(int &current_active);
    void scroll_up(int &current_active);
    void scroll_helper(int dir, int &pos);
    
    int active = 0;
    int delta = 0;
    int current_active = 0;
    
    std::map<std::string, RosterItem> users;
};
