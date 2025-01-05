import 'package:flutter/material.dart';
// import 'package:control_app_flutter/login_page/login_page.dart';
import 'package:control_app_flutter/tabs_manager_page/tabs_manager_page.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      // home: LoginPage(),
      home: TabsManager(),
    );
  }
}
