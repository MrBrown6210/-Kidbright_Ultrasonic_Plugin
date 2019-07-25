var ultrasonic_color_hue = 190;

Blockly.Blocks["ultrasonic.data"] = {
    init: function() {
        this.appendDummyInput()
            .appendField("distance");

        this.setOutput(true, 'Number');
        this.setInputsInline(false);
        this.setColour(ultrasonic_color_hue);
        this.setTooltip("distance from ultrasonic");
        this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
    }
};

Blockly.Blocks["ultrasonic.recieved"] = {
    init: function() {
        this.appendDummyInput()
            .appendField("recieved ultrasonic");

        this.appendStatementInput("DO")
            .appendField("do");

        this.setInputsInline(false);
        this.setPreviousStatement(true);
        this.setNextStatement(true);
        this.setColour(ultrasonic_color_hue);
        this.setTooltip("18 = trig, 19 = echo");
        this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
    }
};

Blockly.Blocks["ultrasonic.didEnter"] = {
    init: function() {
        this.appendDummyInput()
            .appendField("did enter ultrasonic");

        this.appendStatementInput("DO")
            .appendField("do");

        this.setInputsInline(false);
        this.setPreviousStatement(true);
        this.setNextStatement(true);
        this.setColour(ultrasonic_color_hue);
        this.setTooltip("18 = trig, 19 = echo");
        this.setHelpUrl(Blockly.Msg.BLINK_START_HELPURL);
    }
};


