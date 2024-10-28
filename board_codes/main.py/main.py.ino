#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

#include <WiFi.h>
#include "SPIFFS.h"


const String INDEX_HTML = R"V0G0N(
<style>

:root 
  {
  --nik_comp_color:#e02e28;
  --nik_comp_bg_color_red:#180000;

  }


body
  {
  width: 100vw;
  height: 100vh;
  background: linear-gradient(0deg, #1a1a1a, rgb(34, 34, 34));
  }

html
  {
  width: 100vw;
  height: 100vh;
  }


*
  {
  font-family: sans-serif;
  font-size: 3vh;
  border-radius: 2vh;
  font-weight: bold;
  }

input
  {
  display: block;
  background: white;
  color: black;
  width: 80%;
  margin-bottom: 5%;
  padding-top: 2%;
  padding-left: 0;
  padding-right: 0;
  padding-bottom: 2%;
  text-align:center;
  border-color:black;
  border-width: 2px;
  border-style: solid;
  -webkit-appearance: none;
  -moz-appearance: textfield;

  }

input:focus
{
  border-color: var(--nik_comp_color);
  outline-style: none;



}




button
  {
  border-style: none;
  padding: 5%;
  transition: 0.3s;
  background-color: var(--nik_comp_color);
  color: white;
  }

button:hover
  {
  transform: translateY(-20px);
  transition: 0.3s;
  }

button:active
  {
  transform: translateY(+20px); 
  transition: 0.3s;
  }



#main_form
  {
  display: flex;
  flex-direction: column;
  align-items: center;
  }


label
  {
  background: -webkit-linear-gradient(var(--nik_comp_color), var(--nik_comp_color));
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
  }


</style>

<title>HEXA FORM</title>


<form id="main_form"> 
  <label>Field 0</label>
  <input type="number" placeholder="0" name="0" min="0" max="255">

  <label>Field 1</label>
  <input type="number" placeholder="0" name="1" min="0" max="255">

  <label>Field 2</label>
  <input type="number" placeholder="0" name="2" min="0" max="255">

  <label>Field 3</label>
  <input type="number" placeholder="0" name="3" min="0" max="255">

  <label>Field 4</label>
  <input type="number" placeholder="0" name="4" min="0" max="255">

  <label>Field 5</label>
  <input type="number" placeholder="0" name="5" min="0" max="255">

  <button type="submit"> Submit!</button>  
</form>
)V0G0N";







WebServer server(80);





void hh_ui_index()
{
  // استفاده از strtol برای تبدیل رشته هگزادسیمال به عدد
  int hexa0 = strtol(server.arg("0").c_str(), NULL, 16);
  int hexa1 = strtol(server.arg("1").c_str(), NULL, 16);
  int hexa2 = strtol(server.arg("2").c_str(), NULL, 16);
  int hexa3 = strtol(server.arg("3").c_str(), NULL, 16);
  int hexa4 = strtol(server.arg("4").c_str(), NULL, 16);
  int hexa5 = strtol(server.arg("5").c_str(), NULL, 16);

  // ذخیره‌سازی اعداد در فایل
  File file = SPIFFS.open("/data.txt", FILE_WRITE);
  file.println(hexa0);
  file.println(hexa1);
  file.println(hexa2);
  file.println(hexa3);
  file.println(hexa4);
  file.println(hexa5);
  file.close();

  // خواندن داده‌ها از فایل برای نمایش مجدد در فرم
  file = SPIFFS.open("/data.txt", FILE_READ);

  int array_index = 0;
  int data[6] = {0, 0, 0, 0, 0, 0};
  String buffer = "";

  while (file.available())
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

  // آماده‌سازی صفحه HTML با جایگزینی مقادیر هگزادسیمال
  String output = INDEX_HTML;
  output.replace("HEX_VALUE_PLACE_HOLDER_0", "0x" + String(data[0], HEX));
  output.replace("HEX_VALUE_PLACE_HOLDER_1", "0x" + String(data[1], HEX));
  output.replace("HEX_VALUE_PLACE_HOLDER_2", "0x" + String(data[2], HEX));
  output.replace("HEX_VALUE_PLACE_HOLDER_3", "0x" + String(data[3], HEX));
  output.replace("HEX_VALUE_PLACE_HOLDER_4", "0x" + String(data[4], HEX));
  output.replace("HEX_VALUE_PLACE_HOLDER_5", "0x" + String(data[5], HEX));
  output += WiFi.softAPmacAddress();
  // ارسال صفحه HTML به کلاینت
  server.send(200, "text/html", output);
}

void setup()
{
  Serial.begin(9600);

  

  if (!SPIFFS.begin(true))
  {
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  }

  // بررسی وجود فایل data.txt و ایجاد آن در صورت عدم وجود
  File file = SPIFFS.open("/data.txt");
  if (!file)
  {
    Serial.write("file data.txt does not exist; creating new file");
    file = SPIFFS.open("/data.txt", FILE_WRITE);
    for (int i = 0; i < 6; i++)
    {
      file.println(0); // مقدار اولیه صفر برای هر سلول
    }
    file.close();
  }
  else
  {
    Serial.write("file data.txt exists");
    file.close();
  }

  delay(1200);
  const String ssid = String("NIK_GTW_")+String(WiFi.softAPmacAddress()).substring(12);
  WiFi.softAP(ssid, "12345678");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // تنظیم روت برای نمایش فرم
  server.on("/index", hh_ui_index);

  server.begin();
}

void loop()
{
  server.handleClient();
}
