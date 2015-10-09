#include <iostream>
#include <mutex>
#include <thread>
#include <future>

#include "SlackUI.hpp"

int main(int argc, char const* argv[]) {

    SlackUI ui;
    SlackClient c;

    ui.set_client(&c);
    c.set_ui(&ui);

    std::thread([&]() { c.start(); }).detach();

    ui.show();
}
