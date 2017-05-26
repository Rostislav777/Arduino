void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  pinMode(10, OUTPUT);//Relay A out
  digitalWrite(10, HIGH);
  pinMode(11, OUTPUT);//Relay B out
  digitalWrite(11, HIGH);


  pinMode(9, INPUT); //Key "Actvate Relay"
  pinMode(8, INPUT); //Key "4"
  pinMode(7, INPUT); //Key "3"
  pinMode(6, INPUT); //Key "2"
  pinMode(5, INPUT); //Key "1"


}

void open()
{
  digitalWrite(10, LOW);
  Serial.println("Opening");
  delay(2000);
  digitalWrite(10, HIGH);
  Serial.println("Opened");
}

void close()
{
  digitalWrite(11, LOW);
  Serial.println("Closing");
  delay(2000);
  digitalWrite(11, HIGH);
  Serial.println("Closed");
}


void activateRelay()
{
  open();
  delay(4000);
  close();
}

char getKey()
{
  if (digitalRead(9) == 0)
  {
    //   Serial.println("Key 5");
    return 5;
  }
  if (digitalRead(8) == 0)
  {
    //  Serial.println("Key 4");
    return 4;
  }
  if (digitalRead(7) == 0)
  {
    //    Serial.println("Key 3");
    return 3;
  }
  if (digitalRead(6) == 0)
  {
    //  Serial.println("Key 2");

    return 2;
  }
  if (digitalRead(5) == 0)
  {
    //  Serial.println("Key 1");
    return 1;
  }
  return 0;
}
char newKey = 0;
char getNewKey()
{
  while (getKey() == 0)//втыкаем пока не получим код нужной кнопки
  {
    delay(50);
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
  case 5: 
    Serial.print("found key 5");
    break;
  }
  return newKey;
}

char currentKey = 0;
char password[] = {
  1, 2, 3, 4, 2}; //Собственно пароль
char keyLength = 5; // длинна пароля
char idx = 0;


void loop() {
  // put your main code here, to run repeatedly:
  currentKey = getNewKey();
  if(idx == -1) {
    close();
    idx++;
  }
  if (currentKey == password[idx])
  {
    switch (idx)
    {
    case 0: 
      Serial.println("> key 1 is true");
      break;
    case 1: 
      Serial.println("> key 2 is true");
      break;
    case 2: 
      Serial.println("> key 3 is true");
      break;
    case 3: 
      Serial.println("> key 4 is true");
      break;
    case 4: 
      Serial.println("> key 5 is true");
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
      break;
    case 1: 
      Serial.println("key 2 is false");
      break;
    case 2: 
      Serial.println("key 3 is false");
      break;
    case 3: 
      Serial.println("key 4 is false");
      break;
    case 4: 
      Serial.println("key 5 is false");
      break;
    }
  }
  if (idx == keyLength || currentKey == 5)
  {
    idx = -1;
    open();
  }
  delay(200);
}


