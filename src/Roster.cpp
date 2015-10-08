#include "Roster.hpp"

void Roster::draw() {
    int i = 0;
    for (const auto& kv: roster) {
        mvwprintw(win, i+1, 3, "%.*s", 17, kv.second.name.c_str());
        i++;
    }
    mvwprintw(win, active + 1, 1, "* ");

    wrefresh(win);
}

void Roster::add_item(const std::string& id,
                      const std::string& name,
                      const std::string& channel) {

    roster.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, name, channel));

    roster_channels.push_back(channel);
    messages.resize(roster.size());
}

RosterItem Roster::get_item(const std::string& id) {
    return roster[id];
}

void Roster::resize_win(int y, int x, int start_y, int start_x) {
    Window::resize_win(y, x, start_y, start_x);
    draw();
}

const std::string Roster::get_active_channel() {
    return roster_channels[active];
}

const int Roster::get_active() {
    return active;
}

void Roster::wait() {
    int c, old_active;

    wattron(win, A_BOLD);
    mvwprintw(win, active + 1, 1, "* ");
    do {
        old_active = active;
        c = wgetch(win);
        switch (c) {
        case KEY_UP:
            if (active > 0) {
                active--;
            }
            break;
        case KEY_DOWN:
            if (active < roster.size() - 1) {
                active++;
            }
            break;
        default:
            break;
        }

        if (active != old_active) {
            mvwprintw(win, active + 1, 1, "* ");
            mvwprintw(win, old_active + 1, 1, "  ");
        }
    } while (c != 10);
    wattroff(win, A_BOLD);
}

void Roster::add_message(const std::string& message) {
    messages[active].emplace_back(message);
}

std::vector <std::string>& Roster::get_messages() {
    return messages[active];
}