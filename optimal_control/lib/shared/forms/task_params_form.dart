import 'package:flutter/material.dart';
import '../ui/text.dart';
import '../ui/input.dart';
class TaskParamsForm extends StatelessWidget {
  final ValueChanged<String> t0Callback;
  final ValueChanged<String> t1Callback;
  final ValueChanged<String> tStepCallback;
  final ValueChanged<String> u0Callback;
  final ValueChanged<String> x0Callback;
  final ValueChanged<String> deltaCallback;
  const TaskParamsForm({
    super.key,
    required this.t0Callback,
    required this.t1Callback,
    required this.u0Callback,
    required this.x0Callback,
    required this.tStepCallback,
    required this.deltaCallback,
  });
  @override 
  Widget build(BuildContext context) {
    return Column( 
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Inscription( 
          text: 'Введите параметры задачи:'
        ),
        SizedBox(height: 10),
        InputFieldCast( 
          width: 300,
          height: 70,
          label: 't0',
          hintLabel: 'Начальное время',
          changeCallback: t0Callback
        ),
        InputFieldCast( 
          width: 300,
          height: 70,
          label: 't1',
          hintLabel: 'Конечное время',
          changeCallback: t1Callback
        ),
        InputFieldCast( 
          width: 300,
          height: 70,
          label: 't step',
          hintLabel: 'Шаг по времени',
          changeCallback: tStepCallback
        ),
        InputFieldCast( 
          width: 300,
          height: 70,
          label: 'u0',
          hintLabel: 'Начальные приближения u',
          changeCallback: u0Callback
        ),
        InputFieldCast( 
          width: 300,
          height: 70,
          label: 'x0',
          hintLabel: 'Начальные значения x',
          changeCallback: x0Callback
        ),
        InputFieldCast( 
          width: 300,
          height: 70,
          label: 'delta',
          hintLabel: 'Точность',
          changeCallback: deltaCallback
        ),
      ]
    );
  }
}
