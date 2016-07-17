#!/usr/bin/env node

var leapjs      = require('leapjs');

var controller  = new leapjs.Controller({enableGestures: true});

controller.on('connect', function() {
  console.log("Successfully connected.");
});

controller.on('deviceFrame', function(frame) {
  // loop through available gestures
  for(var i = 0; i < frame.gestures.length; i++){
    var gesture = frame.gestures[i];
    var type    = gesture.type;

    switch( type ){

      case "circle":
        if (gesture.state == "stop") {
          console.log('circle');
        }
        break;

      case "swipe":
        if (gesture.state == "stop") {
          console.log('swipe');
        }
        break;

      case "screenTap":
        if (gesture.state == "stop") {
          console.log('screenTap');
        }
        break;

      case "keyTap":
        if (gesture.state == "stop") {
          console.log('keyTap');
        }
        break;

      }
    }
});

controller.connect();
