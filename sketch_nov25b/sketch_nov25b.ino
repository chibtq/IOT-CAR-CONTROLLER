
#include <NewPing.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>

// Khai báo các chân điều khiển cho động cơ thứ nhất
#define IN1 12
#define IN2 14
#define ENA 13  // PWM cho động cơ thứ nhất

// Khai báo các chân điều khiển cho động cơ thứ hai
#define IN3 26
#define IN4 25
#define ENB 27  // PWM cho động cơ thứ hai

// const char* ssid ="crvt4722";     
// const char* password ="00000000";  

const char* ssid ="nguyendong";     
const char* password ="12345678";  

// Thinkspeak
String UrlThingspeak = "http://api.thingspeak.com/update?api_key=1IGDENIQUHLMEXVZ";
String httpGETRequest(const char* Url);

//MQTT
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_USER "dongnd"
#define MQTT_PASSWORD "dongnd"
WiFiClient wifiClient;
PubSubClient client(wifiClient);
//END_MQTT

int cnt_to_cloud = 1;

NewPing sonar(22, 23); 

// const int ENA = 13;
// const int IN1 = 12;
// const int IN2 = 14;
// const int ENB = 27;
// const int IN3 = 26;
// const int IN4 = 25;

void setup()
{
  Serial.begin(115200);     // giao tiếp Serial với baudrate 115200

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  //***********************setup wifi
  WiFi.begin(ssid,password);
  Serial.println("conecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  //END_wifi

  //MQTT
  client.setServer(MQTT_SERVER, MQTT_PORT );
  client.setCallback(callback);
  connect_to_broker();
}

void callback(char* topic, byte *payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: ");
  Serial.write(payload, length);
  Serial.println();
  
  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if(message == "forward") dithang();
  else if (message == "left") retrai();
  else if (message == "right") rephai();
  else if (message == "stop") stopMotors();
  else if (message == "backward") lui();
  else if (message == "rotate") xoay();
}

void loop()
{

  client.loop();
  if (!client.connected()) {
    connect_to_broker();
  }

   int distance = sonar.ping_cm();
   Serial.print(distance);
   Serial.println(" cm");
  
  if (distance > 1 && distance < 20) luikhigapvancan();

  int count_object_check = distance < 20? 1:0;
  if(cnt_to_cloud %2 ==0){
    String url = UrlThingspeak + "&field1=" + String(distance) +  "&field2=" + String(count_object_check);
    HTTPClient http;
    http.begin(url);  // Specify the URL
    int httpCode = http.GET();  // Make the GET request
    
    // Check for a successful response
    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    if (cnt_to_cloud > 100000) cnt_to_cloud = 1;
    http.end();  
  }

  cnt_to_cloud++;
  

  delay(700);
}



void connect_to_broker() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe("iot_crvt4722");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}


void dithang() {
  // Quay theo chiều kim đồng hồ
  // myServo.write(90);
  analogWrite(ENA, 230);  // Giá trị PWM từ 0 đến 255
  analogWrite(ENB, 230);  // Giá trị PWM từ 0 đến 255
  Serial.println("Đi thẳng");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void retrai() {
  // Rẽ trái
  // myServo.write(135);
  analogWrite(ENA, 250);  // Giá trị PWM từ 0 đến 255
  analogWrite(ENB, 0);  // Giá trị PWM từ 0 đến 255
  Serial.println("Rẽ trái");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(2000);  // Đợi 1 giây, bạn có thể điều chỉnh thời gian để rẽ chính xác 45 độ
  stopMotors();


}
void rephai() {
   // Rẽ phải
  // myServo.write(45);
  analogWrite(ENA, 0);  // Giá trị PWM từ 0 đến 255
  analogWrite(ENB, 250);  // Giá trị PWM từ 0 đến 255
  Serial.println("Rẽ phải");
  // digitalWrite(IN1, HIGH);
  digitalWrite(IN1, LOW);
  // digitalWrite(IN2, LOW);
  digitalWrite(IN2, HIGH);
  
  // digitalWrite(IN3, LOW);
  digitalWrite(IN3, HIGH);
  // digitalWrite(IN4, HIGH);
  digitalWrite(IN4, LOW);
  delay(2000);  // Đợi 1 giây, bạn có thể điều chỉnh thời gian để rẽ chính xác 45 độ
  stopMotors();
}

void luikhigapvancan() {
    // Lùi
  // myServo.write(90);
  analogWrite(ENA, 250);  // Giá trị PWM từ 0 đến 255
  analogWrite(ENB, 250);  // Giá trị PWM từ 0 đến 255

  Serial.println("Lùi");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  delay(1000);  // Đợi 1 giây, bạn có thể điều chỉnh thời gian để rẽ chính xác 45 độ
  stopMotors();
}

void lui() {
    // Lùi
  // myServo.write(90);
  analogWrite(ENA, 250);  // Giá trị PWM từ 0 đến 255
  analogWrite(ENB, 250);  // Giá trị PWM từ 0 đến 255

  Serial.println("Lùi");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void xoay() {
   // Xoay vòng
  // myServo.write(180);
  analogWrite(ENA, 0);  // Giá trị PWM từ 0 đến 255
  analogWrite(ENB, 250);  // Giá trị PWM từ 0 đến 255

  Serial.println("Xoay vòng");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}