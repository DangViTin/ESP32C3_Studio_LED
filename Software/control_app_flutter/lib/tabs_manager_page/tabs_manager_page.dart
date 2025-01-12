import 'package:flutter/material.dart';
import 'package:control_app_flutter/tabs_manager_page/tabs/ble_tab.dart';
import 'package:control_app_flutter/tabs_manager_page/tabs/control_tab.dart';

class TabsManager extends StatefulWidget {
  @override
  TabsManagerState createState() => TabsManagerState();
}

class TabsManagerState extends State<TabsManager> {
  int currentIndex = 0;

  final List<Widget> tabs = [
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
        index: currentIndex,
        children: tabs,
      ),
      bottomNavigationBar: BottomNavigationBar(
        currentIndex: currentIndex,
        onTap: (index) {
          setState(() {
            currentIndex = index;
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
