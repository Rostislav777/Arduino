#include <LiquidCrystal.h>
#include <Wire.h>
#include <stdio.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define heater 11 //к этому выводу подключаем реле (шим управление)
int sens1_t;
int sens2_t1_h;
int sens2_t2_t;
float readSens1;
float readSens2;
int interval=20; //min 30sec recomended for 18B20

char question [2][20] = {"Select sens1_t", "Sel. sens2_t1_h"};


OneWire oneWire(12);// вход датчиков 18b20
DallasTemperature ds(&oneWire);
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



void setup()
{	
	lcd.begin(16, 2);
	ds.begin();
	setPwmFrequency(heater, 1024); //division on 1024 for ~30Hz
	lcd.clear();
	menu;
	
	//EEPROM:
	//Адрес: 0-Длительность полива; 1-Задержка между поливами; 2-порог влажности почвы; 10-Длина SSID; 11-27-Символы SSID Wi-Fi;
	//50-Длина Password Wi-Fi; 51-82-Символы Password Wi-Fi; 90-Длина Blynk auth-идентификатора; 91-122-символы Blynk auth-идентификатора
	//EEPROM.write(0, 20);//Записываем в EEPROM новое значение
	sens1_t	= EEPROM.read(0);//Считываем значение длительность полива из памяти EEPROM 0
	sens2_t1_h	= EEPROM.read(1);//Считываем значение длительность полива из памяти EEPROM 0
}

void loop()
{


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
		dsRead();
	}
	if(menu == 2)
	{
		lcd.setCursor(0,0);
		lcd.print("Total menu");
		lcd.setCursor(2,1);
		lcd.print("Mode 1 Start");
		move_possible();
	}
	if(menu == 3)
	{
		lcd.setCursor(0,0);
		lcd.print("Total menu");
		lcd.setCursor(2,1);
		lcd.print("Mode 2 Start");
		move_possible();
	}
	if(menu == 4)
	{
		lcd.setCursor(0,0);
		lcd.print("Total menu");
		lcd.setCursor(1,1);
		lcd.print("Mode 1 Setting");
		move_possible();
	}
	if(menu == 5)
	{
		lcd.setCursor(0,0);
		lcd.print("Total menu");
		lcd.setCursor(1,1);
		lcd.print("Mode 2 Setting");
		move_possible();
	}
	
	//зацикливание главного меню
	if(menu > 5 && menu < 9)
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
	
	//подменю Mode 2
	if(menu == 31)
	{
		lcd.setCursor(0,0);
		lcd.print("Mode 2");
		lcd.setCursor(0,1);
		lcd.print("Head Start");
	}	
	if(menu == 32)
	{
		lcd.setCursor(0,0);
		lcd.print("Mode 2");
		lcd.setCursor(0,1);
		lcd.print("Body Start");
	}	
	if(menu == 33)
	{
		lcd.setCursor(0,0);
		lcd.print("Mode 2");
		lcd.setCursor(0,1);
		lcd.print("Tails Start");
	}	

	//зацикливание подменю Mode 2
	if(menu > 33 && menu < 39)
	{
		menu = 31;
	}
	if(menu < 31 && menu > 29)
	{
		menu = 33;
	}
	
	
	//подменю Head Start
	if(menu == 311)
	{
		ds.requestTemperatures(); // считываем температуру с датчиков
		readSens1 = ds.getTempCByIndex(0);

		if (readSens1 >= sens2_t1_h)
		{
			analogWrite(heater, 0);
			lcd.clear();
			lcd.setCursor(4, 0);  
			lcd.print("Mode 2");
			lcd.setCursor(2, 1);  
			lcd.print("Head FINISH");
			delay(1000);
			lcd.clear();
			
			//menu = 1;
		}
		if (readSens1 < sens2_t1_h)
		{
			analogWrite(heater, 255);
		}
		
		lcd.setCursor(0, 0);  
		lcd.print("Mode2 Head START");

		lcd.setCursor(0, 1);  
		lcd.print("Sensor 1: ");
		lcd.print(ds.getTempCByIndex(0));
		lcd.print("C");
	}
	
	
	//подменю Mode 1 Start
	if(menu == 21)
	{
		ds.requestTemperatures(); // считываем температуру с датчиков
		readSens1 = ds.getTempCByIndex(0);

		if (readSens1 >= sens1_t)
		{
			analogWrite(heater, 0);
			lcd.clear();
			lcd.setCursor(4, 0);  
			lcd.print("Mode 1");
			lcd.setCursor(4, 1);  
			lcd.print("FINISH");
			delay(1000);
			lcd.clear();
			menu = 1;
		}
		if (readSens1 < sens1_t)
		{
			analogWrite(heater, 255);
		}
		
		lcd.setCursor(0, 0);  
		lcd.print("Mode 1 starting");

		lcd.setCursor(0, 1);  
		lcd.print("Sensor 1: ");
		lcd.print(ds.getTempCByIndex(0));
		lcd.print("C");
	}

	//зацикливание подменю Mode 1 Start
	if(menu > 21 && menu < 29)
	{
		menu = 21;
	}
	if(menu < 21 && menu > 19)
	{
		menu = 21;
	}
	
	
	//подменю Mode 1 Settings
	if(menu == 41)
	{
		for (qNum = 0; qNum < 1; qNum++)
		{
			lcd.setCursor(0,0);
			lcd.print(question[qNum]);
			keyIn = 0;
			sens1_tForSet();
		} 
		lcd.clear();
		menu = 1;
	}
	
	//подменю Mode 2 Settings
	if(menu == 51)
	{
		lcd.setCursor(0,0);
		lcd.print("Mode 2 Settings");
		lcd.setCursor(0,1);
		lcd.print("Sens1 head setup");
	}
	if(menu == 52)
	{
		lcd.setCursor(0,0);
		lcd.print("Mode 2 Settings");
		lcd.setCursor(0,1);
		lcd.print("Sens2 body setup");
	}
	if(menu == 53)
	{
		lcd.setCursor(0,0);
		lcd.print("Mode 2 Settings");
		lcd.setCursor(0,1);
		lcd.print("Sens1 Tails setup");
	}
	//зацикливание подменю Mode 2 Settings
	if(menu > 53 && menu < 59)
	{
		menu = 51;
	}
	if(menu < 51 && menu > 59)
	{
		menu = 53;
	}
	
	//подменю Sens1 head setup
	if(menu == 511)
	{
		for (qNum = 1; qNum < 2; qNum++)
		{
			lcd.setCursor(0,0);
			lcd.print(question[qNum]);
			keyIn = 0;
			sens1_tForSet();
		} 
		lcd.clear();
		menu = 1;
	}
}


//-----------------------------------
//Функция отображения возможных вариантов движаения по меню
void move_possible()
{
	lcd.createChar(1, arrow_up);          // Создаем символ под номером 1
	lcd.createChar(2, arrow_down);        // Создаем символ под номером 2
	lcd.setCursor(0, 1);               // Устанавливаем курсор в начало 1 строки
	lcd.print("\1");                   // Выводим смайлик (символ под номером 1) - "\1"
	lcd.setCursor(15, 1); 
	lcd.print("\2");
}


//-----------------------------------
//Функция считывания датчиков ds18b20:
void dsRead()
{
	ds.requestTemperatures(); // считываем температуру с датчиков
	
	lcd.setCursor(0, 0);  
	lcd.print("Sensor 1: ");
	lcd.print(ds.getTempCByIndex(0)); // отправляем температуру
	lcd.print("C");

	lcd.setCursor(0, 1);  
	lcd.print("Sensor 2: ");
	lcd.print(ds.getTempCByIndex(1));
	lcd.print("C");
}

void setPwmFrequency(int pin, int divisor) 
{
	byte mode;
	if(pin == 5 || pin == 6 || pin == 9 || pin == 10) 
	{
		switch(divisor) 
		{
			case 1: mode = 0x01; break;
			case 8: mode = 0x02; break;
			case 64: mode = 0x03; break;
			case 256: mode = 0x04; break;
			case 1024: mode = 0x05; break;
			default: return;
		}
	}
}

//-----------------------------------
//Функция настройки температуры срабатывания 1го датчика sens1_t
void sens1_tForSet()
{
  switch (qNum) 
	{
	  case 0:
		tempVar = sens1_t; 
		maxVar = 100; 
		minVar = 20; 
		break;
	  case 1:
		tempVar = sens2_t1_h; 
		maxVar = 100; 
		minVar = 20;
		break;
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
			sens1_t = tempVar; 
			break;
		  case 1:
			sens2_t1_h = tempVar;
			//break;
		}
		keyIn = 4;
	} 
	
	if (key >= 0) 
	{ // any other key pressed
		KeyLoop();
		lcd.setCursor(0,1);
		displayVars();
		//назначаем данные
		EEPROM.write(0, sens1_t);//Записываем в EEPROM новое значение
		EEPROM.write(1, sens2_t1_h);//Записываем в EEPROM новое значение
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