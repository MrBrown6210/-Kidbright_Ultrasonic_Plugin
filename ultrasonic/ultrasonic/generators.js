Blockly.JavaScript['ultrasonic.random-text'] = function (block) {
  return [`DEV_IO.ultrasonic().random()`, Blockly.JavaScript.ORDER_ATOMIC]
}

Blockly.JavaScript['ultrasonic.basic_string'] = function (block) {
  return [`DEV_IO.ultrasonic().random()`, Blockly.JavaScript.ORDER_ATOMIC]
}

Blockly.JavaScript['ultrasonic.recieved'] = function(block) {
	var statement = Blockly.JavaScript.statementToCode(block, 'DO');
	return 'DEV_IO.ultrasonic().callback([](){'+statement+'});\n';
};

Blockly.JavaScript['ultrasonic.didEnter'] = function(block) {
	var statement = Blockly.JavaScript.statementToCode(block, 'DO');
	return 'DEV_IO.ultrasonic().didEnter([](){'+statement+'});\n';
};

Blockly.JavaScript['ultrasonic.data'] = function (block) {
  return [`DEV_IO.ultrasonic().distance()`];
};