import 'package:flutter/material.dart';
import '../shared/forms/task_params_form.dart';
import '../store.dart';
class TasksParamsPage extends StatefulWidget {
  final String title;
  const TasksParamsPage({
    super.key,
    required this.title
  });
  @override 
  State<TasksParamsPage> createState() => _TasksParamsPageState();
}
class _TasksParamsPageState extends State<TasksParamsPage> {
  @override 
  Widget build(BuildContext context) {
    return Scaffold( 
      appBar: AppBar( 
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title)
      ),
      body:Center( 
          child: TaskParamsForm( 
            t0Callback: (value) { AppStore().t0 = value; },
            t1Callback: (value) { AppStore().t1 = value; },
            tStepCallback: (value) { AppStore().tStep = value; },
            u0Callback: (value) { AppStore().u0 = value; },
            x0Callback: (value) { AppStore().x0 = value; },
            deltaCallback: (value) { AppStore().delta = value; },
          )
      )
    );
  }
}
