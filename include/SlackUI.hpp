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

class SlackUI {
    
public:
    ~SlackUI();
    
    void show();
    void add_message(const std::string& message);
    void add_user(const std::string& user);
    
private:
    void setup_ncurses();
    
    void create_roster();
    void create_chat();
    void create_input();
    
    void draw_roster();
    void draw_chat();
    void draw_input();
    
    void wait_input();
    
    
    int max_x;
    int max_y;
    
    WINDOW* roster;
    WINDOW* chat;
    WINDOW* input;
    
    std::mutex draw_lock, show_lock;
    
    std::vector<std::string> users = {};
    std::vector<std::string> messages = {};
};

#endif /* SlackUI_hpp */
