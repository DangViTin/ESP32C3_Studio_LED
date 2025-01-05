import 'package:flutter/material.dart';
import 'package:control_app_flutter/tabs_manager_page/tabs_manager_page.dart';

class LoginPage extends StatelessWidget {
  const LoginPage({Key? key}) : super(key: key);
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        // resizeToAvoidBottomInset: false,
        // appBar is the top bar of the app
        // appBar: AppBar(
        //   title: Text('Flutter Demo', style: TextStyle(color: Colors.white)),
        //   backgroundColor: const Color.fromARGB(255, 9, 20, 172),
        // ),

        // drawer is the side menu of the app
        // drawer: Container(
        //   color: const Color.fromARGB(255, 148, 46, 46),
        //   width: 200,
        //   child: Column(
        //     children: [
        //       Text('data'),
        //       Text('data'),
        //       Text('data'),
        //     ],
        //   ),
        // ),

        // body is the main content of the app
        body: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            // Text('Facebook',
            //     style: TextStyle(color: Colors.black, fontSize: 50)),
            // SizedBox(height: 40),
            Image.asset("assets/images/facebook.png"),
            SizedBox(height: 60),
            SizedBox(
              width: MediaQuery.of(context).size.width *
                  0.9, // 90% of screen width
              child: TextField(
                decoration: InputDecoration(
                  hintText: ' User name',
                  border: OutlineInputBorder(
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  filled: true,
                  fillColor: Colors.white,
                  contentPadding:
                      EdgeInsets.symmetric(vertical: 15.0, horizontal: 10.0),
                ),
              ),
            ),
            SizedBox(height: 20),
            SizedBox(
              width: MediaQuery.of(context).size.width *
                  0.9, // 90% of screen width
              child: TextField(
                obscureText: true,
                decoration: InputDecoration(
                  hintText: ' Password',
                  border: OutlineInputBorder(
                    borderRadius: BorderRadius.circular(8.0),
                  ),
                  filled: true,
                  fillColor: Colors.white,
                  contentPadding:
                      EdgeInsets.symmetric(vertical: 15.0, horizontal: 10.0),
                ),
              ),
            ),
            Align(
              alignment: Alignment.centerRight,
              child: TextButton(
                onPressed: () {},
                child: Text('Fogot password?'),
              ),
            ),
            SizedBox(
              width: MediaQuery.of(context).size.width *
                  0.9, // 90% of screen width
              child: ElevatedButton(
                style: ElevatedButton.styleFrom(
                  backgroundColor:
                      Color.fromARGB(255, 0x19, 0x76, 0xd2), // Facebook blue
                  shape: RoundedRectangleBorder(
                    borderRadius:
                        BorderRadius.circular(4), // Tiny corner radius
                  ),
                ),
                onPressed: () {
                  Navigator.of(context).pushAndRemoveUntil(
                  MaterialPageRoute(builder: (context) => TabsManager()),
                  (Route<dynamic> route) => false,
                  );
                },
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Text('Login', style: TextStyle(color: Colors.white)),
                  ],
                ),
              ),
            ),
            SizedBox(
              width: MediaQuery.of(context).size.width *
                  0.9, // 90% of screen width
              child: ElevatedButton(
                onPressed: () {},
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Image.asset("assets/images/facebook.png",
                        width: 30, height: 30),
                    Text('  Login with Facebook'),
                  ],
                ),
              ),
            ),
            SizedBox(
              width: MediaQuery.of(context).size.width *
                  0.9, // 90% of screen width
              child: ElevatedButton(
                onPressed: () {},
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: [
                    Image.asset("assets/images/youtube.png",
                        width: 30, height: 30),
                    Text('  Login with Youtube'),
                  ],
                ),
              ),
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text('Don\'t have an account?'),
                TextButton(
                  onPressed: () {},
                  child: Text('Sign up'),
                ),
              ],
            ),
          ],
        ),

        // backgroundColor is the background color of the app, excluding the appBar
        // backgroundColor: const Color.fromARGB(255, 145, 143, 150),
      ),
    );
  }
}
