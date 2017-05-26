/*
	======Escape Quest======
	===Ten button===
	
	DESCRIPTIONS:
	
	Есть 10 кнопок. При каждом нажатии звучит зуммер,
	при правильно нажатых по-очереди четырех кнопок –
	звучит другой зуммер подтверждения и подается 
	питание на протяжении 5-7 сек. При неправильно 
	нажатой комбинации из четырех кнопок раздается 
	зуммер ошибки и все возвращается в исходное положение 
 */
 
#define relay 3
#define buzzer 9
int trigger=0;
int delay_button=50;
int trigger_beep=0;

void open()
{
  digitalWrite(relay, HIGH);
  Serial.println("Opening");
  beep_event_open();
  delay(5000);
  
  digitalWrite(relay, LOW);
}

void beep()
{
	tone(buzzer, 1000); //включаем на 1000 Гц
	trigger_beep=100;
}

void beep_event_error()
{
	tone(buzzer, 1500); 
	delay(300);  // Пауза 1 секунда (1000 миллисекунд – значение переменной DelaySound ) – длительность воспроизведения сигнала
	noTone(buzzer); // Выключаем звук 

	tone(buzzer, 800); 
	delay(600);
	noTone(buzzer); // Выключаем звук 
	delay(500); 
	
	tone(buzzer, 1500);  
	delay(300);  
	noTone(buzzer); // Выключаем звук 

	tone(buzzer, 800); 
	delay(600);
	noTone(buzzer); // Выключаем звук 
	delay(500); 
}

void beep_event_open()
{
	for(int i=0; i<=5; i++)
	{
		tone(buzzer, 800);
		delay(300);  
		noTone(buzzer); // Выключаем звук
		delay(300);
	}
	tone(buzzer, 1500);
	delay(700); 
	noTone(buzzer); // Выключаем звук 
}

//настройка реакция на нажатие кнопок
char getKey()
{
	int kpress = analogRead(0);
	if(kpress > 120 && kpress < 140)
	{
		beep();
		Serial.println("Key 1");
		return 1;
	}
	else if(kpress > 190 && kpress < 210)
	{
		beep();
		Serial.println("Key 2");
		return 2;
	}
	else if(kpress > 250 && kpress < 280)
	{
		beep();
		Serial.println("Key 3");
		return 3;
	}
	else if(kpress > 300 && kpress < 340)
	{
		beep();
		Serial.println("Key 4");
		return 4;
	}
	else if(kpress > 350 && kpress < 380)
	{
		beep();
		Serial.println("Key 5");
		return 5;
	}
	else if(kpress > 400 && kpress < 425)
	{
		beep();
		Serial.println("Key 6");
		return 6;
	}
	else if(kpress > 430 && kpress < 460)
	{
		beep();
		Serial.println("Key 7");
		return 7;
	}
	else if(kpress > 465 && kpress < 485)
	{
		beep();
		Serial.println("Key 8");
		return 8;
	}
	else if(kpress > 490 && kpress < 520)
	{
		beep();
		Serial.println("Key 9");
		return 9;
	}
	else if(kpress > 525 && kpress < 560)
	{
		beep();
		Serial.println("Key 10");
		return 10;
	}
	return 0;
}
char newKey = 0;
char getNewKey()
{
	while (getKey() == 0)//втыкаем пока не получим код нужной кнопки
	{
		delay(50);
		if(trigger_beep>10)
		{
			trigger_beep = -1;
			Serial.println("trigger_beep");
		}
		else if(trigger_beep<0)
		{
			trigger_beep=0;
			noTone(buzzer); // Выключаем звук 
			Serial.println("stop_beep");
		}
	}
	while (getKey() == 0)
	{
		delay(10);
	}
	newKey = getKey();

	while (getKey() != 0)
	{
		delay(10);
	}
	switch (newKey)
	{
	case 1: 
	Serial.print("found key 1");
	break;
	case 2: 
	Serial.print("found key 2");
	break;
	case 3: 
	Serial.print("found key 3");
	break;
	case 4: 
	Serial.print("found key 4");
	break;
	}
	return newKey;
}

char currentKey = 0;
char password[] = {
  1, 2, 3, 4}; //Собственно пароль
char keyLength = 4; // длинна пароля
char idx = 0;

void setup()
{
	Serial.begin(9600);

	pinMode(relay, OUTPUT);//Relay out
	digitalWrite(relay, LOW);
	
	pinMode(buzzer, OUTPUT);
	
	Serial.println("Setup");
}

void loop()
{
	
	// put your main code here, to run repeatedly:
	currentKey = getNewKey();
	if(idx == -1) 
	{
		idx++;
	}
	if (currentKey == password[idx])
	{
		switch (idx)
		{
		case 0: 
			Serial.println("> key 1 is true");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
				idx = 0;
			}
			break;
		case 1: 
			Serial.println("> key 2 is true");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
				idx = 0;
			}
			break;
		case 2: 
			Serial.println("> key 3 is true");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
				idx = 0;
			}
			break;
		case 3: 
			Serial.println("> key 4 is true");
			trigger++;
			open();
			break;
		}
		idx++;
	} 
	else 
	{
		idx = 0;
		
		
		switch (idx)
		{
		case 0: 
			Serial.println("key 1 is false");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
			}
			break;
		case 1: 
			Serial.println("key 2 is false");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
			}
			break;
		case 2: 
			Serial.println("key 3 is false");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
			}
			break;
		case 3: 
			Serial.println("key 4 is false");
			trigger++;
			if (trigger>=4)
			{
				Serial.println("ERROR");
				beep_event_error();
				trigger=0;
			}
			break;
		}
	}
	if (idx == keyLength || currentKey == 4)
	{
		idx = -1;
	}
	
	delay(200);
}