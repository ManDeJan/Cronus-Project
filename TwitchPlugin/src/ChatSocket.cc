#pragma once
#include "ChatSocket.hh"
#include <fmt/core.h>

void ChatSocket::setup_wsa() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) { throw std::runtime_error(fmt::format("WSAStartup failed: {}", iResult)); }
}

void ChatSocket::setup_addrinfo() {
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    auto iResult = getaddrinfo("irc.chat.twitch.tv", "6667", &hints, &twitch_chat_addr);
    if (iResult != 0) { throw std::runtime_error(fmt::format("getaddrinfo failed: {}\n", iResult)); }
}

void ChatSocket::setup_socket() {
    ConnectSocket = socket(twitch_chat_addr->ai_family, twitch_chat_addr->ai_socktype, twitch_chat_addr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        throw std::runtime_error(fmt::format("Error at socket(): {}", WSAGetLastError()));
    }
}

ChatSocket::ChatSocket() {
    try {
        setup_wsa();
        setup_addrinfo();
        setup_socket();
    } catch (std::exception &e) {
        fmt::print(stderr, "Exception {}\n", e.what());
        this->~ChatSocket();
        throw;
    }
}

void ChatSocket::run() {
    std::array<char, 100> buffer{};
    std::string message;
    while (running.load()) {
        if (ConnectSocket == INVALID_SOCKET || !connected.load()) { continue; }
        int received = recv(ConnectSocket, buffer.data(), buffer.size(), 0);
        if (received == 0) {
            fmt::print("Connection closed\n");
            connected.store(false);
            continue;
        }
        if (received < 0) { throw std::runtime_error(fmt::format("Error sending msg {}", received)); }
        message.append(std::string_view(buffer.data(), received));
        size_t nl = 0;
        do {

            nl = message.find_first_of('\n');
            if (nl != std::string::npos) {
                fmt::print("received msg {}\n", message.substr(0, nl + 1));
                messages.push(message.substr(0, nl + 1));
                if (nl == message.length()) {
                    message.clear();
                } else {

                    message = message.substr(nl + 1);
                }
            }
        } while (nl != std::string::npos);
    }
}

void ChatSocket::connect() {
    auto iResult = ::connect(ConnectSocket, twitch_chat_addr->ai_addr, (int)twitch_chat_addr->ai_addrlen);
    if (iResult == SOCKET_ERROR) { throw std::runtime_error("Socket error"); }
    connected.store(true);
}

void ChatSocket::disconnect() {
    if (connected.load()) {
        shutdown(ConnectSocket, SD_BOTH);
        connected.store(false);
    }
}

void ChatSocket::send(std::string_view f) const {
    fmt::print("Sending msg: {}\n", f);
    auto iResult = ::send(ConnectSocket, f.data(), f.length(), 0);
    if (iResult == SOCKET_ERROR) { throw std::runtime_error(fmt::format("Socket error {}", WSAGetLastError())); };
}

ChatSocket::~ChatSocket() {
    if (ConnectSocket != INVALID_SOCKET) { closesocket(ConnectSocket); }
    if (twitch_chat_addr) { freeaddrinfo(twitch_chat_addr); }
    WSACleanup();
}
