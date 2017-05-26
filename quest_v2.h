#define Relay A5

// led1_r 0b00000001
// led1_g 0b00000010

// led2_r 0b00000100
// led2_g 0b00001000

// led3_r 0b00010000
// led3_g 0b00100000

// led4_r 0b01000000
// led4_g 0b10000000

#define led5_r 5
#define led5_g 6
#define led6_r 12
#define led6_g 13

#define lmp1 7
#define lmp2 8
#define lmp3 9
#define lmp4 10

#define start 11

#define btn1 A3
#define btn2 A2
#define btn3 A1
#define btn4 A4

int SH_CP = 4;
int ST_CP = 3;
int DS = 2;

//int time = 600;
int modeState = 8;

void setup()
{
	int qwe = 1;
	Serial.begin(19200);
	
	pinMode(SH_CP, OUTPUT);
    pinMode(ST_CP, OUTPUT);
    pinMode(DS, OUTPUT);

	pinMode(led5_r, OUTPUT);
	pinMode(led5_g, OUTPUT);
	pinMode(led6_r, OUTPUT);
	pinMode(led6_g, OUTPUT);
		
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
	
	off();
	lmp_off();
}

void off()
{
	digitalWrite(led5_r, LOW);
	digitalWrite(led5_g, LOW);
	digitalWrite(led6_r, LOW);
	digitalWrite(led6_g, LOW);

	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00000000);
	digitalWrite(ST_CP, HIGH);
	delay(500);
}

void lmp_off()
{
	digitalWrite(lmp1, LOW);
	digitalWrite(lmp2, LOW);
	digitalWrite(lmp3, LOW);
	digitalWrite(lmp4, LOW);	
}

void led1_r()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00000001);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led1_g()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00000010);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led2_r()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00000100);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led2_g()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00001000);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led3_r()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00010000);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led3_g()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00100000);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led4_r()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b01000000);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led4_g()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b10000000);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void led_last()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b10101010);
	digitalWrite(ST_CP, HIGH);
	//delay(50);
}

void game_over()
{
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b01010101);
	digitalWrite(ST_CP, HIGH);
	digitalWrite(led5_r, HIGH);
	digitalWrite(led6_r, HIGH);
	digitalWrite(led5_g, LOW);
	digitalWrite(led6_g, LOW);
	delay(500);
	digitalWrite(ST_CP, LOW);
	shiftOut(DS, SH_CP, MSBFIRST, 0b00000000);
	digitalWrite(ST_CP, HIGH);
	digitalWrite(led5_r, LOW);
	digitalWrite(led6_r, LOW);
	delay(500);
}
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
		digitalWrite(lmp4, HIGH);
		return 4;
	}
	if (digitalRead(btn3) == HIGH)
	{
		Serial.println("Key 3");
		digitalWrite(lmp3, HIGH);
		return 3;
	}
	if (digitalRead(btn2) == HIGH)
	{
		Serial.println("Key 2");
		digitalWrite(lmp2, HIGH);
		return 2;
	}
	if (digitalRead(btn1) == HIGH)
	{
		Serial.println("Key 1");
		digitalWrite(lmp1, HIGH);
		return 1;
	}
	  return 0;
}

char newKey = 0;
char getNewKey()
{
	//Serial.print("getNewKey");
	while (getKey() == 0)//втыкаем пока не получим код нужной кнопки
	{
		lmp_off();
		if (modeState == 0)
		{
			//ожидаем 3ий лед
			digitalWrite(ST_CP, LOW);
			shiftOut(DS, SH_CP, MSBFIRST, 0b00110000);
			digitalWrite(ST_CP, HIGH);
			//delay(50);
		}
		if (modeState == 1)
		{
			//ожидаем 5ый лед
			digitalWrite(led5_r, HIGH);
			digitalWrite(led5_g, HIGH);
		}
		if (modeState == 2)
		{
			//ожидаем 6ой лед
			digitalWrite(led6_r, HIGH);
			digitalWrite(led6_g, HIGH);
		}
		if (modeState == 3)
		{
			//ожидаем 1ый лед
			digitalWrite(ST_CP, LOW);
			shiftOut(DS, SH_CP, MSBFIRST, 0b00100011);
			digitalWrite(ST_CP, HIGH);
			//delay(50);
		}
		if (modeState == 4)
		{
			//ожидаем 2ой лед
			digitalWrite(ST_CP, LOW);
			shiftOut(DS, SH_CP, MSBFIRST, 0b00101110);
			digitalWrite(ST_CP, HIGH);
			//delay(50);
		}
		if (modeState == 5)
		{
			//ожидаем 4ый лед
			digitalWrite(ST_CP, LOW);
			shiftOut(DS, SH_CP, MSBFIRST, 0b11101010);
			digitalWrite(ST_CP, HIGH);
			//delay(50);
		}
		//Serial.print("50");
		delay(50);
	}
	newKey = getKey();

	while (getKey() != 0)
	{
		Serial.print("10_2");
		delay(10);
	}
	return newKey;
}

char currentKey = 0;
char password[] = {2, 4, 3, 4, 1, 3}; //Собственно пароль
char keyLength = 6; // длинна пароля
char idx = 0;

void loop()
{			
	//Serial.println("end anim");
	currentKey = getNewKey();
	if(idx == -1) 
	{
		//Serial.println("idx");
		idx++;
	}
	if (currentKey == password[idx])
	{
		switch (idx)
		{
			case 0: 
			  //3ий верно
			  led3_g();
			  Serial.println("> key 1 is true");
			  modeState = 1;
			  break;
			case 1: 
			  //5ый верно
			  digitalWrite(led5_g, HIGH);
			  digitalWrite(led5_r, LOW);
			  Serial.println("> key 2 is true");
			  modeState = 2;
			  break;
			case 2: 
			  //6ой верно
			  digitalWrite(led6_g, HIGH);
			  digitalWrite(led6_r, LOW);
			  Serial.println("> key 3 is true");
			  modeState = 3;
			  break;
			case 3: 
			  //1ый верно
			  led1_g();
			  Serial.println("> key 4 is true");
			  modeState = 4;
			  break;
			case 4: 
			  //2ой верно
			  led2_g();
			  Serial.println("> key 5 is true");
			  modeState = 5;
			  break;
			case 5: 
			  //4ый верно
			  led_last();
			  Serial.println("> key 6 is true");
			  modeState = 6;
			  break;
		}
		idx++;
	} 
	
	else if (currentKey != 5)
	{
		idx = 0;
		switch (idx)
		{
			case 0: 
			  Serial.println("key 1 is false");
			  modeState = 99;
			  game_over();
			  game_over();
			  game_over();
			  off();
			  break;
			case 1: 
			  Serial.println("key 2 is false");
			  modeState = 99;
			  game_over();
			  game_over();
			  game_over();
			  off();
			  break;
			case 2: 
			  Serial.println("key 3 is false");
			  modeState = 99;
			  game_over();
			  game_over();
			  game_over();
			  off();
			  break;
			case 3: 
			  Serial.println("key 4 is false");
			  modeState = 99;
			  game_over();
			  game_over();
			  game_over();
			  off();
			  break;
			case 4: 
			  Serial.println("key 5 is false");
			  modeState = 99;
			  game_over();
			  game_over();
			  game_over();
			  off();
			  break;
			case 5: 
			  Serial.println("key 6 is false");
			  modeState = 99;
			  //game_over();
			  //game_over();
			  //game_over();
			  off();
			  break;
		}
	}
	if (idx == keyLength)
	{
		idx = -1;
		digitalWrite(Relay, HIGH);
		delay(2000);
		digitalWrite(Relay, LOW);
	}
	if (currentKey == 5) //демо того что надо подобрать
	{
		idx = -1;
		
		led1_g();
		digitalWrite(lmp4, HIGH);
		delay (600);
		digitalWrite(lmp4, LOW);
		off();
		delay (600);
				
		led2_g();
		digitalWrite(lmp1, HIGH);
		delay (600);
		digitalWrite(lmp1, LOW);
		off();
		delay (600);
				
		led3_g();
		digitalWrite(lmp2, HIGH);
		delay (600);
		digitalWrite(lmp2, LOW);
		off();
		delay (600);
				
		led4_g();
		digitalWrite(lmp3, HIGH);
		delay (600);
		digitalWrite(lmp3, LOW);
		off();
		delay (600);
				
		digitalWrite(led5_g, HIGH);
		digitalWrite(lmp4, HIGH);
		delay (600);
		digitalWrite(led5_g, LOW);
		digitalWrite(lmp4, LOW);
		delay (600);
				
		digitalWrite(led6_g, HIGH);
		digitalWrite(lmp3, HIGH);
		delay (600);
		digitalWrite(led6_g, LOW);
		digitalWrite(lmp3, LOW);
				
		modeState = 0;
	}
	delay(200);	
		
}