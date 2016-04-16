#pragma once

#include <fstream>

class Log {

public:
    static std::fstream& d() {
        return Log::get_instance().debug;
    }

private:
    static Log& get_instance() {
        static Log instance;

        return instance;
    }

    Log() {
        debug.open("slack.logs", std::fstream::out);
    }

    Log(Log const&)            = delete;
    void operator=(Log const&) = delete;

private:
    std::fstream debug;

};
