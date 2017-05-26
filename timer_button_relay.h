/*
	======Escape Quest======
	===TIMER BUTTON RELAY===
	
	DESCRIPTIONS:
	
	При замыкании контактов в течение 3 сек разрывается питание на 10 сек .
*/

#define Relay 2
#define btn1 3
int count=0;
int operating_time=10000;	//время замыкания реле
int hold_button_time=3000;	//время удержания кнопки
int trigger = 0;

void setup()
{
  pinMode(btn1, INPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
}

void loop()
{
  if(digitalRead(btn1) == HIGH && trigger == 0)
  {
	count++;
	delay(1);
  }
  if(count == hold_button_time)
  {
	digitalWrite(Relay, HIGH);
    delay(operating_time);
	digitalWrite(Relay, LOW);
	count=0;
	trigger=1;
  }
}