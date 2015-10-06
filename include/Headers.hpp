//
//  Headers.hpp
//  slack++
//
//  Created by Gabriele Carrettoni on 03/10/15.
//
//

#ifndef Headers_h
#define Headers_h

#include <iostream>
#include <cstdlib>
#include <thread>

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
#include <rapidjson/document.h>
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Log.h"

#endif /* Headers_h */
