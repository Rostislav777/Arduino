/*
	======Escape Quest======
	===CRYSTAL ACTIVATION===
	
	DESCRIPTIONS:
	
	При попадании красного лазера на датчик необходимо замкнуть два контакта на 5 сек.
 */


#define Relay 2
#define light_sensor A0
#define potentiometr A1

int trigger = 0;
int operating_time=5000; 	//время замыкания реле
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int potentiometr_value = 0;
int potentiometr_output = 0;

void setup() {
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(light_sensor);
  potentiometr_value = analogRead(potentiometr);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  potentiometr_output = map(potentiometr_value, 0, 1023, 500, 1023);


  // print the results to the serial monitor:
  Serial.print("sensor = ");
  Serial.print(potentiometr_value);
  Serial.print("\t output = ");
  Serial.println(potentiometr_output);

  if(sensorValue < potentiometr_output)
  {
	digitalWrite(Relay, LOW);
  }
  else if(sensorValue > potentiometr_output && trigger == 0)
  {
	digitalWrite(Relay, HIGH);
	delay(operating_time);
	digitalWrite(Relay, LOW);
	trigger=1;
  }
  
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);
}