#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

#include <WiFi.h>
#include "SPIFFS.h"

const String INDEX_HTML = R"V0G0N(
<style>
*
	{
	background: black;
	color: wheat;
	font-family: monospace;
	border-radius: 3px;
	}
input
	{
	display: block;
	background: white;
	color: black;
	width: 40vh;
	margin-bottom: 2vh;
	padding: 3vh;
	font-size: 12;
	}
button
	{

	}
</style>
<title>HEXA FORM</title>
<form > 
	<label>CELL 0</label>
	<input type="number" value="HEX_VALUE_PLACE_HOLDER_0" name="0" min="0" max="255">

	<label>CELL 1</label>
	<input type="number" value="HEX_VALUE_PLACE_HOLDER_1" name="1" min="0" max="255">

	<label>CELL 2</label>
	<input type="number" value="HEX_VALUE_PLACE_HOLDER_2" name="2" min="0" max="255">

	<label>CELL 3</label>
	<input type="number" value="HEX_VALUE_PLACE_HOLDER_3" name="3" min="0" max="255">

	<label>CELL 4</label>
	<input type="number" value="HEX_VALUE_PLACE_HOLDER_4" name="4" min="0" max="255">

	<label>CELL 5</label>
	<input type="number" value="HEX_VALUE_PLACE_HOLDER_5" name="5" min="0" max="255">

	<button type="submit"> SUBMIT!</button>	 
</form>
)V0G0N";

const char* ssid = "HEX_FORMA";
const char* password = "1243456789";


WebServer server(80);


void hh_ui_index()
  {
  int hexa0 = server.arg("0").toInt();
  int hexa1 = server.arg("1").toInt();
  int hexa2 = server.arg("2").toInt();
  int hexa3 = server.arg("3").toInt();
  int hexa4 = server.arg("4").toInt();
  int hexa5 = server.arg("5").toInt();

  File file = SPIFFS.open("/data.txt", FILE_WRITE);

  file.println(hexa0);
  file.println(hexa1);
  file.println(hexa2);
  file.println(hexa3);
  file.println(hexa4);
  file.println(hexa5);

  file.close();

  file = SPIFFS.open("/data.txt", FILE_READ);

  int array_index = 0;
  int data[6] = {0,0,0,0,0,0};
  String buffer = "";


  while(file.available())
    {
    char charcode = (file.read());
    if (charcode == 10)
      {
      data[array_index] = buffer.toInt();
      buffer = "";
      array_index += 1;
      }
    buffer += String(charcode);
    }
  
  file.close();

  String output = INDEX_HTML;
  int x = 10;
  

  output.replace("HEX_VALUE_PLACE_HOLDER_0", String(data[0]));
  output.replace("HEX_VALUE_PLACE_HOLDER_1", String(data[1]));
  output.replace("HEX_VALUE_PLACE_HOLDER_2", String(data[2]));
  output.replace("HEX_VALUE_PLACE_HOLDER_3", String(data[3]));
  output.replace("HEX_VALUE_PLACE_HOLDER_4", String(data[4]));
  output.replace("HEX_VALUE_PLACE_HOLDER_5", String(data[5]));
  

  server.send(200, "text/html", output); 
  }

void setup() {
  Serial.begin(9600);

	if (!SPIFFS.begin(true)) 
    {
	  Serial.println("An error has occurred while mounting SPIFFS");
	  return;
	  }
  File file = SPIFFS.open("/data.txt");
  if (file.available())
    {
    Serial.write("file data.txt exists; so NO new file will be created");
    }
  else
    {
    Serial.write("file data.txt does not exist; so new file will be created");  
    file.close();
    file = SPIFFS.open("/data.txt", FILE_WRITE);
    file.println(0); 
    file.println(0);
    file.println(0);
    file.println(0);
    file.println(0);
    file.println(0);
    }
  



  file.close();
  file = SPIFFS.open("/test.txt");

  Serial.write(file.read());
  file.close();

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/index", hh_ui_index);

  server.begin();

}


void loop() 
  {
  server.handleClient();
  }