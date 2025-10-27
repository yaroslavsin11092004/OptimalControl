import '../store.dart';
import '../services/chart_api.dart';
import 'package:flutter/material.dart';
class OptPathPage extends StatefulWidget 
{
  const OptPathPage({super.key, required this.title});
  final String title;
  @override 
  State<OptPathPage> createState() => _OptPathPageStats();
}
class _OptPathPageStats extends State<OptPathPage> 
{
  @override 
  Widget build(BuildContext context) 
  {
    if (AppStore().storePath.length != 0)
    {
      return ChartApi(AppStore().storePath, AppStore().sizeRowPath, AppStore().sizeColPath, 'Оптимальные траектории');
    }
    else return Scaffold(
      appBar: AppBar( 
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title)
      )
    );
  }
}
