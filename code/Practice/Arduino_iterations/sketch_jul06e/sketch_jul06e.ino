#define BUTTON_PIN 21
#define LED_PIN 17
#define BOUNCING_DELAY 175

int help_state = LOW;

void button_function();

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  button_function();
}

void button_function() {
  if(digitalRead(BUTTON_PIN)) {
    delay(BOUNCING_DELAY);
    if(digitalRead(BUTTON_PIN)) {
      help_state = !help_state;
    }
  }
  
  digitalWrite(LED_PIN, help_state); 
}
