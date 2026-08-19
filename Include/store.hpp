#pragma once

#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <optional>

class Store {
public:
    Store() = default;
    ~Store() = default;

    Store(const Store&) = delete;
    Store& operator=(const Store&) = delete;

    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool remove(const std::string& key);
    void clear();

private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, std::string> data_;
};
