#include <iostream>
#include "SlackUI.hpp"
#include "SlackClient.hpp"

static void check_cmdline(int, char **);

static std::string TOKEN_NAME;

int main(int argc, char* argv[]) {
    
    check_cmdline(argc, argv);
    
    auto token = std::getenv(TOKEN_NAME.c_str());
    if (!token) {
        // fallback to default token name
        token = std::getenv("SLACK_TOKEN");
        if (!token && argc == 1) {
            std::cerr << "No token specified. Check helper message (\"--help\")" << std::endl;;
            exit(EXIT_FAILURE);
        }
    }
    
    assert(token != nullptr);
    
    SlackUI ui;
    SlackClient c;
    
    ui.set_client(&c);
    c.set_ui(&ui);
    
    c.set_token(token);
    ui.show();
}

static void check_cmdline(int argc, char *argv[]) {
    
    if (argc == 2) {
        if (std::strcmp(argv[1], "--help") == 0) {
            std::cout << std::endl << "  Slack++: https://github.com/ga2arch/slack." << std::endl;
            std::cout << "  Developed by Gabriele Carrettoni and Federico Di Pierro." << std::endl;
            std::cout << "  GPL licensed software." << std::endl << std::endl;
            std::cout << "  How to:" << std::endl;
            std::cout << "  \t* export $your_wanted_name_SLACK_TOKEN. Note that \"_SLACK_TOKEN\" suffix is needed." << std::endl;
            std::cout << "  \t* start program with: " << argv[0] << " $your_wanted_name" << std::endl;
            std::cout << "  \t* slack++ will fallback to $SLACK_TOKEN env variable if no args are passed." << std::endl;
            std::cout << "  Usage:" << std::endl;
            std::cout << "  \t* ESC to quit." << std::endl;
            std::cout << "  \t* tab to change focused win." << std::endl;
            std::cout << "  \t* In roster mode:" << std::endl;
            std::cout << "  \t  * enter to choose an user to chat with." << std::endl;
            std::cout << "  \t  * 'm' to mute current user. 'n' to mute all." << std::endl;
            std::cout << "  \t* In input mode:" << std::endl;
            std::cout << "  \t  * enter to send a message." << std::endl;
            std::cout << "  \t  * key_up/key_down to scroll chat history." << std::endl;
            std::cout << "  Enjoy!" << std::endl << std::endl;
            exit(EXIT_SUCCESS);
        } else {
            TOKEN_NAME = argv[1];
            TOKEN_NAME.append("_SLACK_TOKEN");
        }
    }
}
