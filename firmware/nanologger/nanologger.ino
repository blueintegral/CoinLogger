#include <TinyWireM.h>
#include <USI_TWI_Master.h>

//I2C stuff
#define DS1621_ADDR   0x48              // 7 bit I2C address for DS1621 temperature sensor





// tinySPI
// Written by Nick Gammon
// March 2013

// ATMEL ATTINY45 / ARDUINO pin mappings
//
//                         +-\/-+
// RESET  Ain0 (D 5) PB5  1|    |8  Vcc
// CLK1   Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1  SCK  / USCK / SCL
// CLK0   Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1  MISO / DO
//                   GND  4|    |5  PB0 (D 0) pwm0  MOSI / DI / SDA
//                         +----+

namespace tinySPI 
  {

  const byte DI   = 0;  // D0, pin 5  Data In
  const byte DO   = 1;  // D1, pin 6  Data Out (this is *not* MOSI)
  const byte USCK = 2;  // D2, pin 7  Universal Serial Interface clock
  const byte SS   = 3;  // D3, pin 2  Slave Select
  
  void begin ()
    {
    digitalWrite (SS, HIGH);  // ensure SS stays high until needed
    pinMode (USCK, OUTPUT);
    pinMode (DO,   OUTPUT);
    pinMode (SS,   OUTPUT);
    USICR = _BV (USIWM0);  // 3-wire mode
    }  // end of tinySPI_begin
    
  // What is happening here is that the loop executes 16 times.
  // This is because the 4-bit counter in USISR is initially zero, and then
  // toggles 16 times until it overflows, thus counting out 8 bits (16 toggles).
  // The data is valid on the clock leading edge (equivalent to CPHA == 0).
  
  byte transfer (const byte b)
    {
    USIDR = b;  // byte to output
    USISR = _BV (USIOIF);  // clear Counter Overflow Interrupt Flag, set count to zero 
    do
      {
      USICR = _BV (USIWM0)   // 3-wire mode
            | _BV (USICS1) | _BV (USICLK)  // Software clock strobe
            | _BV (USITC);   // Toggle Clock Port Pin
      } while ((USISR & _BV (USIOIF)) == 0);  // until Counter Overflow Interrupt Flag set
      
    return USIDR;  // return read data
    }    // end of tinySPI_transfer

  };  // end of namespace tinySPI 
  
  
  
const byte LATCH_PIN = 4;  //D4, pin 3   
const byte ENABLE_PIN = 3; //D3, pin 2 ALSO Slave Select



int command_mode = 1;

void sendPacket(int, int, int);



void Init_Accel(){ // Setup the DS1621 for one-shot mode
  TinyWireM.beginTransmission(DS1621_ADDR);
  TinyWireM.send(0xAC);                 // Access Command Register
  TinyWireM.send(B00000001);            // Using one-shot mode for battery savings
  //TinyWireM.send(B00000000);          // if setting continious mode for fast reads
  TinyWireM.endTransmission();          // Send to the slave
}


void Get_Accel(){  // Get the temperature from a DS1621
  TinyWireM.beginTransmission(DS1621_ADDR);
  TinyWireM.send(0xEE);                 // if one-shot, start conversions now
  TinyWireM.endTransmission();          // Send 1 byte to the slave
  delay(750);                           // if one-shot, must wait ~750 ms for conversion
  TinyWireM.beginTransmission(DS1621_ADDR);
  TinyWireM.send(0xAA);                 // read temperature (for either mode)
  TinyWireM.endTransmission();          // Send 1 byte to the slave
  TinyWireM.requestFrom(DS1621_ADDR,1); // Request 1 byte from slave
  tempC = TinyWireM.receive();          // get the temperature
  tempF = tempC * 9 / 5 + 32;           // convert to Fahrenheit 
}

void setup() {
    pinMode(LATCH_PIN, OUTPUT);
    
    tinySPI::begin ();
    
    digitalWrite(LATCH_PIN, LOW);
    digitalWrite(ENABLE_PIN, HIGH);  // Clear any garbage in the registers
    delay(1000);
    digitalWrite(ENABLE_PIN, LOW);
    
    sendPacket(120,100,100);
    command_mode = 0;
    
    TinyWireM.begin();                    // initialize I2C lib
    Init_Accel();                          // Setup accelerometer
    delay (3000);
    
}

void sendPacket(int red_val, int green_val, int blue_val)
{
      
    tinySPI::transfer(command_mode << 6 | blue_val >> 4);
    tinySPI::transfer(blue_val << 4 | red_val >> 6);
    tinySPI::transfer(red_val << 2 | green_val >> 8);
    tinySPI::transfer(green_val);    
    
    delayMicroseconds(15);
    digitalWrite(LATCH_PIN, HIGH);
    delayMicroseconds(15);
    digitalWrite(LATCH_PIN, LOW);
    
    
}


void loop() {
    
   
        sendPacket(current_red, current_green, current_blue);        
        delay(step_time);
    }
    
    
}
