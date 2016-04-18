#pragma once

#include "Headers.hpp"
#include "Roster.hpp"
#include "WebsocketClient.hpp"

using namespace rapidjson;

class SlackUI;

class SlackClient {

public:
    void set_ui(SlackUI* ui);
    void start(const std::string token);
    void send_message(const std::wstring& message);
    void update_mark();

private:
    const std::string fetch_data();

    void connect(const std::string& uri);

    void process_event(const std::string& json);

    Document call(const std::string& api, const std::vector<std::string> &query);

    std::string get_direct_channel(const std::string& userid);
    
    std::string format_message(std::string str);
    std::string ts_h_readable(const std::string& rawtime);
    
    SlackUI* ui;
    WebsocketClient wc;

    std::map<int, std::string> sent;
    int sent_id = 0;
    std::string auth_token;

    RosterItem me;
};

#include "SlackUI.hpp"
