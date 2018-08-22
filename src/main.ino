#include <SPI.h>

const int slaveSelect= 10; //pin used to enable the active slave

const int numberofDigits= 2;

const int maxCount= 99;

int number=0;

const int dataIN = 7; //IR sensor INPUT

unsigned long prevmillis; // To store time
unsigned long duration; // To store time difference
unsigned long refresh; // To store time for refresh of reading

int rpm; // RPM value

boolean currentstate; // Current state of IR input scan
boolean prevstate; // State of IR sensor in previous scan

void setup()
{
Serial.begin(9600);
SPI.begin(); //initialize SPI
pinMode(slaveSelect, OUTPUT);
digitalWrite(slaveSelect, LOW); //select Slave

//prepare the 7219 to display 7-segment data
sendCommand (12,1); //normal mode (default is shutdown mode)
sendCommand (15,0); //Display test off
sendCommand (10,8); //set medium intensity (range is 0-15)
sendCommand (11, numberofDigits); //7219 digit scan limit command
sendCommand (9, 255); //decode command, use standard 7-segment digits
digitalWrite(slaveSelect, HIGH); //deselect slave

// sensors
prevmillis = 0;
prevstate = LOW;
pinMode(dataIN, INPUT); // sensor 1
}

void loop()
{
 // RPM Measurement
  currentstate = digitalRead(dataIN); // Read IR sensor state
 if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == HIGH ) // If input only changes from LOW to HIGH
       {
         duration = ( micros() - prevmillis ); // Time difference between revolution in microsecond
         rpm = (60000000/duration); // rpm = (1/ time millis)*1000*1000*60;
         prevmillis = micros(); // store time for nect revolution calculation
       }
   }
  prevstate = currentstate; // store this scan (prev scan) data for next scan
  
  // LCD Display
  if( ( millis()-refresh ) >= 100 )
    {
       Serial.println(rpm);
    }

}

//function to display up to 4 digits on a 7-segment display
void displayNumber (int number)
{
for (int i=0; i < numberofDigits; i++)
{
byte character= number % 10; //get the value of the rightmost digit
if (number == 0 && i > 0)
character = 0xf;
sendCommand(numberofDigits-i, character);
number= number/10;
}
}

void sendCommand(int command, int value)
{
digitalWrite(slaveSelect, LOW); //chip select is active low
SPI.transfer(command);
SPI.transfer(value);
digitalWrite(slaveSelect,HIGH);
}