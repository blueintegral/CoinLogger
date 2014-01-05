/*
 * coinlogger_test.c
 *
 * Created: 1/1/2014 8:51:49 PM
 *  Author: hunter
 * This program tests all the functions of CoinLogger. It turns the LED on for 2 seconds, then reads and write to the accelerometer.
 *  If it works, then the LED is turned on for another 2 seconds. If not, it stays off. It then reads and writes to the EEPROM, and
 *  if that works, it turns the LED on and leaves it on. Otherwise, it stays off.
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay_basic.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define LED PB3
#define MOSI PB0
#define MISO PB1
#define SCLK PB2
#define CS PB3
#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))
#define SCALE 0.0007324  //sets g-level (48 fullscale range)/(2^16bits) = SCALE

#define USIOIF   6
#define USIWM0   4
#define USICS1   3
#define USICLK   1
#define USITC   0
#undef   DDR_WRITE
#define DDR_WRITE   0xE0

//Prototypes
void delay_ms(uint16_t ms);
unsigned char spi(unsigned char val);
void accel_read(void);
void accel(void);
void transferLong(unsigned int trans);
////////////

//Global Variables
volatile double xaccel, yaccel, zaccel;
///////////

ISR(TIMER1_OVF_vect)
{
	//every ms the acceleration values are printed
	
}

void accel_read(){
	//TCNT1 = 55536;//(8/8MHz)*(65536bits-55536)=0.01s
	accel(); //update LIS registers
	//printf("x=%6.2f,", xaccel);
	//printf("y=%6.2f,", yaccel);
	//printf("z=%6.2f\n\r", zaccel);
	}

void accel(void)
{
	uint16_t xl, xh, yl, yh, zl, zh;
	int16_t tempx, tempy, tempz;
	
	cbi(PORTB, CS);
	spi(0xe8); //read bit | consecutive measure bit | 0x28 = 0xe8
	xl = spi(0x00); //must keep clock moving, so tx another byte
	xh = spi(0x00); //must keep clock moving, so tx another byte
	yl = spi(0x00); //must keep clock moving, so tx another byte
	yh= spi(0x00); //must keep clock moving, so tx another byte
	zl = spi(0x00); //must keep clock moving, so tx another byte
	zh = spi(0x00); //must keep clock moving, so tx another byte
	sbi(PORTB, CS);
	
	tempx = (xl|(xh << 8)); //concatenate low and high bits together, load into signed 16 bit
	xaccel = SCALE*tempx;    //multiply by scale factor for g-level
	tempy = (yl|(yh << 8));
	yaccel = SCALE*tempy;
	tempz = (zl|(zh << 8));
	zaccel = SCALE*tempz;
}

void delay_ms(uint16_t ms){
	while (ms > 0) {
		_delay_loop_2(F_CPU/4000);
		ms--;
	}
}

	
	
unsigned char spi(unsigned char val)
{
	USIDR = val;
	USISR = (1<<USIOIF);
	do {
		USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
	} while ((USISR & (1<<USIOIF)) == 0);
	return USIDR;
}
/*#else
static void shft8bit(unsigned char val)
{
	unsigned char cnt;
	sdp8_data &= (~SDP8_CLOCK & ~SDP8_LOAD);   // CLOCK = LOAD = 0 
	for (cnt = 8; cnt; --cnt, val <<= 1) {
		sdp8_data &= ~SDP8_DATA;
		if (val & 0x80) sdp8_data |= SDP8_DATA;
		sdp8_data |= SDP8_CLOCK;
		sdp8_data &= ~SDP8_CLOCK;
	}
}
#endif
*/

void transferLong(unsigned int trans){
	/* Transfer the first half */
	spi(trans >> 8);
	/* Transfer the second half */
	spi((uint8_t)trans);
}

int main(void) {
	// Set Pin 2 (PB3) as an output pin.
	 DDRB |= _BV(LED);
	 DDRB |= _BV(PB1); /* DO: Data out */
	 DDRB |= _BV(PB2); /* USCK: USI clock */
	 DDRB |= _BV(CS);
	unsigned char testvar = 0b11111111;
	// PORTB |= (1<<CS);     // pull-up i.e. disabled

//BEGIN TEST PROCEDURE
	PORTB |= 1 << LED; //Set LED high for 2 seconds initially.
	delay_ms(2000);
	PORTB &= ~(1 << LED); //Turn it off 
	delay_ms(1000); //Give time to see that it has turned off
	accel_read(); //Get data from accelerometer
	if (xaccel && yaccel && zaccel) PORTB |= 1 << LED; //Set LED high for 2 seconds if we got nonzero data
	delay_ms(2000);
	PORTB &= ~(1 << LED); //Turn it off
	delay_ms(1000); //Give time to see that it has turned off
	

	while(1) {
		spi(testvar--);
		// Set pin 2 high.
		//if(testvar % 10) PORTB |= 1 << LED;
		//delay_ms(100);
		// Set pin 2 low.
		//PORTB &= ~(1 << LED);
		//delay_ms(100);
	}
	return 0;
}