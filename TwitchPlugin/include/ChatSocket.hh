#pragma once
#include "AtomicQueue.hh"
#include <atomic>
#include <string>

// clang-format off
// order is super important for windows .....
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <ws2tcpip.h>
// clang-format on

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
