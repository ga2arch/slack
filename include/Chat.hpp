#ifndef Chat_hpp
#define Chat_hpp

#include <string>
#include "Window.hpp"
#include "Session.hpp"

class Chat: public Window {

public:
    Chat(int y, int x, int start_y, int start_x, const std::string &title) : Window(y, x, start_y, start_x, title) {
        scrollok(win, TRUE);
        idlok(win, TRUE);};

    void draw(Session& current_session);
    void chat_context_switch(const Session& current_session);

    void print_starting_message(const std::string& mesg);
};

#endif