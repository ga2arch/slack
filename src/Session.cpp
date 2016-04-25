#include "Session.hpp"

void Session::add_message(const RosterItem& item, const std::string& str, bool x) {
    Message mesg(item, str, x);
    messages.push_back(mesg);
}


void Session::add_history(const RosterItem& item, const std::string& str, bool x) {
    Message mesg(item, str, x);
    messages.insert(messages.begin(), mesg);
}
