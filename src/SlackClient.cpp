#include "SlackClient.hpp"

bool SlackClient::start() {
    return connect(fetch_data());
}

void SlackClient::set_token(std::string token) {
    auth_token = token;
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

curl_socket_t SlackClient::get_socket() {
    return wc.socket;
}

void SlackClient::receive() {
    wc.receive();
    process_event();
}

const std::string SlackClient::fetch_data() {
    Log::d() << "Getting websocket url ...";
    
    std::vector<std::string> v;
    v.push_back("simple_latest=true");
    v.push_back("no_unreads=true");
    Json::Value d;
    
    try {
        d = call("rtm.start", v);
    } catch (int n) {
        return "";
    }
    
    if (d.isNull()) {
        return "";
    }
    
    Log::d() << " OK" << std::endl;

    me.id = d["self"]["id"].asString();

    const auto& users  = d["users"];
    const auto& ims    = d["ims"];
    const auto& groups = d["groups"];
    const auto& channels = d["channels"];
    
    const std::string company_name = d["team"]["name"].asString();
    ui->set_company_name(company_name);
    
    // Get Users
    for (auto i = 0; i < users.size(); i++) {
        const auto& u = users[i];

        std::string name = u["profile"]["real_name"].asString();
        const std::string id = u["id"].asString();
        const std::string status = u["presence"].asString();

        name = name.empty() ? u["name"].asString() : name;

        std::string channel;

        // Get IM Channel
        for (auto j = 0; j < ims.size(); j++) {
            if (ims[j]["user"].asString() == id) {
                channel = ims[j]["id"].asString();
                break;
            }
        }

        if (id == me.id) {
            me.name = name;
            me.channel = channel;
            me.status = status;
            continue;
        };

        ui->roster->add_user(id, name, channel, status);
    }

    // Get Groups
    for (auto i = 0; i < groups.size(); i++) {
        const auto& g = groups[i];

        const std::string name = g["name"].asString();
        const std::string channel = g["id"].asString();

        ui->roster->add_group(channel, name);
    }
    
    // get channels
    for (auto i = 0; i < channels.size(); i++) {
        const auto& c = channels[i];
        
        const std::string name = c["name"].asString();
        const std::string channel = c["id"].asString();
        
        ui->roster->add_channel(channel, name);
    }

    return d["url"].asString();
}

bool SlackClient::connect(const std::string& uri) {
    Log::d() << "Attempting connection ..." << std::endl;
    if (uri == "") {
        return false;
    }
    return wc.connect(uri);
}

void SlackClient::process_event(void) {
    for (const auto json :  wc.events) {
        Json::Value d;
        Json::Reader reader;
        reader.parse(json, d);

        // other users messages
        if (d.isMember("type") && d["type"] == "message") {
            // avoid first old message sent by slack...
            if (d.isMember("reply_to")) {
                continue;
            }
            
            RosterItem user;
        
            try {
                user = ui->roster->get_user(d["user"].asString());
            } catch (std::out_of_range&) {
                user = me;
            }
            std::string channel = d["channel"].asString();
            /* Add username before this message as user sending message switched */
            if (ui->get_last_message_sender(channel) != user.id) {
                ui->add_message(user, user.name + ':', true, false, channel);
            }
            std::string str = format_message(d["text"].asString());
            long ts = std::stol(d["ts"].asString(), nullptr);
            if (ui->get_session().oldest_ts == 0) {
                ui->get_session().oldest_ts = ts - 1;
            }
            std::string timestamp = ts_h_readable(ts);
            /* Actually add message */
            ui->add_message(user, timestamp + " " + str, false, false, channel);
        }
    
        // my messages
        if (d.isMember("ok") && d.isMember("text")) {
            auto const reply_to = d["reply_to"].asInt();
            me.channel = sent[reply_to];
            if (ui->get_last_message_sender(sent[reply_to]) != me.id) {
                ui->add_message(me, me.name + ':', true, true, me.channel);
            }
            std::string str = format_message(d["text"].asString());
            long ts = std::stol(d["ts"].asString(), nullptr);
            if (ui->get_session().oldest_ts == 0) {
                ui->get_session().oldest_ts = ts - 1;
            }
            std::string timestamp = ts_h_readable(ts);
            ui->add_message(me, timestamp + " " + str, false, true, me.channel);
        }
    
        // online/offline events
        if (d.isMember("type") && d["type"] == "presence_change") {
            if (me.id != d["user"].asString()) {
                const RosterItem &x = ui->roster->get_user(d["user"].asString());
                ui->roster->change_status(d["presence"].asString(), x);
            }
        }
    }

    wc.events.clear();
}

static size_t write_data(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::ostream* os = static_cast<std::ostream*>(userdata);
    std::streamsize len = size * nmemb;
    return (os->write(static_cast<char*>(ptr), len)) ? len : 0;
}

Json::Value SlackClient::call(const std::string &api, const std::vector<std::string> &query) {
    Json::Value d;
    Json::Reader reader;
    std::ostringstream os;
    
    const auto base_url = "https://slack.com/api/";
    auto url = base_url + api + "?token=" + auth_token;
    
    for (std::string str : query) {
        url = url + "&" + str;
    }
    
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
        curl_easy_setopt(curl, CURLOPT_FILE, &os);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            goto error;
        }
    
        long respcode; //response code of the http transaction
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respcode);
        if (respcode != 200) {
            goto error;
        }
        curl_easy_cleanup(curl);
        reader.parse(os.str(), d);
        return d;
    }
    
error:
    if (curl) {
        curl_easy_cleanup(curl);
    }
    Log::d() << "An error occurred during the call." << std::endl;
    throw 1;
}

void SlackClient::send_message(const std::wstring& message) {
    Json::Value writer;
    auto channel = ui->roster->get_active_channel().c_str();
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string s = converter.to_bytes(message);
    
    writer["id"] = ++sent_id;
    writer["type"] = "message";
    writer["channel"] = channel;
    writer["text"] = s;
    
    
    sent[sent_id] = channel;
    Json::FastWriter fastWriter;
    std::string jsonMessage = fastWriter.write(writer);
    
    wc.send(jsonMessage);
}

std::string SlackClient::format_message(std::string str) {
    int index = str.find("&amp;");
    while (index != std::string::npos) {
        str.replace(index, 5, "&");
        index = str.find("&amp;");
    }
    index = str.find("&lt;");
    while (index != std::string::npos) {
        str.replace(index, 4, "<");
        index = str.find("&lt;");
    }
    index = str.find("&gt;");
    while (index != std::string::npos) {
        str.replace(index, 4, ">");
        index = str.find("&gt;");
    }
    return str;
}

std::string SlackClient::ts_h_readable(const long ts) {
    struct tm * dt;
    char buffer [30];
    dt = localtime(&ts);
    strftime(buffer, sizeof(buffer), "%H:%M", dt);
    return std::string(buffer);
}

void SlackClient::update_mark(const std::string& channel, const std::string& type) {
    auto now = std::time(nullptr);
    
    std::stringstream ss;
    ss << now;
    
    std::vector<std::string> parameters;
    parameters.push_back("channel=" + channel);
    parameters.push_back("ts=" + ss.str());
    
    auto d = call(type + ".mark", parameters);
    if (!d.isNull() && d["ok"].asBool()) {
        Log::d() << type << " marked." << std::endl;
    }
}

void SlackClient::get_history(const std::string& channel, std::string& type) {
    std::vector<std::string> parameters;
    RosterItem latest_user;
    
    parameters.push_back("channel=" + channel);
    std::stringstream s;
    s << ui->get_session().oldest_ts;
    parameters.push_back("latest=" + s.str());
    s.clear();
    s << 50;
    parameters.push_back("count=" + s.str());
    if (type != "im") {
        type.push_back('s');
    }
    auto d = call(type + ".history", parameters);
    
    if (d.isNull() || !d.isMember("messages")) {
        return;
    }
    
    long ts;
    
    const auto& messages  = d["messages"];
    
    try {
        latest_user = ui->roster->get_user(messages[0]["user"].asString());
    } catch (std::out_of_range&) {
        latest_user = me;
    }
    
    for (auto i = 0; i < messages.size(); i++) {
        const auto& m = messages[i];
        if (m["type"] != "message") {
            continue;
        }
        RosterItem user;
        try {
            user = ui->roster->get_user(m["user"].asString());
        } catch (std::out_of_range&) {
            user = me;
        }
        if (user.id != latest_user.id) {
            ui->add_history(latest_user, latest_user.name + ':', true, channel);
            latest_user = user;
        }
        std::string str = format_message(m["text"].asString());
        ts = std::stol(m["ts"].asString(), nullptr);
        std::string timestamp = ts_h_readable(ts);
        ui->add_history(user, timestamp + " " + str, false, channel);
    }
    // update oldest mesg ts
    ui->get_session().oldest_ts = ts;
    
    ui->get_session().has_more = d["has_more"].asBool();
}
