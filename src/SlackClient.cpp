#include "SlackClient.hpp"

void SlackClient::start() {
    connect(get_uri());
}

const std::string SlackClient::get_uri() {
    std::cout << "Getting websocker url ...";
    
    std::string token = std::getenv("SLACK_TOKEN");
    
    if (token.empty()) {
        throw new std::runtime_error("SLACK_TOKEN is empty");
    }
    
    std::ostringstream os;
    os << curlpp::options::Url("https://slack.com/api/rtm.start?token=" + token);
    
    rapidjson::Document d;
    d.Parse(os.str().c_str());
    
    std::cout << " OK" << std::endl;
    
    return d["url"].GetString();
}

void SlackClient::connect(std::string uri) {
    std::cout << "Attempting connection ...";
    
    wc.set_access_channels(websocketpp::log::alevel::none);
    wc.init_asio();
    
    wc.set_open_handler(bind(&SlackClient::on_open,this,::_1));
    wc.set_tls_init_handler(bind(&SlackClient::on_tls_init,this,::_1));
    wc.set_message_handler(bind(&SlackClient::on_message,this,::_1,::_2));
    
    websocketpp::lib::error_code ec;
    client::connection_ptr con = wc.get_connection(uri, ec);
    
    if (ec) {
        std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        return;
    }
    
    wc.connect(con);
    wc.run();
}

void SlackClient::on_open(websocketpp::connection_hdl hdl) {
    //wc.send(hdl, "ciao", websocketpp::frame::opcode::text);
    std::cout << "Connected !" << std::endl;
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
    std::cout << "Received: " << ptr->get_payload() << std::endl;
}



