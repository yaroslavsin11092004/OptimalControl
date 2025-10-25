#!/bin/bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
/usr/local/bin/protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin ./*.proto
/usr/local/bin/protoc --dart_out=grpc:. ./core.proto
mv ./*.dart ~/source/OptimalControl/optimal_control/lib/gRPC 
