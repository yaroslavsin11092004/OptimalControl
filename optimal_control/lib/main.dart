import 'package:flutter/material.dart';
import 'package:optimal_control/pages/opt_control_page.dart';
import 'package:optimal_control/pages/tasks_params_page.dart';
import './pages/home_page.dart';
import './pages/optimize_params_page.dart';
import 'package:google_fonts/google_fonts.dart';
import './pages/opt_path_page.dart';
import './pages/task_equation_page.dart';
import './pages/tasks_params_page.dart';
import './pages/tasks_result_page.dart';
void main() async {
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
        '/': (context) => HomePage(title: 'Главная'),
        '/optimize_params' : (context) => OptimizeParamsPage(title: 'Параметры оптимизатора'),
        '/tasks_equations' : (context) => TaskEquationPage(title: 'Уравнения задачи'),
        '/opt_path' : (context) => OptPathPage(title: 'Optimal Path'),
        '/opt_control' : (context) => OptControlPage(title: 'Optimal Control'),
        '/tasks_params' : (context) => TasksParamsPage(title: 'Параметры задачи'),
        '/result_pages' : (context) => TasksResultPage(title: 'Результаты расчетов')
      },
    );
  }
}
