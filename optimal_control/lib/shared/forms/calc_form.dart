import 'package:flutter/material.dart';
import '../ui/text.dart';
import '../ui/formuls.dart';
class CalcForm extends StatefulWidget {
  final List<String> equations;
  final List<String> linked;
  final String hamilton;
  final String functional;
  final String t0;
  final String t1;
  final String tstep;
  final List<String> x0;
  final List<String> u0;
  final String delta;
  final VoidCallback calculateCallback;
  final VoidCallback optContrNavCallback;
  final VoidCallback optPathNavCallback;
  const CalcForm({
    super.key,
    required this.hamilton,
    required this.functional,
    required this.linked,
    required this.equations,
    required this.calculateCallback,
    required this.optPathNavCallback,
    required this.optContrNavCallback,
    required this.t0,
    required this.t1,
    required this.x0,
    required this.u0,
    required this.tstep,
    required this.delta
  });
  @override 
  State<CalcForm> createState() => _CalcFormState();
}
class _CalcFormState extends State<CalcForm> {
  final ScrollController _controller = ScrollController();
  @override 
  void dispose() {
    _controller.dispose();
    super.dispose();
  }
  @override 
  Widget build(BuildContext context) {
    List<Widget> formWidgets = [];
    formWidgets.add(Inscription( 
        text: 'Введенная задача:'
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(Inscription( 
        text: 'Уравнения движения:'
    ));
    formWidgets.add(SizedBox(height: 10));
    for (int i = 0; i < widget.equations.length; i++) {
      formWidgets.add(EquationText( 
          equation: widget.equations[i],
          number: (i + 1).toString()
      ));
      formWidgets.add(SizedBox(height: 10));
    }
    formWidgets.add(Inscription( 
        text: 'Уравнения сопряженной задачи:'
    ));
    formWidgets.add(SizedBox(height: 10));
    for (int i = 0; i < widget.linked.length; i++) {
      formWidgets.add(LinkedText( 
          number: (i + 1).toString(),
          linked: widget.linked[i]
      ));
      formWidgets.add(SizedBox(height: 10));
    }
    formWidgets.add(Inscription( 
        text: 'Функционал:'
    ));
    formWidgets.add(FunctionalText( 
        functional: widget.functional
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(Inscription( 
        text: 'Функция Гамильтона-Понтрягина:'
    ));
    formWidgets.add(HamiltonText( 
        hamilton: widget.hamilton
    ));
    formWidgets.add(SizedBox(height: 20));
    formWidgets.add(Inscription( 
        text: 'Введенные параметры:'
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(Tex( 
        text: 't_{0} = ' + widget.t0
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(Tex( 
        text: 't_{1} = ' + widget.t1
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(Tex( 
        text: 't_{step} = ' + widget.tstep
    ));
    formWidgets.add(SizedBox(height: 10));
    for (int i = 0; i < widget.x0.length; i++) {
      formWidgets.add(Tex( 
          text: 'x_{' + (i + 1).toString() + '}^{(0)} = ' + widget.x0[i]
      ));
      formWidgets.add(SizedBox(height: 10));
    }
    for (int i = 0; i < widget.u0.length; i++) {
      formWidgets.add(Tex( 
          text: 'u_{' + (i + 1).toString() + '}^{(0)} = ' + widget.u0[i]
      ));
      formWidgets.add(SizedBox(height: 10));
    }
    formWidgets.add(Tex( 
        text: r'\delta = ' + widget.delta
    ));
    formWidgets.add(SizedBox(height: 40));
    formWidgets.add(ElevatedButton.icon( 
        label: Text('Рассчитать'),
        icon: Icon(Icons.calculate_sharp),
        onPressed: widget.calculateCallback,
        style: ElevatedButton.styleFrom( 
          minimumSize: Size(400,50),
          shape:RoundedRectangleBorder( 
            side:BorderSide(color:Colors.lightBlue, width: 2),
            borderRadius: BorderRadius.only( 
              topLeft: Radius.circular(30),
              topRight: Radius.circular(5),
              bottomLeft: Radius.circular(5),
              bottomRight: Radius.circular(30)
            )
          )
        )
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(ElevatedButton.icon( 
        label: Text('График оптимальной тракетории'),
        icon: Icon(Icons.route),
        onPressed: widget.optPathNavCallback,
        style: ElevatedButton.styleFrom( 
          minimumSize: Size(400,50),
          shape: RoundedRectangleBorder( 
            side: BorderSide(color: Colors.lightBlue, width: 2),
            borderRadius: BorderRadius.only( 
              topLeft: Radius.circular(30),
              topRight: Radius.circular(5),
              bottomLeft: Radius.circular(5),
              bottomRight: Radius.circular(30)
            )
          )
        )
    ));
    formWidgets.add(SizedBox(height: 10));
    formWidgets.add(ElevatedButton.icon( 
      label: Text('График оптимального управления'),
      icon: Icon(Icons.engineering),
      onPressed: widget.optContrNavCallback,
      style: ElevatedButton.styleFrom( 
        minimumSize: Size(400,50),
        shape: RoundedRectangleBorder( 
          side: BorderSide(color: Colors.lightBlue, width: 2),
          borderRadius: BorderRadius.only( 
            topLeft: Radius.circular(30),
            topRight: Radius.circular(5),
            bottomLeft: Radius.circular(5),
            bottomRight: Radius.circular(30)
          )
        )
      )
    ));
    formWidgets.add(SizedBox(height:30));
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
                child: SingleChildScrollView ( 
                  controller: _controller,
                  child:Column( 
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: formWidgets
                  )
                )
              )
            )
          ]
        );
      }
    );
  }
}
