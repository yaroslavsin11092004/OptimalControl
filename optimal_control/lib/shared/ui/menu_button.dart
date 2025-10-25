import 'package:flutter/material.dart';
class CastMenuButton extends StatelessWidget {
  final VoidCallback callback;
  final IconData icon;
  final String text;
  const CastMenuButton({
    super.key,
    required this.callback,
    required this.icon,
    required this.text
  });
  @override 
  Widget build(BuildContext context) {
    return ElevatedButton.icon( 
      onPressed: callback,
      label: Text(this.text),
      icon: Icon(this.icon),
      style: ElevatedButton.styleFrom( 
        minimumSize: Size(300,70),
        shape: RoundedRectangleBorder( 
          borderRadius: BorderRadius.only( 
            topLeft: Radius.circular(30),
            topRight: Radius.circular(5),
            bottomLeft: Radius.circular(5),
            bottomRight: Radius.circular(30)
          )
        ),
        side: BorderSide(color: Colors.lightBlue, width: 2)
      )
    );
  }
}
