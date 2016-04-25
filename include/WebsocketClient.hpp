#pragma once

#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <mutex>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <curl/curl.h>

class WebsocketClient {
    
public:
    curl_socket_t socket;
    std::vector<std::string> events;

    bool connect(std::string uri) {
        auto temp = split(uri, '/');
        
        auto host = temp[2];
        auto path = "/" + temp[3] + "/" + temp[4];
        auto https_url = "https" + uri.substr(3, uri.length()-1);
        
        std::list<std::string> headers;
        headers.push_back("GET " + path + " HTTP/1.1");
        headers.push_back("Host: " + host );
        headers.push_back("Upgrade: websocket");
        headers.push_back("Connection: upgrade");
        headers.push_back("Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==");
        headers.push_back("Sec-WebSocket-Version: 13");
        
        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, https_url.c_str());
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
        CURLcode res = curl_easy_perform(curl);
        
        if (res == CURLE_OK) {
            long s;
            curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &s);
            socket = s;
            
            std::string data;

            for (const auto& h: headers) {
                data += h + "\r\n";
            }

            data += "\r\n";

            _send(data);
            ping_pong();
            return true;
        }
        Log::d() << "An error occurred during connection." << std::endl;
        return false;
    }
    
    std::vector<std::string> split(const std::string &s,
                                   char delim) {

        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;

        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }

        return elems;
    }
    
    void _send(std::string data) {
        size_t sent;

        auto status = curl_easy_send(curl, data.data(), data.size(), &sent);

        if (status != CURLE_OK || sent != data.size()) {
            throw std::exception();
        }
    }

    void receive() {
        std::string data;
        size_t received;

        char buff[4096] = {0};

        auto status = curl_easy_recv(curl, buff, 4096, &received);

        if (status != CURLE_OK) {
            Log::d() << "Failed to receive data." << std::endl;
            return;
        }
        // FIX ME, HANDLE HEADER  
        if (first) { first = false; return;};

        for (auto i = 0; i < received; i++) {
            data.push_back(buff[i]);
        }
           
        process_frame(data);

        data.clear();
        
    }
    
    void ping_pong() {
        std::thread([&]() {
            for (;;) {
                auto ping = "{\"id\":929380, \"type\":\"ping\"}";
                send(ping);
                sleep(5);
            }
        }).detach();
    }

    void process_frame(const std::string& data) {
        int offset = 0;

        while (data.size() > offset && data.size() > 3) {
            auto buff = std::string(data.begin()+offset, data.end());
            if (buff.size() < 3) break;

            int length = buff[1] & 127;
            int index = 2;

            if (length == 126) {

                length = 0;
                length |= ((unsigned char) buff[2]) << 8;
                length |= ((unsigned char) buff[3]) << 0;

                index = 4;

            } else if (length == 127) {
                
                length = 0;
                length |= ((uint64_t) buff[2]) << 56;
                length |= ((uint64_t) buff[3]) << 48;
                length |= ((uint64_t) buff[4]) << 40;
                length |= ((uint64_t) buff[5]) << 32;
                length |= ((uint64_t) buff[6]) << 24;
                length |= ((uint64_t) buff[7]) << 16;
                length |= ((uint64_t) buff[8]) << 8;
                length |= ((uint64_t) buff[9]) << 0;

                index = 10;

            }

            std::string event(buff.begin()+index, buff.begin()+index+length);
            events.push_back(event);
            offset = index+length;
        }
    }

    void send_frame(int type, const std::string& data) {
        std::string frame;

        const uint8_t masking_key[4] = { 0x12, 0x34, 0x56, 0x78 };
        auto len = data.length();
        
        frame.push_back(type);

        if (len <= 125) {
            frame.push_back( (len & 255) | 0x80 );

        } else if (len >= 126 && len <= 65535) {
            frame.push_back( 126 | 0x80 );
            frame.push_back( (len >> 8) & 255 );
            frame.push_back( len        & 255 );
            
        } else {
            frame.push_back( 127 | 0x80 );
            frame.push_back( (len >> 56) & 255 );
            frame.push_back( (len >> 48) & 255 );
            frame.push_back( (len >> 40) & 255 );
            frame.push_back( (len >> 32) & 255 );
            frame.push_back( (len >> 24) & 255 );
            frame.push_back( (len >> 16) & 255 );
            frame.push_back( (len >> 8)  & 255 );
            frame.push_back( len         & 255 );
        }

        frame.push_back( masking_key[0] );
        frame.push_back( masking_key[1] );
        frame.push_back( masking_key[2] );
        frame.push_back( masking_key[3] );

        for (auto i=0; i < len; i++)
            frame.push_back( data[i] ^ masking_key[i & 0x3] );
        
        try {
            _send(frame);
        } catch (std::exception &) {
            Log::d() << "Could not send." << std::endl;
        }

    }

    void send(const std::string& data) {
        send_frame(129, data);
    }

    ~WebsocketClient() {
        curl_easy_cleanup(curl);
    }

private:
    
    bool first = true;
    CURL *curl;
};
