import 'package:flutter/material.dart';
class InputFieldCast extends StatelessWidget {
  final ValueChanged<String> changeCallback;
  final String label;
  final String hintLabel;
  final double width;
  final double height;
  const InputFieldCast({
    super.key,
    required this.changeCallback,
    required this.label,
    required this.hintLabel,
    required this.height,
    required this.width
  });
  @override 
  Widget build(BuildContext context) {
    return SizedBox( 
      width: width,
      height: height,
      child: TextField( 
        decoration: InputDecoration( 
          labelText: label,
          hintText: hintLabel,
          enabledBorder: OutlineInputBorder( 
            borderSide: BorderSide(color: Colors.blueAccent),
            borderRadius: BorderRadius.circular(10)
          ),
          focusedBorder: OutlineInputBorder( 
            borderSide: BorderSide(color:Colors.blue, width: 2),
            borderRadius: BorderRadius.circular(10)
          ),
          border: OutlineInputBorder()
        ),
        onChanged: changeCallback
      )
    );
  }
}
