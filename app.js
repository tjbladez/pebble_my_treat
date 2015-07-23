var UI = require('ui');
var Vibe = require('ui/vibe');
var Settings = require('settings');
var Vector2 = require('vector2');

var main = new UI.Card({
  title: "My Treat",
  subtitle: "",
  body: "Press anything to see whose turn it is to pay."
});

main.show();
main.changePayee = function(currentWindow){
  if (Settings.option("yourTurn")) {
    Settings.option("yourTurn", false);
  } else {
    Settings.option("yourTurn", true);
  }
  var tempWindow = new UI.Window({
    fullscreen: true,
  });
  var textfield = new UI.Text({
    position: new Vector2(0, 65),
    size: new Vector2(144, 30),
    font: 'gothic-18-bold',
    text: 'Payment order updated!',
    textAlign: 'center'
  });
  tempWindow.add(textfield);
  tempWindow.show();
  window.setTimeout(function(){
    tempWindow.remove(textfield);
    tempWindow.hide();
    currentWindow.hide();
  }, 5000);
  Vibe.vibrate('short');
};

main.showPayee = function() {
  var text;
  var card = new UI.Card({title: 'Your turn?'});
  var isYourTurn = Settings.option('yourTurn');
  text = isYourTurn ? "Yes, pay up!" : "Nah, you are good!";
  card.subtitle(text);
  card.body("Press 'select' to update it, 'up' or 'down' to close.")
  card.on('click', 'up', function(e){
    card.hide();
  });
  card.on('click', 'down', function(e){
    card.hide();
  });
  card.on('click', 'select', function(e){
    main.changePayee(card);
  });
  card.show();
};

main.on('click', 'up', function(e) {
  main.showPayee();
});

main.on('click', 'select', function(e) {
  main.showPayee();
});

main.on('click', 'down', function(e) {
  main.showPayee();
});
