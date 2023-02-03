import 'package:firebase_database/firebase_database.dart';
import 'package:firebase_database/ui/firebase_animated_list.dart';
import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:room_details_app/room.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();
  runApp(MaterialApp(
    home: MyApp(),
    debugShowCheckedModeBanner: false,
  ));
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  late final dbRef = FirebaseDatabase.instance.ref();
  late DatabaseReference databaseReference;
  setData() {
    dbRef.child("test").set({
      'id': "01",
      'name': "Arslan",
      'contact': "053923x1611",
      'country': "Turkey"
    }).asStream();
  }

  getData() {
    dbRef.once().then((snapshot) {
      print(snapshot.snapshot.value);
    });
  }

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    databaseReference = dbRef;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Room Details App"),
      ),
      body: Column(
        children: [
          TextButton(
              onPressed: setData,
              child: Text(
                "Store Data",
                style: TextStyle(color: Colors.blue),
              )),
          TextButton(
              onPressed: getData,
              child: Text(
                "Get Data",
                style: TextStyle(color: Colors.blue),
              )),
          FirebaseAnimatedList(
              shrinkWrap: true,
              query: databaseReference,
              itemBuilder: (BuildContext context, DataSnapshot snapshot,
                  Animation animation, int index) {
                return Center(
                    child: Text(snapshot.key.toString() + " : " + snapshot.value.toString(), style: TextStyle(fontSize: 20),)
                );
              })
        ],
      ),
    );
  }
}
