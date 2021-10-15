/*
 * @date		2021 10/06
 * @code name	Juggernaut#000 - slide
 * @author		Takana Norimasa <Alignof@outlook.com>
 * @brief		Educational bomb disposal game
 * @repository	https://github.com/Alignof/Juggernaut
 */ 

#include <Wire.h>
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define SER      27
#define RCLK     26
#define SRCLK    25
#define BUZZER   12
#define SYSSW    36
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
    while(digitalRead(SYSSW) == HIGH);
    digitalWrite(BUZZER, LOW);
    while(1) delay(1e5);
}

//=============================================================================
//  START of giver code (copy the below code you wrote into the specification)
//=============================================================================

int time_limit = 900;
uint8_t acce_data[6];
const uint8_t DEVICE_ADDRESS = 0x1D;

// giver pin assgin
const uint8_t mySCL  = 22;
const uint8_t mySDA  = 23;
const uint8_t trans_l = 16;
const uint8_t trans_r = 17;
const uint8_t slide_sw = 15;
const uint8_t gray_button = 2;
const uint8_t flat_button = 4;
const uint8_t black_button = 21;

void setup_pin(void) {
	pinMode(mySCL, INPUT_PULLUP);
	pinMode(mySDA, INPUT_PULLUP);
	pinMode(slide_sw, INPUT_PULLUP);
	pinMode(gray_button, INPUT_PULLUP);
	pinMode(flat_button, INPUT_PULLUP);
	pinMode(black_button, INPUT_PULLUP);
	pinMode(trans_r, OUTPUT);
	pinMode(trans_l, OUTPUT);
    Serial.begin(115200);

    Wire.begin(mySDA, mySCL);
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(0x31);
	Wire.write(0x0B);
	Wire.endTransmission();
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(0x2d);
	Wire.write(0x08);
	Wire.endTransmission();
}

void getAccelerationData(void) {
	Wire.beginTransmission(DEVICE_ADDRESS);
	Wire.write(0x32);
	Wire.endTransmission();

	Wire.requestFrom(DEVICE_ADDRESS, 6);

	for(int i = 0; i < 6; i++) {
		delay(1);
		acce_data[i] = Wire.read();
	}
}

void gaming(void *pvParameters) {
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	bool flag4 = false;
    double az = 0;

	while(1) {
        if (digitalRead(slide_sw) == HIGH) {
            digitalWrite(trans_l, LOW);
            digitalWrite(trans_r, HIGH);

            if (digitalRead(flat_button) == LOW) {
                flag1 = true;
            }
        } else {
            digitalWrite(trans_r, LOW);
            digitalWrite(trans_l, HIGH);

            if (flag1 && digitalRead(black_button) == LOW) {
                flag2 = true;
            }

            if (flag1 && flag2 && digitalRead(black_button) == HIGH &&
                digitalRead(gray_button) == LOW) {
                flag3 = true;
            }
        }

        getAccelerationData();
        az = (int16_t)((acce_data[5] << 8) | acce_data[4]) * 0.0392266;
        flag4 = az > 12.0;
        Serial.printf("az: %g\n", az);

		// succeeded
		if(flag3) {
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

        if(digitalRead(SYSSW) == LOW) {
            digitalWrite(BUZZER, LOW);
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
	pinMode(SYSSW,  INPUT);

    setup_pin();

    while(digitalRead(SYSSW) == HIGH);

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

