#include "SlackClient.hpp"

void SlackClient::start() {
    std::thread u([&]() { fetch_users(); });
    fetch_groups();
    fetch_user_info();
    
    ui->roster->draw();
    connect(get_uri());
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

const std::string SlackClient::get_uri() {
    Log::d() << "Getting websocket url ...";

    auto d = call("rtm.start", "");

    Log::d() << " OK" << std::endl;

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

void SlackClient::fetch_users() {
    Log::d() << "Fetching users" << std::endl;

    auto d = call("users.list", "");

    const auto& members = d["members"];

    for (auto i = 0; i < members.Size(); i++) {
        const auto& m = members[i];

        std::string name = m["profile"]["real_name"].GetString();
        const std::string id = m["id"].GetString();

        name = name.empty() ? m["name"].GetString() : name;

        if (name == me.name) continue;

        const auto& channel = get_direct_channel(id);
        ui->roster->add_user(id, name, channel);
    }
}

void SlackClient::fetch_groups() {
    Log::d() << "Fetching groups" << std::endl;
    
    auto d = call("groups.list", "");

    const auto& members = d["groups"];

    for (auto i = 0; i < members.Size(); i++) {
        const auto& m = members[i];

        const std::string name = m["name"].GetString();
        const std::string channel = m["id"].GetString();

        ui->roster->add_group(channel, name);
    }
}

void SlackClient::fetch_user_info() {
    Log::d() << "Fetching user info" << std::endl;

    auto d = call("auth.test", "");
    
    const std::string user_id = d["user_id"].GetString();

    d = call("users.info", "user=" + user_id);
    std::string name = d["user"]["profile"]["real_name"].GetString();
    name = name.empty() ? d["user"]["name"].GetString() : name;
    
    ui->roster->remove_user(user_id);
    me = RosterItem(user_id, name, ui->roster->get_active_channel());
}

std::string SlackClient::get_direct_channel(const std::string& userid) {
    auto d = call("im.open", "user=" + userid);
    return d["channel"]["id"].GetString();
}


