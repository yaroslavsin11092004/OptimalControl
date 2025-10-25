import 'dart:async';
import 'package:grpc/grpc.dart';
import '../gRPC/core.pb.dart';
import '../gRPC/core.pbgrpc.dart';
class CoreService {
  final ClientChannel _channel;
  final CoreOptimalControlServiceClient _client;
  CoreService(String host, int port):
    _channel = ClientChannel(host, port: port, options:const ChannelOptions(credentials: ChannelCredentials.insecure())),
    _client = CoreOptimalControlServiceClient(ClientChannel(host, port: port));
  Future<OptimalResponse> calcOptimal(OptimalRequest request) async {
    return await _client.calcOptimalTask(request);
  }
  Future<void> close() async {
    _channel.shutdown();
  }
  Future<OptimizeParamsResponse> setParams(OptimizeParamsRequest request) async {
    return await _client.setOptimizerParams(request);
  }
}
