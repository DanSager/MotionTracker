int led = 13;
int sensor = 2;
int state = LOW;
int val = 0;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(sensor);
  if (val == HIGH) {
    digitalWrite(led, HIGH);
    //delay(10);

    
    Serial.println("Motion detected!");
    if (state == LOW) {
      state = HIGH;
      count = count +1;
    }
  }
  else {
    digitalWrite(led, LOW);
    //delay(20);

    if (state == HIGH) {
      Serial.println("Motion stopped!");
      Serial.print(count);
      Serial.println(" -> Number of times motion was detected");
      state = LOW;
    }
  }
}
