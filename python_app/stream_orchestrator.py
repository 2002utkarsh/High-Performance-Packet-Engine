import ctypes
import os
import sys
import time
class Interface:
    def __init__(self, lib_path):
        if not os.path.exists(lib_path):
            sys.exit(1)
        self.lib = ctypes.CDLL(lib_path)
        self.lib.engine_create.restype = ctypes.c_void_p
        self.lib.engine_destroy.argtypes = [ctypes.c_void_p]
        self.lib.engine_get_processed_count.argtypes = [ctypes.c_void_p]
        self.lib.engine_get_processed_count.restype = ctypes.c_uint64
        self.handle = self.lib.engine_create()
    def __del__(self):
        if hasattr(self, 'handle') and self.handle:
            self.lib.engine_destroy(self.handle)
    def stats(self) -> int:
        return self.lib.engine_get_processed_count(self.handle)
if __name__ == "__main__":
    driver = Interface("./libingestion_engine.so")
    try:
        while True:
            time.sleep(1)
            sys.stdout.write(f"\rprocessed: {driver.stats()}")
            sys.stdout.flush()
    except KeyboardInterrupt:
        sys.exit(0)
