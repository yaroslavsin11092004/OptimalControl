import 'package:flutter/material.dart';
import '../store.dart';
class TasksParamsPage extends StatefulWidget 
{
  const TasksParamsPage({super.key, required this.title});
  final String title;
  @override 
  State<TasksParamsPage> createState() => _TasksParamsPageState();
}
class _TasksParamsPageState extends State<TasksParamsPage> 
{
  String t0 = '';
  String t1 = '';
  String tStep = '';
  String u0 = '';
  String x0 = '';
  String delta = '';
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
        duration: Duration(seconds: 3),
        behavior: SnackBarBehavior.floating,
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
                  labelText: 't\u2080',
                  hintText: 'Начальное время',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { t0 = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 't\u2081',
                  hintText: 'Конечное время',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { t1 = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Шаг по времени',
                  hintText: 'Введите шаг по времени',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder(
                    borderSide: BorderSide(color: Colors.blue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { tStep = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Начальное приближение u',
                  hintText: 'Введите u\u2080',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { u0 = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Начальные значения x',
                  hintText: 'Введите x\u2080',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { x0 = value; }
              )
            ),
            SizedBox( 
              width: 300,
              height: 70,
              child: TextField( 
                decoration: InputDecoration( 
                  labelText: 'Точность',
                  hintText: 'Введите погрешность',
                  enabledBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blueAccent),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  focusedBorder: OutlineInputBorder( 
                    borderSide: BorderSide(color: Colors.blue, width: 2),
                    borderRadius: BorderRadius.circular(10)
                  ),
                  border: OutlineInputBorder()
                ),
                onChanged: (value) { delta = value; }
              )
            ),
            Padding( 
              padding: EdgeInsets.only(left: 100),
              child: ElevatedButton.icon( 
                onPressed: () async { 
                  try 
                  {
                    List<double> x0Data = coreApi.parseStringArray(x0);
                    List<double> u0Data = coreApi.parseStringArray(u0);
                    Map<String, dynamic> request_result = await coreApi.fetchSuccesiveApprox(double.parse(t0), double.parse(t1), double.parse(tStep), u0Data, x0Data, double.parse(delta));
                    storePath = (request_result['optim_path'] as List).cast<double>().toList();
                    storeControl = (request_result['optim_control'] as List).cast<double>().toList();
                    sizeRowPath = request_result['optim_path_size_row'] as int;
                    sizeColPath = request_result['optim_path_size_col'] as int;
                    sizeRowControl = request_result['optim_control_size_row'] as int;
                    sizeColControl = request_result['optim_control_size_col'] as int;
                    _showSuccessMessage(context, 'Calculation successful!');
                  }
                  catch(e) 
                  {
                    _showErrorMessage(context,'$e');
                  }
                },
                label: Text('Принять'),
                icon: Icon(Icons.check),
                style: ElevatedButton.styleFrom( 
                  minimumSize: Size(200, 50),
                  shape: RoundedRectangleBorder( 
                    borderRadius: BorderRadius.only( 
                      topLeft: Radius.circular(30),
                      bottomRight: Radius.circular(30),
                      bottomLeft: Radius.circular(5),
                      topRight: Radius.circular(5)
                    )
                  ),
                  side: BorderSide(color: Colors.lightBlue, width: 2)
                )
              )
            )
          ]
        )
      )
    );
  }
}
