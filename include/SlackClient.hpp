#ifndef SlackClient_H
#define SlackClient_H

#include "Headers.hpp"
#include "Roster.hpp"
#include "WebsocketClient.hpp"

using namespace rapidjson;

class SlackUI;

class SlackClient {

public:
    void set_ui(SlackUI* ui);
    void start();
    std::string get_uri();
    void send_message(const std::string& message);

private:
    const std::string fetch_data();

    void connect(const std::string& uri);

    void process_event(const std::string& json);

    Document call(const std::string& api, const std::string& query);

    std::string get_direct_channel(const std::string& userid);

    SlackUI* ui;
    WebsocketClient wc;

    std::map<int, std::string> sent;
    int sent_id = 0;

    RosterItem me;
    
    std::mutex lock;
};

#include "SlackUI.hpp"

#endif