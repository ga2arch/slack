#ifndef SlackClient_H
#define SlackClient_H

#include "Headers.hpp"
#include "Roster.hpp"

using namespace rapidjson;

class SlackUI;

class SlackClient {

public:
    void set_ui(SlackUI* ui);
    void start();

    void send_message(const std::string& message);

private:
    const std::string fetch_data();

    void connect(const std::string& uri);

    void on_open(websocketpp::connection_hdl hdl);
    context_ptr on_tls_init(websocketpp::connection_hdl);
    void on_message(websocketpp::connection_hdl hdl, message_ptr ptr);
    void process_event(const std::string& json);

    Document call(const std::string& api, const std::string& query);

    std::string get_direct_channel(const std::string& userid);

    SlackUI* ui;
    client wc;
    websocketpp::connection_hdl hdl;

    std::map<int, std::string> sent;
    int sent_id = 0;

    RosterItem me;
};

#include "SlackUI.hpp"

#endif