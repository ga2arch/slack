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
    
    std::string token = "BDwJUlfnKfyrgxlOnNPBEpLIcHNQ0DpTm2tLwuxuhgVBD59Hh_rIFvfnfbdPR1gOMKkediB_kybCj9VEHvW76SMeAulLpB1w-CyztxnhalRTz1DMw34pUZZzcBp6Vuh1_ww3Tgs4JCl7Zn6NIDf8JQ==";
    
    Websockets wc("ms411.slack-msgs.com", "/websocket/" + token);
    wc.connect();
    
}
