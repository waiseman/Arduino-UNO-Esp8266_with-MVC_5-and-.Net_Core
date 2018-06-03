#include "SoftwareSerial.h"
#include <DHT22.h>
String ssid ="your ssid here";

String password="your wifi password here";

SoftwareSerial esp(2, 3);// RX, TX

String data;

String server = " www.example.com"; //url to access hosting server

String uri = "/home/read?";// path to MVC controller

#define DHT22_PIN 7
DHT22 myDHT22(DHT22_PIN);
String temp="0" ,hum="0";

void setup() {

//pinMode (DHpin, OUTPUT);

esp.begin(115200);

Serial.begin(115200);

reset();

connectWifi();

}

//reset the esp8266 module

void reset() {

esp.println("AT+RST");

delay(1000);

if(esp.find("OK") ) Serial.println("Module Reset");

}

//connect to your wifi network

void connectWifi() {

String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";

esp.println(cmd);

delay(4000);

if(esp.find("OK")) {

Serial.println("Connected!");

}

else {



Serial.println("Cannot connect to wifi"); }

}



void loop () {

DHT22_ERROR_t errorCode;
   // The sensor can only be read from every 1-2s, and requires a minimum
   // 2s warm-up after power-on.
delay(2000);
int Moist= analogRead(A0);
int Light= analogRead(A1);
errorCode = myDHT22.readData();
   switch(errorCode)
   {
    case DHT_ERROR_NONE:
   temp =(String)myDHT22.getTemperatureC();
    hum=(String)myDHT22.getHumidity();
    data = "temp="+temp+"&hum="+hum+"&light="+Light+"&moist="+Moist;// data sent must be under this form //name1=value1&name2=value2.

  httppost();
   }

}

void httppost () {

esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.

if( esp.find("OK")) {

Serial.println("TCP connection ready");

} delay(1000);

String postRequest =

"POST " + uri + " HTTP/1.0\r\n" +

"Host: " + server + "\r\n" +

"Accept: *" + "/" + "*\r\n" +

"Content-Length: " + data.length() + "\r\n" +

"Content-Type: application/x-www-form-urlencoded\r\n" +

"\r\n" + data;

String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

esp.print(sendCmd);

esp.println(postRequest.length() );

delay(500);

if(esp.find(">")) { Serial.println("Sending.."); esp.print(postRequest);

if( esp.find("SEND OK")) { Serial.println("Packet sent");

// close the connection

esp.println("AT+CIPCLOSE");
//delay(1000000);

}
else
{
   Serial.println("Packet Not sent");

// close the connection

esp.println("AT+CIPCLOSE");
  }

}}
