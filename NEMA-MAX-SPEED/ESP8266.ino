
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"


#define TFT_DC    2
#define TFT_CS    5
#define TFT_Reset 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
int nemaDriverPin = 0;
int  currentDelayForDriver = 5000;
int minDelay = 140*5;
int maxDelay = 900*5;
uint32_t millisTimer = 0;
int direction = -1;
bool nemaHighLow = false;
int nextStop = 600;
uint32_t stepsCounter = 0;
int delayDivider = 2;


void ICACHE_RAM_ATTR onTimerISR() {
	digitalWrite(nemaDriverPin, HIGH);  
	digitalWrite(nemaDriverPin, LOW);

	if (stepsCounter++ % delayDivider != 0)
		return;
	if ((direction < 0 && currentDelayForDriver >= nextStop) || (direction > 0 && currentDelayForDriver <= nextStop)) {
		timer1_write(currentDelayForDriver);
		currentDelayForDriver += direction;
		return;
	}

	if (millisTimer > 0)
	{
		if (millisTimer < millis())
		{

			direction = direction * (-1);
			if (direction < 0)
			{
				nextStop = minDelay + random(50);

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


void setup() {
	pinMode(nemaDriverPin, OUTPUT);
	digitalWrite(nemaDriverPin, LOW);
	Serial.begin(115200);
	timer1_attachInterrupt(onTimerISR);
	timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
	timer1_write(5000); 
	Serial.println("test");
	tft.begin(); tft.setRotation(3); tft.fillScreen(ILI9341_BLACK); tft.setTextSize(7);
	delay(100);
	tft.fillScreen(ILI9341_BLACK);
}

	
int lastDriverDelay = 0;
void loop() {

	int temp = 1000000 * 5 / currentDelayForDriver / 200;
	if (lastDriverDelay != temp)
	{
		lastDriverDelay = temp;
		tft.fillScreen(ILI9341_BLACK);
		tft.setCursor(90, 70);
		tft.println(temp);
	}
	delay(100);


}
