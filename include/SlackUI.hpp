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
#include <memory>

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

    std::unique_ptr<Roster> roster;
    std::unique_ptr<Chat>   chat;
    std::unique_ptr<Input>  input;

private:
    void setup_ncurses();
    void resize();

    SlackClient *client;
};

#endif