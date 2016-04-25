#include "Roster.hpp"

void Roster::draw() {
    wattron(win, A_BOLD);
    for (int i = 0; i < get_real_rows() && i < users.size(); i++) {
        RosterItem &user = get_roster(i);
        draw_user(i, user);
    }
    mvwprintw(win, current_active + 1 - delta, 1, "*");
    wrefresh(win);
}

/*
 * Function that checks every property
 * of the current user, and prints it.
 */
void Roster::draw_user(int i, RosterItem &user) {
    if (user.muted) {
        wattroff(win, A_BOLD);
    }
    mvwprintw(win, i + 1 - delta, 3, "%c:", user.type);
    if (user.status == "active") {
        wattron(win, COLOR_PAIR(2));
    }
    if (user.unread_counter > 0) {
        wattron(win, COLOR_PAIR(3));
        if (user.unread_counter < 10) {
            mvwprintw(win, i + 1 - delta, cols - 2, "%d", user.unread_counter);
        } else {
            mvwprintw(win, i + 1 - delta, cols - 3, "+9");
        }
    } else {
        mvwprintw(win, i + 1 - delta, cols - 3, "  ");
    }
    mvwprintw(win, i + 1 - delta, 5, "%.*s", cols - 8, user.name.c_str());
    wattroff(win, COLOR_PAIR(2));
    wattroff(win, COLOR_PAIR(3));
    wattron(win, A_BOLD);
}

void Roster::add_user(const std::string& id,
                      const std::string& name,
                      const std::string& channel,
                      const std::string& status) {

    users.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, name, channel, status, 'U'));
}

void Roster::add_group(const std::string& channel,
                       const std::string& name) {
    
    users.emplace(std::piecewise_construct,
                   std::forward_as_tuple(channel),
                   std::forward_as_tuple(channel, name, channel, "", 'G'));
}

void Roster::add_channel(const std::string& channel,
                       const std::string& name) {
    
    users.emplace(std::piecewise_construct,
                   std::forward_as_tuple(channel),
                   std::forward_as_tuple(channel, name, channel, "", 'C'));
}

RosterItem Roster::get_user(const std::string& id) {
    return users.at(id);
}

void Roster::remove_user(const std::string& id) {
    users.erase(id);
}

int Roster::wait(Session &sess) {
    const int KEY_ESC = 27;
    int c;
    
    c = wgetch(win);
    switch (c) {
        case KEY_UP:
            if (current_active > 0) {
                scroll_up(current_active);
            }
            break;
        case KEY_DOWN:
            if (current_active < users.size() - 1) {
                scroll_down(current_active);
            }
            break;
        case KEY_ESC:
            return c; // value to return in case of ESC
        case 'm':
        case 'M':
            mute_current(current_active);
            break;
        case 'n':
        case 'N':
            mute_all();
            break;
        case 10:
            active = current_active;
            return c;
        default:
            break;
    }
    wrefresh(win);
    return 0;
}

int Roster::get_active() {
    return active;
}

void Roster::set_active(int x) {
    active = x;
}

void Roster::set_current_active() {
    current_active = active;
}

void Roster::scroll_down(int &current_active) {
    mvwprintw(win, current_active + 1 - delta, 1, " ");
    current_active++;
    if (current_active - get_real_rows() >= delta) {
        scroll_helper(1, current_active);
    }
    mvwprintw(win, current_active + 1 - delta, 1, "*");
}

void Roster::scroll_up(int &current_active) {
    mvwprintw(win, current_active + 1 - delta, 1, " ");
    current_active--;
    if (current_active < delta) {
        scroll_helper(-1, current_active);
    }
    mvwprintw(win, current_active + 1 - delta, 1, "*");
}

void Roster::scroll_helper(int dir, int &pos) {
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wscrl(win, dir);
    wattroff(win, A_BOLD);
    draw_borders();
    wattron(win, A_BOLD);
    delta += dir;
    RosterItem &user = get_roster(pos);
    draw_user(pos, user);
}

void Roster::remove_highlight() {
    RosterItem &user = get_roster(active);
    
    user.unread_counter = 0;
    draw_user(active, user);
    wrefresh(win);
}

std::string Roster::get_active_channel() {
    if (active == -1) {
        return "";
    }
    return get_roster(active).channel;
}

RosterItem& Roster::get_roster(int x) {
    auto it = users.begin();
    std::advance(it, x);
    return it->second;
}

void Roster::mute_current(int& current) {
    RosterItem &x = get_roster(current);
    x.muted = !x.muted;
    if (current - delta < get_real_rows() && current >= delta) {
        draw_user(current, x);
    }
}

void Roster::mute_all() {
    int i;
    
    for (i = 0; i < users.size(); i++) {
        mute_current(i);
    }
}

std::string Roster::get_active_type() {
    switch (get_roster(active).type) {
    case 'U':
        return "im";
    case 'G':
        return "group";
    case 'C':
        return "channel";
    default:
        return "";
    }
}

void Roster::change_status(const std::string& status, const RosterItem& user) {
    int i = 0;

    for (auto& kv: users) {
        if (kv.second.name == user.name) {
            kv.second.status = status;
            draw_user(i, kv.second);
            wrefresh(win);
            break;
        }
        i++;
    }
}

void Roster::highlight_user(const std::string &channel) {
    int i = 0;

    for (auto& kv: users) {
        if (kv.second.channel == channel) {
            kv.second.unread_counter++;
            draw_user(i, kv.second);
            break;
        }
        i++;
    }
    wrefresh(win);
}

std::string Roster::get_active_name() {
    return get_roster(active).name;
}
