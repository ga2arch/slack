#include "Roster.hpp"

void Roster::draw() {
    int i = 0;
    int line = 2;

    if (users_cont > 0) {
        mvwprintw(win, 1, 1, "USERS:");
    }
    for (const auto& kv: roster) {
        if (i == users_cont) {
            mvwprintw(win, i + line, 1, "GROUPS:");
            line++;
        }
        mvwprintw(win, i + line, 3, "%.*s", 18, kv.second.name.c_str());
        i++;
    }
    wrefresh(win);
}

void Roster::add_item(const std::string& id,
                      const std::string& name,
                      const std::string& channel) {

    roster.emplace(std::piecewise_construct,
                   std::forward_as_tuple(id),
                   std::forward_as_tuple(id, name, channel));
}

RosterItem Roster::get_item(const std::string& id) {
    return roster.at(id);
}

void Roster::resize_win(int y, int x, int start_y, int start_x) {
    Window::resize_win(y, x, start_y, start_x);
    draw();
}

int Roster::wait() {
    const int KEY_ESC = 27;
    int c, old_active;
    int line = 2;

    wattron(win, A_BOLD);

    if ((active >= users_cont) && (users_cont != 0)) {
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
            if (active < roster.size() - 1) {
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
            if ((active >= users_cont) && (line == 2)) {
                line++;
            } else if ((active < users_cont) && (line == 3)) {
                line--;
            }
            mvwprintw(win, active + line, 1, "* ");
        }
    } while (c != 10);
    wattroff(win, A_BOLD);
    return c;
}

std::string Roster::get_active_channel() {
    auto it = roster.begin();
    std::advance(it, active);
    return it->second.channel;
}