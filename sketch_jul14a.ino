#include <memorysaver.h>
#include <UTFT.h>
#include <SD.h>
#include <SPI.h>

File myFile;

extern uint8_t SmallFont[]; 
extern uint8_t BigFont[];  
extern uint8_t SevenSegNumFont[]; 

UTFT myGLCD(TFT32MEGA_2, 38,39,40,41); 
// Standard Arduino Mega/Due shield : ,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega : ,38,39,40,41
String str;
String str2;
String t_time;
String latitude;
String longitude;
String indicator;
String course; 
String flag; 
String speedknots;
String speedinkm;
String temp;
String sd;

void setup() {
  Serial.begin(9600); //setup serial
  myGLCD.InitLCD(); //setup display
  myGLCD.setBackColor(0, 0, 255); //setup color
  myGLCD.setFont(BigFont); //setup font
  if (SD.begin()) {
    sd="ok";  
  }
}

void loop() {
  str = Serial.readString(); //read string from serial
  if (str.indexOf("GGA") != -1)
  {
     str.remove(0, 7);
     splitting(str);
  }
 
  str2 = Serial.readString();
  if (str.indexOf("VTG") != -1)
  {
      str2.remove(0, 7);
      splitting_VTG(str2);
  }
  printData();
}

void printData()
{
  myGLCD.clrScr(); //clear display
  
  myGLCD.print("time: ", 10, 10); 
  myGLCD.print(t_time, 10, 30); //print time
  
  myGLCD.print("latitude: ", 10, 70);
  myGLCD.print(latitude, 10, 90); //print latitude
  
  myGLCD.print("longitude: ", 10, 125);
  myGLCD.print(longitude, 10, 145); //print longitude
  
  myGLCD.print("course: "+course, 10, 195); //print course
  myGLCD.print("flag: "+flag, 10, 215);   //print flag
  
  myGLCD.print("speedknots: ", 235, 10);
  myGLCD.print(speedknots, 235, 30); //print knots speed
  
  myGLCD.print("speedinkm: ", 235, 70);
  myGLCD.print(speedinkm, 235, 90); //print km/h speed
 
  myGLCD.print("Sd card: "+sd, 255, 130); //print sd status
  
  str="time: "+t_time+" latitude: "+latitude+" longitude: "+longitude+" course: "+course+" speed in knots: "+speedknots+" speed in km/h: "+speedinkm;
  myFile=SD.open("test.txt", FILE_WRITE); 
  myFile.println(str); //write to sd
  myFile.close();
  delay(250);
}

void splitting(String& str)
{
  temp = str.substring(0, str.indexOf(","));
  str.remove(0, str.indexOf(",") + 1);
  t_time = temp.substring(0, 2);
  t_time += ":";
  t_time += temp.substring(2, 4);
  t_time += ":";
  t_time += temp.substring(4, 9);

  temp = str.substring(0, str.indexOf(",") + 2);
  str.remove(0, str.indexOf(",") + 3);
  temp.remove(temp.indexOf("."), 1);
  latitude = temp.substring(0, 2);
  latitude += ".";
  latitude += temp.substring(2, 4);
  latitude += ".";
  latitude += temp.substring(4, 11);

  temp = str.substring(0, str.indexOf(",") + 2);
  str.remove(0, str.indexOf(",") + 3);

  temp.remove(temp.indexOf('.'), 1);
  longitude = temp.substring(0, 3);
  longitude += '.';
  longitude += temp.substring(3, 5);
  longitude += '.';
  longitude += temp.substring(5, 12);
}

void splitting_VTG(String& str)
{
  course = str.substring(0, str.indexOf(","));
  str.remove(0, str.indexOf(",") + 1);

  flag = str.substring(0, str.indexOf(",") + 2);
  str.remove(0, str.indexOf(",") + 4);
 
  speedknots = str.substring(0, str.indexOf(",") + 2);
  
  str.remove(0, str.indexOf(",") + 3);
  speedinkm = str.substring(0, str.indexOf(",") + 2);

  if (course.length() != 0)
  {
    course+=" " + flag;
  }
  else
  {
    course = "no data";
    flag = "no data";
  }
}
