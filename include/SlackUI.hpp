//
//  SlackUI.hpp
//  slack++
//
//  Created by Gabriele Carrettoni on 04/10/15.
//
//

#ifndef SlackUI_hpp
#define SlackUI_hpp

#include <map>
#include <mutex>

#include "Log.hpp"
#include "Chat.hpp"
#include "Input.hpp"
#include "Roster.hpp"
#include "Session.hpp"

class SlackClient;

class SlackUI {

public:
    ~SlackUI();

    void set_client(SlackClient* client);
    void show();
    void main_ui_cycle();
    Session& get_session();
    void add_message(const RosterItem& item,
                     const std::string& content);
    const std::string get_last_message_sender(const std::string& channel);

    std::unique_ptr<Roster> roster;
    std::unique_ptr<Chat>   chat;
    std::unique_ptr<Input>  input;
    
    std::mutex ui_lock;
    
    bool ready = false;

private:
    void setup_ncurses();
    void resize();

    SlackClient *client;
    std::map<std::string, Session> sessions;
};

#endif