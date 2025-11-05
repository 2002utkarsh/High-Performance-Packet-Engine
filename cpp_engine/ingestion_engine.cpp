#include "ingestion_engine.h"
#include "include/ZeroCopyRingBuffer.hpp"
#include "include/EventLoop.hpp"
#include "include/CustomProtocol.hpp"
#include <thread>
#include <atomic>
using namespace engine::memory;
using namespace engine::network;
class IngestionEngine {
public:
    IngestionEngine() : is_running_(false), processed_count_(0) {
        event_loop_.SetPacketIngestionHandler([this](const PacketHeader* packet, size_t length) {
            ring_buffer_.Emplace(*packet);
        });
    }
    ~IngestionEngine() { Stop(); }
    void Start() {
        if (is_running_) return;
        is_running_ = true;
        worker_thread_ = std::thread(&IngestionEngine::ProcessingLoop, this);
    }
    void Stop() {
        if (!is_running_) return;
        is_running_ = false;
        event_loop_.Stop();
        if (worker_thread_.joinable()) worker_thread_.join();
    }
    void MockInject(const void* buffer, size_t length) {
        if (length >= sizeof(PacketHeader)) {
            ring_buffer_.Emplace(*reinterpret_cast<const PacketHeader*>(buffer));
        }
    }
    uint64_t GetProcessedCount() const {
        return processed_count_.load(std::memory_order_relaxed);
    }
private:
    void ProcessingLoop() {
        PacketHeader packet;
        while (is_running_) {
            if (ring_buffer_.Pop(packet)) {
                if (packet.ack_flag == 0x01) continue; 
                processed_count_.fetch_add(1, std::memory_order_relaxed);
            } else { std::this_thread::yield(); }
        }
    }
    EventLoop event_loop_;
    ZeroCopyRingBuffer<PacketHeader, 16384> ring_buffer_; 
    std::atomic<bool> is_running_;
    std::atomic<uint64_t> processed_count_;
    std::thread worker_thread_;
};
extern "C" {
SYSTEM_API EngineHandle engine_create() {
    auto* engine = new IngestionEngine();
    engine->Start();
    return static_cast<EngineHandle>(engine);
}
SYSTEM_API void engine_destroy(EngineHandle handle) {
    if (handle) delete static_cast<IngestionEngine*>(handle);
}
SYSTEM_API void engine_inject_stream(EngineHandle handle, const void* buffer, size_t length) {
    if (handle) static_cast<IngestionEngine*>(handle)->MockInject(buffer, length);
}
SYSTEM_API uint64_t engine_get_processed_count(EngineHandle handle) {
    if (handle) return static_cast<IngestionEngine*>(handle)->GetProcessedCount();
    return 0;
}
}
