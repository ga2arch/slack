#include "SlackClient.hpp"

void SlackClient::start() {
    connect(get_uri());
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

const std::string SlackClient::get_uri() {
    fetch_user_info();
    fetch_roster();

    Log::d() << "Getting websocker url ...";

    auto d = call("rtm.start", "");

    Log::d() << " OK" << std::endl;

    return d["url"].GetString();
}

void SlackClient::connect(std::string uri) {
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
    my_hdl = hdl;
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
        const auto name = roster[d["user"].GetString()];
        o << name << ": " << d["text"].GetString();

        ui->add_message(o.str());
        o.clear();
    }

    if (d.HasMember("ok") && d.HasMember("text")) {
        o << me << ": " << d["text"].GetString();

        ui->add_message(o.str());
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

    Writer<StringBuffer> writer(buffer);
    writer.StartObject();
    writer.String("id");
    writer.Uint(3);
    writer.String("type");
    writer.String("message");
    writer.String("channel");
    writer.String("D0BTB0RLY");
    writer.String("text");
    writer.String(message.c_str());
    writer.EndObject();

    wc.send(my_hdl, buffer.GetString(), websocketpp::frame::opcode::text, ec);

    if (ec) {
        Log::d() << "> Sending message error: " << ec.message() << std::endl;
        return;
    }
}

void SlackClient::fetch_roster() {
    auto d = call("users.list", "");

    const auto& members = d["members"];

    for (auto i=0; i<members.Size(); i++) {
        const auto& m = members[i];
        
        std::string name     = m["profile"]["real_name"].GetString();
        const std::string id = m["id"].GetString();

        name = name.empty() ? m["name"].GetString() : name;
        
        if (name == me) continue;
        
        roster[id] = name;
        ui->add_user(name);
    }
}

void SlackClient::fetch_user_info() {
    auto d = call("auth.test", "");
    const std::string user_id = d["user_id"].GetString();

    d = call("users.info", "user=" + user_id);
    me = d["user"]["profile"]["real_name"].GetString();
    
    me = me.empty() ? d["user"]["name"].GetString() : me;
}



