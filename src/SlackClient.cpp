#include "SlackClient.hpp"

void SlackClient::start() {
    connect(get_uri());
}

void SlackClient::set_ui(SlackUI* ui) {
    this->ui = ui;
}

void SlackClient::send_message(const std::string& message) {
    websocketpp::lib::error_code ec;

    wc.send(my_hdl, message, websocketpp::frame::opcode::text, ec);

    if (ec) {
        std::cerr << "> Sending message error: " << ec.message() << std::endl;
        return;
    }
}

const std::string SlackClient::get_uri() {
    fetch_roster();

    std::cerr << "Getting websocker url ...";

    auto d = call("rtm.start", "");

    std::cerr << " OK" << std::endl;

    return d["url"].GetString();
}

void SlackClient::connect(std::string uri) {
    std::cerr << "Attempting connection ...";

    wc.set_access_channels(websocketpp::log::alevel::none);
    wc.init_asio();

    wc.set_open_handler(bind(&SlackClient::on_open, this, ::_1));
    wc.set_tls_init_handler(bind(&SlackClient::on_tls_init, this, ::_1));
    wc.set_message_handler(bind(&SlackClient::on_message, this, ::_1, ::_2));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = wc.get_connection(uri, ec);

    if (ec) {
        std::cerr << "> Connect initialization error: " << ec.message() << std::endl;
        return;
    }

    wc.connect(con);
    wc.run();
}

void SlackClient::on_open(websocketpp::connection_hdl hdl) {
    my_hdl = hdl;
    std::cerr << "  Connected !" << std::endl;
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
    std::cerr << "Received: " << ptr->get_payload() << std::endl;
    process_event(ptr->get_payload());
}

void SlackClient::process_event(const std::string& json) {
    Document d;
    std::ostringstream o;

    d.Parse(json.c_str());

    if (d["type"] == "message") {
        const auto name = roster[d["user"].GetString()];
        o << name << ": " << d["text"].GetString();

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

void SlackClient::fetch_roster() {
    auto d = call("users.list", "");

    const auto& members = d["members"];

    for (auto i=0; i<members.Size(); i++) {
        const auto name = members[i]["profile"]["real_name"].GetString();
        const auto id   = members[i]["id"].GetString();

        roster[id] = name;
        ui->add_user(name);
    }
}



