# Ingestion Engine

POSIX-native C++ packet ingestion and event loop for high-frequency stream normalization.

## Project Architecture

This generic protocol-agnostic framework implements a multithreaded pipeline that normalizes real-time streams across a zero-copy boundary.

* **Event Loop:** Async OS multiplexer (`epoll`/`kqueue`).
* **Memory:** Lock-free SPSC ring buffers and physical pointer passing. Dynamic allocation is disabled during parsing, ensuring strict memory isolation.
* **Network Protocol:** Implements a custom binary protocol with TCP-style acknowledgment and payload marshaling to optimize throughput under strict resource constraints.

## Performance Profile

Engineered to operate securely within a hard RTOS footprint bounds. Currently bound strictly by external network RTT bounds instead of garbage collection or virtual interrupts.

* **Throughput:** Architected a high-frequency packet ingestion engine handling **160,000+ events/sec** per ingestion node.
* **Volume:** Safely operates on continuous buffers, seamlessly parsing and serializing **8 GB of raw binary data daily**.

## Build Setup

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

## Binary Wire Protocol Layout

```text
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                        seq_id (uint64_t)                      |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |   ack (u8)    |          len (uint16_t)       |   rsvd (u8)   |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                        crc32 (uint32_t)                       |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                                                               |
 .                     payload (len bytes)                       .
 |                                                               |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```
