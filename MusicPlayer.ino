/**
  Adruino Music Player
  Copyright 2013 Max Bright
*/

const int buzzerPin = 9;
const int sensorPin = 0;

const int led1Pin = 4;
const int led2Pin = 7;

const int buttonPin = 2;

int songLength = 99;

char playerState = 'I';

int songFreq[100];

bool led1State = false;
bool led2State = false;

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
}

void loop()
{
  int sensorValue;
  int frequency;
  int songFreqLastCount;
  
  bool buttonHeld;
  
  if (playerState == 'I')
  {
    if (digitalRead(buttonPin) == LOW) //If button pressed, playerState = recording
    {
      playerState = 'R';
      buttonHeld = true;
    }
  }
  
  
  if (playerState == 'R') //if in recording mode
  {
    if (!led1State) digitalWrite(led1Pin, HIGH);
    for (int count = 0; count <= songLength; count++) //For 5 seconds (100 loop iterations), record each frequency into an array
    {
      if (count % 20 == 0)
        digitalWrite(led1Pin, LOW); 
      else
        digitalWrite(led1Pin, HIGH);
      
      sensorValue = analogRead(sensorPin); //get our sensor value
      if (sensorValue == 0) //If no input to the sensor
      {
        songFreq[count] = 0;
      }
      else
      {
        songFreq[count] = 200 + (sensorValue * .8);//get our sensor value onto the desired frequency range
        tone(buzzerPin, songFreq[count], 60); //play the preview note
      }
     
      
      if (digitalRead(buttonPin) == LOW && !buttonHeld) //abort if button is pressed
      {
        playerState = 'I';
        count = songLength;
      }
      else if (digitalRead(buttonPin) == HIGH)
      {
        buttonHeld = false;
      }
      
      delay(50);
    }
    digitalWrite(led1Pin, LOW);
    led1State = false;
    playerState = 'P';
  }
  
  
  if (playerState == 'P') //if in playback mode
  {
    if (!led2State) digitalWrite(led2Pin, HIGH);
    led2State = true;
    for (int count = 0; digitalRead(buttonPin) == HIGH; count++) //For loop plays each note back, and stop condition happens if button is pressed
    {
      if (count > songLength) count = 0; //Reset if we completed the song
      tone(buzzerPin, songFreq[count], 60);
      delay(50);
    }
    playerState = 'I';
    digitalWrite(led1Pin, HIGH);
    delay(333);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led1Pin, LOW);
    delay(333);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led1Pin, HIGH);
    delay(334);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led1Pin, LOW);
    led2State = false;
  }
}
