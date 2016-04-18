#include "SlackClient.hpp"

void SlackClient::start(const std::string token) {
    auth_token = token;
    connect(fetch_data());
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

const std::string SlackClient::fetch_data() {
    Log::d() << "Getting websocket url ...";
    
    std::vector<std::string> v;
    auto d = call("rtm.start", v);

    Log::d() << " OK" << std::endl;

    me.id = d["self"]["id"].GetString();

    const auto& users  = d["users"];
    const auto& ims    = d["ims"];
    const auto& groups = d["groups"];
    //const auto& channels = d["channels"];
    
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
        for (auto j=0; j < ims.Size(); j++) {
            if (ims[j]["user"].GetString() == id) {
                channel = ims[j]["id"].GetString();
            }
        }

        if (id == me.id) {
            me.name = name;
            me.channel = channel;
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

    ui->roster->draw();
    ui->ui_lock.unlock();

    return d["url"].GetString();
}

void SlackClient::connect(const std::string& uri) {
    Log::d() << "Attempting connection ..." << std::endl;
    
    wc.set_on_message([&](std::string event) {
        process_event(event);
    });
    wc.connect(uri);
}

void SlackClient::process_event(const std::string& json) {
    Document d;

    d.Parse(json.c_str());
    
    // other users messages
    if (d.HasMember("type") && d["type"] == "message") {
        RosterItem user;
        
        try {
            user = ui->roster->get_user(d["user"].GetString());
        } catch (std::out_of_range&) {
            return;
        }

        if (ui->get_last_message_sender(user.channel) != user.id) {
            ui->add_message(user, user.name + ':', true, false);
        }
        std::string str = format_message(d["text"].GetString());
        std::string timestamp = ts_h_readable(d["ts"].GetString());
        ui->add_message(user, timestamp + " " + str, false, false);
    }
    
    // my messages
    if (d.HasMember("ok") && d.HasMember("text")) {
        auto const reply_to = d["reply_to"].GetInt();
        me.channel = sent[reply_to];
        if (ui->get_last_message_sender(sent[reply_to]) != me.id) {
            ui->add_message(me, me.name + ':', true, true);
        }
        std::string str = format_message(d["text"].GetString());
        std::string timestamp = ts_h_readable(d["ts"].GetString());
        ui->add_message(me, timestamp + " " + str, false, true);
    }
    
    // online/offline events
    if (d.HasMember("type") && d["type"] == "presence_change") {
        if (me.id != d["user"].GetString()) {
            const RosterItem &x = ui->roster->get_user(d["user"].GetString());
            ui->roster->change_status(d["presence"].GetString(), x);
        }
    }
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
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    assert(res == CURLE_OK);
    
    long respcode; //response code of the http transaction
    curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &respcode);
    
    assert(respcode == 200);
    
    d.Parse(os.str().c_str());
    return d;
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

std::string SlackClient::ts_h_readable(const std::string& rawtime) {
    struct tm * dt;
    char buffer [30];
    long time = std::stol(rawtime, nullptr);
    dt = localtime(&time);
    strftime(buffer, sizeof(buffer), "%H:%M", dt);
    return std::string(buffer);
}

void SlackClient::update_mark() {
    auto channel = ui->roster->get_active_channel();
    auto type = ui->roster->get_active_type();
    auto now = std::time(nullptr);
    std::stringstream ss;
    ss << now;
    
    std::vector<std::string> parameters;
    parameters.push_back("channel=" + channel);
    parameters.push_back("ts=" + ss.str());
    
    call(type + ".mark", parameters);
}
