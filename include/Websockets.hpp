//
//  Websockets.hpp
//  slack++
//
//  Created by Gabriele Carrettoni on 10/10/15.
//
//

#ifndef Websockets_h
#define Websockets_h

#include <iostream>
#include <sstream>

#include <sys/time.h>
#include <sys/types.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>


class Websockets {
    
public:
    Websockets(const std::string& _host,
               const std::string& _path): host(_host), path(_path) {
    }
    
    void connect() {
        https_url = "https://" + host + path;
        std::cerr << https_url;

        std::list<std::string> headers;
        headers.push_back("GET " + path + " HTTP/1.1");
        headers.push_back("Host: " + host + ":443" );
        headers.push_back("Upgrade: websocket");
        headers.push_back("Connection: upgrade");
        headers.push_back("Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==");
        headers.push_back("Sec-WebSocket-Protocol: chat");
        headers.push_back("Sec-WebSocket-Version: 13");

        curl = curl_easy_init();
        curl_easy_setopt(curl, CURLOPT_URL, https_url.c_str());
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_perform(curl);
        
        long s;
        auto status = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &s);
        socket = s;
        
        if (status != CURLE_OK) {
            std::cerr << "error" << std::endl;
        } else
            std::cerr << "connected" << std::endl;
        
        std::string data;
        
        for (const auto& h: headers) {
            data += h + "\r\n";
        }
        
        data += "\r\n";
        
        send(data);
        std::cerr << receive() << std::endl;
        
        std::cerr << "Received: " << recv_frame() << std::endl;
    }
    
    int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms) {
        struct timeval tv;
        fd_set infd, outfd, errfd;
        int res;
        
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec= (timeout_ms % 1000) * 1000;
        
        FD_ZERO(&infd);
        FD_ZERO(&outfd);
        FD_ZERO(&errfd);
        
        FD_SET(sockfd, &errfd); /* always check for error */
        
        if(for_recv) {
            FD_SET(sockfd, &infd);
        }
        else {
            FD_SET(sockfd, &outfd);
        }
        
        /* select() returns the number of signalled sockets or -1 */
        res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
        return res;
    }
    
    void send(const std::string& data) {
        if(!wait_on_socket(socket, 0, 600L)) {
            throw std::out_of_range("");
        }
        
        size_t sent;

        auto status = curl_easy_send(curl, data.c_str(), data.length(), &sent);
        
        if (status != CURLE_OK || sent != data.length()) {
            throw std::exception();
        }
    }
    
    std::string receive() {
        std::string data;
        size_t received;
        
        for (;;) {
            char buff[1024]={0};

            wait_on_socket(socket, 1, 60000L);
            auto status = curl_easy_recv(curl, buff, 1024, &received);
            
            if (status != CURLE_OK) {
                break;
            }
            
            data += buff;
        }
        
        return data;
    }
    
    void send_frame(const std::string& data) {
        std::string frame;

        auto len = data.length();
        frame.push_back(129);
        
        if (len <= 125) {
            frame.push_back(len);
            
        } else if (len >= 126 && len <= 65535) {
            frame.push_back(126);
            frame.push_back( (len >> 8) & 255 );
            frame.push_back( len        & 255 );
            
        } else {
            frame.push_back(127);
            frame.push_back( (len >> 56) & 255 );
            frame.push_back( (len >> 48) & 255 );
            frame.push_back( (len >> 40) & 255 );
            frame.push_back( (len >> 32) & 255 );
            frame.push_back( (len >> 24) & 255 );
            frame.push_back( (len >> 16) & 255 );
            frame.push_back( (len >> 8)  & 255 );
            frame.push_back( len         & 255 );
        }
        
        for (auto c: data)
            frame.push_back(c);

        send(frame);
    }
    
    std::string recv_frame() {
        auto frame = receive();
        
        auto length = frame[1] & 127;
        int index = 2;
        
        if (length == 126) {
            index = 4;
            
        } else if (length == 127) {
            index = 10;
        }
        
        return std::string(frame.begin()+index, frame.end());
    }
    
    ~Websockets() {
        curl_easy_cleanup(curl);
    }
    
private:
    std::string host;
    std::string path;
    
    std::string https_url;
    
    CURL* curl;
    curl_socket_t socket;
    
};

#endif /* Websockets_h */
