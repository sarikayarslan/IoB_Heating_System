import 'dart:core';
import 'dart:convert';

Room roomFromJson(String str) => Room.fromJson(json.decode(str));

class Room {
  double? temperature ;
  double? humidity;
  double? occupancy;

  Room({
    this.temperature,
    this.humidity,
    this.occupancy
});

factory Room.fromJson(Map<String,dynamic> json) => Room(
  temperature: json["Temperature"],
  humidity: json["Humadity"],
  occupancy: json["Occupancy"]
);

}