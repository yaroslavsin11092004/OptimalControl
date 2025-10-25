// This is a generated file - do not edit.
//
// Generated from core.proto.

// @dart = 3.3

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names
// ignore_for_file: curly_braces_in_flow_control_structures
// ignore_for_file: deprecated_member_use_from_same_package, library_prefixes
// ignore_for_file: non_constant_identifier_names

import 'dart:core' as $core;

import 'package:protobuf/protobuf.dart' as $pb;

export 'package:protobuf/protobuf.dart' show GeneratedMessageGenericExtensions;

class OptimalRequest extends $pb.GeneratedMessage {
  factory OptimalRequest({
    $core.Iterable<$core.String>? equations,
    $core.Iterable<$core.String>? linked,
    $core.String? hamilton,
    $core.Iterable<$core.double>? u0,
    $core.Iterable<$core.double>? x0,
    $core.double? t0,
    $core.double? t1,
    $core.double? tstep,
    $core.double? delta,
  }) {
    final result = create();
    if (equations != null) result.equations.addAll(equations);
    if (linked != null) result.linked.addAll(linked);
    if (hamilton != null) result.hamilton = hamilton;
    if (u0 != null) result.u0.addAll(u0);
    if (x0 != null) result.x0.addAll(x0);
    if (t0 != null) result.t0 = t0;
    if (t1 != null) result.t1 = t1;
    if (tstep != null) result.tstep = tstep;
    if (delta != null) result.delta = delta;
    return result;
  }

  OptimalRequest._();

  factory OptimalRequest.fromBuffer($core.List<$core.int> data,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromBuffer(data, registry);
  factory OptimalRequest.fromJson($core.String json,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(
      _omitMessageNames ? '' : 'OptimalRequest',
      package: const $pb.PackageName(_omitMessageNames ? '' : 'core_api'),
      createEmptyInstance: create)
    ..pPS(1, _omitFieldNames ? '' : 'equations')
    ..pPS(2, _omitFieldNames ? '' : 'linked')
    ..aOS(3, _omitFieldNames ? '' : 'hamilton')
    ..p<$core.double>(4, _omitFieldNames ? '' : 'u0', $pb.PbFieldType.KD)
    ..p<$core.double>(5, _omitFieldNames ? '' : 'x0', $pb.PbFieldType.KD)
    ..aD(6, _omitFieldNames ? '' : 't0')
    ..aD(7, _omitFieldNames ? '' : 't1')
    ..aD(8, _omitFieldNames ? '' : 'tstep')
    ..aD(9, _omitFieldNames ? '' : 'delta')
    ..hasRequiredFields = false;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimalRequest clone() => deepCopy();
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimalRequest copyWith(void Function(OptimalRequest) updates) =>
      super.copyWith((message) => updates(message as OptimalRequest))
          as OptimalRequest;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OptimalRequest create() => OptimalRequest._();
  @$core.override
  OptimalRequest createEmptyInstance() => create();
  static $pb.PbList<OptimalRequest> createRepeated() =>
      $pb.PbList<OptimalRequest>();
  @$core.pragma('dart2js:noInline')
  static OptimalRequest getDefault() => _defaultInstance ??=
      $pb.GeneratedMessage.$_defaultFor<OptimalRequest>(create);
  static OptimalRequest? _defaultInstance;

  @$pb.TagNumber(1)
  $pb.PbList<$core.String> get equations => $_getList(0);

  @$pb.TagNumber(2)
  $pb.PbList<$core.String> get linked => $_getList(1);

  @$pb.TagNumber(3)
  $core.String get hamilton => $_getSZ(2);
  @$pb.TagNumber(3)
  set hamilton($core.String value) => $_setString(2, value);
  @$pb.TagNumber(3)
  $core.bool hasHamilton() => $_has(2);
  @$pb.TagNumber(3)
  void clearHamilton() => $_clearField(3);

  @$pb.TagNumber(4)
  $pb.PbList<$core.double> get u0 => $_getList(3);

  @$pb.TagNumber(5)
  $pb.PbList<$core.double> get x0 => $_getList(4);

  @$pb.TagNumber(6)
  $core.double get t0 => $_getN(5);
  @$pb.TagNumber(6)
  set t0($core.double value) => $_setDouble(5, value);
  @$pb.TagNumber(6)
  $core.bool hasT0() => $_has(5);
  @$pb.TagNumber(6)
  void clearT0() => $_clearField(6);

  @$pb.TagNumber(7)
  $core.double get t1 => $_getN(6);
  @$pb.TagNumber(7)
  set t1($core.double value) => $_setDouble(6, value);
  @$pb.TagNumber(7)
  $core.bool hasT1() => $_has(6);
  @$pb.TagNumber(7)
  void clearT1() => $_clearField(7);

  @$pb.TagNumber(8)
  $core.double get tstep => $_getN(7);
  @$pb.TagNumber(8)
  set tstep($core.double value) => $_setDouble(7, value);
  @$pb.TagNumber(8)
  $core.bool hasTstep() => $_has(7);
  @$pb.TagNumber(8)
  void clearTstep() => $_clearField(8);

  @$pb.TagNumber(9)
  $core.double get delta => $_getN(8);
  @$pb.TagNumber(9)
  set delta($core.double value) => $_setDouble(8, value);
  @$pb.TagNumber(9)
  $core.bool hasDelta() => $_has(8);
  @$pb.TagNumber(9)
  void clearDelta() => $_clearField(9);
}

class OptimalResponse extends $pb.GeneratedMessage {
  factory OptimalResponse({
    $core.Iterable<$core.double>? optimPath,
    $core.Iterable<$core.double>? optimControl,
    $core.int? optimPathSizeRow,
    $core.int? optimPathSizeCol,
    $core.int? optimControlSizeRow,
    $core.int? optimControlSizeCol,
    $core.double? functionalValue,
  }) {
    final result = create();
    if (optimPath != null) result.optimPath.addAll(optimPath);
    if (optimControl != null) result.optimControl.addAll(optimControl);
    if (optimPathSizeRow != null) result.optimPathSizeRow = optimPathSizeRow;
    if (optimPathSizeCol != null) result.optimPathSizeCol = optimPathSizeCol;
    if (optimControlSizeRow != null)
      result.optimControlSizeRow = optimControlSizeRow;
    if (optimControlSizeCol != null)
      result.optimControlSizeCol = optimControlSizeCol;
    if (functionalValue != null) result.functionalValue = functionalValue;
    return result;
  }

  OptimalResponse._();

  factory OptimalResponse.fromBuffer($core.List<$core.int> data,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromBuffer(data, registry);
  factory OptimalResponse.fromJson($core.String json,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(
      _omitMessageNames ? '' : 'OptimalResponse',
      package: const $pb.PackageName(_omitMessageNames ? '' : 'core_api'),
      createEmptyInstance: create)
    ..p<$core.double>(1, _omitFieldNames ? '' : 'optimPath', $pb.PbFieldType.KD)
    ..p<$core.double>(
        2, _omitFieldNames ? '' : 'optimControl', $pb.PbFieldType.KD)
    ..aI(3, _omitFieldNames ? '' : 'optimPathSizeRow')
    ..aI(4, _omitFieldNames ? '' : 'optimPathSizeCol')
    ..aI(5, _omitFieldNames ? '' : 'optimControlSizeRow')
    ..aI(6, _omitFieldNames ? '' : 'optimControlSizeCol')
    ..aD(7, _omitFieldNames ? '' : 'functionalValue')
    ..hasRequiredFields = false;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimalResponse clone() => deepCopy();
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimalResponse copyWith(void Function(OptimalResponse) updates) =>
      super.copyWith((message) => updates(message as OptimalResponse))
          as OptimalResponse;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OptimalResponse create() => OptimalResponse._();
  @$core.override
  OptimalResponse createEmptyInstance() => create();
  static $pb.PbList<OptimalResponse> createRepeated() =>
      $pb.PbList<OptimalResponse>();
  @$core.pragma('dart2js:noInline')
  static OptimalResponse getDefault() => _defaultInstance ??=
      $pb.GeneratedMessage.$_defaultFor<OptimalResponse>(create);
  static OptimalResponse? _defaultInstance;

  @$pb.TagNumber(1)
  $pb.PbList<$core.double> get optimPath => $_getList(0);

  @$pb.TagNumber(2)
  $pb.PbList<$core.double> get optimControl => $_getList(1);

  @$pb.TagNumber(3)
  $core.int get optimPathSizeRow => $_getIZ(2);
  @$pb.TagNumber(3)
  set optimPathSizeRow($core.int value) => $_setSignedInt32(2, value);
  @$pb.TagNumber(3)
  $core.bool hasOptimPathSizeRow() => $_has(2);
  @$pb.TagNumber(3)
  void clearOptimPathSizeRow() => $_clearField(3);

  @$pb.TagNumber(4)
  $core.int get optimPathSizeCol => $_getIZ(3);
  @$pb.TagNumber(4)
  set optimPathSizeCol($core.int value) => $_setSignedInt32(3, value);
  @$pb.TagNumber(4)
  $core.bool hasOptimPathSizeCol() => $_has(3);
  @$pb.TagNumber(4)
  void clearOptimPathSizeCol() => $_clearField(4);

  @$pb.TagNumber(5)
  $core.int get optimControlSizeRow => $_getIZ(4);
  @$pb.TagNumber(5)
  set optimControlSizeRow($core.int value) => $_setSignedInt32(4, value);
  @$pb.TagNumber(5)
  $core.bool hasOptimControlSizeRow() => $_has(4);
  @$pb.TagNumber(5)
  void clearOptimControlSizeRow() => $_clearField(5);

  @$pb.TagNumber(6)
  $core.int get optimControlSizeCol => $_getIZ(5);
  @$pb.TagNumber(6)
  set optimControlSizeCol($core.int value) => $_setSignedInt32(5, value);
  @$pb.TagNumber(6)
  $core.bool hasOptimControlSizeCol() => $_has(5);
  @$pb.TagNumber(6)
  void clearOptimControlSizeCol() => $_clearField(6);

  @$pb.TagNumber(7)
  $core.double get functionalValue => $_getN(6);
  @$pb.TagNumber(7)
  set functionalValue($core.double value) => $_setDouble(6, value);
  @$pb.TagNumber(7)
  $core.bool hasFunctionalValue() => $_has(6);
  @$pb.TagNumber(7)
  void clearFunctionalValue() => $_clearField(7);
}

class OptimizeParamsRequest extends $pb.GeneratedMessage {
  factory OptimizeParamsRequest({
    $core.double? learningRate,
    $core.Iterable<$core.double>? leftEdge,
    $core.Iterable<$core.double>? rightEdge,
    $core.int? epochs,
  }) {
    final result = create();
    if (learningRate != null) result.learningRate = learningRate;
    if (leftEdge != null) result.leftEdge.addAll(leftEdge);
    if (rightEdge != null) result.rightEdge.addAll(rightEdge);
    if (epochs != null) result.epochs = epochs;
    return result;
  }

  OptimizeParamsRequest._();

  factory OptimizeParamsRequest.fromBuffer($core.List<$core.int> data,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromBuffer(data, registry);
  factory OptimizeParamsRequest.fromJson($core.String json,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(
      _omitMessageNames ? '' : 'OptimizeParamsRequest',
      package: const $pb.PackageName(_omitMessageNames ? '' : 'core_api'),
      createEmptyInstance: create)
    ..aD(1, _omitFieldNames ? '' : 'learningRate')
    ..p<$core.double>(2, _omitFieldNames ? '' : 'leftEdge', $pb.PbFieldType.KD)
    ..p<$core.double>(3, _omitFieldNames ? '' : 'rightEdge', $pb.PbFieldType.KD)
    ..aI(4, _omitFieldNames ? '' : 'epochs')
    ..hasRequiredFields = false;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimizeParamsRequest clone() => deepCopy();
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimizeParamsRequest copyWith(
          void Function(OptimizeParamsRequest) updates) =>
      super.copyWith((message) => updates(message as OptimizeParamsRequest))
          as OptimizeParamsRequest;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OptimizeParamsRequest create() => OptimizeParamsRequest._();
  @$core.override
  OptimizeParamsRequest createEmptyInstance() => create();
  static $pb.PbList<OptimizeParamsRequest> createRepeated() =>
      $pb.PbList<OptimizeParamsRequest>();
  @$core.pragma('dart2js:noInline')
  static OptimizeParamsRequest getDefault() => _defaultInstance ??=
      $pb.GeneratedMessage.$_defaultFor<OptimizeParamsRequest>(create);
  static OptimizeParamsRequest? _defaultInstance;

  @$pb.TagNumber(1)
  $core.double get learningRate => $_getN(0);
  @$pb.TagNumber(1)
  set learningRate($core.double value) => $_setDouble(0, value);
  @$pb.TagNumber(1)
  $core.bool hasLearningRate() => $_has(0);
  @$pb.TagNumber(1)
  void clearLearningRate() => $_clearField(1);

  @$pb.TagNumber(2)
  $pb.PbList<$core.double> get leftEdge => $_getList(1);

  @$pb.TagNumber(3)
  $pb.PbList<$core.double> get rightEdge => $_getList(2);

  @$pb.TagNumber(4)
  $core.int get epochs => $_getIZ(3);
  @$pb.TagNumber(4)
  set epochs($core.int value) => $_setSignedInt32(3, value);
  @$pb.TagNumber(4)
  $core.bool hasEpochs() => $_has(3);
  @$pb.TagNumber(4)
  void clearEpochs() => $_clearField(4);
}

class OptimizeParamsResponse extends $pb.GeneratedMessage {
  factory OptimizeParamsResponse() => create();

  OptimizeParamsResponse._();

  factory OptimizeParamsResponse.fromBuffer($core.List<$core.int> data,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromBuffer(data, registry);
  factory OptimizeParamsResponse.fromJson($core.String json,
          [$pb.ExtensionRegistry registry = $pb.ExtensionRegistry.EMPTY]) =>
      create()..mergeFromJson(json, registry);

  static final $pb.BuilderInfo _i = $pb.BuilderInfo(
      _omitMessageNames ? '' : 'OptimizeParamsResponse',
      package: const $pb.PackageName(_omitMessageNames ? '' : 'core_api'),
      createEmptyInstance: create)
    ..hasRequiredFields = false;

  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimizeParamsResponse clone() => deepCopy();
  @$core.Deprecated('See https://github.com/google/protobuf.dart/issues/998.')
  OptimizeParamsResponse copyWith(
          void Function(OptimizeParamsResponse) updates) =>
      super.copyWith((message) => updates(message as OptimizeParamsResponse))
          as OptimizeParamsResponse;

  @$core.override
  $pb.BuilderInfo get info_ => _i;

  @$core.pragma('dart2js:noInline')
  static OptimizeParamsResponse create() => OptimizeParamsResponse._();
  @$core.override
  OptimizeParamsResponse createEmptyInstance() => create();
  static $pb.PbList<OptimizeParamsResponse> createRepeated() =>
      $pb.PbList<OptimizeParamsResponse>();
  @$core.pragma('dart2js:noInline')
  static OptimizeParamsResponse getDefault() => _defaultInstance ??=
      $pb.GeneratedMessage.$_defaultFor<OptimizeParamsResponse>(create);
  static OptimizeParamsResponse? _defaultInstance;
}

const $core.bool _omitFieldNames =
    $core.bool.fromEnvironment('protobuf.omit_field_names');
const $core.bool _omitMessageNames =
    $core.bool.fromEnvironment('protobuf.omit_message_names');
