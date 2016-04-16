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
    void start(const std::string token);
    void send_message(const std::wstring& message);

private:
    const std::string fetch_data(const std::string& token);

    void connect(const std::string& uri);

    void process_event(const std::string& json);

    Document call(const std::string& api, const std::string& query, const std::string& token);

    std::string get_direct_channel(const std::string& userid);
    
    std::string format_message(std::string str);
    
//     size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata);

    SlackUI* ui;
    WebsocketClient wc;

    std::map<int, std::string> sent;
    int sent_id = 0;

    RosterItem me;
    
    std::mutex lock;
};

#include "SlackUI.hpp"

#endif
