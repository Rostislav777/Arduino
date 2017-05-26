#define Relay A5

#define led1 12
#define led2 2
#define led3 3
#define led4 4
#define led5 5
#define led6 6

#define lmp1 7
#define lmp2 8
#define lmp3 9
#define lmp4 10

#define start 11

#define btn1 A1
#define btn2 A2
#define btn3 A3
#define btn4 A4

	int time = 600;
	int modeState = 8;

	void setup()
	{
	int qwe = 1;
		Serial.begin(19200);

		pinMode(led1, OUTPUT);
		pinMode(led2, OUTPUT);
		pinMode(led3, OUTPUT);
		pinMode(led4, OUTPUT);
		pinMode(led5, OUTPUT);
		pinMode(led6, OUTPUT);
		
		pinMode(lmp1, OUTPUT);
		pinMode(lmp2, OUTPUT);
		pinMode(lmp3, OUTPUT);
		pinMode(lmp4, OUTPUT);
		
		pinMode(start, INPUT);
		
		pinMode(btn1, INPUT);
		pinMode(btn2, INPUT);
		pinMode(btn3, INPUT);
		pinMode(btn4, INPUT);
		
		digitalWrite(Relay, LOW);
		pinMode(Relay, OUTPUT);//Relay A out
	}

	void off()
	{
	  digitalWrite(led1, LOW);
	  digitalWrite(led2, LOW);
	  digitalWrite(led3, LOW);
	  digitalWrite(led4, LOW);
	  digitalWrite(led5, LOW);
	  digitalWrite(led6, LOW);
	}
	/*void open()
	{
	  digitalWrite(Relay, HIGH);
	  delay(2000);
	  digitalWrite(Relay, LOW);
	}*/
	char getKey()
	{
	  if (digitalRead(start) == HIGH)
	  {
		   Serial.println("Key 5");
		return 5;
	  }
	  if (digitalRead(btn4) == HIGH)
	  {
		  Serial.println("Key 4");
		return 4;
	  }
	  if (digitalRead(btn3) == HIGH)
	  {
			Serial.println("Key 3");
		return 3;
	  }
	  if (digitalRead(btn2) == HIGH)
	  {
		  Serial.println("Key 2");
		return 2;
	  }
	  if (digitalRead(btn1) == HIGH)
	  {
		  Serial.println("Key 1");
		return 1;
	  }
	  return 0;
	}

	char newKey = 0;
	char getNewKey()
	{
	  Serial.print("getNewKey");
	  while (getKey() == 0)//втыкаем пока не получим код нужной кнопки
	  {

		if (modeState == 0)
		{
			if (digitalRead(led3) == LOW)
			{
			digitalWrite(led3, HIGH);
			}
			else
			{
			digitalWrite(led3, LOW);
			}
		}
		if (modeState == 1)
		{
			if (digitalRead(led5) == LOW)
			{
			digitalWrite(led5, HIGH);
			}
			else
			{
			digitalWrite(led5, LOW);
			}
		}
		if (modeState == 2)
		{
			if (digitalRead(led6) == LOW)
			{
			digitalWrite(led6, HIGH);
			}
			else
			{
			digitalWrite(led6, LOW);
			}
		}
		if (modeState == 3)
		{
			if (digitalRead(led1) == LOW)
			{
			digitalWrite(led1, HIGH);
			}
			else
			{
			digitalWrite(led1, LOW);
			}
		}
		if (modeState == 4)
		{
			if (digitalRead(led2) == LOW)
			{
			digitalWrite(led2, HIGH);
			}
			else
			{
			digitalWrite(led2, LOW);
			}
		}
		if (modeState == 5)
		{
			if (digitalRead(led4) == LOW)
			{
			digitalWrite(led4, HIGH);
			}
			else
			{
			digitalWrite(led4, LOW);
			}
		}
		Serial.print("50");
		delay(50);
	  }
	  /*while (getKey() == 0)
	  {
		Serial.print("10");
		delay(10);
	  }*/
	  newKey = getKey();

	  while (getKey() != 0)
	  {
		Serial.print("10_2");
		delay(10);
	  }
	  /*switch (newKey)
	  {
	  case 1: 
		off();
		// Serial.print("found key 1");
		break;
	  case 2: 
		off();
		//Serial.print("found key 3");
		break;
	  case 4: 
		off();
		//Serial.print("found key 4");
		break;
	  case 5: 
	   off();
	   // Serial.print("found key 5");
		break;
	  }*/
	  return newKey;
	}

	char currentKey = 0;
	char password[] = {
	  2, 4, 3, 4, 1, 3}; //Собственно пароль
	char keyLength = 6; // длинна пароля
	char idx = 0;

	void loop()
	{			
	  Serial.println("end anim");
	  currentKey = getNewKey();
	  if(idx == -1) 
	  {
		//close();
		Serial.println("idx");
		idx++;
	  }
	  if (currentKey == password[idx])
	  {
		switch (idx)
		{
		case 0: 
		  digitalWrite(led3, HIGH);
		  Serial.println("> key 1 is true");
		  modeState = 1;
		  break;
		case 1: 
		  digitalWrite(led5, HIGH);
		  Serial.println("> key 2 is true");
		  modeState = 2;
		  break;
		case 2: 
		  digitalWrite(led6, HIGH);
		  Serial.println("> key 3 is true");
		  modeState = 3;
		  break;
		case 3: 
		  digitalWrite(led1, HIGH);
		  Serial.println("> key 4 is true");
		  modeState = 4;
		  break;
		case 4: 
		  digitalWrite(led2, HIGH);
		  Serial.println("> key 5 is true");
		  modeState = 5;
		  break;
		case 5: 
		  digitalWrite(led4, HIGH);
		  Serial.println("> key 6 is true");
		  modeState = 6;
		  break;
		}
		idx++;

	  } 
	  else {
		 idx = 0;
		switch (idx)
		{
		case 0: 
		  Serial.println("key 1 is false");
		  modeState = 99;
		  off();
		  break;
		case 1: 
		  modeState = 99;
		  off();
		  break;
		case 2: 
		  Serial.println("key 3 is false");
		  modeState = 99;
		  off();
		  break;
		case 3: 
		  Serial.println("key 4 is false");
		  modeState = 99;
		  off();
		  break;
		case 4: 
		  Serial.println("key 5 is false");
		  modeState = 99;
		  off();
		  break;
		case 5: 
		  Serial.println("key 5 is false");
		  modeState = 99;
		  off();
		  break;
		}
	  }
	  if (idx == keyLength)
	  {
		idx = -1;
		//open();
		digitalWrite(Relay, HIGH);
	  delay(2000);
	  digitalWrite(Relay, LOW);
	  }
	  if (currentKey == 5)
	  {
		idx = -1;
				digitalWrite(led1, HIGH);
				digitalWrite(lmp4, HIGH);
				delay (time);
				digitalWrite(led1, LOW);
				digitalWrite(lmp4, LOW);
				delay (time);
				
				digitalWrite(led2, HIGH);
				digitalWrite(lmp1, HIGH);
				delay (time);
				digitalWrite(led2, LOW);
				digitalWrite(lmp1, LOW);
				delay (time);
				
				digitalWrite(led3, HIGH);
				digitalWrite(lmp2, HIGH);
				delay (time);
				digitalWrite(led3, LOW);
				digitalWrite(lmp2, LOW);
				delay (time);
				
				digitalWrite(led4, HIGH);
				digitalWrite(lmp3, HIGH);
				delay (time);
				digitalWrite(led4, LOW);
				digitalWrite(lmp3, LOW);
				delay (time);
				
				digitalWrite(led5, HIGH);
				digitalWrite(lmp4, HIGH);
				delay (time);
				digitalWrite(led5, LOW);
				digitalWrite(lmp4, LOW);
				delay (time);
				
				digitalWrite(led6, HIGH);
				digitalWrite(lmp3, HIGH);
				delay (time);
				digitalWrite(led6, LOW);
				digitalWrite(lmp3, LOW);
				
				modeState = 0;
	  }
	  delay(200);	
		
	}