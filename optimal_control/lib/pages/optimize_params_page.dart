import 'package:flutter/material.dart';
import 'package:optimal_control/services/core_api.dart';
import '../store.dart';
class OptimizeParamsPage extends StatefulWidget 
{
  const OptimizeParamsPage({super.key, required this.title});
  final String title;
  @override 
  State<OptimizeParamsPage> createState() => _StateOptimizeParamsPage();
}
class _StateOptimizeParamsPage extends State<OptimizeParamsPage> 
{
  String learningRate = '';
  String leftEdge = '';
  String rightEdge = '';
  String epochs = '';
  String delta = '';
  CoreApi api = coreApi;
  void _showErrorMessage(BuildContext context, String message) {
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text(message),
        backgroundColor: Colors.red,
        duration: Duration(seconds: 3),
        behavior: SnackBarBehavior.floating,
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
      ),
    );
  }
  void _showSuccessMessage(BuildContext context, String message) 
  {
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar( 
        content: Text(message),
        backgroundColor: Colors.green,
        behavior: SnackBarBehavior.floating,
        duration: Duration(seconds:3),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10))
      )
    );
  }
  @override 
  Widget build(BuildContext context)
  {
    return Scaffold(
      appBar: AppBar( 
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title)  
      ),
      body: Center( 
        child: Column( 
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'Введите параметры:',
              style: TextStyle( 
                fontSize: 24,
                color: Colors.blueAccent
              )
            ),
            SizedBox(height: 30),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Cкорость спуска',
                  hintText: 'Введите скорость спуска',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value){learningRate = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Левые границы u(t)',
                  hintText: 'Введите левые границы',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { leftEdge = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Правые границы u(t)',
                  hintText: 'Введите правые границы',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { rightEdge = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Эпохи',
                  hintText: 'Введите количество эпох',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.lightBlue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { epochs = value; }
              )
            ),
            Padding( 
              padding: EdgeInsets.only(left: 100),
              child: ElevatedButton.icon( 
                onPressed: () async { 
                  try 
                  {
                    List<double> leftEdgeStore = api.parseStringArray(leftEdge);
                    List<double> rightEdgeStore = api.parseStringArray(rightEdge);
                    await api.fetchSetAdamParams(double.parse(learningRate), leftEdgeStore, rightEdgeStore, int.parse(epochs));
                    _showSuccessMessage(context, 'Параметры успешно применены');
                  }
                  catch(e)
                  {
                    _showErrorMessage(context, 'Ошибка ввода');
                  }
                },
                label: Text('Принять'),
                icon: Icon(Icons.check),
                style: ElevatedButton.styleFrom( 
                  minimumSize: Size(200,50),
                  shape: RoundedRectangleBorder( 
                    borderRadius: BorderRadius.only( 
                      topLeft: Radius.circular(30),
                      bottomRight: Radius.circular(30),
                      topRight: Radius.circular(5),
                      bottomLeft: Radius.circular(5)
                    ),
                    side: BorderSide(color: Colors.lightBlue, width: 2)
                  )
                )
              )
            )
          ]
        )
      )
    );
  } 
}
