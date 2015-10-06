//
//  SlackUI.hpp
//  slack++
//
//  Created by Gabriele Carrettoni on 04/10/15.
//
//

#ifndef SlackUI_hpp
#define SlackUI_hpp

#include <stdio.h>
#include <iostream>
#include <mutex>
#include <curses.h>

class SlackClient;

class SlackUI {

public:
    ~SlackUI();

    void set_client(SlackClient* client);
    void show();
    void add_message(const std::string& message);
    void add_user(const std::string& user);

private:
    void setup_ncurses();

    void create_roster();
    void create_chat();
    void create_input();

    void draw_border(WINDOW *win);
    void draw_roster();
    void draw_chat();

    void wait_input();


    int max_x;
    int max_y;

    WINDOW* roster;
    WINDOW* chat;
    WINDOW* input;

    std::mutex draw_lock;

    SlackClient* client;
    
    std::vector<std::string> users = {};
    std::vector<std::string> messages = {};
};

#include "SlackClient.hpp"

#endif /* SlackUI_hpp */
