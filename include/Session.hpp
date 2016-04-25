#pragma once

#include <vector>
#include <array>
#include <ctime>

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
    void add_history(const RosterItem& item, const std::string& str, bool x);
    
    std::vector<Message> messages;
    
    // used in input window
    std::wstring input_str;
    int col;
    
    /* used in chat window */
    int delta;
    int scrolled_back;
    int latest_updated_msg;
    long oldest_ts = std::time(nullptr);
    bool first_time = true;
    bool has_more;
};
