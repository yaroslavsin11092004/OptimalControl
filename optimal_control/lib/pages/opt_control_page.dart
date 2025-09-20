import '../store.dart';
import '../services/chart_api.dart';
import 'package:flutter/material.dart';
class OptControlPage extends StatefulWidget 
{
  OptControlPage({super.key, required this.title});
  final String title;
  @override 
  State<OptControlPage> createState() => _OptControlPageState();
}
class _OptControlPageState extends State<OptControlPage> 
{
  @override 
  Widget build(BuildContext context) 
  {
    if (storeControl.length != 0)
      return ChartApi(storeControl, sizeRowControl, sizeColControl, 'Оптимальное управление');
    else 
      return Scaffold( 
        appBar: AppBar( 
          title: Text(widget.title),
          backgroundColor: Theme.of(context).colorScheme.inversePrimary
        )
      );
  }
}
