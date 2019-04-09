int leds[4] = { 20, 18, 16, 14 };

int buttons[4] = {36, 35, 34, 33};
boolean lastButtonState[4] = {LOW, LOW, LOW, LOW};
boolean buttonState[4] = {LOW, LOW, LOW, LOW};
boolean on[4] = { false, false, false, false };

int notes[4] = { 60, 64, 67, 72};

int delayRange[2] = {40, 600};

int tempoPot = A18;
int tempo = 0;

int flipToggle = 38;

int counter = 0;
unsigned long lastStepTime = 0;

void setup() {
  for (int i = 0; i < 4; i++) { //set up 4 buttons as inputs
    pinMode(buttons[i], INPUT);
  }

  for (int i = 0; i < 4; i++) { //set up 4 leds as inputs
    pinMode(leds[i], OUTPUT);
  }

  pinMode(flipToggle, INPUT); //set up toggle as input

}

void loop() {
  checkButtons();
  sequence();
}

void sequence() {

  tempo = map(analogRead(tempoPot), 0, 1023, delayRange[LOW], delayRange[HIGH]);
  // map tempoPot to usable tempo value

  if (millis() > lastStepTime + tempo) { // if tempo interval has passed...
    digitalWrite(leds[counter], LOW); // turn last led off
    usbMIDI.sendNoteOff(notes[counter], 0, 1); //send note off for last note

    if (digitalRead(flipToggle) == HIGH) { //if toggle is on...
      counter++; // increase counter by 1
      if (counter > 3) { // reset counter if over 3
        counter = 0;
      }
    }

    else if (digitalRead(flipToggle) == LOW) { //if toggle is off
      counter--; // decrease counter by 1
      if (counter < 0) { // reset counter if under 0
        counter = 3;
      }
    }

    lastStepTime = millis(); //set lastStepTime to current time
    
    if (on[counter] == true) { // if current step is on...
      usbMIDI.sendNoteOn(notes[counter], 127, 1); // send note on
      digitalWrite(leds[counter], HIGH);
    }
  }
}

void checkButtons() {
  
  lastButtonState[counter] = buttonState[counter];
  buttonState[counter] = digitalRead(buttons[counter]); 

  if (buttonState[counter] == HIGH && lastButtonState[counter] == LOW) {
  // check for the moment when the button changes state. at this moment...
    on[counter] = !on[counter];
    // switch state of current step
  }
}
