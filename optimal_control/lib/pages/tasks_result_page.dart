import 'package:flutter/material.dart';
import '../store.dart';
import '../shared/forms/calc_form.dart';
import '../services/expr_parser.dart';
import '../gRPC/core.pb.dart';
import '../gRPC/core.pbgrpc.dart';
class TasksResultPage extends StatefulWidget {
  final String title;
  const TasksResultPage({ 
    super.key,
    required this.title
  });
  @override 
  State<TasksResultPage> createState() => _TasksResultPageState();
}
class _TasksResultPageState extends State<TasksResultPage> {
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
  Widget build(BuildContext context) {
    OptimalRequest request = OptimalRequest();
    try {
      ExprParser parser = ExprParser();
      List<String> eq = [];
      List<String> lk = [];
      String ham = '';
      String func = '';
      for (int i = 0; i < AppStore().equations.length; i++) {
        eq.add(parser.parse(AppStore().equations[i]));
        lk.add(parser.parse(AppStore().linked[i]));
      }
      ham = parser.parse(AppStore().hamilton);
      func = parser.parse(AppStore().functional);
      return Scaffold( 
        appBar: AppBar( 
          title: Text(widget.title),
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        ),
        body: CalcForm( 
          hamilton: ham,
          equations: eq,
          linked: lk,
          functional: func,
          calculateCallback: () async {
            try {
              request.t0 = double.parse(AppStore().t0);
              request.t1 = double.parse(AppStore().t1);
              request.tstep = double.parse(AppStore().tStep);
              request.delta = double.parse(AppStore().delta);
              request.x0.addAll(AppStore().x0.split(',').map((value)=>double.parse(value)).toList());
              request.u0.addAll(AppStore().u0.split(',').map((value)=>double.parse(value)).toList());
              request.equations.addAll(AppStore().equations);
              request.linked.addAll(AppStore().linked);
              request.hamilton = AppStore().hamilton;
              request.functional = AppStore().functional;
              final resp =  await AppStore().service.calcOptimal(request);
              AppStore().storePath.clear();
              AppStore().storeControl.clear();
              AppStore().storePath.addAll(resp.optimPath);
              AppStore().storeControl.addAll(resp.optimControl);
              AppStore().sizeRowPath = resp.optimPathSizeRow;
              AppStore().sizeColPath = resp.optimPathSizeCol;
              AppStore().sizeRowControl = resp.optimControlSizeRow;
              AppStore().sizeColControl = resp.optimControlSizeCol;
              AppStore().functionalValue = resp.functionalValue;
              print(AppStore().functionalValue);
              _showSuccessMessage(context, 'Рассчеты выполены успешно!');
            } catch(e) {
              _showErrorMessage(context, 'Некорректные данные::$e');
            }
          },
          optPathNavCallback: () => { Navigator.pushNamed(context, '/opt_path') },
          optContrNavCallback: () => { Navigator.pushNamed(context, '/opt_control') },
          t0: AppStore().t0,
          t1: AppStore().t1,
          x0: AppStore().x0.split(','),
          u0: AppStore().u0.split(','),
          delta: AppStore().delta,
          tstep: AppStore().tStep,
        )
      );
    }catch(e) {
      return Scaffold( 
        appBar: AppBar( 
          title: Text('Некорректно введенные данные!'),
          backgroundColor: Colors.red
        )
      );
    }
  }
}
