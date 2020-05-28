const int ON = 144;
const int OFF = 128;

//enter number of buttons
const int numButtons = 5;
//enter pin numbers in order
const int buttons[numButtons] {12, 13};
//enter base note; 60 is C3, 61 is C#3 and so on
const int BaseNote = 60;
//enter velocity
const int velocity = 100;
//enter the scale: 0 = Chromatic, 1 = Major, 2 = Minor
const int Type = 1;

const int ShiftButton = 8;
const int BaseButtons[2] = {12, 13};
const int OctaveButtons[2] = {10, 11};
const int VelocityButtons[2] = {8, 9};
const int TypeButtons[3] = {5, 6, 7};

bool IsPressed[numButtons]{};
bool IsOffed[numButtons]{};
int State[numButtons] {};
int Notes[100]{};

const int latchPin = 9;
const int clockPin = 8;
const int dataPin = 10;

byte SwitchVar = 72;


void setup() {
  Serial.begin(9600);
  for(int i = 0; i < numButtons; i++){
    pinMode(buttons[i], INPUT);
    IsPressed[i] = false;
    IsOffed[i] = false;
  }
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
}

void loop() {

  digitalWrite(latchPin,1);
  delayMicroseconds(20);
  digitalWrite(latchPin,0);

  SwitchVar = ShiftIn(dataPin, clockPin);

  //Serial.println(SwitchVar, BIN);

  TestType();

  int Shift = digitalRead(ShiftButton);
  
    for(int i = 0; i < numButtons; i++){
      State[i] = TestVal(i);
  
      if(State[i] == HIGH && IsPressed[i] == false){
        SendMidi(ON, Notes[i], 100);
        IsPressed[i] = true;
        IsOffed[i] = false;
        //Serial.println("60");
      }
      else if(State[i] == LOW && IsOffed[i] == false){
        SendMidi(OFF, Notes[i], 100);
        IsOffed[i] = true;
        IsPressed[i] = false;
        //Serial.println("60 OFF");
      }
    }
}

void SendMidi(int command, int note, int velocity){
  Serial.write(command);
  Serial.write(note);
  Serial.write(velocity);
}

byte ShiftIn(int Data, int CLOCK){
  int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(Data, INPUT);
  pinMode(CLOCK, OUTPUT);

  for(int i = 2; i > 0; i--){
    digitalWrite(CLOCK, 0);
    delayMicroseconds(20);
    temp = digitalRead(Data);
    if(temp){
      pinState = 1;
      myDataIn = myDataIn | (1 << i);
    }
    else{
      pinState = 0;
    }
    digitalWrite(CLOCK, 1);
  }

  return myDataIn;
}

int TestVal(int num){
    if (SwitchVar & (1 << num) ){
      return HIGH;
    }
    else{
      return LOW;
    }
}





void TestType(){
  int next_note[7] = {2,2,1,2,2,2,1};
  int next_note1[7] = {2,1,2,2,1,2,2};
  
  switch(Type){
      case 0:
      for(int j = 0; j < 100; j++){
         Notes[j] = BaseNote + j;
      }
      break;
      
      case 1:
      for(int j=0; j < 100; j++) {
        if (j == 0) {
          Notes[j] = BaseNote;
        }
        else if (j%7 == 0){
          Notes[j] = Notes[j-1] + next_note[6];
        }
        else {
          Notes[j] = Notes[j-1] + next_note[(j%7)-1];
        }
      }
      break;
      
      case 2:
      for(int j=0; j < 100; j++) {
        if (j == 0) {
          Notes[j] = BaseNote;
        }
        else if (j%7 == 0){
          Notes[j] = Notes[j-1] + next_note1[6];
        }
        else {
          Notes[j] = Notes[j-1] + next_note1[(j%7)-1];
        }
      }
      break;
    }
}
