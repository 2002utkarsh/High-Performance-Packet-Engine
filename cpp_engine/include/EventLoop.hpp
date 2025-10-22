#ifndef ENGINE_NETWORK_EVENT_LOOP_HPP
#define ENGINE_NETWORK_EVENT_LOOP_HPP
#include <stdint.h>
#include <vector>
#include <memory>
#include <functional>
#if defined(__linux__)
    #include <sys/epoll.h>
#elif defined(__APPLE__) || defined(__FreeBSD__)
    #include <sys/types.h>
    #include <sys/event.h>
    #include <sys/time.h>
#else
    #define EPOLLIN 0x001
#endif
namespace engine { namespace network {
struct PacketHeader;
class EventLoop {
public:
    explicit EventLoop(size_t max_concurrent_connections = 10000);
    ~EventLoop();
    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    void RegisterSocket(int fd);
    void Run();
    void Stop();
    using PacketHandler = std::function<void(const PacketHeader* packet, size_t length)>;
    void SetPacketIngestionHandler(PacketHandler handler);
private:
    void HandleSocketRead(int fd);
    void DispatchAcknowledgment(int fd, uint64_t sequence_id);
    int multiplexer_fd_;
    bool is_running_;
    size_t max_events_;
    PacketHandler ingestion_callback_;
    struct alignas(64) ConnectionState;
    std::vector<std::unique_ptr<ConnectionState>> connections_;
};
}}
#endif
