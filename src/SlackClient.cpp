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
    auto d = call("rtm.start", v);

    if (d.IsNull()) {
        return "";
    }
    
    Log::d() << " OK" << std::endl;

    me.id = d["self"]["id"].GetString();

    const auto& users  = d["users"];
    const auto& ims    = d["ims"];
    const auto& groups = d["groups"];
    const auto& channels = d["channels"];
    
    const std::string company_name = d["team"]["name"].GetString();
    ui->set_company_name(company_name);
    
    // Get Users
    for (auto i = 0; i < users.Size(); i++) {
        const auto& u = users[i];

        std::string name = u["profile"]["real_name"].GetString();
        const std::string id = u["id"].GetString();
        const std::string status = u["presence"].GetString();

        name = name.empty() ? u["name"].GetString() : name;

        std::string channel;

        // Get IM Channel
        for (auto j = 0; j < ims.Size(); j++) {
            if (ims[j]["user"].GetString() == id) {
                channel = ims[j]["id"].GetString();
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
    for (auto i = 0; i < groups.Size(); i++) {
        const auto& g = groups[i];

        const std::string name = g["name"].GetString();
        const std::string channel = g["id"].GetString();

        ui->roster->add_group(channel, name);
    }
    
    // get channels
    for (auto i = 0; i < channels.Size(); i++) {
        const auto& c = channels[i];
        
        const std::string name = c["name"].GetString();
        const std::string channel = c["id"].GetString();
        
        ui->roster->add_channel(channel, name);
    }

    return d["url"].GetString();
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
        Document d;
        d.Parse(json.c_str());

        // other users messages
        if (d.HasMember("type") && d["type"] == "message") {
            // avoid first old message sent by slack...
            if (d.HasMember("reply_to")) {
                continue;
            }
            
            RosterItem user;
        
            try {
                user = ui->roster->get_user(d["user"].GetString());
            } catch (std::out_of_range&) {
                user = me;
            }
            std::string channel = d["channel"].GetString();
            if (ui->get_last_message_sender(channel) != user.id) {
                ui->add_message(user, user.name + ':', true, false, channel);
            }
            std::string str = format_message(d["text"].GetString());
            long ts = std::stol(d["ts"].GetString(), nullptr);
            if (ui->get_session().oldest_ts == 0) {
                ui->get_session().oldest_ts = ts - 1;
            }
            std::string timestamp = ts_h_readable(ts);
            ui->add_message(user, timestamp + " " + str, false, false, channel);
        }
    
        // my messages
        if (d.HasMember("ok") && d.HasMember("text")) {
            auto const reply_to = d["reply_to"].GetInt();
            me.channel = sent[reply_to];
            if (ui->get_last_message_sender(sent[reply_to]) != me.id) {
                ui->add_message(me, me.name + ':', true, true, me.channel);
            }
            std::string str = format_message(d["text"].GetString());
            long ts = std::stol(d["ts"].GetString(), nullptr);
            if (ui->get_session().oldest_ts == 0) {
                ui->get_session().oldest_ts = ts - 1;
            }
            std::string timestamp = ts_h_readable(ts);
            ui->add_message(me, timestamp + " " + str, false, true, me.channel);
        }
    
        // online/offline events
        if (d.HasMember("type") && d["type"] == "presence_change") {
            if (me.id != d["user"].GetString()) {
                const RosterItem &x = ui->roster->get_user(d["user"].GetString());
                ui->roster->change_status(d["presence"].GetString(), x);
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

Document SlackClient::call(const std::string &api, const std::vector<std::string> &query) {
    Document d;
    std::ostringstream os;
    
    const auto base_url = "https://slack.com/api/";
    auto url = base_url + api + "?token=" + auth_token;
    
    for (std::string str : query) {
        url = url + "&" + str;
    }
    
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_data);
    curl_easy_setopt(curl, CURLOPT_FILE, &os);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
       goto error;
    }
    
    long respcode; //response code of the http transaction
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respcode);
    
    if (respcode != 200) {
        goto error;
    }
    
    d.Parse(os.str().c_str());
    return d;
    
error:
    Log::d() << "An error occurred during the call." << std::endl;
    return nullptr; 
}

void SlackClient::send_message(const std::wstring& message) {
    StringBuffer buffer;
    auto channel = ui->roster->get_active_channel().c_str();

    Writer<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.String("id");
    writer.Uint(++sent_id);
    writer.String("type");
    writer.String("message");
    writer.String("channel");
    writer.String(channel);
    writer.String("text");
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string s = converter.to_bytes(message);
    writer.String(s);
    writer.EndObject();
    
    sent[sent_id] = channel;
    wc.send(buffer.GetString());
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
    if (!d.IsNull() && d["ok"].GetBool()) {
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
    
    if (d.IsNull() || !d.HasMember("messages")) {
        return;
    }
    
    long ts;
    
    const auto& messages  = d["messages"];
    
    try {
        latest_user = ui->roster->get_user(messages[0]["user"].GetString());
    } catch (std::out_of_range&) {
        latest_user = me;
    }
    
    for (auto i = 0; i < messages.Size(); i++) {
        const auto& m = messages[i];
        if (m["type"] != "message") {
            continue;
        }
        RosterItem user;
        try {
            user = ui->roster->get_user(m["user"].GetString());
        } catch (std::out_of_range&) {
            user = me;
        }
        if (user.id != latest_user.id) {
            ui->add_history(latest_user, latest_user.name + ':', true, channel);
            latest_user = user;
        }
        std::string str = format_message(m["text"].GetString());
        ts = std::stol(m["ts"].GetString(), nullptr);
        std::string timestamp = ts_h_readable(ts);
        ui->add_history(user, timestamp + " " + str, false, channel);
    }
    // update oldest mesg ts
    ui->get_session().oldest_ts = ts;
    
    ui->get_session().has_more = d["has_more"].GetBool();
}
