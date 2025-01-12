/// A Flutter widget that represents a tab for Bluetooth Low Energy (BLE) scanning and device management.
///
/// This widget allows users to scan for BLE devices, connect to a selected device, read characteristics,
/// and disconnect from the connected device. It uses the `flutter_blue_plus` package for BLE functionality.
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';

/// The main widget for the BLE tab.
class BleTab extends StatefulWidget {
  @override
  BleTabState createState() => BleTabState();
}

// The state class for the [BleTab] widget.
class BleTabState extends State<BleTab> {
  // A map to store the scan results with the device identifier as the key.
  Map<DeviceIdentifier, ScanResult> scanResults = {};

  // A list to store all discovered characteristics.
  static List<BluetoothCharacteristic> allCharacteristics = [];

  // The currently connected Bluetooth device.
  static BluetoothDevice? connectedDevice;

  // Read data
  static List<int> readData = [];

  @override
  void initState() {
    super.initState();
    requestBluetoothPermission();
  }

  /// Requests Bluetooth permission and checks if Bluetooth is supported on the device.
  Future<void> requestBluetoothPermission() async {
    if (!await FlutterBluePlus.isSupported) {
      print("Bluetooth not supported by this device");
      return;
    }
    print("Bluetooth is supported by this device");

    var subscription = FlutterBluePlus.adapterState.listen((state) {
      print(state);
      if (state == BluetoothAdapterState.on) {
        print("Bluetooth is on");
      } else {
        print("Bluetooth is off");
      }
    });

    subscription.cancel();
  }

  /// Starts scanning for BLE devices.
  Future<void> startScan() async {
    await FlutterBluePlus.startScan(timeout: Duration(seconds: 5));
    FlutterBluePlus.scanResults.listen((results) {
      setState(() {
        for (var result in results) {
          scanResults[result.device.remoteId] = result;
        }
      });
    });
  }

  /// Sends data to a given characteristic.
  ///
  /// [characteristic] The Bluetooth characteristic to send data to.
  /// [data] The data to send.
  Future<int> sendDataToCharacteristic(String characteristicUuid, List<int> dataToWrite) async {
    if (connectedDevice != null) {
      try {
        for (BluetoothCharacteristic characteristic in allCharacteristics) {
          if (characteristic.uuid.toString() == characteristicUuid) {
            await characteristic.write(dataToWrite);
            print("Data sent to characteristic: $dataToWrite");
            return 0;
          }
        }
        print("Characteristic with UUID $characteristicUuid not found");
        return -1;
      } catch (e) {
        print("Error sending data to characteristic: $e");
        return -1;
      }
    } else {
      print("No device connected. Cannot send data");
      return -1;
    }
  }

  /// Read data from a given characteristic.
  ///
  /// [characteristic] The Bluetooth characteristic to read data from.
  Future<int> readDatafromCharacteristic(String characteristicUuid) async {
    if (connectedDevice != null) {
      try {
        for (BluetoothCharacteristic characteristic in allCharacteristics) {
          if (characteristic.uuid.toString() == characteristicUuid) {
            if (characteristic.properties.read) {
              readData = await characteristic.read();
              print("Data read from characteristic: $readData");
              return 0;
            }
          }
        }
        print("Characteristic with UUID $characteristicUuid not found");
        return -1;
      } catch (e) {
        print("Error reading data from characteristic: $e");
        return -1;
      }
    } else {
      print("No device connected. Cannot read data");
      return -1;
    }
  }

  /// Discovers characteristics of a connected Bluetooth device.
  ///
  /// [device] The Bluetooth device to discover services for.
  Future<void> discoverServices(BluetoothDevice device) async {
    List<BluetoothService> services = await device.discoverServices();
    for (BluetoothService service in services) {
      for (BluetoothCharacteristic characteristic in service.characteristics) {
        characteristic.read();
        allCharacteristics.add(characteristic);
      }
    }
  }

  /// Connects to a given Bluetooth device.
  ///
  /// [device] The Bluetooth device to connect to.
  Future<void> connectToDevice(BluetoothDevice device) async {
    try {
      await device.connect();
      setState(() {
        connectedDevice = device;
      });
      discoverServices(device);
    } catch (e) {
      print("Error connecting to device: $e");
    }
  }

  /// Disconnects from the currently connected Bluetooth device.
  Future<void> disconnectDevice() async {
    if (connectedDevice != null) {
      await connectedDevice!.disconnect();
      setState(() {
        connectedDevice = null;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('BLE Scanner'),
        actions: [
          Padding(
            padding: const EdgeInsets.only(right: 25.0),
            child: ElevatedButton(
              onPressed: () async {
                if (!(await FlutterBluePlus.isScanning.first)) {
                  await startScan();
                }
              },
              child: StreamBuilder<bool>(
                stream: FlutterBluePlus.isScanning,
                initialData: false,
                builder: (context, snapshot) {
                  return Text(snapshot.data! ? 'Scanning...' : 'Scan');
                },
              ),
            ),
          ),
        ],
      ),
      body: Column(
        children: [
          if (connectedDevice != null)
            ListTile(
              title: Text('Connected Device'),
              subtitle: Text(connectedDevice!.remoteId.toString()),
              trailing: ElevatedButton(
                onPressed: disconnectDevice,
                child: Text('DISCONNECT'),
              ),
            ),
          Expanded(
            child: ListView.builder(
              itemCount: scanResults.length,
              itemBuilder: (context, index) {
                var sortedResults = scanResults.values.toList()..sort((a, b) => b.rssi.compareTo(a.rssi));
                ScanResult result = sortedResults[index];
                BluetoothDevice device = result.device;
                return ListTile(
                  leading: CircleAvatar(child: Text(result.rssi.toString()),),
                  title: Text(device.advName.isNotEmpty? device.advName: 'Unknown Device'),
                  subtitle: Text(device.remoteId.toString()),
                  trailing: connectedDevice?.remoteId == device.remoteId
                      ? ElevatedButton(
                          onPressed: disconnectDevice,
                          child: Text('DISCONNECT'),
                        )
                      : ElevatedButton(
                          onPressed: () => connectToDevice(device),
                          child: Text('CONNECT'),
                        ),
                );
              },
            ),
          ),
        ],
      ),
    );
  }

  @override
  void dispose() {
    FlutterBluePlus.stopScan();
    super.dispose();
  }
}
