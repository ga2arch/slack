//
//  Session.h
//  slack++
//
//  Created by Gabriele Carrettoni on 08/10/15.
//
//

#ifndef Session_h
#define Session_h

#include <iostream>
#include <vector>

#include "Roster.hpp"

struct Message {
    RosterItem item;
    std::string content;
    
    Message(const RosterItem& i,
            const std::string& c): item(i), content(c) {}
};

class Session {
    
public:
    std::vector<Message> messages;
        
};

#endif /* Session_h */
