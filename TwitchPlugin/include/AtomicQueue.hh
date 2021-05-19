#pragma once
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class AtomicQueue {
public:
    void push(const T &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queque.push(value);
    }

    std::optional<T> pop() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queque.empty()) { return std::nullopt; }
        T t = m_queque.front();
        m_queque.pop();
        return t;
    }

    T pop_blocking() {
        std::optional<T> t;
        while (!(t = pop())) {}
        return *t;
    }

    std::optional<T> peek() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queque.empty()) { return std::nullopt; }
        return m_queque.front();
    }

private:
    std::queue<T> m_queque;
    mutable std::mutex m_mutex{};
};
