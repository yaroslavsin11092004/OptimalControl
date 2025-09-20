import 'package:flutter/material.dart';
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
            ElevatedButton.icon( 
              onPressed: (){ Navigator.pushNamed(context, '/optimize_params'); },
              label: Text('Параметры оптимизатора'),
              icon: Icon(Icons.flash_on),
              style: ElevatedButton.styleFrom( 
                minimumSize: Size(250,50),
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
            ),
            SizedBox(height: 30),
            ElevatedButton.icon( 
              onPressed: (){  Navigator.pushNamed(context, '/tasks_params'); },
              label: Text('Параметры задачи'),
              icon: Icon(Icons.calculate),
              style: ElevatedButton.styleFrom( 
                minimumSize: Size(250,50),
                shape: RoundedRectangleBorder( 
                  borderRadius: BorderRadius.only( 
                    topLeft: Radius.circular(30),
                    bottomRight: Radius.circular(30),
                    topRight: Radius.circular(5),
                    bottomLeft: Radius.circular(5)
                  )
                ),
                side: BorderSide(color: Colors.lightBlue, width: 2)
              )
            ),
            SizedBox(height: 30),
            ElevatedButton.icon( 
              onPressed: (){ Navigator.pushNamed(context, '/opt_path'); },
              label: Text('Оптимальный путь'),
              icon: Icon(Icons.route),
              style: ElevatedButton.styleFrom( 
                minimumSize: Size(250,50),
                shape: RoundedRectangleBorder( 
                  borderRadius: BorderRadius.only( 
                    topLeft: Radius.circular(30),
                    bottomRight: Radius.circular(30),
                    topRight: Radius.circular(5),
                    bottomLeft: Radius.circular(5)
                  )
                ),
                side: BorderSide(color: Colors.lightBlue, width: 2)
              )
            ),
            SizedBox(height: 30),
            ElevatedButton.icon( 
              onPressed: (){Navigator.pushNamed(context, '/opt_control');},
              label: Text('Оптимальное управление'),
              icon: Icon(Icons.tune),
              style: ElevatedButton.styleFrom( 
                minimumSize: Size(250,50),
                shape: RoundedRectangleBorder( 
                  borderRadius: BorderRadius.only( 
                    topLeft: Radius.circular(30),
                    bottomRight: Radius.circular(30),
                    topRight: Radius.circular(5),
                    bottomLeft: Radius.circular(5)
                  )
                ),
                side: BorderSide(color: Colors.lightBlue, width: 2)
              )
            )
          ]
        )
      )
    );
  }
}

