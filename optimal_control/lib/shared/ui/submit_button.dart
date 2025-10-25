import 'package:flutter/material.dart';
class SubmitButton extends StatelessWidget {
  final VoidCallback onPressed;
  final String text;
  final IconData icon;
  final bool isLoading;
  const SubmitButton({
    super.key,
    required this.onPressed,
    required this.text,
    this.icon = Icons.check,
    this.isLoading = false 
  });
  @override 
  Widget build(BuildContext context) {
    return Padding( 
      padding: const EdgeInsets.only(left:200),
      child: ElevatedButton.icon( 
        onPressed: isLoading ? null : onPressed,
        label: isLoading ? SizedBox(width: 20, height: 20, child: CircularProgressIndicator(strokeWidth: 2))
        : Text(text),
        icon: isLoading ? SizedBox() : Icon(icon),
        style: ElevatedButton.styleFrom( 
          minimumSize: Size(200,50),
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
      )
    );
  }
}
