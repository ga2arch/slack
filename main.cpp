#include <iostream>
#include <mutex>
#include <thread>
#include <future>

#include "SlackUI.hpp"
#include "SlackClient.hpp"
#include "WebsocketClient.hpp"

int main(int argc, char const* argv[]) {
    SlackUI ui;
    SlackClient c;

    ui.set_client(&c);
    c.set_ui(&ui);
    
    auto token = std::getenv("SLACK_TOKEN");
    if (!token) {
        if (argc == 1) {
            std::cerr << "No token specified. Run with " << argv[0] << " YOUR_COMPANY_TOKEN";
            exit(EXIT_FAILURE);
        }
        strcpy(token, argv[1]);
    }
    
    assert(token != nullptr);

    std::thread([&]() { c.start(token); }).detach();
    ui.ui_lock.lock();
    ui.show();
}
