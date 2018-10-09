#include <OBD2UART.h>
#include <UTFT.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

COBD obd;
UTFT    myGLCD(SSD1289, 38, 39, 40, 41);
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
int x, y;

TinyGPS gps;
SoftwareSerial ss(10, 9);
static void smartdelay(unsigned long ms);


void setup()
{
  // Initial setup
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  Serial.begin(115200);
  myGLCD.setBackColor(0, 0, 255);
  drawButtons();
  // start communication with OBD-II UART adapter
   obd.begin();
  myGLCD.setFont(BigFont); 
  
  ss.begin(57600);
  }

int speed = 0,rpm = 0, throttle = 0, tadv = 0, fuel = 0, compass;
double speedNew = 0;
float yaw, pitch, roll;
   
void loop()
{
smartdelay(1000);
location();
obd.readPID(PID_SPEED, speed);
speedNew = speed * 0.621371;
//Serial.print("Speed: ");
//Serial.print(speed);
myGLCD.printNumF(speedNew, 2 , 20, 160); //(Number,Decimal Places, X, Y)

obd.readPID(PID_RPM, rpm);
//Serial.print("       RPM: ");
//Serial.println(rpm);
myGLCD.printNumI(rpm,170 , 160); //(Number, X, Y)

obd.readPID(PID_THROTTLE, throttle);
myGLCD.printNumI(throttle,5 + (2 * 60), 15); //(Number, X, Y) 

obd.readPID(PID_TIMING_ADVANCE, tadv);
myGLCD.printNumI(tadv,5 + (0 * 60), 15); //(Number, X, Y)  

obd.readPID(PID_FUEL_LEVEL, fuel);
myGLCD.printNumI(fuel,5 + (1 * 60), 15); //(Number, X, Y)  
myGLCD.print("%",35 + (1 * 60), 15);

//
//myGLCD.setFont(SmallFont); 
//obd.memsOrientation(yaw, pitch, roll);
//  myGLCD.printNumF(yaw, 2,30 + (0 * 60), 75);
//  myGLCD.printNumF(pitch, 2,90 + (0 * 60), 75);
//  myGLCD.printNumF(roll, 2,150 + (0 * 60), 75);
//  delay(100);
// myGLCD.setFont(BigFont); 

}

void drawButtons()
{
  // Draw the upper row of buttons
  for (x = 0; x<5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect(10 + (x * 60), 1, 64 + (x * 60), 70);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect(10 + (x * 60), 1, 64 + (x * 60), 70);
  }
  myGLCD.print("Ignition", 5 + (0 * 60), 1); //(Number, X, Y);
  myGLCD.print("Fuel", 13+ (1 * 60), 1); //(Number, X, Y);
  myGLCD.print("Throttle", 5 + (2 * 60), 1); //(Number, X, Y);
  myGLCD.print("N/A", 18 + (3 * 60), 1); //(Number, X, Y);
  myGLCD.print("Compass", 13 + (4 * 60), 1); //(Number, X, Y);

// Draw the lower row of buttons
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(10, 130, 150, 220);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(10, 130, 150, 220);
  myGLCD.print("Speed", 12, 132);
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect(160, 130, 300, 220);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect(160, 130, 300, 220);
  myGLCD.print("RPM", 164, 132);
  myGLCD.setBackColor(0, 0, 0);

//  if (obd.memsOrientation(yaw, pitch, roll)) {
//  myGLCD.print("Orientation: ",5 + (0 * 60), 75);
//  }

}

void Direction()
{
  int16_t acc[3] = {0};
  int16_t gyro[3] = {0};
  int16_t mag[3] = {0};

  obd.memsRead(acc, gyro, mag);
//  
//  Serial.print("ACC:");
//  Serial.print(acc[0]);
//  Serial.print('/');
//  Serial.print(acc[1]);
//  Serial.print('/');
//  Serial.print(acc[2]);
//
//  Serial.print(" GYRO:");
//  Serial.print(gyro[0]);
//  Serial.print('/');
//  Serial.print(gyro[1]);
//  Serial.print('/');
//  Serial.print(gyro[2]);
//
//  Serial.print(" MAG:");
//  Serial.print(mag[0]);
//  Serial.print('/');
//  Serial.print(mag[1]);
//  Serial.print('/');
//  Serial.print(mag[2]);
//
//  Serial.println();

  
 
   
    
//
//  delay(100);
}

void location(){
  unsigned long age, date, time, chars = 0;
  float flat, flon;
  myGLCD.setFont(SmallFont); 
  gps.f_get_position(&flat, &flon, &age);
  myGLCD.printNumF(flat, 5,30 + (0 * 60), 75);
  myGLCD.printNumF(flon, 5,90 + (1 * 60), 75);
  delay(100);
 myGLCD.setFont(BigFont); 
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
