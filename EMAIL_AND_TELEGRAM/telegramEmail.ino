
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* googleHost = "script.google.com";
const char* telegramIp = "149.154.167.220";
const int httpsPort = 443;
String GOOGLE_SCRIPT_ID = "";
const char* telegramToken = "";
const char* telegramChatId = "";
const char* ssid = "";
const char* password = "";
//https://api.telegram.org/bot/getUpdates


WiFiClientSecure client;
void setup() {
	Serial.begin(921600);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	Serial.println("Started");
	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		
	}
	
	sendToTelegram("Telegram hello from esp");
	sendEmail("Hello, from esp");
}

void loop() {
  
}

String sendEmail(String text)
{
	text = urlencode(text);

	String params = "/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?text=" + text;
	String result = "";
	client.setInsecure();
	Serial.print("connecting to ");
	Serial.println(googleHost);
	if (!client.connect(googleHost, httpsPort)) {
		Serial.println("connection failed");
		return "";
	}

	Serial.print("requesting URL: ");
	client.print(String("GET ") + params + " HTTP/1.1\r\n" +
		"Host: " + googleHost + "\r\n" +
		"Connection: close\r\n\r\n");

	Serial.println("request sent");
	while (client.connected()) {

		String line = client.readStringUntil('\n');
		Serial.println(line);

		if (line == "\r") {
			Serial.println("headers received");
			break;
		}
	}
	while (client.available()) {
		String line = client.readStringUntil('\n');
		Serial.println(line);
	}

	return result;
}


String sendToTelegram(String text) {
	String params = "/bot" + String(telegramToken) + "/sendMessage?chat_id=" + telegramChatId + "&text="+urlencode(text);
	String result = "";
	client.setInsecure();
	Serial.print("connecting to ");
	if (!client.connect(telegramIp, httpsPort)) {
		Serial.println("connection failed");
		return "";
	}
	Serial.print("requesting URL: ");
	Serial.println(params);

	client.print(String("GET ") + params + " HTTP/1.1\r\n" +
		"Host: api.telegram.org\r\n" +
		"Connection: close\r\n\r\n");

	Serial.println("request sent");
	while (client.connected()) {

		String line = client.readStringUntil('\n');
		Serial.println(line);
		
		if (line == "\r") {
			Serial.println("headers received");
			break;
		}
	}
	while (client.available()) {
		String line = client.readStringUntil('\n');
		Serial.println(line);
	}

	return result;

}

String urlencode(String str)
{
	String encodedString = "";
	char c;
	char code0;
	char code1;
	char code2;
	for (int i = 0; i < str.length(); i++) {
		c = str.charAt(i);
		if (c == ' ') {
			encodedString += '+';
		}
		else if (isalnum(c)) {
			encodedString += c;
		}
		else {
			code1 = (c & 0xf) + '0';
			if ((c & 0xf) > 9) {
				code1 = (c & 0xf) - 10 + 'A';
			}
			c = (c >> 4) & 0xf;
			code0 = c + '0';
			if (c > 9) {
				code0 = c - 10 + 'A';
			}
			code2 = '\0';
			encodedString += '%';
			encodedString += code0;
			encodedString += code1;
			//encodedString+=code2;
		}
		yield();
	}
	return encodedString;

}






