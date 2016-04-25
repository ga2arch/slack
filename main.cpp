#include <iostream>
#include "SlackUI.hpp"
#include "SlackClient.hpp"

static void check_help(int, char **);

int main(int argc, char* argv[]) {
    
    check_help(argc, argv);
    
    auto token = std::getenv("SLACK_TOKEN");
    if (!token) {
        if (argc == 1) {
            std::cerr << "No token specified. Run with:" << std::endl << argv[0] << " $YOUR_COMPANY_TOKEN" << std::endl;
            exit(EXIT_FAILURE);
        }
        token = argv[1];
    }
    
    assert(token != nullptr);
    
    SlackUI ui;
    SlackClient c;
    
    ui.set_client(&c);
    c.set_ui(&ui);
    
    c.set_token(token);
    ui.show();
}

static void check_help(int argc, char *argv[]) {
    
    if (argc == 2) {
        if (std::strcmp(argv[1], "--help") == 0) {
            std::cout << std::endl << "  Slack++: https://github.com/ga2arch/slack." << std::endl;
            std::cout << "  Developed by Gabriele Carrettoni and Federico Di Pierro." << std::endl;
            std::cout << "  GPL licensed software." << std::endl << std::endl;
            std::cout << "  Usage:" << std::endl;
            std::cout << "  \t* ESC to quit." << std::endl;
            std::cout << "  \t* tab to change focused win." << std::endl;
            std::cout << "  \t* In roster mode:" << std::endl;
            std::cout << "  \t  * enter to choose an user to chat with." << std::endl;
            std::cout << "  \t  * 'm' to mute current user. 'n' to mute all." << std::endl;
            std::cout << "  \t* In input mode:" << std::endl;
            std::cout << "  \t  * enter to send a message." << std::endl;
            std::cout << "  \t  * key_up/key_down to scroll chat history." << std::endl << std::endl;
            std::cout << "  Enjoy!" << std::endl << std::endl;
            exit(EXIT_SUCCESS);
        }
    }
}
