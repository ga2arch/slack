#include "Roster.hpp"

void Roster::draw() {
    int i = 0;
    for (const auto& kv: roster) {
        mvwprintw(win, i+2, 1, "%.*s", 20, kv.second.name.c_str());
        i++;
    }

    wrefresh(win);
}

void Roster::add_item(const std::string& id, const std::string& name) {
    roster.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, name));
    draw();
}

RosterItem Roster::get_item(const std::string& id) {
    return roster[id];
}