import 'package:flutter_math_fork/flutter_math.dart';
import 'package:flutter/material.dart';
import './input.dart';
class Equation extends StatelessWidget{
  final String number;
  final ValueChanged<String> callback;
  const Equation({
    super.key,
    required this.number,
    required this.callback
  });
  @override
  Widget build(BuildContext context) {
    return Row( 
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Math.tex( 
          r'\frac{dx_{'+ this.number + r'}}{dt} = ',
          mathStyle: MathStyle.display,
          textStyle: TextStyle(fontSize: 24)
        ),
        SizedBox(width: 10),
        InputFieldCast( 
          width: 300,
          height: 50,
          changeCallback: callback,
          label: 'equation',
          hintLabel: 'f(x,u,t)'
        )
      ]
    );
  }
}
class Linked extends StatelessWidget{
  final String number;
  final ValueChanged<String> callback;
  const Linked({
    super.key,
    required this.callback,
    required this.number
  });
  @override 
  Widget build(BuildContext context) {
    return Row( 
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Math.tex( 
          r'\frac{d\psi_{' + this.number + r'}}{dt} = ',
          mathStyle: MathStyle.display,
          textStyle: TextStyle(fontSize: 24)
        ),
        SizedBox(width: 10),
        InputFieldCast( 
          changeCallback: callback,
          width: 300,
          height: 50,
          label: 'linked',
          hintLabel: 'g(x,u,psi,t)',
        )
      ]
    );
  }
}
class Functional extends StatelessWidget {
  final ValueChanged<String> callback;
  const Functional({
    super.key,
    required this.callback
  });
  @override 
  Widget build(BuildContext context){
    return Row( 
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Math.tex( 
          r'J[x,u] = \int_{t_0}^{t_1}',
          mathStyle: MathStyle.display,
          textStyle: TextStyle(fontSize: 24)
        ),
        InputFieldCast( 
          label: 'functional',
          hintLabel: 'F(x,u,t)',
          changeCallback: callback,
          width: 300,
          height: 50
        ),
        SizedBox(width: 10),
        Math.tex( 
          r'dt',
          mathStyle: MathStyle.display,
          textStyle: TextStyle(fontSize: 24)
        )
      ]
    );
  }
}
class Hamilton extends StatelessWidget {
  final ValueChanged<String> callback;
  const Hamilton({
    super.key,
    required this.callback
  });
  @override 
  Widget build(BuildContext context){
    return Row( 
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        Math.tex( 
          r'H(x,u,\psi,t) = ',
          mathStyle: MathStyle.display,
          textStyle: TextStyle(fontSize: 24)
        ),
        SizedBox(width: 10),
        InputFieldCast(
          changeCallback: callback,
          label: 'hamiltonian',
          hintLabel: 'H(x,u,psi,t)',
          width: 300,
          height: 50
        )
      ]
    );
  }
}
class EquationText extends StatelessWidget {
  final String equation;
  final String number;
  const EquationText({
    super.key,
    required this.equation,
    required this.number
  });
  @override 
  Widget build(BuildContext context) {
    return Math.tex( 
      r'\frac{dx_{' + this.number + r'}}{dt} = ' + this.equation,
      mathStyle: MathStyle.display,
      textStyle: TextStyle(fontSize: 24)
    );
  }
}
class LinkedText extends StatelessWidget {
  final String linked;
  final String number;
  const LinkedText({
    super.key,
    required this.number,
    required this.linked
  });
  @override
  Widget build(BuildContext context) {
    return Math.tex( 
      r'\frac{d\psi_{' + this.number + r'}}{dt} = ' + this.linked,
      mathStyle: MathStyle.display,
      textStyle: TextStyle(fontSize: 24),
    );
  }
}
class FunctionalText extends StatelessWidget {
  final String functional;
  const FunctionalText({
    super.key,
    required this.functional
  });
  @override 
  Widget build(BuildContext context) {
    return Math.tex( 
      r'J[x,u] = \int_{t_0}^{t_1}(' + this.functional + r')dt',
      mathStyle: MathStyle.display,
      textStyle: TextStyle(fontSize: 24)
    );
  }
}
class HamiltonText extends StatelessWidget {
  final String hamilton;
  const HamiltonText({
    super.key,
    required this.hamilton
  });
  @override 
  Widget build(BuildContext context) {
    return Math.tex( 
      r'H(x,u,\psi,t) = ' + this.hamilton,
      mathStyle: MathStyle.display,
      textStyle: TextStyle(fontSize: 24),
    );
  }
}
class Tex extends StatelessWidget {
  final String text;
  const Tex({
    super.key,
    required this.text
  });
  @override 
  Widget build(BuildContext context) {
    return Math.tex( 
      text,
      mathStyle: MathStyle.display,
      textStyle: TextStyle(fontSize: 24),
    );
  }
}
