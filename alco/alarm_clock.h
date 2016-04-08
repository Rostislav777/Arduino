#include <LiquidCrystal.h>
#include <Wire.h>
#include <DS1307.h>
#include <stdio.h>
#include <EEPROM.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Битовая маска дополнительных символов
byte arrow_up[8] =
{
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
};
byte arrow_down[8] =
{
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
};
byte symbol_o[8] =
{
  B11111,
  B10001,
  B10101,
  B10101,
  B10101,
  B10001,
  B11111,
};
byte symbol_n[8] =
{
  B11111,
  B11111,
  B10111,
  B10001,
  B10101,
  B10101,
  B11111,
};
byte symbol_f[8] =
{
  B11111,
  B10001,
  B10111,
  B10011,
  B10111,
  B10111,
  B11111,
};

//Пин, отвечающий за подключение buzzer
#define alarm_1 2

//Пин, отвечающий за подсветку дисплея
#define BACKLIGHT_PIN 10

unsigned int menu = 1;
unsigned char lcd_buf[32];

//переменные для сохранения данных часов
int hr_00; //часы
int mn_00; //минуты
int sc_00; //секунды
int DD_00; //дни
int MM_00; //мес¤цы
int YY_00; //годы
char delitel = ':'; //разделитель времени дл¤ отображени¤
char slash = '/';

//переменные для настройки часов
int hr; //часы
int mn; //минуты
int sc; //секунды
int dd; //секунды
int mm; //секунды
int yy; //секунды

int qNum = 0;
int keyIn;
int tempVar = 0;
int maxVar = 0;
int minVar = 0;
int adc_key_in;
int key=-1;
int NUM_KEYS = 5;
int adc_key_val[5] ={ 50, 200, 400, 600, 800 };
int oldkey=-1;
String spaceChar = " ";
int time_out;

//alarm_1 время срабатывания
short int hh_on1 = EEPROM.read(1);
short int mm_on1 = EEPROM.read(2);
short int ss_on1 = EEPROM.read(3);

//переменная состояния будильника
short int al_state = EEPROM.read(4);

char question [3][17] = {"Select hour:","Select minute:","Select seconds:"};
char question_1 [3][17] = {"Select day:","Select month:","Select year:"};

void setup()
{	
	// ВНИМАНИЕ!!! перед первым запуском разкоментировать нижний код, загрузить в устройство, запустить устройство, затем закоментировать обратно
	/*
	EEPROM.write(1, 0);	
	EEPROM.write(2, 0);	
	EEPROM.write(3, 0);	
	EEPROM.write(4, 0);	
	EEPROM.write(5, 0);	
	EEPROM.write(6, 0);	
	*/
	pinMode(BACKLIGHT_PIN, OUTPUT);
	digitalWrite(BACKLIGHT_PIN, 1);
	pinMode(alarm_1, OUTPUT);
	digitalWrite(alarm_1, HIGH);
	lcd.begin(16, 2);
	read_rtc1307();
	hello();
	lcd.clear();
	menu;
}

void loop()
{
	//Отработка работы будильника alarm_1
	if (hr_00==EEPROM.read(1)&&mn_00==EEPROM.read(2)&&sc_00==EEPROM.read(3)&&EEPROM.read(4)==1)
	{
		alarm();
	}
	else
	{
		digitalWrite(alarm_1, LOW);
	}

	//настройка реакция на нажатие кнопок
	int kpress = analogRead(0);
	if(kpress < 200)	//up
	{
		delay(250);
		menu = menu - 1;
		lcd.clear();
	}
	else if(kpress < 400) //down
	{
		delay(250);
		menu = menu + 1;
		lcd.clear();
	}
	else if(kpress < 600) //back
	{
		delay(250);
		menu = menu / 10;
		lcd.clear();
	}
	else if(kpress < 800) //next
	{
		delay(250);
		menu  = menu * 10 + 1;
		lcd.clear();	
	}
	
	//если ничего не нажимается, то переходит в режим ожидания
	else if(kpress == 1023 && menu == 2 || kpress == 1023 && menu == 3)
	{
		time_out++;
		if(time_out == 1000)
		{
			lcd.clear();
			menu = 1;
			time_out = 0;
		}
	}
	
	//Главное меню
	if(menu == 1) //режим ожидания
	{
		read_rtc1307();
		rtc1307LCD();
		rtc1307LCD_date();
	}
	if(menu == 2)
	{
		read_rtc1307();
		rtc1307LCD();
		lcd.setCursor(4,1);
		lcd.print("Alarm SET");
		move_possible();
	}
	if(menu == 3)
	{
		read_rtc1307();
		rtc1307LCD();
		lcd.setCursor(4,1);
		lcd.print("Settings");
		move_possible();
	}
	
	//зацикливание главного меню
	if(menu > 3 && menu < 9)
	{
		menu = 2;
	}
	if(menu == 0)
	{
		menu = 2;
	}
	
	if(menu == 11)
	{
		menu = 1;
	}
	
	//подменю settings
	if(menu == 31)
	{
		move_possible();
		lcd.setCursor(4,0);
		lcd.print("Settings");
		lcd.setCursor(4,1);
		lcd.print("Time SET");
	}	
	if(menu == 32)
	{
		move_possible();
		lcd.setCursor(4,0);
		lcd.print("Settings");
		lcd.setCursor(4,1);
		lcd.print("Data SET");
	}
	//зацикливание подменю settings
	if(menu > 32 && menu < 39)
	{
		menu = 31;
	}
	if(menu < 31 && menu > 29)
	{
		menu = 32;
	}
	
	//настройка времени Time SET
	if(menu == 311)
	{
		for (qNum = 0; qNum < 3; qNum++)
		{
			lcd.setCursor(0,0);
			lcd.print(question[qNum]);
			keyIn = 0;
			timeForSet();
		} 
		lcd.clear();
		menu = 1;
	}	
	
	//настройка даты Data SET
	if(menu == 321)
	{
		for (qNum = 0; qNum < 3; qNum++)
		{
			lcd.setCursor(0,0);
			lcd.print(question_1[qNum]);
			keyIn = 0;
			dateForSet();
		} 
		lcd.clear();
		menu = 1;
	}	
	
	//подменю alarm_1
	if(menu == 21)
	{
		move_possible();
		alarm_LCD();
		lcd.setCursor(1,1);
		lcd.print("AlarmState:");
		lcd.setCursor(12,1);
		alarm_state_on();

	}
	if(menu == 22)
	{
		move_possible();
		alarm_LCD();
		lcd.setCursor(1,1);
		lcd.print("AlarmState:");
		lcd.setCursor(12,1);
		alarm_state_off();
	}	
	//зацикливание подменю alarm_1
	if(menu > 22 && menu < 29)
	{
		menu = 21;
	}
	if(menu < 21 && menu > 19)
	{
		menu = 22;
	}
	
	//настройка включения alarm_1
	if(menu == 211)
	{
		for (qNum = 0; qNum < 3; qNum++)
		{
			lcd.setCursor(0,0);
			lcd.print(question[qNum]);
			keyIn = 0;
			Alarm_1onForSet();
		} 
		lcd.clear();
		al_state = 1;
		EEPROM.write(4, al_state);
		menu = 21;
	}	
	if(menu == 221)
	{
		al_state = 0;
		EEPROM.write(4, al_state);
		menu = 1;
		//change_alarm_state();
	}	
}

void read_rtc1307() //функция чтения данных из ds1307
{
	hr_00=RTC.get(DS1307_HR,true);
	mn_00=RTC.get(DS1307_MIN,false);
	sc_00=RTC.get(DS1307_SEC,false);
	DD_00=RTC.get(DS1307_DATE,false);
	MM_00=RTC.get(DS1307_MTH,false);
	YY_00=RTC.get(DS1307_YR,false);
}

void rtc1307LCD() //функция вывода данных времени на дисплей
{
	lcd.setCursor(4, 0);
	if (hr_00 < 10) {lcd.print("0");}
	lcd.print (hr_00);
	lcd.setCursor (6,0);
	lcd.print (delitel);
	lcd.setCursor (7,0);
	if (mn_00 < 10) {lcd.print("0");}
	lcd.print (mn_00);
	lcd.setCursor (9,0);
	lcd.print (delitel);
	lcd.setCursor (10,0);
	if (sc_00 < 10) {lcd.print("0");}
	lcd.print (sc_00);
}

void alarm_LCD() //функция вывода данных времени на дисплей
{
	lcd.setCursor(4,0);
	if (hh_on1 < 10) {lcd.print("0");}
	lcd.print(hh_on1);
	lcd.print(":");
	if (mm_on1 < 10) {lcd.print("0");}
	lcd.print(mm_on1);
	lcd.print(":");
	if (ss_on1 < 10) {lcd.print("0");}
	lcd.print(ss_on1);	
}

void rtc1307LCD_date() //функция вывода данных даты на дисплей
{
	lcd.setCursor (3,1);
	if (DD_00 < 10) {lcd.print("0");}
	lcd.print (DD_00);
	lcd.setCursor (5,1);
	lcd.print (slash);
	lcd.setCursor (6,1);
	if (MM_00 < 10) {lcd.print("0");}
	lcd.print (MM_00);
	lcd.setCursor (8,1);
	lcd.print (slash);
	lcd.setCursor (9,1);
	//if (YY_00 < 10) {lcd.print("0");}
	lcd.print (YY_00);	
}

void timeForSet() //функция настройки часов
{
  switch (qNum) 
	{
	  case 0:
		tempVar = hr; 
		maxVar = 23; 
		minVar = 0; 
		break;
	  case 1:
		tempVar = mn; 
		maxVar = 59; 
		minVar = 0;
		break;
	  case 2:
		tempVar = sc; 
		maxVar = 59; 
		minVar = 0;
	}

  lcd.setCursor(0,1);
  displayVars();

  while (keyIn != 4)
  { 
	adc_key_in = analogRead(0); 
	key = get_key(adc_key_in); 
	delay(100);  
	adc_key_in = analogRead(0);   
	key = get_key(adc_key_in);   
	oldkey = key;

	if (key == 4)
	{ // SELECT button pressed
		switch (qNum) 
		{
		  case 0:
			hr = tempVar; 
			break;
		  case 1:
			mn = tempVar;
			break;
		  case 2:
			sc = tempVar;
		}
		keyIn = 4;
	} 
	
	if (key >= 0) 
	{ // any other key pressed
	  KeyLoop();
	  lcd.setCursor(0,1);
	  displayVars();
	  RTC.stop();
	  RTC.set(DS1307_HR,hr);
	  RTC.set(DS1307_MIN,mn);
	  RTC.set(DS1307_SEC,sc);
	  RTC.start();
	}
	delay(50);
  }
}

void dateForSet() //функция настройки даты
{
  switch (qNum) 
	{
	  case 0:
		tempVar = DD_00; 
		maxVar = 31; 
		minVar = 0; 
		break;
	  case 1:
		tempVar = MM_00; 
		maxVar = 12; 
		minVar = 0;
		break;
	  case 2:
		tempVar = 16; 
		maxVar = 30; 
		minVar = 16; //я указал до 2030 года максимум
	}

  lcd.setCursor(0,1);
  displayVars();

  while (keyIn != 4)
  { 
	adc_key_in = analogRead(0); 
	key = get_key(adc_key_in); 
	delay(100);  
	adc_key_in = analogRead(0);   
	key = get_key(adc_key_in);   
	oldkey = key;

	if (key == 4)
	{ // SELECT button pressed
		switch (qNum) 
		{
		  case 0:
			dd = tempVar; 
			break;
		  case 1:
			mm = tempVar;
			break;
		  case 2:
			yy = tempVar;
		}
		keyIn = 4;
	} 
	
	if (key >= 0) 
	{ // any other key pressed
	  KeyLoop();
	  lcd.setCursor(0,1);
	  displayVars();
	  RTC.stop();
	  RTC.set(DS1307_YR,yy);
	  RTC.set(DS1307_MTH,mm);
	  RTC.set(DS1307_DATE,dd);
	  RTC.start();
	}
	delay(50);
  }
}

void KeyLoop()
{
	switch (key) 
	{
		case 1: // UP button
		tempVar++;
		if (tempVar > maxVar)
		{
			tempVar = minVar;
		}
		break;
		case 2: // DOWN button 
		tempVar--;
		if (tempVar < minVar)
		{
			tempVar = maxVar;
		}
	}
}

void displayVars() 
{
	switch (qNum)
	{
	  case 0:
		move_possible();
		lcd.setCursor(7, 1);
		lcd.print(tempVar + spaceChar); 
		break;
	  case 1:
		move_possible();
		lcd.setCursor(7, 1);
		lcd.print(tempVar + spaceChar); 
		break;
	  case 2:
		move_possible();
		lcd.setCursor(7, 1);
		lcd.print(tempVar + spaceChar);
	}        
}	

void Alarm_1onForSet() //функция настройки будильника
{
	switch (qNum) 
	{
	  case 0:
		tempVar = EEPROM.read(1); 
		maxVar = 23; 
		minVar = 0; 
		break;
	  case 1:
		tempVar = EEPROM.read(2); 
		maxVar = 59; 
		minVar = 0;
		break;
	  case 2:
		tempVar = EEPROM.read(3); 
		maxVar = 59; 
		minVar = 0;
	}

	lcd.setCursor(0,1);
	displayVars();

	while (keyIn != 4) 
	{ 
	adc_key_in = analogRead(0); 
	key = get_key(adc_key_in); 
	delay(100);  
	adc_key_in = analogRead(0);   
	key = get_key(adc_key_in);   
	oldkey = key;

	if (key == 4) 
	{ // SELECT button pressed
		switch (qNum) 
		{
			case 0:
				hh_on1 = tempVar;
				delay(300);
				break;
			case 1:
				mm_on1 = tempVar;
				delay(300);
				break;
			case 2:
				ss_on1 = tempVar;
				delay(300);
		}
		keyIn = 4;
	} 

	if (key >= 0) 
	{ // any other key pressed
		KeyLoop();
		lcd.setCursor(0,1);
		displayVars();
		EEPROM.write(1, hh_on1);
		EEPROM.write(2, mm_on1);
		EEPROM.write(3, ss_on1);
	}
	delay(50);
	}
}

void alarm_state_on() //функция вывода на экран состояния будильника
{
	if(EEPROM.read(4)==1)
	{
		lcd.createChar(3, symbol_o);          // Создаем символ под номером 1
		lcd.createChar(4, symbol_n);        // Создаем символ под номером 2
		lcd.setCursor(12, 1);               // Устанавливаем курсор в начало 1 строки
		lcd.print("\3");                   // Выводим смайлик (символ под номером 1) - "\1"
		lcd.setCursor(13, 1); 
		lcd.print("\4");
	}
	if(EEPROM.read(4)==0)
	{
		lcd.print("ON");
	}
}

void alarm_state_off() //функция вывода на экран состояния будильника
{
	if(EEPROM.read(4)==0)
	{
		lcd.createChar(3, symbol_o);          // Создаем символ под номером 1
		lcd.createChar(5, symbol_f);        // Создаем символ под номером 2
		lcd.setCursor(12, 1);               // Устанавливаем курсор в начало 1 строки
		lcd.print("\3");                   // Выводим смайлик (символ под номером 1) - "\1"
		lcd.setCursor(13, 1); 
		lcd.print("\5");
		lcd.setCursor(14, 1); 
		lcd.print("\5");
	}
	if(EEPROM.read(4)==1)
	{
		lcd.print("OFF");
	}
}

int get_key(unsigned int input) 
{
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
	if (input < adc_key_val[k])
	{
	  return k;
	}
  }   
  if (k >= NUM_KEYS) k = -1; 
  return k;
}

void alarm() //функция срабатывания будильника
{
	for (int i=0; i <= 30; i++)
	{
		lcd.clear();
		lcd.setCursor(5,0);
		lcd.print("ALARM");
		delay(100);
		digitalWrite(alarm_1, 0);
		digitalWrite(BACKLIGHT_PIN, 0);
		delay(100);
		digitalWrite(alarm_1, 1);
		digitalWrite(BACKLIGHT_PIN, 1);
	}
}

void move_possible()
{
	lcd.createChar(1, arrow_up);          // Создаем символ под номером 1
	lcd.createChar(2, arrow_down);        // Создаем символ под номером 2
	lcd.setCursor(0, 1);               // Устанавливаем курсор в начало 1 строки
	lcd.print("\1");                   // Выводим смайлик (символ под номером 1) - "\1"
	lcd.setCursor(15, 1); 
	lcd.print("\2");
}

void hello()
{
	read_rtc1307();
	if (hr_00 >= 21 && hr_00 < 23 || hr_00 >= 0 && hr_00 < 5)
	{
		lcd.setCursor(3, 0); 
		lcd.print("GOOD NIGHT");
	}
	if (hr_00 >= 5 && hr_00 < 10)
	{
		lcd.setCursor(2, 0); 
		lcd.print("GOOD MORNING");
	}
	if (hr_00 >= 10 && hr_00 < 13)
	{
		lcd.setCursor(4, 0); 
		lcd.print("GOOD DAY");
	}
	if (hr_00 >= 13 && hr_00 < 15)
	{
		lcd.setCursor(5, 0); 
		lcd.print("DINNER");
	}
	if (hr_00 >= 15 && hr_00 < 17)
	{
		lcd.setCursor(4, 0); 
		lcd.print("GOOD DAY");
	}
	if (hr_00 >= 17 && hr_00 < 21)
	{
		lcd.setCursor(2, 0); 
		lcd.print("GOOD EVENING");
	}
	delay(1000);
}