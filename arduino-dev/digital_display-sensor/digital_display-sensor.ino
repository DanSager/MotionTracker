#include <TimerOne.h>
//the pins of 4-digit 7-segment display attach to pin2-13 respectively 
int a = 36;
int b = 37;//         a
int c = 38;//        ---
int d = 39;//      f| g |b
int e = 40;//        ---
int f = 41;//      e|   |c
int g = 42;//        ---
int p = 43;//         d
int d4 = 35;// one's 7 segment display
int d3 = 34;// ten's 7 segment display
int d2 = 33;// hundred's 7 segment display
int d1 = 32;// thousand's 7 segment display

long n = 0;// n represents the value displayed on the LED display. For example, when n=0, 0000 is displayed. The maximum value is 9999. 
int x = 100;
int del = 5;//Set del as 5; the value is the degree of fine tuning for the clock
int limit = 10000;
//int count = 0;//Set count=0. Here count is a count value that increases by 1 every 0.1 second, which means 1 second is counted when the value is 10

//motion sensor
int sensor = 25; //pin of motion sensor on arduino
int detect = LOW; //current state of motion sensor
int prev = LOW; //previous state of motion sensor

//reset button
int reset = 14; //pin of the button on the breadboard
int buttonState = 0; //wheither or not the button is pressed
int wipeDelay = 0; //breaks up the wipe so it doesn't go as quickly

//bluetooth
String incoming_string = "";
char cha;
int index = 0;
boolean flag = false;
char data[10];
int rotate = 0;

void setup()
{
  //set all the pins of the LED display as output
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(p, OUTPUT);

  pinMode(sensor, INPUT);

  pinMode(reset, INPUT_PULLUP);
  Serial.begin(9600);

  wipe(2);

  //Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  //Timer1.attachInterrupt( add ); // attach the service routine here
}
/***************************************/ 
void loop()
{
  scan();
  bluetooth();
  if (n > limit)
    //Serial.println("ERROR: Limit exceeded!");
  
  clearLEDs();//clear the 7-segment display screen
  pickDigit(0);//Light up 7-segment display d1
  pickNumber((n/1000));// get the value of thousand
  delay(del);//delay 5ms

  clearLEDs();//clear the 7-segment display screen
  pickDigit(1);//Light up 7-segment display d2
  pickNumber((n%1000)/100);// get the value of hundred
  delay(del);//delay 5ms

  clearLEDs();//clear the 7-segment display screen
  pickDigit(2);//Light up 7-segment display d3
  pickNumber(n%100/10);//get the value of ten
  delay(del);//delay 5ms

  clearLEDs();//clear the 7-segment display screen
  pickDigit(3);//Light up 7-segment display d4
  pickNumber(n%10);//Get the value of single digit
  delay(del);//delay 5ms
}
/*******************************************/
void scan()
{ 
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(reset);
  if (buttonState == LOW){
    revwipe(5);
    n=0;
  }

  if (n >= limit)
     n = 0;
  
  detect = digitalRead(sensor);
  if (detect == HIGH) {
    if (prev == LOW) {
      prev = HIGH;
      n = n+1;
      //Serial.println("Motion detected!");
      //Serial.print(n);
      //Serial.println(" -> Number of times motion was detected");
    }    
  }
  else {
    if (prev == HIGH) {
      //Serial.println("Motion stopped!");
      prev = LOW;
    }
  }
}
/**************************************/ 
void bluetooth()
{
  //Send to bluetooth count
  String sendStr = "count:" + String(n);
  Serial.println(sendStr);
  
  if(Serial.available() > 0){ 
    while(Serial.available() > 0){ 
      cha = Serial.read();
      //delay(10); //Delay required 
      data[index] = cha;
      index++; 
    } 
    data[index] = '\0'; 
    flag = true;   
  }  
  if(flag){
    handle_bt();
    flag = false; 
    index = 0; 
    data[0] = '\0';
  } 
}
/**************************************/ 
void handle_bt()
{
  char command = data[0];
  switch(command){
    case 'i':
      //Serial.println("Command: increment");
      n++;
      break;
    case 'd':
      //Serial.println("Command: decrement");
      n--;
      break;
    case 'r':
      //Serial.println("Command: reset");
      revwipe(5);
      n = 0;
      break;
  }
}
/**************************************/ 
void wipe(int wipeSpeed)
{
  while(n != 9999) {
    if (wipeDelay == wipeSpeed) {
      if (n%10 != 9) {
        n = n+1;
      } else if ((n%100)/10 != 9) {
        n = n+10;
      } else if ((n%1000)/100 != 9) {
        n = n+100;
      } else if (n/1000 != 9) {
        n = n+1000;
      }
      wipeDelay = 0;
    }    

    clearLEDs();//clear the 7-segment display screen
    pickDigit(0);//Light up 7-segment display d1
    pickNumber((n/1000));// get the value of thousand
    delay(del);//delay 5ms

    clearLEDs();//clear the 7-segment display screen
    pickDigit(1);//Light up 7-segment display d2
    pickNumber((n%1000)/100);// get the value of hundred
    delay(del);//delay 5ms

    clearLEDs();//clear the 7-segment display screen
    pickDigit(2);//Light up 7-segment display d3
    pickNumber(n%100/10);//get the value of ten
    delay(del);//delay 5ms

    clearLEDs();//clear the 7-segment display screen
    pickDigit(3);//Light up 7-segment display d4
    pickNumber(n%10);//Get the value of single digit
    delay(del);//delay 5ms

    wipeDelay = wipeDelay+1;
  }
  revwipe(wipeSpeed);
}
/**************************************/ 
void revwipe(int wipeSpeed)
{
  while(n != 0000) {
    if (wipeDelay == wipeSpeed) {
      if (n/1000 != 0) {
        n = n-1000;
      } else if ((n%1000)/100 != 0) {
        n = n-100;
      } else if ((n%100)/10 != 0) {
        n = n-10;
      } else if (n%10 != 0) {
        n = n-1;
      } 
      wipeDelay = 0;
    }    

    clearLEDs();//clear the 7-segment display screen
    pickDigit(0);//Light up 7-segment display d1
    pickNumber((n/1000));// get the value of thousand
    delay(del);//delay 5ms

    clearLEDs();//clear the 7-segment display screen
    pickDigit(1);//Light up 7-segment display d2
    pickNumber((n%1000)/100);// get the value of hundred
    delay(del);//delay 5ms

    clearLEDs();//clear the 7-segment display screen
    pickDigit(2);//Light up 7-segment display d3
    pickNumber(n%100/10);//get the value of ten
    delay(del);//delay 5ms

    clearLEDs();//clear the 7-segment display screen
    pickDigit(3);//Light up 7-segment display d4
    pickNumber(n%10);//Get the value of single digit
    delay(del);//delay 5ms

    wipeDelay = wipeDelay+1;
  }
  clearLEDs();
  delay(del*100);
}
/**************************************/ 
void pickDigit(int x) //light up a 7-segment display
{
  //The 7-segment LED display is a common-cathode one. So also use digitalWrite to  set d1 as high and the LED will go out
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);

  switch(x)
  {
    case 0: 
    digitalWrite(d1, LOW);//Light d1 up 
    break;
    case 1: 
    digitalWrite(d2, LOW); //Light d2 up 
    break;
    case 2: 
    digitalWrite(d3, LOW); //Light d3 up 
    break;
    default: 
    digitalWrite(d4, LOW); //Light d4 up 
    break;
  }
}
//The function is to control the 7-segment LED display to display numbers. Here x is the number to be displayed. It is an integer from 0 to 9 
void pickNumber(int x)
{
  switch(x)
  {
    default: 
    zero(); 
    break;
    case 1: 
    one(); 
    break;
    case 2: 
    two(); 
    break;
    case 3: 
    three(); 
    break;
    case 4: 
    four(); 
    break;
    case 5: 
    five(); 
    break;
    case 6: 
    six(); 
    break;
    case 7: 
    seven(); 
    break;
    case 8: 
    eight(); 
    break;
    case 9: 
    nine(); 
    break;
  }
} 
void clearLEDs() //clear the 7-segment display screen
{
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, LOW);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
  digitalWrite(p, LOW);
}

void zero() //the 7-segment led display 0
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, LOW);
}

void one() //the 7-segment led display 1
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void two() //the 7-segment led display 2
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, LOW);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}
void three() //the 7-segment led display 3
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
}

void four() //the 7-segment led display 4
{
  digitalWrite(a, LOW);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void five() //the 7-segment led display 5
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void six() //the 7-segment led display 6
{
  digitalWrite(a, HIGH);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void seven() //the 7-segment led display 7
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, LOW);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, LOW);
}

void eight() //the 7-segment led display 8
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, HIGH);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}

void nine() //the 7-segment led display 9
{
  digitalWrite(a, HIGH);
  digitalWrite(b, HIGH);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, HIGH);
  digitalWrite(g, HIGH);
}
