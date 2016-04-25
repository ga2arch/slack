#pragma once

#include "Headers.hpp"
#include "Roster.hpp"
#include "WebsocketClient.hpp"

using namespace rapidjson;

class SlackUI;

class SlackClient {

public:
    void set_ui(SlackUI* ui);
    bool start();
    void set_token(std::string token);
    curl_socket_t get_socket();
    void receive();
    void send_message(const std::wstring& message);
    void update_mark(const std::string& channel, const std::string& type);
    void get_history(const std::string& channel, std::string& type);

private:
    const std::string fetch_data();

    bool connect(const std::string& uri);
    
    void process_event(void);

    Document call(const std::string& api, const std::vector<std::string> &query);

    std::string get_direct_channel(const std::string& userid);
    
    std::string format_message(std::string str);
    std::string ts_h_readable(const long ts);
    
    SlackUI* ui;
    WebsocketClient wc;

    std::map<int, std::string> sent;
    int sent_id = 0;
    std::string auth_token;

    RosterItem me;
};

#include "SlackUI.hpp"
