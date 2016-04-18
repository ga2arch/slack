#include "Roster.hpp"

void Roster::draw() {
    int i = 0;
    int line = 2;

    wattron(win, A_BOLD);
    if (users.size() > 0) {
        mvwprintw(win, 1, 1, "USERS:");
        for (const auto& kv: users) {
            if (kv.second.status == "active") {
                wattron(win, COLOR_PAIR(2));
            }
            mvwprintw(win, i + line, 3, "%.*s", 16, kv.second.name.c_str());
            wattroff(win, COLOR_PAIR(2));
            i++;
        }
    }

    line += users.size();
    i = 0;

    if (groups.size() > 0) {
        mvwprintw(win, line++, 1, "GROUPS:");
        for (const auto& kv: groups) {
            mvwprintw(win, i + line, 3, "%.*s", 16, kv.second.name.c_str());
            i++;
        }
    }

    wrefresh(win);
}

void Roster::add_user(const std::string& id,
                      const std::string& name,
                      const std::string& channel,
                      const std::string& status) {

    users.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, name, channel, status));
}

void Roster::add_group(const std::string& channel,
                       const std::string& name) {

    groups.emplace(std::piecewise_construct,
                   std::forward_as_tuple(channel),
                   std::forward_as_tuple(channel, name, channel, ""));
}

RosterItem Roster::get_user(const std::string& id) {
    return users.at(id);
}

RosterItem Roster::get_group(const std::string& id) {
    return groups.at(id);
}

void Roster::remove_user(const std::string& id) {
    users.erase(id);
}

int Roster::wait() {
    const int KEY_ESC = 27;
    int c, old_active;
    int line = 2;
    int current_active = active;

    if ((current_active >= users.size()) && (users.size() != 0)) {
        line++;
    }

    mvwprintw(win, current_active + line, 1, "* ");
    do {
        old_active = current_active;
        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                if (current_active > 0) {
                    current_active--;
                }
                break;
            case KEY_DOWN:
                if (current_active < users.size()+groups.size() - 1) {
                    current_active++;
                }
                break;
            case KEY_ESC:
                return c;
            default:
                break;
        }

        if (current_active != old_active) {
            mvwprintw(win, old_active + line, 1, "  ");
            if ((current_active >= users.size()) && (line == 2)) {
                line++;
            } else if ((current_active < users.size()) && (line == 3)) {
                line--;
            }
            mvwprintw(win, current_active + line, 1, "* ");
        }
    } while (c != 10);
    if (active != current_active) {
        active = current_active;
    } else {
        c = 0;
    }

    return c;
}

void Roster::remove_highlight() {
    if (active < users.size()) {
        auto it = users.begin();
        std::advance(it, active);
        if (it->second.status == "active") {
            wattron(win, COLOR_PAIR(2));
        }
        mvwprintw(win, active + 2, 3, "%.*s", 16, it->second.name.c_str());
        it ->second.unread_counter = 0;
        mvwprintw(win, active + 2, 19, "  ");
        wattroff(win, COLOR_PAIR(2));
        wrefresh(win);
    }
}

std::string Roster::get_active_channel() {
    if (active < users.size()) {
        auto it = users.begin();
        std::advance(it, active);
        return it->second.channel;
    } else {
        auto it = groups.begin();
        std::advance(it, active - users.size());
        return it->second.channel;
    }
}

std::string Roster::get_active_type() {
    if (active < users.size()) {
        return "im";
    } else {
        return "group";
    }
}

void Roster::change_status(const std::string& status, const RosterItem& user) {
    int i = 0;

    for (auto& kv: users) {
        if (kv.second.name == user.name) {
            kv.second.status = status;
            if (status == "active") {
                wattron(win, COLOR_PAIR(2));
            }
            mvwprintw(win, i + 2, 3, "%.*s", 16, kv.second.name.c_str());
            wattroff(win, COLOR_PAIR(2));
            break;
        }
        i++;
    }
    wrefresh(win);
}

void Roster::highlight_user(const std::string &channel) {
    int i = 0;

    for (auto& kv: users) {
        if (kv.second.channel == channel) {
            wattron(win, COLOR_PAIR(3));
            mvwprintw(win, i + 2, 3, "%.*s", 16, kv.second.name.c_str());
            kv.second.unread_counter++;
            if (kv.second.unread_counter < 10) {
                mvwprintw(win, i + 2, 20, "%d", kv.second.unread_counter);
            } else {
                mvwprintw(win, i + 2, 19, "9+");
            }
            wattroff(win, COLOR_PAIR(3));
            break;
        }
        i++;
    }
    wrefresh(win);
}
