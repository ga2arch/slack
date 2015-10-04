#include <iostream>
#include <mutex>
#include <thread>
#include <curses.h>
#include "SlackClient.hpp"

int main(int argc, char const* argv[]) {

    SlackClient c;
    c.start();
    
}
