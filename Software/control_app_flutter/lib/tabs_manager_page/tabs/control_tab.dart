import 'package:flutter/material.dart';
import 'dart:async';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';
import 'ble_tab.dart';

const rgbUUID = "0193fe95-239a-7f23-826d-2b53e38f42b6";
const effectUUID = "0193fe95-239a-7f23-826d-2b53e38f42b7";
const fanUUID = "0193fe95-239a-7f23-826d-2b53e38f42b9";
const batteryUUID = "2a19";

class CtrlTab extends StatefulWidget {
  @override
  CtrlTabState createState() => CtrlTabState();
}

class CtrlTabState extends State<CtrlTab> {
  int alreadyRead = 0;

  String option = "";
  List<String> optionList = [];
  String currentEffect = "";
  String dropdownListVal = "";

  Color currentColor = Colors.blue;
  Timer? timer;

  // Store the last time the color was updated
  DateTime? lastUpdatedTime;
  static const Duration debounceDuration =
      Duration(milliseconds: 100); // 100ms debounce

  void changeColor(Color color) async {
    final now = DateTime.now();

    // Limit updates to 100ms
    if (lastUpdatedTime == null ||
        now.difference(lastUpdatedTime!) > debounceDuration) {
      lastUpdatedTime = now; // Update the last processed time

      // Check if the color is the same to avoid unnecessary updates
      if (currentColor == color) {
        return;
      }

      setState(() {
        currentColor = color;
      });

      // Convert the color to a list of integers (e.g., RGB values)
      List<int> colorData = [color.red, color.green, color.blue];

      // Send the color data to the BLE device
      try {
        int ret = await BleTabState().sendDataToCharacteristic(rgbUUID, colorData);
        if (ret == 0) {
          print("Sending data ok");
        } else {
          print("Sending failed, code $ret");
        }
      } catch (e) {
        print("Error sending data to BLE: $e");
      }
    }
  }

  void changeEffect(String effect) async {
    // Check if the effect is the same to avoid unnecessary updates
    if (currentEffect == effect) {
      return;
    }

    setState(() {
      currentEffect = effect;
    });

    try {
      List<int> effectToSend = effect.codeUnits;
      int ret = await BleTabState().sendDataToCharacteristic(effectUUID, effectToSend);
      if (ret == 0) {
        print("Sending data ok");
      } else {
        print("Sending failed, code $ret");
      }
    } catch (e) {
      print("Error sending data to BLE: $e");
    }
  }

  void readEffect() async {
    await BleTabState().readDatafromCharacteristic(effectUUID);
  }

  @override
  void initState() {
    super.initState();

    // Polling periodically until effects have been read
    timer = Timer.periodic(Duration(milliseconds: 200), (timer) {
      if (BleTabState.connectedDevice != null && alreadyRead == 0) {
        readEffect();
        option = String.fromCharCodes(BleTabState.readData);

        optionList = option.split('\n');
        dropdownListVal = optionList.first;

        if (optionList.length > 1) {
          setState(() {
            alreadyRead = 1;
            timer.cancel();
          });
        }
      }
    });
  }

  @override
  void dispose() {
    timer?.cancel(); // Cancel the timer to avoid memory leaks
    super.dispose();
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
            ColorPicker(
              pickerColor: currentColor,
              onColorChanged: changeColor,
              enableAlpha: false,
              paletteType: PaletteType.hueWheel,
              pickerAreaHeightPercent:
                  0.8, // Adjust this value to adjust color wheel size
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
                      changeEffect(dropdownListVal);
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
          ],
        ),
      ),
    );
  }
}
