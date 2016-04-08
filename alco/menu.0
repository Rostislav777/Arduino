#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
 
OneWire oneWire(11);// вход датчиков 18b20
DallasTemperature ds(&oneWire);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Опрелеляем значения для считывания кнопок с кейпадшилда:
#define noneVal 900
#define rightVal 50
#define upVal 200
#define downVal 350
#define leftVal 550
#define selectVal 850

//Массив для навигации по меню:
int arrMenu[4]={1, 0, 0, 0};//По умолчанию - режим по сенсору

void setup() 
{
	lcd.begin(16, 2);
	Serial.begin(9600);
	ds.begin();
  

  
}

void loop() 
{
	readLCDbtn();//Опрос кнопки
	Menu(arrMenu);
	
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

//-----------------------------------
//Функция вывода на экран двух произвольных строк:
void LCDWright(String a1, String b1) 
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(a1);
	lcd.setCursor(0, 1);
	lcd.print(b1);
}

//-----------------------------------
//Функция вывода МЕНЮ на LCD-дисплей:
void Menu(int arr[4])
{
	int keyIN2;
	int intArr=1000*arr[0]+100*arr[1]+10*arr[2]+arr[3];
	switch (intArr)
	{
		//Страница, включающаяся только во время полива
		case 0000:
		break;    

		//Верхний уровень меню (1-й)    
		case 1000:
		LCDWright("Schedule mode", "");
		break;
		case 2000:
		LCDWright("Sensor mode", "");
		break;
		case 3000:
		LCDWright("Manual mode", "");
		break;
		case 4000:
		LCDWright("Watering", " settings");
		break;
		case 5000:
		LCDWright("Wi-Fi settings", "");
		break;
	}
}

//---------------------------------
//Функция считывания кнопки (глобально)
void readLCDbtn()
{
	int keyIN = analogRead(0); //Чтение нажатой кнопки с аналогового пина 0

	if (keyIN > noneVal) {}

	else if (keyIN < rightVal) //Нажата кнопка Right
	{ 
		RightPress();
	}

	else if (keyIN < upVal && keyIN>rightVal) //Нажата кнопка Up
	{
		UpPress();
	}

	else if (keyIN < downVal && keyIN>upVal) //Нажата кнопка DOWN
	{
		DownPress();
	}

	else if (keyIN < leftVal && keyIN>downVal) //Нажата кнопка Left
	{ 
		LeftPress();
	}

	else if (keyIN< selectVal && keyIN>leftVal) //Нажата кнопка Select
	{ 
		SelectPress();
	}
}

//----------------------------------
//Функция обработки нажатия клавиши "ВПРАВО":
void RightPress()
{
	if (arrMenu[0]!=0) //Проверка того, что полив не осуществляется
	{
		if (arrMenu[1]==0) //Проверка того, что мы находимся на верхнем уровне меню (1-м)
		{
			arrMenu[1]=1; //Проваливаемся на нижестоящий пункт меню-2-й уровень
		}
		else if (arrMenu[2]==0 && (arrMenu[0]==4 || arrMenu[0]==5)) //только в случае, если верхний уровень меню равен 4 или 5
		{
			arrMenu[2]=1; //Проваливаемся на нижестоящий пункт меню-3-й уровень 
		}
		else if (arrMenu[3]==0 && arrMenu[0]==5 && arrMenu[1]==2 && (arrMenu[2]==1 || arrMenu[2])==2) //только в случае, если верхний уровень меню равен 5, 2-й равен 2, 3-й - 1 или 2
		{
			arrMenu[3]=1; //Проваливаемся на нижестоящий пункт меню-4-й уровень
		}
		else
		{
			// do nothing
		}
		delay(150);
	}
}

//----------------------------------
//Функция обработки нажатия клавиши "ВЛЕВО":
void LeftPress()
{
	if (arrMenu[3]!=0) //Проверка того, что мы находимся 4-м уровне меню
	{
		arrMenu[3]=0; //Поднимаемся на вышестоящий пункт меню-3-й уровень
	}
	else if (arrMenu[2]!=0) //Проверка того, что мы находимся 3-м уровне меню
	{
		arrMenu[2]=0; //Поднимаемся на вышестоящий пункт меню-2-й уровень 
		if(arrMenu[0]==1 && arrMenu[1]==1) //Проверяем, если мы вышли из режима по расписанию (пункт меню 1110)
		{

		}
	}
	else if (arrMenu[1]!=0) //Проверка того, что мы находимся 2-м уровне меню
	{
		arrMenu[1]=0; //Поднимаемся на вышестоящий пункт меню-1-й уровень
		if(arrMenu[0]==2) //Проверяем, если мы вышли из режима по сенсору (пункт меню 2100)
		{

		}
	}
	else if (arrMenu[0]==0) //Проверка того, что идет полив
	{

		//arrMenu[0]=1; //Переходим в главное меню (1,0,0,0)
	}
	delay(150); 
}

//----------------------------------
//Функция обработки нажатия клавиши "ВНИЗ":
void DownPress()
{
	if (arrMenu[0]!=0) //Проверка того, что полив не осуществляется
	{
		if (arrMenu[1]==0) //Проверка того, что мы находимся 1-м уровне меню
		{
			arrMenu[0] += 1; //Двигаемся по тому же уровню вверх
			if (arrMenu[0]>5) //На самом верхнем уровне только 5 пунктов
			{
				arrMenu[0]=1;
			}
		}
		else if (arrMenu[2]==0) //Проверка того, что мы находимся 2-м уровне меню
		{
			if (arrMenu[0]==1 || arrMenu[0]==3 || arrMenu[0]==3) //На 2-м уровне меня для пунктов 1,2,3 - 3-й уровень состоит только из одного элемента
			{ 
				arrMenu[1]=1;
			}
			else if (arrMenu[0]==4)
			{
				arrMenu[1]+=1;
				if (arrMenu[1]>3)  //Для пункта 4 верхнего уровня меню есть только 3 значения: 1,2,3
				{
					arrMenu[1]=1;
				}
			}
			else if (arrMenu[0]==5)
			{
				arrMenu[1]+=1;
				if (arrMenu[1]>6) //Для пункта 5 верхнего уровня меню есть только 6 значений: 1,2,3,4,5,6
				{ 
					arrMenu[1]=1;
				}
			}
		}
		else if (arrMenu[3]==0) //Проверка того, что мы находимся 3-м уровне меню
		{
			if ((arrMenu[0]==4  && (arrMenu[1]==1 || arrMenu[1]==2 || arrMenu[1]==3)) || (arrMenu[0]==5 && (arrMenu[1]==1 || arrMenu[1]==3 || arrMenu[1]==4 || arrMenu[1]==5 || arrMenu[1]==6)))
			{
				arrMenu[2]=1; //Для пунктов 4.1, 4.2, 4.3, 5.1, 5.3, 5.4, 5.5, 5.6 - есть только один пункт 3-го уровня меню
			}
			else if (arrMenu[0]==5 && arrMenu[1]==2) 
			{
				arrMenu[2] += 1;
				if (arrMenu[2]>2) //Для пункта 5.2 - 3-й уровень может принимать только значения 1 или 2
				{
				  arrMenu[2]=1;
				}
			}
		}
		delay(150); 
	}
}

//----------------------------------
//Функция обработки нажатия клавиши "ВВЕРХ":
void UpPress()
{
	if (arrMenu[0]!=0) //Проверка того, что полив не осущестляется
	{
		if (arrMenu[1]==0) //Проверка того, что мы находимся 1-м уровне меню
		{
			arrMenu[0] -= 1; //Двигаемся по тому же уровню вниз
			if (arrMenu[0]<1) //На самом верхнем уровне только 5 пунктов
			{ 
				arrMenu[0]=5;
			}
		}
		else if (arrMenu[2]==0) //Проверка того, что мы находимся 2-м уровне меню
		{
			if (arrMenu[0]==1 || arrMenu[0]==3 || arrMenu[0]==3) //На 2-м уровне меня для пунктов 1,2,3 - 3-й уровень состоит только из одного элемента
			{
				arrMenu[1]=1;
			}
			else if (arrMenu[0]==4)
			{
				arrMenu[1]-=1;
				if (arrMenu[1]<1) //Для пункта 4 верхнего уровня меню есть только 3 значения: 1,2,3
				{ 
					arrMenu[1]=3;
				}
			}
			else if (arrMenu[0]==5)
			{
				arrMenu[1]-=1;
				if (arrMenu[1]<1) //Для пункта 5 верхнего уровня меню есть только 6 значений: 1,2,3,4,5,6
				{ 
					arrMenu[1]=6;
				}
			}
		}
		else if (arrMenu[3]==0) //Проверка того, что мы находимся 3-м уровне меню
		{
			if ((arrMenu[0]==4  && (arrMenu[1]==1 || arrMenu[1]==2 || arrMenu[1]==3)) || (arrMenu[0]==5 && (arrMenu[1]==1 || arrMenu[1]==3 || arrMenu[1]==4 || arrMenu[1]==5)))
			{
				arrMenu[2]=1; //Для пунктов 4.1, 4.2, 4.3, 5.1, 5.3, 5.4, 5.5 - есть только один пункт 3-го уровня меню
			}
			else if (arrMenu[0]==5 && arrMenu[1]==2) 
			{
				arrMenu[2] -= 1;
				if (arrMenu[2]<1) //Для пункта 5.2 - 3-й уровень может принимать только значения 1 или 2
				{ 
					arrMenu[2]=2;
				}
			}
		}
		delay(150);  
	}
}

//----------------------------------
//Функция обработки нажатия клавиши "ВЫБОР":
void SelectPress()
{
	if (arrMenu[2]==0) //Проверка того, что мы находимся 2-м уровне меню
	{
		if (arrMenu[0]==3 && arrMenu[1]==1) //Пункт меню 3.1
		{

		}
		else if(arrMenu[0]==1 && arrMenu[1]==1)//Пункт меню 1.1
		{

		}
	}
	delay(150);
}
