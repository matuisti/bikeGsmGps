
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

static const uint32_t GPSBaud = 4800;

SoftwareSerial GPS(9, 5);
SoftwareSerial SIM800(11, 10);
TinyGPSPlus gps;
char inchar;
double Lat;
double Lon;
double Speed;
double Alt;

void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  delay(300);
  digitalWrite(8, LOW);

  Serial.begin(115200);
  SIM800.begin(9600);
  GPS.begin(GPSBaud);

  delay(3000);
  Serial.println("");
  Serial.print("ODOTA");
  Serial.print(".");
  delay(3000);
  Serial.print(".");
  delay(3000);
  Serial.println(".");
  delay(3000);
  SIM800.print("AT+CMGF=1\r");
  delay(500);
  SIM800.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  batteryTest();
  Serial.println("Valmis!");
  delay(100);
  digitalWrite(13, LOW);

}

void reset()
{
  delay(100);
  SIM800.println("AT+CMGD=1,4");
  delay(1000);
  SIM800.print("AT+CNMI=2,2,0,0,0\r");
  delay(1000);
  Serial.println("reset");
  digitalWrite(13, LOW);
  loop();
}
void Gps() {

  GPS.listen();
  delay(100);
  int gpsBoot = 0;
  while (gpsBoot < 15) {
    delay(250);
    while (GPS.available() > 0)
      if (gps.encode(GPS.read()))
        Serial.print(F(""));
    if (gps.location.isValid())
    {
      delay(200);
      Serial.print(gps.location.lat(), 6);
      Lat = gps.location.lat();
      Serial.print(F(", "));
      Serial.print(gps.location.lng(), 6);
      Lon = gps.location.lng();
      Serial.print(F(", "));
      Serial.print(gps.speed.kmph());
      Speed = gps.speed.kmph();
      Serial.print(F(" Kmh"));
      Serial.print(F(", "));
      Serial.print(gps.altitude.meters());
      Alt = gps.altitude.meters();
    }
    else
    {
      delay(100);
      Serial.print(F("SIJAINTIA EI LÖYDY"));
    }

    Serial.println();
    delay(100);

    gpsBoot++;
  }

}

void loop() {
  SIM800.listen();
  if (SIM800.available() > 0)

  {
    inchar = SIM800.read();
    if (inchar == 'g')
    {
      delay(10);

      inchar = SIM800.read();
      if (inchar == 'p')
      {
        delay(10);

        inchar = SIM800.read();
        if (inchar == 's')
        {
          Gps();
          Serial.print("LON :");
          Serial.println(Lat, 6);
          delay(200);
          Serial.print("LAT :");
          Serial.println(Lon, 6);
          delay(100);
          digitalWrite(13, HIGH);
          delay(100);
          SIM800.print("AT+CMGF=1\r");
          delay(1000);
          SIM800.println("AT+CMGS=\"+358409629998\"");
          delay(1000);
          SIM800.print("https://www.google.com/maps?q=");
          delay(100);
          SIM800.print(Lat, 6);
          delay(100);
          SIM800.print(",");
          delay(100);
          SIM800.println(Lon, 6);
          delay(100);
          SIM800.print("Nopeus: ");
          delay(100);
          SIM800.print(Speed);
          delay(100);
          SIM800.println(" kmh");
          delay(100);
          SIM800.print("Korkeus: ");
          delay(100);
          SIM800.print(Alt);
          delay(100);
          SIM800.println(" m");
          delay(100);
          SIM800.print("Akku: ");
          delay(100);
          int sensorValue = analogRead(A5);
          float voltage = (sensorValue * (5.05 / 1023.0)) - 3.45;
          int prosent = (voltage / 0.75) * 100;
          Serial.print(prosent);
          delay(10);
          SIM800.print(prosent);
          delay(100);
          SIM800.print("%");
          delay(1000);
          SIM800.print((char)26);
          delay(1000);
          reset();

        }
      }
    }
  }
}
void batteryTest() {
  int sensorValue = analogRead(A5);
  float voltage = (sensorValue * (5.05 / 1023.0)) - 3.45;
  int prosent = (voltage / 0.75) * 100;
  if (prosent >= 10) {
    digitalWrite(8, HIGH);
    delay(500);
    digitalWrite(8, LOW);
    delay(500);
    if (prosent >= 25) {
      digitalWrite(8, HIGH);
      delay(500);
      digitalWrite(8, LOW);
      delay(500);
      if (prosent >= 50) {
        digitalWrite(8, HIGH);
        delay(500);
        digitalWrite(8, LOW);
        delay(500);
        if (prosent >= 75) {
          digitalWrite(8, HIGH);
          delay(500);
          digitalWrite(8, LOW);
          delay(500);
          if (prosent >= 90) {
            digitalWrite(8, HIGH);
            delay(500);
            digitalWrite(8, LOW);
            delay(500);
          }
        }
      }
    }
  }
  Serial.print("Akku: ");
  Serial.print(prosent);
  Serial.println("%");
}
