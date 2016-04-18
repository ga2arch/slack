//
//  Session.cpp
//  slack++
//
//  Created by Gabriele Carrettoni on 08/10/15.
//
//

#include "Session.hpp"

void Session::add_message(const RosterItem& item, const std::string& str, bool x) {
    Message mesg(item, str, x);
    messages.at(last_mess) = mesg;
    last_mess = (last_mess + 1) % 5000;
}
