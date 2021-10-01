#include <Wire.h>
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define SER      27
#define RCLK     26
#define SRCLK    25
#define BUZZER   12
#define DATASIZE 16

typedef enum {
	RED,
	YELLOW,
	GREEN,
} SIGNAL;

bool timer_stop = false;
SIGNAL signal = YELLOW;
EventGroupHandle_t eg_handle;

void succeeded(void) {
    signal     = GREEN;
    timer_stop = true;
    while(1) delay(1e5);
}

void failed(void) {
    signal = RED;
    timer_stop = true;
    digitalWrite(BUZZER, HIGH);
    delay(5000);
    digitalWrite(BUZZER, LOW);
    while(1) delay(1e5);
}

//=============================================================================
//  START of giver code (copy the below code you wrote into the specification)
//=============================================================================

typedef enum {
    LED_R = 1,
    LED_G,
    LED_B,
} Color;

int time_limit = 300;
const uint8_t threshold = 18;

// giver pin assgin
const uint8_t touch0 = 4;
const uint8_t touch1 = 13;
const uint8_t touch2 = 2;
const uint8_t touch3 = 15;
const uint8_t touch4 = 14;
const uint8_t touch5 = 33;
//const uint8_t touch6 = 32;
//const uint8_t touch7 = 0;
const uint8_t led_red   = 21;
const uint8_t led_green = 22;
const uint8_t led_blue  = 23;

void gaming(void *pvParameters) {
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;
    bool flag4 = false;
    uint8_t red = 170;
    uint8_t green = 100;
    uint8_t blue = 30;

	while(1) {
        red     += touchRead(touch0) < threshold ? 10 : 0;
        red     -= touchRead(touch1) < threshold ? 10 : 0;
        green   += touchRead(touch2) < threshold ? 10 : 0;
        green   -= touchRead(touch3) < threshold ? 10 : 0;
        blue    += touchRead(touch4) < threshold ? 10 : 0;
        blue    -= touchRead(touch5) < threshold ? 10 : 0;
        delay(200);

        ledcWrite(LED_R, 255 - red);
        ledcWrite(LED_G, 255 - green);
        ledcWrite(LED_B, 255 - blue);

        Serial.printf("red:%d, green:%d, blue:%d\n", red, green, blue);

        flag1 = green > 200;
        flag2 = red < 20;
        flag3 = blue < 10;
        flag4 = red > 230;
		
		// succeeded
		if(flag1 && flag2 && flag3) {
            succeeded();
		}

		// failed
		if(flag4) {
            failed();
		}
	}
}

//=============================================================================
//  END of giver code
//=============================================================================

void display(void *pvParameters) {
	int ms;
	long long i,j;
	long long start;
	long long minits,second;

	start  = millis();
	second = time_limit%60;
	minits = time_limit/60;

	while(time_limit > 0) {
		if(!(timer_stop)) {
			ms = millis()-start;
			if(ms>1000) {
				time_limit--;
				minits = time_limit/60;
				second = time_limit%60;
				start  = millis();
				digitalWrite(BUZZER, HIGH);
			}else{
				digitalWrite(BUZZER, LOW);
			}
		}

		data_send(4, (minits/10)%10, signal);
		data_send(3, minits%10,	     signal);
		data_send(5, 10, signal);
		data_send(2, (second/10)%10, signal);
		data_send(1, second%10,      signal);
	}

	// time over
	signal = RED;
	digitalWrite(BUZZER, HIGH);
	while(1) {
		data_send(5, 10, signal);
		for(int i=1;i<=4;i++) {
			data_send(i, 0, signal);
		}
	}
}

void loop() { delay(1e5); }

void setup() {
	eg_handle=xEventGroupCreate();

	pinMode(SER,    OUTPUT);
	pinMode(RCLK,   OUTPUT);
	pinMode(SRCLK,  OUTPUT);
	pinMode(BUZZER, OUTPUT);

//==== declared by giver ======================================================
    Serial.begin(115200);
    
    ledcAttachPin(led_red,  LED_R);
    ledcAttachPin(led_green,LED_G);
    ledcAttachPin(led_blue, LED_B);
    ledcSetup(LED_R, 12000, 8);
    ledcSetup(LED_G, 12000, 8);
    ledcSetup(LED_B, 12000, 8);
//=============================================================================

	xTaskCreatePinnedToCore(gaming,  "gaming",  8192, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(display, "display", 8192, NULL, 1, NULL, 1);
	delay(100);
}

void data_send(int digit, int num, SIGNAL rgb) {
	int i;
	uint16_t data;
	int seg[11] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x03};

	/*
	 *                QH <--------- QA
	 * register Left  :a,b,c,d,e,f,g,#,
	 * register Right :                R,Y,G,4,3,2,1,DP 
	 */
	data = (1<<(digit+10))+(1<<(rgb+8))+(seg[num]);

	digitalWrite(RCLK, LOW);
	for(i = 0;i < DATASIZE;i++) {
		digitalWrite(SER, (data>>i)&1);
		digitalWrite(SRCLK, LOW);
		digitalWrite(SRCLK, HIGH);
	}
	digitalWrite(RCLK, HIGH);
    delay(1);
}

