import 'package:flutter/material.dart';
import 'dart:async';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';
import 'ble_tab.dart';

Color _currentColor = Colors.blue;
String _currentEffect = "";
String dropdownListVal = "";
List<String> option_list = [];
String log = "";
const RGB_UUID = "0193fe95-239a-7f23-826d-2b53e38f42b6";
const effect_UUID = "0193fe95-239a-7f23-826d-2b53e38f42b7";
int alreadyRead = 0;

class CtrlTab extends StatefulWidget {
  @override
  CtrlTabState createState() => CtrlTabState();
}

class CtrlTabState extends State<CtrlTab> {
  String option = "";
  Timer? _timer;

  // Store the last time the color was updated
  DateTime? _lastUpdatedTime;
  static const Duration debounceDuration =
      Duration(milliseconds: 100); // 100ms debounce

  void _changeColor(Color color) async {
    final now = DateTime.now();

    // Limit updates to 100ms
    if (_lastUpdatedTime == null ||
        now.difference(_lastUpdatedTime!) > debounceDuration) {
      _lastUpdatedTime = now; // Update the last processed time

      // Check if the color is the same to avoid unnecessary updates
      if (_currentColor == color) {
        return;
      }

      setState(() {
        _currentColor = color;
      });

      // Convert the color to a list of integers (e.g., RGB values)
      List<int> colorData = [color.red, color.green, color.blue];

      // Send the color data to the BLE device
      try {
        String response =
            await BleTabState().sendDataToCharacteristic(RGB_UUID, colorData);
        print("Response: $response");
      } catch (e) {
        print("Error sending data to BLE: $e");
      }
    }
  }

  void _changeEffect(String effect) async {
    // Check if the effect is the same to avoid unnecessary updates
    if (_currentEffect == effect) {
      return;
    }

    setState(() {
      _currentEffect = effect;
    });

    try {
      List<int> effectToSend = effect.codeUnits;
      String response = await BleTabState()
          .sendDataToCharacteristic(effect_UUID, effectToSend);
      print("Response: $response");
    } catch (e) {
      print("Error sending data to BLE: $e");
    }
  }

  void readEffect() async {
    await BleTabState().readDatafromCharacteristic(effect_UUID);
  }

  @override
  void initState() {
    super.initState();

    // Polling periodically
    _timer = Timer.periodic(Duration(milliseconds: 200), (timer) {
      if (BleTabState.connectedDevice != null && alreadyRead == 0) {
        readEffect();
        option = String.fromCharCodes(BleTabState.read_data);
        log += "Read raw: ${BleTabState.read_data} \n";
        log += "Read list: $option\n";

        option_list = option.split('\n');
        dropdownListVal = option_list.first;

        if (option_list.length > 1) {
          setState(() {
            alreadyRead = 1;
            _timer?.cancel();
          });
        }
      }
    });
  }

  @override
  void dispose() {
    _timer?.cancel(); // Cancel the timer to avoid memory leaks
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    // if (BleTabState.connectedDevice != null && alreadyRead == 0) {
    //   readEffect();
    //   option = String.fromCharCodes(BleTabState.read_data);
    //   log += "Read raw: ${BleTabState.read_data} \n";
    //   log += "Read list: $option\n";

    //   option_list = option.split('\n');
    //   dropdownListVal = option_list.first;

    //   if (option_list.length > 1) {
    //     alreadyRead = 1;
    //   //   _timer?.cancel();
    //   }
    // }

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
                      _changeEffect(dropdownListVal);
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
