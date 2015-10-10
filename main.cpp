#include <iostream>
#include <mutex>
#include <thread>
#include <future>

#include "Websockets.hpp"

int main(int argc, char const* argv[]) {

    /*SlackUI ui;
    SlackClient c;

    ui.set_client(&c);
    c.set_ui(&ui);

    std::thread([&]() { c.start(); }).detach();

    ui.show();*/
    
    std::string token("B2KM4l-BgOrBtRV3sQvH8RCnl_4FJB2tkHzwFtsz7UtU09E77BULdK7MHT0fWCaEuOjael6XzYSzt5tDbwSNoy6hLpC0yKvZRq2QFZnnkqOYqFTWIiGaszrPXY1y4gJ4SdE6vSL4OX5UjEaMPdV5Ow==");
    
    Websockets wc("ms411.slack-msgs.com", "/websocket/" + token);
    wc.connect();
    
}
