import 'dart:io';
import 'dart:convert';
import 'package:http/http.dart' as http;

class CoreApi 
{
  String _endpointPref = '';
  
  CoreApi(String confPath) { _loadConfig(confPath); }
  Future<void> _loadConfig(String confPath) async 
  {
    try 
    {
      final file = File(confPath);
      final String buffer = await file.readAsString();
      final Map<String, dynamic> confData = jsonDecode(buffer);
      final String host = confData['CoreServer']?['host']?.toString() ?? 'localhost';
      final String port = confData['CoreServer']?['port']?.toString() ?? '45000';
      _endpointPref = 'http://$host:$port';
    }
    catch(e)
    {
      print('Error::$e');
    }
  }
  Future<void> fetchSetAdamParams(double learningRate, List<double> leftEdge, List<double> rightEdge, int epochs) async 
  {
    final response = await http.post( 
      Uri.parse('$_endpointPref/set_params'),
      headers: {
        'Content-Type' : 'application/json'
      },
      body: jsonEncode( 
      {
        'learning_rate' : learningRate,
        'left_edge' : leftEdge,
        'right_edge' : rightEdge,
        'epochs' : epochs
      })
    );
    if (response.statusCode == 200) print('Request set params successfull!');
    else print('Request set params failed!');
  }
  Future<Map<String, dynamic>> fetchSuccesiveApprox(double t0, double t1, double tStep, List<double> u0, List<double> x0, double delta) async
  {
    final response = await http.post( 
      Uri.parse('$_endpointPref/successive_approx'),
      headers: {
        'Content-Type' : 'application/json'
      },
      body: jsonEncode( 
      {
        't0' : t0,
        't1' : t1,
        't_step' : tStep,
        'u0' : u0,
        'x0' : x0,
        'delta' : delta
      })
    );
    if (response.statusCode == 200)
    {
      print('Request successive approx successfull!');
      return jsonDecode(response.body);
    }
    else
    {
      print('Request successive approx failed');
      return {}; 
    }
  }
}
