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

    using OnMessageCallback = std::function<void(std::string)>;
    
public:
    void connect(std::string uri) {
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
        curl_easy_perform(curl);

        long s;
        auto status = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &s);
        socket = s;
        
        std::string data;

        for (const auto& h: headers) {
            data += h + "\r\n";
        }

        data += "\r\n";

        _send(data);
        ping_pong();

        receive();
    }

    void set_on_message(OnMessageCallback cb) {
        on_message = cb;
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

    int wait_on_socket(bool for_recv) {
        int result;
        fd_set infd, outfd, errfd;

        do {
            FD_ZERO(&infd);
            FD_ZERO(&outfd);
            FD_ZERO(&errfd);

            FD_SET(socket, &errfd);

            if (for_recv) {
                FD_SET(socket, &infd);
            } else {
                FD_SET(socket, &outfd);
            }

            result = select(socket + 1, &infd, &outfd, &errfd, nullptr);
        } while (result == -1 && errno == EINTR);

        return result;
    }
    
    void _send(std::string data) {
        if (wait_on_socket(0) < 0) {
            throw std::out_of_range("");
        }

        size_t sent;

        auto status = curl_easy_send(curl, data.data(), data.size(), &sent);

        if (status != CURLE_OK || sent != data.size()) {
            throw std::exception();
        }
    }

    void receive() {
        std::string data;
        size_t received;
        bool first = true;

        for (;;) {
            char buff[4096] = {0};

            wait_on_socket(1);
            auto status = curl_easy_recv(curl, buff, 4096, &received);
 
            if (status != CURLE_OK) {
                Log::d() << "Failed to receive data." << std::endl;
                break;
            }
            // FIX ME, HANDLE HEADER  
            if (first) { first = false; continue;};

            for (auto i=0; i < received; i++)
                data.push_back(buff[i]);
            
            for (auto& event: process_frame(data)) {
                if (on_message != nullptr) {
                    std::async(std::launch::async, [&]() { on_message(event); });
                }
            }

            data.clear();
        }
        
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

    std::vector<std::string> process_frame(const std::string& data) {
        std::vector<std::string> events;

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
        
        return events;
    }

    void send_frame(int type, const std::string& data) {
        lock.lock();
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

        lock.unlock();
    }

    void send(const std::string& data) {
        send_frame(129, data);
    }

    ~WebsocketClient() {
        curl_easy_cleanup(curl);
    }

private:
    OnMessageCallback on_message;

    std::mutex lock;
    CURL *curl;
    curl_socket_t socket;
};
