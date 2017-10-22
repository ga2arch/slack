#pragma once

#include <map>
#include <mutex>
#include <poll.h>

#ifdef __linux__
#include <sys/signalfd.h>
#include <signal.h>
#endif

#ifdef LIBNOTIFY_FOUND
#include <libnotify/notify.h>
#endif

#include "Log.hpp"
#include "Chat.hpp"
#include "Input.hpp"
#include "Roster.hpp"

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
                     bool sender,
                     bool me,
                     const std::string &channel);
    void add_history(const RosterItem& item, 
                     const std::string& content, 
                     bool sender, 
                     const std::string &channel);
    const std::string get_last_message_sender(const std::string& channel);
    void set_company_name(const std::string& name);

    std::unique_ptr<Roster> roster;
    std::unique_ptr<Chat>   chat;
    std::unique_ptr<Input>  input;
    
private:
    void setup_ncurses();
    void switch_session();
    void change_context();
    void scroll_up(int lines);
    void scroll_down(int lines);
#ifdef LIBNOTIFY_FOUND
    void notify_send(const RosterItem &item, const std::string& mesg);
#endif
    void quit_notification();
    void remove_notification();
    void start_mark_thread(Session &sess, const std::string& channel, const std::string& type);
    void update_mark(Session& sess, const std::string& channel, const std::string& type);
    
#ifdef LIBNOTIFY_FOUND
    NotifyNotification *n = NULL;
#endif
    SlackClient *client;
    std::map<std::string, Session> sessions;
};
