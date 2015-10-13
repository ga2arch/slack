#include "SlackClient.hpp"

void SlackClient::start() {
    connect(fetch_data());
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

std::string SlackClient::get_uri() {
    auto d = call("rtm.start", "");
    return d["url"].GetString();
}

const std::string SlackClient::fetch_data() {
    Log::d() << "Getting websocket url ...";

    auto d = call("rtm.start", "");

    Log::d() << " OK" << std::endl;

    me.id = d["self"]["id"].GetString();

    const auto& users  = d["users"];
    const auto& ims    = d["ims"];
    const auto& groups = d["groups"];

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
        lock.lock();
        process_event(event);
        lock.unlock();
    });

    wc.connect(uri);
}

void SlackClient::process_event(const std::string& json) {
    Document d;
    
    d.Parse(json.c_str());

    if (d.HasMember("type") && d["type"] == "message") {
        auto user = me;

        try {
            user = ui->roster->get_user(d["user"].GetString());
        } catch (std::out_of_range&) {
            user.channel = d["channel"].GetString();
        }

        if (ui->get_last_message_sender(user.channel) != user.id) {
            ui->add_message(user, user.name);
            if ((user.channel == ui->roster->get_active_channel()) && (ui->ready)) {
                ui->chat->draw(ui->get_session());
            }
        }
        std::string str = format_message(d["text"].GetString());
        ui->add_message(user, " " + str);
        if ((user.channel == ui->roster->get_active_channel()) && (ui->ready)) {
            ui->chat->draw(ui->get_session());
        } else {
            ui->roster->highlight_user(user.channel);
        }
    }

    if (d.HasMember("ok") && d.HasMember("text")) {
        auto const reply_to = d["reply_to"].GetInt();
        me.channel = sent[reply_to];
        if (ui->get_last_message_sender(sent[reply_to]) != me.id) {
            ui->add_message(me, me.name);
            if ((me.channel == ui->roster->get_active_channel()) && (ui->ready)) {
                ui->chat->draw(ui->get_session());
            }
        } 
        std::string str = format_message(d["text"].GetString());
        ui->add_message(me, " " + str);
        if ((me.channel == ui->roster->get_active_channel()) && (ui->ready)) {
            ui->chat->draw(ui->get_session());
        } else {
            ui->roster->highlight_user(me.channel);
        }
    }
    // online/offline events
    if (d.HasMember("type") && d["type"] == "presence_change") { // why it throws an exception here during program startup?
        if (me.id != d["user"].GetString()) {
            const RosterItem &x = ui->roster->get_user(d["user"].GetString());
            ui->roster->change_status(d["presence"].GetString(), x);
        }
    }
}

Document SlackClient::call(const std::string &api, const std::string &query) {
    Document d;
    std::ostringstream os;

    const auto token = std::getenv("SLACK_TOKEN");
    const auto base_url = "https://slack.com/api/";
    const auto url = base_url + api + "?token=" + token + "&" + query;

    os << curlpp::options::Url(url);
    d.Parse(os.str().c_str());

    return d;
}

void SlackClient::send_message(const std::string& message) {
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
    writer.String(message);
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