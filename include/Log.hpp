//
//  Log.h
//  slack++
//
//  Created by Gabriele Carrettoni on 06/10/15.
//
//

#ifndef Log_h
#define Log_h

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

#endif /* Log_h */
