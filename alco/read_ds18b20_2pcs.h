#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
 
OneWire oneWire(11);// вход датчиков 18b20
DallasTemperature ds(&oneWire);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//byte qty =2; // количество градусников на шине 

void setup() {
	lcd.begin(16, 2);
  Serial.begin(9600);
  ds.begin();
  
  //qty = ds.getDeviceCount(); 
  //Serial.print("Found ");
//  Serial.print(qty);
//  Serial.println(" devices.");
    //
  
}

void loop() 
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