import 'package:flutter/material.dart';
import 'package:optimal_control/pages/opt_control_page.dart';
import 'package:optimal_control/pages/tasks_params_page.dart';
import './pages/home_page.dart';
import './pages/optimize_params_page.dart';
import 'package:google_fonts/google_fonts.dart';
import './pages/opt_path_page.dart';
void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Optimal Control',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.lightBlueAccent),
        textTheme: GoogleFonts.ralewayTextTheme(Theme.of(context).textTheme)
      ),
      initialRoute: '/',
      routes: {
        '/': (context) => HomePage(title: 'Home'),
        '/optimize_params' : (context) => OptimizeParamsPage(title: 'Optimize Params'),
        '/tasks_params' : (context) => TasksParamsPage(title: 'Tasks Params'),
        '/opt_path' : (context) => OptPathPage(title: 'Optimal Path'),
        '/opt_control' : (context) => OptControlPage(title: 'Optimal Control')
      },
    );
  }
}
