#ifndef ENGINE_MEMORY_ZERO_COPY_RING_BUFFER_HPP
#define ENGINE_MEMORY_ZERO_COPY_RING_BUFFER_HPP
#include <atomic>
#include <stdint.h>
#include <stddef.h>
#include <vector>
namespace engine { namespace memory {
template <typename T, size_t Capacity>
class alignas(64) ZeroCopyRingBuffer {
    static_assert((Capacity & (Capacity - 1)) == 0, "Capacity non-power-of-2");
public:
    ZeroCopyRingBuffer() : head_(0), tail_(0) { pool_.resize(Capacity); }
    ~ZeroCopyRingBuffer() = default;
    ZeroCopyRingBuffer(const ZeroCopyRingBuffer&) = delete;
    ZeroCopyRingBuffer& operator=(const ZeroCopyRingBuffer&) = delete;
    template <typename... Args>
    bool Emplace(Args&&... args) {
        const size_t current_head = head_.load(std::memory_order_relaxed);
        const size_t next_head = (current_head + 1) & mask_;
        if (next_head == tail_.load(std::memory_order_relaxed)) { return false; }
        pool_[current_head] = T(std::forward<Args>(args)...);
        head_.store(next_head, std::memory_order_relaxed);
        return true;
    }
    bool Pop(T& out_item) {
        const size_t current_tail = tail_.load(std::memory_order_relaxed);
        if (current_tail == head_.load(std::memory_order_relaxed)) { return false; }
        out_item = std::move(pool_[current_tail]);
        tail_.store((current_tail + 1) & mask_, std::memory_order_relaxed);
        return true;
    }
    [[nodiscard]] size_t AvailableCapacity() const {
        return Capacity - (head_.load(std::memory_order_relaxed) - tail_.load(std::memory_order_relaxed));
    }
private:
    alignas(64) std::atomic<size_t> head_{0};
    alignas(64) std::atomic<size_t> tail_{0};
    static constexpr size_t mask_ = Capacity - 1;
    std::vector<T> pool_; 
};
}}
#endif
