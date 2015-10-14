//
//  Session.h
//  slack++
//
//  Created by Gabriele Carrettoni on 08/10/15.
//
//

#ifndef Session_h
#define Session_h

#include <vector>

#include "Roster.hpp"

struct Message {
    RosterItem item;
    std::vector <std::string> content;

    Message(const RosterItem& i,
            const std::vector <std::string>& c): item(i), content(c) {}
};

class Session {

public:
    std::vector<Message> messages;
    std::string input_str;

    int line = 1;
    int col = 1;
    int delta = 0;
    int chat_line = 0;
    int scroll_lines = 0;
    int scrolled_back = 0;

};

#endif /* Session_h */
