import 'package:flutter/material.dart';
import '../store.dart';
import '../shared/forms/input_task_form.dart';
class TaskEquationPage extends StatefulWidget {
  final String title;
  const TaskEquationPage({
    super.key,
    required this.title
  });
  @override 
  State<TaskEquationPage> createState() => _TaskEquationPageState();
}
class _TaskEquationPageState extends State<TaskEquationPage> {
  @override 
  Widget build(BuildContext context) {
    try {
      int dim_val = int.parse(AppStore().dim);
      List<ValueChanged<String>> callbacks = [];
      AppStore().equations.clear();
      AppStore().linked.clear();
      for (int i = 0; i < dim_val; i++) {
        AppStore().equations.add('');
        AppStore().linked.add('');
        int curIndex = i;
        callbacks.add((value) {
          AppStore().equations[curIndex] = value;
        });
      }
      for (int i = 0; i < dim_val; i++) {
        int curIndex = i;
        callbacks.add((value) {
          AppStore().linked[curIndex] = value;
        });
      }
      callbacks.add((value) {
        AppStore().functional = value;
      });
      callbacks.add((value) {
        AppStore().hamilton = value;
      });
      return Scaffold( 
        appBar: AppBar( 
          backgroundColor: Theme.of(context).colorScheme.inversePrimary,
          title: Text(widget.title)
        ),
        body: TaskForm( 
          dim: dim_val,
          callbacks: callbacks,
        )
      );
    }catch(e) {
      return Scaffold( 
        appBar: AppBar( 
          backgroundColor: Colors.red,
          title: Text('Некорректно введенная размерность задачи!')
        )
      );
    }
  }
}
