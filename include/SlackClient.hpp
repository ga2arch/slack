#include <iostream>
#include <cstdlib>

// WebSocket
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

using client = websocketpp::client<websocketpp::config::asio_tls_client>;
using message_ptr = websocketpp::config::asio_tls_client::message_type::ptr;
using context_ptr = websocketpp::lib::shared_ptr<boost::asio::ssl::context>;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// CurlCPP
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

// Rapidjson
#include "rapidjson/document.h"

class SlackClient {
    
public:
    void start();
    const std::string get_uri();
    
    void connect(std::string uri);
    void on_open(websocketpp::connection_hdl hdl);
    
    context_ptr on_tls_init(websocketpp::connection_hdl);
    
    void on_message(websocketpp::connection_hdl hdl, message_ptr ptr);
    
private:
    client wc;
};