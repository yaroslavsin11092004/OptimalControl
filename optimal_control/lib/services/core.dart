import 'dart:async';
import 'package:grpc/grpc.dart';
import '../gRPC/core.pb.dart';
import '../gRPC/core.pbgrpc.dart';
class CoreService {
  final ClientChannel _channel;
  final CoreOptimalControlServiceClient _client;

  CoreService._internal(this._channel) : _client = CoreOptimalControlServiceClient(_channel);
  factory CoreService(String host, int port) {
    final channel = ClientChannel( 
      host,
      port: port,
      options: ChannelOptions(credentials: ChannelCredentials.insecure(),
      codecRegistry: CodecRegistry(codecs: const []),
      connectionTimeout: Duration(seconds: 5),
        idleTimeout: Duration(seconds: 30))
    );
    return CoreService._internal(channel);
  }

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
