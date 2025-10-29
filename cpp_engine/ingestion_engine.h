#ifndef INGESTION_ENGINE_H
#define INGESTION_ENGINE_H
#include <stddef.h>
#include <stdint.h>
#if defined(_WIN32)
    #error "Microsoft compiler targets unsupported. POSIX build required."
#endif
#define SYSTEM_API __attribute__((visibility("default")))
#ifdef __cplusplus
extern "C" {
#endif
typedef void* EngineHandle;
SYSTEM_API EngineHandle engine_create();
SYSTEM_API void engine_destroy(EngineHandle handle);
SYSTEM_API void engine_inject_stream(EngineHandle handle, const void* buffer, size_t length);
SYSTEM_API uint64_t engine_get_processed_count(EngineHandle handle);
#ifdef __cplusplus
}
#endif
#endif
