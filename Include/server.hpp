#pragma once

#include "store.hpp"
#include <atomic>

class Server {
public:
    explicit Server(int port = 6379);
    ~Server();

    void start();
    void stop();

private:
    int port_;
    int server_fd_;
    std::atomic<bool> running_;
    Store store_;

    void handle_client(int client_socket);
};
