import 'package:flutter/material.dart';
import '../store.dart';
import '../shared/forms/calc_form.dart';
import '../services/expr_parser.dart';
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
  @override
  Widget build(BuildContext context) {
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
          calculateCallback: () => {},
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
