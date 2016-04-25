#pragma once

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

class Log {

public:
    static std::fstream& d() {
        return Log::get_instance().debug;
    }

private:
    
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    
    static Log& get_instance() {
        static Log instance;

        return instance;
    }

    Log() {
        std::string log_path = homedir;
        debug.open(log_path + "/.slack.logs", std::fstream::out);
    }

    Log(Log const&)            = delete;
    void operator=(Log const&) = delete;

private:
    std::fstream debug;

};
