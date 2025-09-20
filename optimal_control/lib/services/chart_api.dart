import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';
import 'dart:math' as math;
class ChartApi extends StatelessWidget
{
  List<List<FlSpot>> _mesh = [];
  double _minx = 0;
  double _maxx = 0;
  String _title = '';
  ChartApi(List<double> store, int sizeRow, int sizeCol, String title) 
  {
    _title = '$title';
     _minx = double.infinity;
     _maxx = double.negativeInfinity;
      _mesh.clear();
  for (int col = 1; col < sizeCol; col++) {
    List<FlSpot> buffer = [];
    for (int row = 0; row < sizeRow; row++) {
      int index = row * sizeCol;
      double x = store[index];
      double y = store[index + col];
      _minx = math.min(_minx, x);
      _maxx = math.max(_maxx, x);
      
      buffer.add(FlSpot(x, y));
    }
    
    _mesh.add(buffer);
    }
  }
  Color _colorForLine(int idx)
  {
    final colors = [Colors.blue, Colors.red, Colors.green, Colors.orange, Colors.pink, Colors.yellowAccent, Colors.redAccent];
    return colors[idx % colors.length];
  }
  List<LineChartBarData> _lineBarsData()
  {
    return _mesh.map((spots) => LineChartBarData( 
        spots: spots,
        isCurved: true,
        color: _colorForLine(_mesh.indexOf(spots)),
        barWidth: 2,
        isStrokeCapRound: true,
        dotData: FlDotData(show: true),
        belowBarData: BarAreaData(show: true, color: _colorForLine(_mesh.indexOf(spots)).withOpacity(0.3))
    )).toList();
  }
  @override 
  Widget build(BuildContext context)
  {
    return Scaffold( 
      appBar: AppBar(title: Text(_title), backgroundColor: Theme.of(context).colorScheme.inversePrimary),
      body: Padding( 
        padding: const EdgeInsets.all(16.0),
        child: LineChart( 
          LineChartData( 
            gridData: FlGridData(show: true),
            titlesData: FlTitlesData( 
              show: true,
              bottomTitles: AxisTitles(sideTitles: SideTitles(showTitles: true)),
              leftTitles: AxisTitles(sideTitles: SideTitles(showTitles: true)) 
            ),
            borderData: FlBorderData(show: true),
            minX: _minx,
            maxX: _maxx,
            lineBarsData: _lineBarsData(),
          ),
        ),
      ),
    );
  }
}
