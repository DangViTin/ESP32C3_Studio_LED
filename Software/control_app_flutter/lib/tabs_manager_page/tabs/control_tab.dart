import 'package:flutter/material.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';
// import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'ble_tab.dart';

Color _currentColor = Colors.blue;
var dropdownListVal;
String log = "";

class CtrlTab extends StatefulWidget {
  @override
  CtrlTabState createState() => CtrlTabState();
}

// class CtrlTabState extends State<CtrlTab> {
//   String option = "Option 1\nOption 2\nOption 3";

// void _changeColor(Color color) async {
//   // Check if the color is the same as the current color to avoid unnecessary updates
//   if (_currentColor == color) return;

//   setState(() {
//     _currentColor = color;
//     // log += "Color changed to: $color\n";
//   });

//   // Convert the color to a list of integers (e.g., RGB values)
//   List<int> colorData = [color.red, color.green, color.blue];

//   // Send the color data to the BLE device
//   String response = await BleTabState().sendDataToCharacteristic("0193fe95-239a-7f23-826d-2b53e38f42b6", colorData);
//   // setState(() {
//   //   // log += "Response: $response\n";
//   // }
//   // );
// }

class CtrlTabState extends State<CtrlTab> {
  String option = "Option 1\nOption 2\nOption 3";

  // Store the last time the color was updated
  DateTime? _lastUpdatedTime;
  static const Duration debounceDuration = Duration(milliseconds: 100); // 100ms debounce

  void _changeColor(Color color) async {
    final now = DateTime.now();

    // Throttle updates to 100ms
    if (_lastUpdatedTime == null ||
        now.difference(_lastUpdatedTime!) > debounceDuration) {
      _lastUpdatedTime = now; // Update the last processed time

      // Check if the color is the same to avoid unnecessary updates
      if (_currentColor == color) 
      {
        return;
      }

      setState(() 
      {
        _currentColor = color;
      }
      );

      // Convert the color to a list of integers (e.g., RGB values)
      List<int> colorData = [color.red, color.green, color.blue];

      // Send the color data to the BLE device
      try
      {
        String response = await BleTabState().sendDataToCharacteristic("0193fe95-239a-7f23-826d-2b53e38f42b6", colorData,);
        print("Response: $response");
      }
      catch (e)
      {
        print("Error sending data to BLE: $e");
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Color(0xFF1877F2),
        toolbarHeight: 0,
      ),
      body: SingleChildScrollView(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            SizedBox(height: 20),
            Text(
              'RGB Color',
              style: TextStyle(fontSize: 30),
            ),
            // SizedBox(height: 20),
            ColorPicker(
              pickerColor: _currentColor,
              onColorChanged: _changeColor,
              enableAlpha: false,
              paletteType: PaletteType.hueWheel,
              pickerAreaHeightPercent:
                  0.8, // Adjust this value to make the color wheel more compact
            ),
            Divider(
              height: 40,
              thickness: 2,
            ),
            Text(
              'Effects',
              style: TextStyle(fontSize: 30),
            ),
            SizedBox(height: 10),
            Container(
              padding: EdgeInsets.symmetric(horizontal: 10),
              decoration: BoxDecoration(
                border: Border.all(color: Colors.grey),
                borderRadius: BorderRadius.circular(10),
              ),
              child: DropdownButtonHideUnderline(
                child: DropdownButton<String>(
                  value: dropdownListVal,
                  icon: Icon(Icons.arrow_drop_down, color: Colors.black),
                  style: TextStyle(color: Colors.black, fontSize: 18),
                  dropdownColor: Colors.white,
                  borderRadius: BorderRadius.circular(10),
                  onChanged: (String? newValue) {
                    // Handle change here
                    setState(() {
                      dropdownListVal = newValue as String;
                      // log += "Effect changed to: $dropdownListVal\n";
                    });
                  },
                  items: option
                      .split('\n')
                      .map<DropdownMenuItem<String>>((String value) {
                    return DropdownMenuItem<String>(
                      value: value,
                      child: Text(value),
                    );
                  }).toList(),
                ),
              ),
            ),
            Divider(
              height: 40,
              thickness: 2,
            ),
            Text(
              'Log',
              style: TextStyle(fontSize: 30),
            ),
            SizedBox(height: 10),
            Container(
              padding: EdgeInsets.symmetric(horizontal: 10),
              decoration: BoxDecoration(
                border: Border.all(color: Colors.grey),
                borderRadius: BorderRadius.circular(10),
              ),
              child: TextField(
                controller: TextEditingController(text: log),
                maxLines: 10,
                readOnly: true,
                decoration: InputDecoration(
                  border: InputBorder.none,
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
