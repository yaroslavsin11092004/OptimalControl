import 'package:flutter/material.dart';
import '../ui/text.dart';
import '../ui/formuls.dart';
class TaskForm extends StatefulWidget {
  final List<ValueChanged<String>> callbacks;
  final int dim;
  const TaskForm({
    super.key,
    required this.callbacks,
    required this.dim,
  });
  @override 
  State<TaskForm> createState() => _TaskFormState();
}
class _TaskFormState extends State<TaskForm> {
  final ScrollController _controller = ScrollController();
  @override 
  void dispose() {
    _controller.dispose();
    super.dispose();
  }
  @override 
  Widget build(BuildContext context) {
    List<Widget> tasks_formuls = [];
    for (int i = 0; i < widget.dim * 2; i++) {
      if (i < widget.dim) {
        if (i == 0) {
          tasks_formuls.add( 
            Inscription(
              text: 'Уравнения движения:'
            )
          );
          tasks_formuls.add(SizedBox(width:10));
        }
        tasks_formuls.add( 
          Equation( 
            number: (i + 1).toString(),
            callback: widget.callbacks[i] 
          )
        );
        tasks_formuls.add(SizedBox(height: 10));
      }
      else {
        if (i == widget.dim) {
          tasks_formuls.add( 
            Inscription( 
              text: 'Уравнения сопряженной задачи:'
            )
          );
          tasks_formuls.add(SizedBox(height:10));
        }
        tasks_formuls.add( 
          Linked( 
            number: (i - widget.dim + 1).toString(),
            callback: widget.callbacks[i]
          )
        );
        tasks_formuls.add(SizedBox(height: 10));
      }
    }
    tasks_formuls.add( 
      Inscription( 
        text: 'Функционал:'
      )
    );
    tasks_formuls.add(SizedBox(height: 10));
    tasks_formuls.add( 
      Functional( 
        callback: widget.callbacks[widget.dim * 2],
      )
    );
    tasks_formuls.add(SizedBox(height: 10));
    tasks_formuls.add( 
      Inscription(
        text: 'Функция Гамильтона-Понтрягина:'
      )
    );
    tasks_formuls.add(SizedBox(height: 10));
    tasks_formuls.add( 
      Hamilton( 
        callback: widget.callbacks[widget.dim * 2 + 1]
      )
    );
    tasks_formuls.add(SizedBox(height: 30));
    return LayoutBuilder( 
      builder: (BuildContext context, BoxConstraints constraints) {
        return Row(
          children: [
            SizedBox( 
            width: constraints.maxWidth,
            height: constraints.maxHeight,
            child: Scrollbar( 
              thickness: 20.0,
              thumbVisibility: false,
              controller: _controller,
              child: SingleChildScrollView( 
                controller: _controller,
                child: Column( 
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: tasks_formuls 
                )
              )
            ))
          ]
        );
      });
  }
}
