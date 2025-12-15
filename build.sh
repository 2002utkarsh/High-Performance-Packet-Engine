#!/bin/bash
set -e

echo "Building Ingestion Engine..."
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
echo "Build complete. Shared objects generated in build/"
