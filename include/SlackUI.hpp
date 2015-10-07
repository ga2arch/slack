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

#include "Log.h"
#include "Chat.hpp"
#include "Input.hpp"
#include "Roster.hpp"

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

    int max_x;
    int max_y;

    Roster *roster;
    Chat *chat;
    Input *input;

    std::mutex draw_lock;

    SlackClient *client;
};

#endif