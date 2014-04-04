/*
  6-8-10
  Aaron Weiss
  SparkFun Electronics
  
  Example GPS Parser based off of arduiniana.org TinyGPS examples.
  
  Parses NMEA sentences from an EM406 running at 4800bps into readable 
  values for latitude, longitude, elevation, date, time, course, and 
  speed. 
  
  For the SparkFun GPS Shield. Make sure the switch is set to DLINE.
  
  Once you get your longitude and latitude you can paste your 
  coordinates from the terminal window into Google Maps. Here is the 
  link for SparkFun's location.  
  http://maps.google.com/maps?q=40.06477,+-105.20997
  
  Uses the NewSoftSerial library for serial communication with your GPS, 
  so connect your GPS TX and RX pin to any digital pin on the Arduino, 
  just be sure to define which pins you are using on the Arduino to 
  communicate with the GPS module. 
  
  REVISIONS:
  1-17-11 
    changed values to RXPIN = 2 and TXPIN = to correspond with
    hardware v14+. Hardware v13 used RXPIN = 3 and TXPIN = 2.
  
*/ 

// In order for this sketch to work, you will need to download 
// TinyGPS library from arduiniana.org and put them 
// into the hardware->libraries folder in your ardiuno directory.
//#include <SoftwareSerial.h>
#include <TinyGPS.h>

//for SD card logging
#include <SPI.h>
#include <SD.h>

#include <iostream>
#include <fstream>
#include <iomanip>    // Needed for stream modifiers fixed and setprecision

// Define which pins you will use on the Arduino to communicate with your 
// GPS. In this case, the GPS module's TX pin will connect to the 
// Arduino's RXPIN which is pin 3.
//#define RXPIN 10
//#define TXPIN 11
//Set this value equal to the baud rate of your GPS
//#define GPSBAUD 9600

const int chipSelect = 10;

File dataFile;


// Create an instance of the TinyGPS object
TinyGPS gps;
// Initialize the NewSoftSerial library to the pins you defined above
//SoftwareSerial uart_gps(RXPIN, TXPIN);

// This is where you declare prototypes for the functions that will be 
// using the TinyGPS library.
void getgps(TinyGPS &gps);

// In the setup function, you need to initialize two serial ports; the 
// standard hardware serial port (Serial()) to communicate with your 
// terminal program an another serial port (NewSoftSerial()) for your 
// GPS.
void setup()
{
  // This is the serial rate for your terminal program. It must be this 
  // fast because we need to print everything before a new sentence 
  // comes in. If you slow it down, the messages might not be valid and 
  // you will likely get checksum errors.
  Serial.begin(38400);
  //Sets baud rate of your GPS
  //uart_gps.begin(GPSBAUD);
  Serial1.begin(9600);
  
  Serial.println("");
  Serial.println("GPS Shield QuickStart Example Sketch v12");
  Serial.println("       ...waiting for lock...           ");
  Serial.println("");
  Serial.println("Satellites      hdop     Latitude     Longitude     Age      Date           Time         Alt      Course     Speed     Checksum");
  Serial.println("                          (deg)         (deg)                                            (m)     (degrees)   (kmh)       Fail");
  Serial.println("--------------------------------------------------------------------------------------------------------------------------------");

   // Open serial communications and wait for port to open:
  //Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

   Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(10, 11, 12, 13)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening datalog.txt");
    // Wait forever since we cant write data
    while (1) ;
  }

}
    

// This is the main loop of the code. All it does is check for data on 
// the RX pin of the ardiuno, makes sure the data is valid NMEA sentences, 
// then jumps to the getgps() function.
void loop()
{
  while(Serial1.available())     // While there is data on the RX pin...
  {
      int c = Serial1.read();    // load the data into a variable...
      if(gps.encode(c))      // if there is a new valid sentence...
      {
        getgps(gps);         // then grab the data.
      }
  }

}

// The getgps function will get and print the values we want.
void getgps(TinyGPS &gps)
{
  // To get all of the data into varialbes that you can use in your code, 
  // all you need to do is define variables and query the object for the 
  // data. To see the complete list of functions see keywords.txt file in 
  // the TinyGPS and NewSoftSerial libs.
  
  // Define the variables that will be used
  float latitude, longitude;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
  /*Serial.print("Lat/Long: "); 
  Serial.print(latitude,5); 
  Serial.print(", "); 
  Serial.println(longitude,5);
  
  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  // Print data and time
  Serial.print("Date: "); Serial.print(month, DEC); Serial.print("/"); 
  Serial.print(day, DEC); Serial.print("/"); Serial.print(year);
  Serial.print("  Time: "); Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
  Serial.print("."); Serial.println(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr


  
  // Here you can print the altitude and course values directly since 
  // there is only one value for the function
  Serial.print("Altitude (meters): "); Serial.println(gps.f_altitude());  
  // Same goes for course
  Serial.print("Course (degrees): "); Serial.println(gps.f_course()); 
  // And same goes for speed
  Serial.print("Speed(kmph): "); Serial.println(gps.f_speed_kmph());
  Serial.println();
    
  // Here you can print statistics on the sentences.
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  gps.stats(&chars, &sentences, &failed_checksum);
  Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
  Serial.println(); Serial.println();*/

  //reformatted code
  
   // Same goes for date and time
  int year;
  unsigned long age;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths,&age);
 
  Serial.print(gps.satellites()); Serial.print("     "); 
  Serial.print(gps.hdop()); Serial.print("     "); 
  Serial.print(latitude,5);  Serial.print("     "); 
  Serial.print(longitude,5); Serial.print("     "); 
  Serial.print(age, DEC);Serial.print("     ");

 
  // Print data and time
  Serial.print(month, DEC); Serial.print("/"); 
  Serial.print(day, DEC); Serial.print("/"); Serial.print(year); Serial.print("     "); 
  Serial.print(hour, DEC); Serial.print(":"); 
  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC); 
  Serial.print("."); Serial.print(hundredths, DEC); Serial.print("     "); 
  //Since month, day, hour, minute, second, and hundr

   
  // Here you can print the altitude and course values directly since 
  // there is only one value for the function
  Serial.print(gps.f_altitude()); Serial.print("     ");   
  // Same goes for course
  Serial.print(gps.f_course());  Serial.print("     "); 
  // And same goes for speed
  Serial.print(gps.f_speed_kmph()); Serial.print("          "); 
 
   // Here you can print statistics on the sentences.
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  gps.stats(&chars, &sentences, &failed_checksum);
  Serial.print(failed_checksum);
  Serial.println(); 

  //write to data file
  // make a string for assembling the data to log:
  char buffer[11];
  String dataString = "";

  dataString+= dtostrf(gps.satellites(),4,5 ,buffer) ;dataString+= "      ";
  dataString+= dtostrf(gps.hdop(),4,5 ,buffer) ;dataString+= "      ";
  dataString+= dtostrf(latitude,4,5 ,buffer) ;dataString+= "      ";
  dataString+= dtostrf(longitude,4,5,buffer) ;dataString+= "      ";
  dataString+= String(age);dataString+= "      ";
  dataString+= String(month);dataString+= "/";
  dataString+= String(day);dataString+= "/";
  dataString+= String(year);dataString+= "      ";
  dataString+= String(hour);dataString+= ":";
  dataString+= String(minute);dataString+= ":";
  dataString+= String(second);dataString+= ".";
  dataString+= String(hundredths);dataString+= "      ";
  dataString+= dtostrf(gps.f_altitude(),4,5,buffer) ;dataString+= "      ";
  dataString+= dtostrf(gps.f_course(),4,5,buffer) ;dataString+= "      ";
  dataString+= dtostrf(gps.f_speed_kmph(),4,5,buffer) ;dataString+= "      ";

 



  //dataString+=(longitude,5 ,latitude,5 , month,DEC,day  ,  hour,DEC,minute,DEC, second, DEC, hundredths, DEC,gps.f_altitude, gps.f_course,gps.f_speed_kmph, failed_checksum  );
   

  /* read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ","; 
    }
  }*/

  dataFile.println(dataString);

  // print to the serial port too:
  Serial.println(dataString);
  
  // The following line will 'save' the file to the SD card after every
  // line of data - this will use more power and slow down how much data
  // you can read but it's safer! 
  // If you want to speed up the system, remove the call to flush() and it
  // will save the file only every 512 bytes - every time a sector on the 
  // SD card is filled with data.
  dataFile.flush();
  
  // Take 1 measurement every 500 milliseconds
  delay(500);

}
