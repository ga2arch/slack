#include "Roster.hpp"

void Roster::draw(std::map<std::string, std::string>& users_status) {
    int i = 0;
    int line = 2;

    if (users.size() > 0) {
        wattron(win, A_BOLD);
        mvwprintw(win, 1, 1, "USERS:");

        for (const auto& kv: users) {
            wattroff(win, A_BOLD);
            if (users_status.at(kv.first) == "active") {
                wattron(win, A_BOLD);
                wattron(win, COLOR_PAIR(2));
            }
            mvwprintw(win, i + line, 3, "%.*s", 18, kv.second.name.c_str());
            wattroff(win, COLOR_PAIR(2));
            i++;
        }
    }

    line += users.size();
    i = 0;

    if (groups.size() > 0) {
        wattron(win, A_BOLD);
        mvwprintw(win, line++, 1, "GROUPS:");
        wattroff(win, A_BOLD);
        for (const auto& kv: groups) {
            mvwprintw(win, i + line, 3, "%.*s", 18, kv.second.name.c_str());
            i++;
        }
    }

    wrefresh(win);
}

void Roster::add_user(const std::string& id,
                      const std::string& name,
                      const std::string& channel) {

    users.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, name, channel));
}

void Roster::add_group(const std::string& channel,
                       const std::string& name) {

    groups.emplace(std::piecewise_construct,
                   std::forward_as_tuple(channel),
                   std::forward_as_tuple(channel, name, channel));
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

    wattron(win, A_BOLD);

    if ((active >= users.size()) && (users.size() != 0)) {
        line++;
    }

    mvwprintw(win, active + line, 1, "* ");
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
                if (active < users.size()+groups.size() - 1) {
                    active++;
                }
                break;
            case KEY_ESC:
                return c;
            default:
                break;
        }

        if (active != old_active) {
            mvwprintw(win, old_active + line, 1, "  ");
            if ((active >= users.size()) && (line == 2)) {
                line++;
            } else if ((active < users.size()) && (line == 3)) {
                line--;
            }
            mvwprintw(win, active + line, 1, "* ");
        }
    } while (c != 10);
    wattroff(win, A_BOLD);
    return c;
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

void Roster::change_status(const std::string& status, const RosterItem& user) {
    int i = 0;

    for (const auto& kv: users) {
        if (kv.second.name == user.name) {
            wattron(win, A_BOLD);
            if (status == "away") {
                wattroff(win, A_BOLD);
            } else if (status == "active") {
                wattron(win, COLOR_PAIR(2));
            }
            mvwprintw(win, i + 2, 3, "%.*s", 18, kv.second.name.c_str());
            break;
        }
        i++;
    }
    wattroff(win, COLOR_PAIR(2));
    wattroff(win, A_BOLD);
    wrefresh(win);
}