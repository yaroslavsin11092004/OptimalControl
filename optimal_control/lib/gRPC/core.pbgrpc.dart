// This is a generated file - do not edit.
//
// Generated from core.proto.

// @dart = 3.3

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names
// ignore_for_file: curly_braces_in_flow_control_structures
// ignore_for_file: deprecated_member_use_from_same_package, library_prefixes
// ignore_for_file: non_constant_identifier_names

import 'dart:async' as $async;
import 'dart:core' as $core;

import 'package:grpc/service_api.dart' as $grpc;
import 'package:protobuf/protobuf.dart' as $pb;

import 'core.pb.dart' as $0;

export 'core.pb.dart';

@$pb.GrpcServiceName('core_api.CoreOptimalControlService')
class CoreOptimalControlServiceClient extends $grpc.Client {
  /// The hostname for this service.
  static const $core.String defaultHost = '';

  /// OAuth scopes needed for the client.
  static const $core.List<$core.String> oauthScopes = [
    '',
  ];

  CoreOptimalControlServiceClient(super.channel,
      {super.options, super.interceptors});

  $grpc.ResponseFuture<$0.OptimalResponse> calcOptimalTask(
    $0.OptimalRequest request, {
    $grpc.CallOptions? options,
  }) {
    return $createUnaryCall(_$calcOptimalTask, request, options: options);
  }

  $grpc.ResponseFuture<$0.OptimizeParamsResponse> setOptimizerParams(
    $0.OptimizeParamsRequest request, {
    $grpc.CallOptions? options,
  }) {
    return $createUnaryCall(_$setOptimizerParams, request, options: options);
  }

  // method descriptors

  static final _$calcOptimalTask =
      $grpc.ClientMethod<$0.OptimalRequest, $0.OptimalResponse>(
          '/core_api.CoreOptimalControlService/CalcOptimalTask',
          ($0.OptimalRequest value) => value.writeToBuffer(),
          $0.OptimalResponse.fromBuffer);
  static final _$setOptimizerParams =
      $grpc.ClientMethod<$0.OptimizeParamsRequest, $0.OptimizeParamsResponse>(
          '/core_api.CoreOptimalControlService/SetOptimizerParams',
          ($0.OptimizeParamsRequest value) => value.writeToBuffer(),
          $0.OptimizeParamsResponse.fromBuffer);
}

@$pb.GrpcServiceName('core_api.CoreOptimalControlService')
abstract class CoreOptimalControlServiceBase extends $grpc.Service {
  $core.String get $name => 'core_api.CoreOptimalControlService';

  CoreOptimalControlServiceBase() {
    $addMethod($grpc.ServiceMethod<$0.OptimalRequest, $0.OptimalResponse>(
        'CalcOptimalTask',
        calcOptimalTask_Pre,
        false,
        false,
        ($core.List<$core.int> value) => $0.OptimalRequest.fromBuffer(value),
        ($0.OptimalResponse value) => value.writeToBuffer()));
    $addMethod($grpc.ServiceMethod<$0.OptimizeParamsRequest,
            $0.OptimizeParamsResponse>(
        'SetOptimizerParams',
        setOptimizerParams_Pre,
        false,
        false,
        ($core.List<$core.int> value) =>
            $0.OptimizeParamsRequest.fromBuffer(value),
        ($0.OptimizeParamsResponse value) => value.writeToBuffer()));
  }

  $async.Future<$0.OptimalResponse> calcOptimalTask_Pre($grpc.ServiceCall $call,
      $async.Future<$0.OptimalRequest> $request) async {
    return calcOptimalTask($call, await $request);
  }

  $async.Future<$0.OptimalResponse> calcOptimalTask(
      $grpc.ServiceCall call, $0.OptimalRequest request);

  $async.Future<$0.OptimizeParamsResponse> setOptimizerParams_Pre(
      $grpc.ServiceCall $call,
      $async.Future<$0.OptimizeParamsRequest> $request) async {
    return setOptimizerParams($call, await $request);
  }

  $async.Future<$0.OptimizeParamsResponse> setOptimizerParams(
      $grpc.ServiceCall call, $0.OptimizeParamsRequest request);
}
