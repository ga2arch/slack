#include "slackclient.h"

void SlackClient::start() {
    connect(fetch_data());
}

const std::string SlackClient::fetch_data() {
    QJsonObject d = call("rtm.start", "").object();

    const auto& users  = d["users"].toArray();
    const auto& ims    = d["ims"].toArray();
    const auto& groups = d["groups"].toArray();

       // Get Users
       for (const auto& x : users) {
           QJsonObject u = x.toObject();
           QString name = u["profile"]["real_name"].toString();
           const QString id = u["id"].toString();
           const QString status = u["presence"].toString();

           name = name.empty() ? u["name"].toString() : name;

           QString channel;

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

void SlackClient::process_event(const std::string& json) {
    QJsonDocument d;

    d.toJson(json.c_str());

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
        ui->add_message(user, str);
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
        ui->add_message(me, str);
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

QJsonDocument SlackClient::call(const std::string &api, const std::string &query) {
    QJsonDocument d;
    std::ostringstream os;

    const auto token = std::getenv("SLACK_TOKEN");
    const auto base_url = "https://slack.com/api/";
    const auto url = base_url + api + "?token=" + token + "&" + query;

    os << curlpp::options::Url(url);
    d.fromJson(os.str().c_str());

    return d;
}

SlackClient::connect(const std::string& uri) {
    connect(&m_webSocket, &QWebSocket::connected, this, &SlackClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SlackClient::closed);
    m_webSocket.open(QUrl(uri));
}

void SlackClient::onConnected()
{
    ui->add_message("connected!");
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &SlackClient::process_event);
    m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}


void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

void SlackClient::closed() {
    m_webSocket.close();
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
         m_webSocket.sendTextMessage(QString(buffer));

 }
