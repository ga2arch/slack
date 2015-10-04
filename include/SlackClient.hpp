#ifndef SlackClient_H
#define SlackClient_H

#include "Headers.hpp"

using namespace rapidjson;

class SlackClient {
    
public:
    void start();
    
private:
    const std::string get_uri();
    
    void connect(std::string uri);
    
    void on_open(websocketpp::connection_hdl hdl);
    context_ptr on_tls_init(websocketpp::connection_hdl);
    void on_message(websocketpp::connection_hdl hdl, message_ptr ptr);
    
    void process_event(const std::string& json);
    Document call(const std::string& api, const std::string& query);
    
    void fetch_roster();
    
    client wc;
    SlackUI ui;
};

#endif