#define LED_PIN_1 A0
#define LED_PIN_2 A1
#define LED_PIN_3 A2
#define LED_PIN_4 A3
#define PERIOD 15

char* location_1 = "SLocation_06";

int string_length_1,string_length_2,string_length_3,string_length_4;

void setup() 
{
  Serial.begin(9600);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  string_length_1 = strlen(location_1);
}

void loop() 
{
  for(int i = 0; i < string_length_1; i ++)
  {
    send_byte(LED_PIN_1, location_1[i]);
  }
}

void send_byte(int LED_PIN, char my_byte)
{
  digitalWrite(LED_PIN, LOW);
  delay(PERIOD);
  
  //transmission of bits
  for(int i = 0; i < 8; i++)
  {
    Serial.println(my_byte);
    Serial.println((my_byte&(0x01 << i))!=0);
    digitalWrite(LED_PIN, (my_byte&(0x01 << i))!=0 );
    delay(PERIOD);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(PERIOD);
  return 0;
}
