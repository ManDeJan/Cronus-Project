#pragma once
#include "AtomicQueue.hh"
#include <atomic>
#include <string>

#include <shellapi.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

class ChatSocket {
private:
    SOCKET ConnectSocket = INVALID_SOCKET;
    addrinfo *twitch_chat_addr = nullptr;

    void setup_wsa();
    void setup_addrinfo();
    void setup_socket();

public:
    std::atomic_bool running;
    std::atomic_bool connected;
    AtomicQueue<std::string> messages;

    ChatSocket();

    ~ChatSocket();

    void run();

    void connect();
    void disconnect();
    void send(std::string_view f) const;
};
