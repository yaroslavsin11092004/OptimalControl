import 'package:flutter/material.dart';
class Inscription extends StatelessWidget {
  final String text;
  const Inscription({ 
    super.key,
    required this.text
  });
  @override 
  Widget build(BuildContext context) {
    return Text( 
      text,
      style: TextStyle( 
        fontSize: 24,
        color: Colors.blueAccent
      )
    );
  }
}
