#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 02);
#define DS1307 0x68
#define BtnXacNhan 2
#define BtnTang 3
#define BtnGiam 4
#define Coi A6
void DongCo( uint8_t TocDo);
void FuncBienTro();
void LCD();
void Buzz(int Muc);
int PWM = 50, tThoiGian = 1, ThoiGian=100, Lan1 = 0, BatDau = 0, S01 = 0, Giay = 0, Phut = 0,KhoiDong=0;
volatile int second=0, minute=0, hour, day, date, month, year;
void setup()
{

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(Coi, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(BtnTang, INPUT_PULLUP);
  pinMode(BtnGiam, INPUT_PULLUP);
  pinMode(BtnXacNhan, INPUT_PULLUP);
  digitalWrite(Coi,0);
  Wire.begin();
  Serial.println("FINISH SET TIME");
  DS1307_SetSQWPin1Hz();
}
void loop()
{
  DS1307_ReadTime();
 // Serial.print(hour); Serial.print(":"); Serial.print(minute); Serial.print(":"); Serial.println(second);
  FuncBienTro();
  LCD();
  if (digitalRead(BtnTang) == 0)
  {
    delay(50);
    while (digitalRead(BtnTang) == 0);
    delay(50);
    tThoiGian++;
  }
  if (digitalRead(BtnGiam) == 0)
  {
    delay(50);
    while (digitalRead(BtnGiam) == 0);
    delay(50);
    tThoiGian--;
    if (tThoiGian < 0)
    {
      tThoiGian = 0;
    }
  }
  if (digitalRead(BtnXacNhan) == 0)
  {
    
    delay(50);
    while (digitalRead(BtnXacNhan) == 0);
    delay(50);
    Buzz(0);
    ThoiGian = tThoiGian;
    Lan1=1;
   
  }
  if(ThoiGian==minute)
  {
    Lan1=2;
  }
}

void LCD()
{
  if (Lan1 == 0)
  {
    Buzz(0);
    DongCo(0);
    lcd.setCursor(0, 0);
    lcd.print(" Choose Minute ? " );
    lcd.setCursor(0, 1);
    lcd.print("      " +  String(tThoiGian) + " " + "Min"  );
  }
  else if (Lan1 == 1)
  {
     Buzz(0);
    if(KhoiDong==0)
    {
      KhoiDong=1;
      DS1307_SetTime(0, 0, 0, 0, 0, 0, 0);
      DS1307_ReadTime();
    }
    DongCo(PWM);
    lcd.setCursor(0, 0);
    lcd.print("  Timer " + String(ThoiGian) + " Min  " );
    lcd.setCursor(0, 1);
    lcd.print("  " + String(minute) + " Min " + String(second) + " Sec" );
  }
  else if (Lan1 == 2)
  {
    DongCo(0);
    delay(100);
    Buzz(1);
    delay(100);
    Buzz(0);
    if(minute-ThoiGian==1)
    {
       Buzz(0);
    }
    lcd.setCursor(0, 0);
    lcd.print("    The End      " );
    lcd.setCursor(0, 1);
    lcd.print(" Please Turn Off " );
  }

}
void FuncBienTro()
{
  uint32_t BT1 = analogRead(A0);
  PWM = map(BT1, 0, 1023, 0, 255);
}
void DongCo( uint8_t TocDo)
{
  digitalWrite(5, 0);
  analogWrite(6, TocDo);
}
void Buzz(int Muc)
{
  digitalWrite(Coi, Muc);
}
void DS1307_ReadTime()
{
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(DS1307, 7);

  second = bcdtodec(Wire.read() & 0x7f);
  minute = bcdtodec(Wire.read());
  hour   = bcdtodec(Wire.read() & 0x3f);
  day    = bcdtodec(Wire.read());
  date   = bcdtodec(Wire.read());
  month  = bcdtodec(Wire.read());
  year   = bcdtodec(Wire.read());
}

int bcdtodec(byte num)      //chuy?n d?i t? h? 16bit sang h? 10bit  //Ð? d?c th?i gian
{
  return ((num / 16 * 10) + (num % 16));
}

int dectobcd(byte num)      //chuy?n d?i t? h? 10bit sáng h? 16bit  // Ð? cài d?t th?i gian
{
  return ((num / 10 * 16) + (num % 10));
}

void DS1307_SetTime(byte hr, byte mnt, byte sec, byte dt, byte mth, byte y, byte d)
{
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0x00));
  Wire.write(dectobcd(sec));
  Wire.write(dectobcd(mnt));
  Wire.write(dectobcd(hr));
  Wire.write(dectobcd(d));
  Wire.write(dectobcd(dt));
  Wire.write(dectobcd(mth));
  Wire.write(dectobcd(y));
  Wire.endTransmission();
}

void DS1307_SetSQWPin1Hz()
{
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0x07));
  Wire.write(byte(0x10));
  Wire.endTransmission();
}
