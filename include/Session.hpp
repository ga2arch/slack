#pragma once

#include <vector>
#include <array>

#include "Roster.hpp"

struct Message {
    RosterItem item;
    std::string content;
    bool sender;

    Message(const RosterItem& i,
            const std::string& c,
            bool x): item(i), content(c), sender(x) {}

    Message() {}
};

class Session {

public:
    void add_message(const RosterItem& item, const std::string& str, bool x);
    
    std::array<Message, 5000> messages;
    std::wstring input_str;

    int col = 0;
    int delta = 0;
    int chat_line = 0;
    int scrolled_back = 0;
    int last_mess = 0;
    int latest_updated_msg = 0;
};
