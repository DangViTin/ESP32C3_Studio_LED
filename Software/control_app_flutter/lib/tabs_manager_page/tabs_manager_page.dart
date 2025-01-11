import 'package:flutter/material.dart';
import 'package:control_app_flutter/tabs_manager_page/tabs/ble_tab.dart';
import 'package:control_app_flutter/tabs_manager_page/tabs/control_tab.dart';

class TabsManager extends StatefulWidget {
  @override
  _TabsManagerState createState() => _TabsManagerState();
}

class _TabsManagerState extends State<TabsManager> {
  int _currentIndex = 0;

  final List<Widget> _tabs = [
    BleTab(),
    CtrlTab(),
  ];

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('BLE Tabs Manager'),
        backgroundColor: Color(0xFF1877F2), // Facebook blue color
        toolbarHeight: 40.0, // Set the height of the AppBar
      ),
      body: IndexedStack(
        index: _currentIndex,
        children: _tabs,
      ),
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: _currentIndex,
        onTap: (index) {
          setState(() {
            _currentIndex = index;
          });
        },
        items: const [
          BottomNavigationBarItem(
            icon: Icon(Icons.bluetooth),
            label: 'Bluetooth',
          ),
          BottomNavigationBarItem(
            icon: Icon(Icons.gamepad),
            label: 'Control',
          ),
        ],
      ),
    );
  }
}