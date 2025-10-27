// This is a generated file - do not edit.
//
// Generated from core.proto.

// @dart = 3.3

// ignore_for_file: annotate_overrides, camel_case_types, comment_references
// ignore_for_file: constant_identifier_names
// ignore_for_file: curly_braces_in_flow_control_structures
// ignore_for_file: deprecated_member_use_from_same_package, library_prefixes
// ignore_for_file: non_constant_identifier_names, unused_import

import 'dart:convert' as $convert;
import 'dart:core' as $core;
import 'dart:typed_data' as $typed_data;

@$core.Deprecated('Use optimalRequestDescriptor instead')
const OptimalRequest$json = {
  '1': 'OptimalRequest',
  '2': [
    {'1': 'equations', '3': 1, '4': 3, '5': 9, '10': 'equations'},
    {'1': 'linked', '3': 2, '4': 3, '5': 9, '10': 'linked'},
    {'1': 'hamilton', '3': 3, '4': 1, '5': 9, '10': 'hamilton'},
    {'1': 'functional', '3': 4, '4': 1, '5': 9, '10': 'functional'},
    {'1': 'u0', '3': 5, '4': 3, '5': 1, '10': 'u0'},
    {'1': 'x0', '3': 6, '4': 3, '5': 1, '10': 'x0'},
    {'1': 't0', '3': 7, '4': 1, '5': 1, '10': 't0'},
    {'1': 't1', '3': 8, '4': 1, '5': 1, '10': 't1'},
    {'1': 'tstep', '3': 9, '4': 1, '5': 1, '10': 'tstep'},
    {'1': 'delta', '3': 10, '4': 1, '5': 1, '10': 'delta'},
  ],
};

/// Descriptor for `OptimalRequest`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List optimalRequestDescriptor = $convert.base64Decode(
    'Cg5PcHRpbWFsUmVxdWVzdBIcCgllcXVhdGlvbnMYASADKAlSCWVxdWF0aW9ucxIWCgZsaW5rZW'
    'QYAiADKAlSBmxpbmtlZBIaCghoYW1pbHRvbhgDIAEoCVIIaGFtaWx0b24SHgoKZnVuY3Rpb25h'
    'bBgEIAEoCVIKZnVuY3Rpb25hbBIOCgJ1MBgFIAMoAVICdTASDgoCeDAYBiADKAFSAngwEg4KAn'
    'QwGAcgASgBUgJ0MBIOCgJ0MRgIIAEoAVICdDESFAoFdHN0ZXAYCSABKAFSBXRzdGVwEhQKBWRl'
    'bHRhGAogASgBUgVkZWx0YQ==');

@$core.Deprecated('Use optimalResponseDescriptor instead')
const OptimalResponse$json = {
  '1': 'OptimalResponse',
  '2': [
    {'1': 'optim_path', '3': 1, '4': 3, '5': 1, '10': 'optimPath'},
    {'1': 'optim_control', '3': 2, '4': 3, '5': 1, '10': 'optimControl'},
    {
      '1': 'optim_path_size_row',
      '3': 3,
      '4': 1,
      '5': 5,
      '10': 'optimPathSizeRow'
    },
    {
      '1': 'optim_path_size_col',
      '3': 4,
      '4': 1,
      '5': 5,
      '10': 'optimPathSizeCol'
    },
    {
      '1': 'optim_control_size_row',
      '3': 5,
      '4': 1,
      '5': 5,
      '10': 'optimControlSizeRow'
    },
    {
      '1': 'optim_control_size_col',
      '3': 6,
      '4': 1,
      '5': 5,
      '10': 'optimControlSizeCol'
    },
    {'1': 'functional_value', '3': 7, '4': 1, '5': 1, '10': 'functionalValue'},
  ],
};

/// Descriptor for `OptimalResponse`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List optimalResponseDescriptor = $convert.base64Decode(
    'Cg9PcHRpbWFsUmVzcG9uc2USHQoKb3B0aW1fcGF0aBgBIAMoAVIJb3B0aW1QYXRoEiMKDW9wdG'
    'ltX2NvbnRyb2wYAiADKAFSDG9wdGltQ29udHJvbBItChNvcHRpbV9wYXRoX3NpemVfcm93GAMg'
    'ASgFUhBvcHRpbVBhdGhTaXplUm93Ei0KE29wdGltX3BhdGhfc2l6ZV9jb2wYBCABKAVSEG9wdG'
    'ltUGF0aFNpemVDb2wSMwoWb3B0aW1fY29udHJvbF9zaXplX3JvdxgFIAEoBVITb3B0aW1Db250'
    'cm9sU2l6ZVJvdxIzChZvcHRpbV9jb250cm9sX3NpemVfY29sGAYgASgFUhNvcHRpbUNvbnRyb2'
    'xTaXplQ29sEikKEGZ1bmN0aW9uYWxfdmFsdWUYByABKAFSD2Z1bmN0aW9uYWxWYWx1ZQ==');

@$core.Deprecated('Use optimizeParamsRequestDescriptor instead')
const OptimizeParamsRequest$json = {
  '1': 'OptimizeParamsRequest',
  '2': [
    {'1': 'learning_rate', '3': 1, '4': 1, '5': 1, '10': 'learningRate'},
    {'1': 'left_edge', '3': 2, '4': 3, '5': 1, '10': 'leftEdge'},
    {'1': 'right_edge', '3': 3, '4': 3, '5': 1, '10': 'rightEdge'},
    {'1': 'epochs', '3': 4, '4': 1, '5': 5, '10': 'epochs'},
  ],
};

/// Descriptor for `OptimizeParamsRequest`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List optimizeParamsRequestDescriptor = $convert.base64Decode(
    'ChVPcHRpbWl6ZVBhcmFtc1JlcXVlc3QSIwoNbGVhcm5pbmdfcmF0ZRgBIAEoAVIMbGVhcm5pbm'
    'dSYXRlEhsKCWxlZnRfZWRnZRgCIAMoAVIIbGVmdEVkZ2USHQoKcmlnaHRfZWRnZRgDIAMoAVIJ'
    'cmlnaHRFZGdlEhYKBmVwb2NocxgEIAEoBVIGZXBvY2hz');

@$core.Deprecated('Use optimizeParamsResponseDescriptor instead')
const OptimizeParamsResponse$json = {
  '1': 'OptimizeParamsResponse',
};

/// Descriptor for `OptimizeParamsResponse`. Decode as a `google.protobuf.DescriptorProto`.
final $typed_data.Uint8List optimizeParamsResponseDescriptor =
    $convert.base64Decode('ChZPcHRpbWl6ZVBhcmFtc1Jlc3BvbnNl');
