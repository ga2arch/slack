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

#ifdef LIBNOTIFY_FOUND
#include <libnotify/notify.h>
#endif

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
                     const std::string& content,
                     bool sender);
    const std::string get_last_message_sender(const std::string& channel);

    std::unique_ptr<Roster> roster;
    std::unique_ptr<Chat>   chat;
    std::unique_ptr<Input>  input;
    
    std::timed_mutex ui_lock;

private:
    void setup_ncurses();
#ifdef LIBNOTIFY_FOUND
    void notify_send(const std::string& name, const std::string& mesg);
#endif
    void quit_notification();
    
#ifdef LIBNOTIFY_FOUND
    NotifyNotification *n = NULL;
#endif
    bool ready = false;
    SlackClient *client;
    std::map<std::string, Session> sessions;
};

#endif
