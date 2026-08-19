#include "server.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(int port) : port_(port), server_fd_(-1), running_(false) {}

Server::~Server() {
    stop();
}

void Server::start() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == -1) {
        std::cerr << "Failed to create socket\n";
        return;
    }

    // Allow immediate port reuse after shutdown
    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed on port " << port_ << "\n";
        close(server_fd_);
        return;
    }

    if (listen(server_fd_, SOMAXCONN) < 0) {
        std::cerr << "Listen failed\n";
        close(server_fd_);
        return;
    }

    running_ = true;
    std::cout << "Mini-Redis Server listening on port " << port_ << "...\n";

    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_fd_, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            if (!running_) break;
            std::cerr << "Accept failed\n";
            continue;
        }

        // Spawn a C++20 jthread for safe, automatic thread joining per client connection
        std::jthread([this, client_socket]() {
            handle_client(client_socket);
        });
    }
}

void Server::stop() {
    if (running_) {
        running_ = false;
        if (server_fd_ != -1) {
            close(server_fd_);
            server_fd_ = -1;
        }
        std::cout << "\nServer shut down successfully.\n";
    }
}

void Server::handle_client(int client_socket) {
    char buffer[1024];
    std::string line_accum = "";

    while (true) {
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if (bytes_read <= 0) {
            break; // Client disconnected or socket error
        }

        line_accum.append(buffer, bytes_read);
        size_t pos = 0;
        while ((pos = line_accum.find('\n')) != std::string::npos) {
            std::string line = line_accum.substr(0, pos);
            line_accum.erase(0, pos + 1);

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string cmd;
            ss >> cmd;
            for (auto& c : cmd) c = static_cast<char>(toupper(c));

            std::string response;
            if (cmd == "QUIT" || cmd == "EXIT") {
                response = "BYE\n";
                write(client_socket, response.c_str(), response.size());
                break;
            } 
            else if (cmd == "SET") {
                std::string key, val;
                if (ss >> key >> val) {
                    store_.set(key, val);
                    response = "OK\n";
                } else {
                    response = "(error) ERR wrong number of arguments for 'set'\n";
                }
            } 
            else if (cmd == "GET") {
                std::string key;
                if (ss >> key) {
                    auto val = store_.get(key);
                    if (val) {
                        response = "\"" + *val + "\"\n";
                    } else {
                        response = "(nil)\n";
                    }
                } else {
                    response = "(error) ERR wrong number of arguments for 'get'\n";
                }
            } 
            else if (cmd == "DEL") {
                std::string key;
                if (ss >> key) {
                    bool deleted = store_.remove(key);
                    response = "(integer) " + std::string(deleted ? "1" : "0") + "\n";
                } else {
                    response = "(error) ERR wrong number of arguments for 'del'\n";
                }
            } 
            else {
                response = "(error) ERR unknown command '" + cmd + "'\n";
            }

            write(client_socket, response.c_str(), response.size());
        }
    }

    close(client_socket);
}
