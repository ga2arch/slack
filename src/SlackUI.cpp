//
//  SlackUI.cpp
//  slack++
//
//  Created by Gabriele Carrettoni on 04/10/15.
//
//

#include "SlackUI.hpp"

void SlackUI::set_client(SlackClient* client) {
    this->client = client;
}

void SlackUI::show() {
    using S = std::chrono::seconds;
    
    const std::string start_mesg = "The client is connecting...";
    const std::string choose_mesg = "Please select an user to chat with using arrow keys (up/down) and enter.";
    const std::string connection_issue = "Could not connect. Check your connection and retry.";

    setup_ncurses();

    roster = std::make_unique<Roster>(LINES, 22, 0, 0, "Roster");
    chat   = std::make_unique<Chat>(LINES-4, COLS-22, 0, 22, "Chat");
    input  = std::make_unique<Input>(4, COLS-22, LINES-4, 22, "InputBox", client);

    chat->print_starting_message(start_mesg);
    if (ui_lock.try_lock_for(S(30))) {
        chat->print_starting_message(choose_mesg);
        ui_lock.unlock();
        return main_ui_cycle();
    }
    chat->print_starting_message(connection_issue);
    sleep(2);
}

void SlackUI::main_ui_cycle() {
    int c;

#ifdef LIBNOTIFY_FOUND
    notify_init("Slack++");
#endif
    c = roster->wait();
    if (c != 27) {
        chat->draw(get_session(), LINES - 6);
        ready = true;
        roster->remove_highlight();
        quit_notification();
    }
    while (c != 27) {
        c = input->wait(get_session().input_str, get_session().col);
        if (c == 9) {
            c = roster->wait();
            if ((c != 27) && (c != 0)) {
                chat->draw(get_session(), LINES - 6);
                input->input_context_switch(get_session());
                if (get_session().scrolled_back == 0) {
                    roster->remove_highlight();
                    quit_notification();
                }
            }
        } else if (c == KEY_UP) {
            chat->scroll_back(get_session());
        } else if (c == KEY_DOWN) {
            if (chat->scroll_forward(get_session()) == 0) {
                roster->remove_highlight();
                quit_notification();
            }
        }
    }
#ifdef LIBNOTIFY_FOUND
    notify_uninit();
#endif
}

Session& SlackUI::get_session() {
    return sessions[roster->get_active_channel()];
}

SlackUI::~SlackUI() {
    endwin();
    delwin(stdscr);
}

void SlackUI::setup_ncurses() {
    setlocale(LC_ALL, "");
    initscr();
    raw();
    noecho();
    ESCDELAY = 25;
    curs_set(2);
    notimeout(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
}

void SlackUI::add_message(const RosterItem& item, const std::string& content, bool sender) {
    int j = 0;
    bool check = false;

    do {
        sessions[item.channel].add_message(item, content.substr(j, COLS - 24).c_str(), sender);
        j += COLS - 24;
        sessions[item.channel].chat_line++;
        if (sessions[item.channel].chat_line > LINES - 6) {
            sessions[item.channel].delta++;
        }
        // check if user is the active one, if ui is ready and if current session is not scrolled back
        if ((item.channel == roster->get_active_channel()) && (ready) && (sessions[item.channel].scrolled_back == 0)) {
            chat->draw(sessions[item.channel], 1);
            check = true;
        }
    } while (j < content.length());

    if (!check) {
        if (!sender) {
            roster->highlight_user(item.channel);
#ifdef LIBNOTIFY_FOUND
            notify_send(item.name, content.substr(0, 40));
#endif
        }
        if (sessions[item.channel].scrolled_back > 0) {
            sessions[item.channel].scrolled_back += j / (COLS - 24);
        }
    }

    input->highlight(sessions[item.channel].col);
}

#ifdef LIBNOTIFY_FOUND
void SlackUI::notify_send(const std::string& name, const std::string& mesg) {
    std::string noti = "New message from " + name + ":\n" + mesg;
    n = notify_notification_new ("Slack++", noti.c_str(), 0);
    notify_notification_set_timeout(n, 5000);
    notify_notification_show(n, 0);
}
#endif

void SlackUI::quit_notification() {
#ifdef LIBNOTIFY_FOUND
    if (n) {
        notify_notification_close(n, NULL);
    }
#endif
}

const std::string SlackUI::get_last_message_sender(const std::string& channel) {
    int size = sessions[channel].last_mess;
    try {
        return sessions[channel].messages.at(size - 1).item.id;
    } catch (std::out_of_range&) {
        return "";
    }
}
