#include "DynRPG/Character/AnimationType.hpp"
#include "DynRPG/DynCore/PluginCallbacks.hpp"
#include "DynRPG/Map/Map.hpp"
#include <DynRPG/DynRPG.hpp>

#include <fmt/core.h>
#include <../test/scan.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <shellapi.h>

#include <httplib.h>

#include <thread>
#include <memory>
#include <string_view>
#include <atomic>
#include <string>
#include <queue>

struct TwitchOAUTHServer
{
    httplib::Server svr;
    std::unique_ptr<std::thread> http_thread;
    std::string oauth_token;

    TwitchOAUTHServer()
    {
        svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
            res.set_content("Hello World!", "text/plain");
        });
        svr.Get("/oauth_redirect", [](const httplib::Request &, httplib::Response &res) {
            res.set_content(R"(<h1>success</h1>
            <script>
            var myHeaders = new Headers();
            myHeaders.append("Content-Type", "text/plain");

            var requestOptions = {
            method: 'POST',
            headers: myHeaders,
            body: document.location.hash.replace(/#access_token=([^&]+).*/g,"$1"),
            redirect: 'follow'
            };

            fetch("http://localhost:42069/oauth_token", requestOptions)
            .then(result => window.close())
            </script>
            )",
                            "text/html");
        });
        svr.Post("/oauth_token", [&](const httplib::Request &req, httplib::Response &res) {
            fmt::print("Oauth token is {}\n", req.body);
            oauth_token = req.body;
            res.set_content("Ok", "text/plain");
        });
    }
    void run()
    {
        fmt::print("Starting http server\n");
        http_thread = std::make_unique<std::thread>([&]() { svr.listen("0.0.0.0", 42069); });
    };
    void stop()
    {
        if (!http_thread)
        {
            fmt::print(stderr, "Server not running, can't stop nothing\n");
            return;
        }
        fmt::print("Stopping http server\n");
        svr.stop();
        http_thread->join();
    };

    void openLoginBrowser()
    {
        ShellExecuteA(
            0,
            0,
            "https://id.twitch.tv/oauth2/authorize?client_id=xic4pienyb7kqh5hat3ona545wsmyw&redirect_uri=http://localhost:42069/oauth_redirect&response_type=token&scope=chat:read chat:edit",
            0,
            0,
            SW_SHOW);
    }
};

template <typename T>
class atomic_queue
{
public:
    void push(const T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queque.push(value);
    }

    std::optional<T> pop()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queque.empty())
        {
            return std::nullopt;
        }
        T t = m_queque.front();
        m_queque.pop();
        return t;
    }

    T pop_blocking()
    {
        std::optional<T> t;
        while (!(t = pop()))
        {
        }
        return *t;
    }

    std::optional<T> peek() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queque.empty())
        {
            return std::nullopt;
        }
        return m_queque.front();
    }

private:
    std::queue<T> m_queque;
    mutable std::mutex m_mutex;
};

class ChatSocket
{
private:
    SOCKET ConnectSocket = INVALID_SOCKET;
    addrinfo *twitch_chat_addr;

    void setup_wsa()
    {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0)
        {
            throw(fmt::format("WSAStartup failed: {}", iResult));
        }
    }

    void setup_addrinfo()
    {
        addrinfo hints{};
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        auto iResult = getaddrinfo("irc.chat.twitch.tv", "6667", &hints, &twitch_chat_addr);
        if (iResult != 0)
        {
            throw std::runtime_error(fmt::format("getaddrinfo failed: {}\n", iResult));
        }
    }

    void setup_socket()
    {
        ConnectSocket = socket(twitch_chat_addr->ai_family, twitch_chat_addr->ai_socktype,
                               twitch_chat_addr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            throw std::runtime_error(fmt::format("Error at socket(): {}", WSAGetLastError()));
        }
    }

public:
    std::atomic_bool running;
    std::atomic_bool connected;
    atomic_queue<std::string> messages;

    ChatSocket()
    {
        try
        {
            setup_wsa();
            setup_addrinfo();
            setup_socket();
        }
        catch (std::exception &e)
        {
            fmt::print(stderr, "Exception {}\n", e.what());
            this->~ChatSocket();
            throw;
        }
    }

    ~ChatSocket()
    {
        if (ConnectSocket != INVALID_SOCKET)
        {
            closesocket(ConnectSocket);
        }
        if (twitch_chat_addr)
        {
            freeaddrinfo(twitch_chat_addr);
        }
        WSACleanup();
    }

    void run()
    {
        std::array<char, 100> buffer;
        std::string message;
        while (running.load())
        {
            if (ConnectSocket == INVALID_SOCKET || !connected.load())
            {
                continue;
            }
            int received = recv(ConnectSocket, buffer.data(), buffer.size(), 0);
            if (received == 0)
            {
                fmt::print("Connection closed\n");
                connected.store(false);
                continue;
            }
            if (received < 0)
            {
                throw std::runtime_error(fmt::format("Error sending msg {}", received));
            }
            message.append(std::string_view(buffer.data(), received));
            size_t nl = 0;
            do
            {

                nl = message.find_first_of('\n');
                if (nl != std::string::npos)
                {
                    fmt::print("received msg {}\n", message.substr(0, nl + 1));
                    messages.push(message.substr(0, nl + 1));
                    if (nl == message.length())
                    {
                        message.clear();
                    }
                    else
                    {

                        message = message.substr(nl + 1);
                    }
                }
            } while (nl != std::string::npos);
        }
    };

    void connect()
    {
        auto iResult = ::connect(ConnectSocket, twitch_chat_addr->ai_addr, (int)twitch_chat_addr->ai_addrlen);
        if (iResult == SOCKET_ERROR)
        {
            throw std::runtime_error("Socket error");
        }
        connected.store(true);
    }
    void disconnect()
    {
        if (connected.load())
        {
            shutdown(ConnectSocket, SD_BOTH);
            connected.store(false);
        }
    }
    void send(std::string_view f)
    {
        auto iResult = ::send(ConnectSocket, f.data(), f.length(), 0);
        if (iResult == SOCKET_ERROR)
        {
            throw std::runtime_error(fmt::format("Socket error {}", WSAGetLastError()));
        };
    };
};

class TwitchChat
{
private:
    std::thread ChatThread;
    ChatSocket cs;

public:
    decltype(cs.messages) &message_queue = cs.messages;

    void connect()
    {
        cs.connect();
    }
    void run()
    {
        cs.running.store(true);
        ChatThread = std::thread([&]() { 
        try{
            cs.run();
         }
         catch(std::exception &e){
             fmt::print(stderr, "Error {}", e.what());
             throw;
         } });
    };

    void stop()
    {
        cs.running.store(false);
        cs.disconnect();
        ChatThread.join();
    }

    void login(std::string_view oauth_token, std::string_view user)
    {
        assert(oauth_token.find("\n") == std::string_view::npos);
        assert(user.find("\n") == std::string_view::npos);

        auto login_msg = fmt::format("PASS oauth:{}\nNICK {}\n", oauth_token, user);
        fmt::print("Trying to login with msg: \n{}\n", login_msg);
        cs.send(login_msg);
        std::string msg;
        for (int i = 0; i < 7; i++)
        {
            msg = message_queue.pop_blocking();
        }
        if(msg.find(" 376 ") == std::string::npos)
        {
            throw std::runtime_error("Login failed");
        };
    };

    void join(std::string_view channel_name)
    {
        assert(channel_name.find("\n") == std::string_view::npos);
        fmt::print("Joining channel {}", channel_name);
        cs.send(fmt::format("JOIN #{}\n", channel_name));
        std::string msg;
        do
        {
            msg = message_queue.pop_blocking();
            fmt::print("{}", msg);
        } while (msg.find("End of /NAMES list") == std::string::npos);
    }
    void sendMessage(std::string_view channel_name, std::string_view msg)
    {
        assert(channel_name.find("\n") == std::string_view::npos);
        assert(msg.find("\n") == std::string_view::npos);
        cs.send(fmt::format("PRIVMSG #{} : {}\n", channel_name, msg));
    };
};

TwitchOAUTHServer srv;
TwitchChat tc;

void onInitFinished()
{
    srv.run();
    srv.openLoginBrowser();
    fmt::print("Twitch plugin loaded.\n");
}

void onNewGame()
{
    srv.stop();
}
void onLoadGame(int id, char *data, int length)
{
    srv.stop();
}

int main(int argc, char *argv[])
{
    assert(argc == 3);
    try
    {
        tc.connect();
        tc.run();
        tc.login(argv[2], argv[1]);
        tc.join("vagabonddog");
        tc.sendMessage("vagabonddog", "This was sent from my c++ plugin :D");
        while (true)
        {
            fmt::print("Msg: {}", tc.message_queue.pop_blocking());
        };
    }
    catch (std::exception &e)
    {
        tc.stop();
        fmt::print(stderr, "Exception: {}", e.what());
    }
    while (true)
    {
    }
}