import './services/core_api.dart';
CoreApi coreApi = CoreApi('/home/yaroslavsinyakov/source/OptimalControl/release/config_system.json');
List<double> storePath = [];
List<double> storeControl = [];
int sizeRowPath = 0;
int sizeRowControl = 0;
int sizeColPath = 0;
int sizeColControl = 0;

class AppStore {
  static final AppStore _instance = AppStore._internal();
  factory AppStore() => _instance;
  AppStore._internal();
  String dim = "";
  List<double> storePath = [];
  List<double> sotreControl = [];
  int sizeRowPath = 0;
  int sizeColPath = 0;
  int sizeRowControl = 0;
  int sizeColControl = 0;
  List<String> equations = [];
  List<String> linked = [];
  String hamilton = '';
  String functional = '';
  String t0 = '';
  String t1 = '';
  String u0 = '';
  String x0 = '';
  String tStep = '';
  String delta = '';
}
class ParserStore {
  static final ParserStore _instance = ParserStore._internal();
  factory ParserStore() => _instance;
  ParserStore._internal();
}
