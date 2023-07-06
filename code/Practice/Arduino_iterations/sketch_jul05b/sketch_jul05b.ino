
#define buttonPin 14
#define ledPin 2

bool buttonState = false;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);

} 

void loop() {
  if(digitalRead(buttonPin) != buttonState) {
    buttonState = !buttonState;
  }

  digitalWrite(buttonPin, buttonState);

  delay(250);
}
