import 'package:flutter/material.dart';

class SettingsTab extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        // title: Text('Settings'),
        backgroundColor: Color(0xFF1877F2), // Facebook blue color
        toolbarHeight: 40.0, // Set the height of the AppBar
      ),
      body: Center(
        child: Text('Settings Tab Content'),
      ),
    );
  }
}