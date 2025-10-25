import 'package:flutter/material.dart';
import '../store.dart';
import '../shared/ui/input.dart';
import '../shared/ui/menu_button.dart';
import '../shared/ui/text.dart';
class HomePage extends StatefulWidget 
{
  const HomePage({super.key, required this.title});
  final String title;
  @override 
  State<HomePage> createState() => _HomePageState();
}
class _HomePageState extends State<HomePage>
{
  @override 
  Widget build(BuildContext context)
  {
    return Scaffold( 
      appBar: AppBar( 
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title)
      ),
      body: Center( 
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Row( 
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Inscription( 
                  text: 'Размерность задачи:'
                ),
                SizedBox(width: 10),
                InputFieldCast( 
                  width: 80,
                  height: 40,
                  label: 'dim',
                  hintLabel: 'dim',
                  changeCallback: (value) { AppStore().dim = value; }
                )
              ]
            ),
            SizedBox(height: 30),
            CastMenuButton( 
              text: 'Параметры оптимизатора',
              icon: Icons.flash_on,
              callback: ()=>{Navigator.pushNamed(context, '/optimize_params') },
            ),
            SizedBox(height: 30),
            CastMenuButton( 
              text: 'Уравнения задачи',
              icon: Icons.task,
              callback: () => { Navigator.pushNamed(context, '/tasks_equations') }
            ),
            SizedBox(height: 30),
            CastMenuButton( 
              text: 'Параметры задачи',
              icon: Icons.air,
              callback: () => { Navigator.pushNamed(context, '/tasks_params') }
            ),
            SizedBox(height: 30),
            CastMenuButton( 
              text: 'Результаты расчетов',
              icon: Icons.calculate,
              callback: () => { Navigator.pushNamed(context, '/result_pages') }
            ),
          ]
        )
      )
    );
  }
}

