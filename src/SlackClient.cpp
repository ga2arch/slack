#include "SlackClient.hpp"

void SlackClient::start() {


    connect(fetch_data());
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

const std::string SlackClient::fetch_data() {
    std::map<std::string, std::string> users_status;

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

        users_status.emplace(id, u["presence"].GetString());

        ui->roster->add_user(id, name, channel);
    }

    // Get Groups
    for (auto i = 0; i < groups.Size(); i++) {
        const auto& g = groups[i];

        const std::string name = g["name"].GetString();
        const std::string channel = g["id"].GetString();

        ui->roster->add_group(channel, name);
    }

    ui->roster->draw(users_status);

    return d["url"].GetString();
}

void SlackClient::connect(const std::string& uri) {
    Log::d() << "Attempting connection ...";

    wc.set_access_channels(websocketpp::log::alevel::none);
    wc.init_asio();

    wc.set_open_handler(bind(&SlackClient::on_open, this, ::_1));
    wc.set_tls_init_handler(bind(&SlackClient::on_tls_init, this, ::_1));
    wc.set_message_handler(bind(&SlackClient::on_message, this, ::_1, ::_2));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = wc.get_connection(uri, ec);

    if (ec) {
        Log::d() << "> Connect initialization error: " << ec.message() << std::endl;
        return;
    }

    wc.connect(con);
    wc.run();
}

void SlackClient::on_open(websocketpp::connection_hdl hdl) {
    this->hdl = hdl;
    Log::d() << "  Connected !" << std::endl;
}

context_ptr SlackClient::on_tls_init(websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv1);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return ctx;
}

void SlackClient::on_message(websocketpp::connection_hdl hdl, message_ptr ptr) {
    Log::d() << "Received: " << ptr->get_payload() << std::endl;
    process_event(ptr->get_payload());
}

void SlackClient::process_event(const std::string& json) {
    Document d;
    std::ostringstream o;

    d.Parse(json.c_str());

    if (d.HasMember("type") && d["type"] == "message") {
        auto user = me;

        try {
            user = ui->roster->get_user(d["user"].GetString());
        } catch (std::out_of_range&) {
            user.channel = d["channel"].GetString();
        }

        o << user.name << ": " << d["text"].GetString();

        ui->add_message(user, o.str());
        if (user.channel == ui->roster->get_active_channel()) {
            ui->chat->draw(ui->get_session());
        } else {
            ui->roster->highlight_user(user.id);
        }
        o.clear();
    }

    if (d.HasMember("ok") && d.HasMember("text")) {
        auto const reply_to = d["reply_to"].GetInt();
        o << me.name << ": " << d["text"].GetString();
        me.channel = sent[reply_to];

        ui->add_message(me, o.str());
        if (me.channel == ui->roster->get_active_channel()) {
            ui->chat->draw(ui->get_session());
        }
        o.clear();
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
    websocketpp::lib::error_code ec;
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
    writer.String(message.c_str());
    writer.EndObject();

    sent[sent_id] = channel;
    wc.send(hdl, buffer.GetString(), websocketpp::frame::opcode::text, ec);

    if (ec) {
        Log::d() << "> Sending message error: " << ec.message() << std::endl;
        return;
    }
}