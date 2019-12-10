
//#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
//#include <Wire.h>
#define _cs   15  // goes to TFT CS
#define _dc   2  // goes to TFT DC
#define _rst  5   // goes to TFT RESET
Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _rst);

int nemaDriverPin = 14;
int minDelay = 140;
int maxDelay = 900;
int currentDelayForDriver = 1200;
hw_timer_t* timer = NULL;
uint32_t millisTimer = 0;
int direction = -1;
bool nemaHighLow = false;
int nextStop = 600;
uint32_t stepsCounter = 0;
int delayDivider = 2;


void IRAM_ATTR onTimer() {
	
	digitalWrite(nemaDriverPin, nemaHighLow);
	nemaHighLow = !nemaHighLow;
	if (stepsCounter++ % delayDivider != 0)
		return;
	if ((direction < 0 && currentDelayForDriver >= nextStop) || (direction > 0 && currentDelayForDriver <= nextStop)) {

		currentDelayForDriver += direction;
		timerAlarmWrite(timer, currentDelayForDriver, true);
		return;
	}

		 if (millisTimer > 0)
		{
			if (millisTimer < millis())
			{

				direction = direction * (-1);
				if (direction < 0)
				{
					nextStop =minDelay+random(10);

				}
				else nextStop = maxDelay;
			millisTimer = 0;
			}
		}
		else {
			 if (direction < 0)
			 {
				 delayDivider = random(5, 10);
			millisTimer = millis() + random(3000, 5000);
			}
			 else {

			    delayDivider = random(1, 2);
				millisTimer = millis() + random(500, 2000);
			 }
		}
	}


	void startNema()
	{
		// Use 1st timer of 4 (counted from zero).
		// Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
		// info).
		timer = timerBegin(0, 80, true);

		// Attach onTimer function to our timer.
		timerAttachInterrupt(timer, &onTimer, true);

		// Set alarm to call onTimer function every second (value in microseconds).
		// Repeat the alarm (third parameter)
		timerAlarmWrite(timer, currentDelayForDriver, true);

		// Start an alarm
		timerAlarmEnable(timer);
	}
	void setup() {
		pinMode(nemaDriverPin, OUTPUT);
		digitalWrite(nemaDriverPin, LOW);
		Serial.begin(115200);
		startNema();
		tft.begin(); tft.setRotation(3); tft.fillScreen(ILI9341_BLACK); tft.setTextSize(7);
		delay(100);
		tft.fillScreen(ILI9341_BLACK);
	}

	int lastDriverDelay = 0;
	void loop() {

		if (lastDriverDelay != currentDelayForDriver)
		{
		lastDriverDelay = currentDelayForDriver;
		tft.fillScreen(ILI9341_BLACK);
		tft.setCursor(70, 70);
		tft.println(1000000/currentDelayForDriver/200);
		}
		delay(500);
	
	}



