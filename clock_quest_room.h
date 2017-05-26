//------------------------------------------------------------------------------------------------
#define Relay 4
#define MINUTE_PIN   A3 
#define HOUR_PIN     A2 

int mins, hrs,temp_mins,temp_hrs;
int i;
//------------------------------------------------------------------------------------------------
void setup() 
{  
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  digitalWrite(A3, LOW);
  digitalWrite(A2, LOW);
   Serial.begin(9600);
}
//------------------------------------------------------------------------------------------------
void loop() 
{
  i = 0;

	   while(chek())
	   {
		 delay(100);
		 i += i < 35;
		 if (i > 30)
		 {
		   digitalWrite(Relay, LOW);
		   delay(500);
		 }
		 else
		 {
			digitalWrite(Relay, HIGH);
		 }
		}
  digitalWrite(Relay, HIGH);
  
  delay(100);
  if (abs(temp_mins - mins) > 3 || abs(temp_hrs - hrs) > 5)
  {
  Serial.println("-------------");
  Serial.print("minute: ");
  Serial.println(mins);
  Serial.print("hour: ");
  Serial.println(hrs);
  }
}
//------------------------------------------------------------------------------------------------
bool chek(){
  temp_mins = mins;
  temp_hrs = hrs;
  mins  = analogRead(MINUTE_PIN);
  hrs = analogRead(HOUR_PIN);
  
  if((hrs > 300 && hrs < 350) && (mins > 300 && mins < 400)) return true;
  else return false;
}
//------------------------------------------------------------------------------------------------