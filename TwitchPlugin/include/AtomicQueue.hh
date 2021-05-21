#pragma once
#include <condition_variable>
#include <fmt/core.h>
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class AtomicQueue {
public:
    void push(const T &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        cv.notify_all();
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
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            cv.wait(lock, [&] { return !m_queque.empty(); });
        }
        return *pop();
    }

    template<typename _Rep, typename _Period>
    std::optional<T> pop_with_timeout(const std::chrono::duration<_Rep, _Period>& time) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            cv.wait_for(lock, time, [&] { return !m_queque.empty(); });
        }
        return pop();
    }

    std::optional<T> peek() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queque.empty()) { return std::nullopt; }
        return m_queque.front();
    }

private:
    std::queue<T> m_queque;
    mutable std::mutex m_mutex{};
    std::condition_variable cv;
};
