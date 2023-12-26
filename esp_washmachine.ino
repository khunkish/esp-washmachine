/*
ESP Wash Machine Nofity V1.5.0 (Update 12/12/2023)
Code By Khun Kish
*/

#include <TridentTD_LineNotify.h>

#define SSID  "ESP_Code404" //ใส่ SSD Wifi ที่จะเชื่อมต่อ
#define PASSWORD  "P4ssword" //ใส่ Wifi Password
#define LINE_TOKEN  "DqqFeQJxuECRBf9w9Tk3M7mJjgIV4x6kcjgi2JChimh" //Line Token Notify
int val = 0; // ตัวแปรเก็บค่าที่อ่านมาจาก LDR (Light Dependent Resistor)
bool machineRunning = false; // ตัวแปรบอกว่าเครื่องกำลังทำงานหรือไม่

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(LINE.getVersion());
  pinMode(D0, OUTPUT);
  digitalWrite(D0, HIGH);
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
  Serial.println("starting....");
}

void loop() {
  val = analogRead(A0); // อ่านค่าสัญญาณ analog ขา A0 ที่ต่อกับ LDR
  Serial.print("val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
  Serial.println(val); // พิมพ์ค่าของตัวแปร val

  // ตรวจสอบสถานะการทำงานของเครื่อง
  if (val < 600 && !machineRunning) { // ค่า 100 สามารถกำหนดปรับได้ตามค่าแสงในห้องต่างๆ
    digitalWrite(D0, HIGH); // สั่งให้ LED ดับ
    Serial.print("เครื่องซักผ้าทำงาน val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
    Serial.println(val); // พิมพ์ค่าของตัวแปร val
    LINE.notify("ESP1:กำลังทำงานอยู่💧"); //ส่งแจ้งเตือนเข้าไลน์กลุ่ม *เครื่องทำงาน*
    machineRunning = true; // เปลี่ยนสถานะเครื่องเป็นกำลังทำงาน
  } else if (val >= 600 && machineRunning) { // ค่า 100 สามารถกำหนดปรับได้ตามค่าแสงในห้องต่าง ๆ
    digitalWrite(D0, LOW); // สั่งให้ LED ติดสว่าง
    LINE.notifySticker("ESP1:ซักเสร็จแล้วเอาผ้าไปตากเร็ว😳", 1, 2 ); //ส่งแจ้งเตือนเข้าไลน์กลุ่ม *ผ้าซักเสร็จแล้ว*
    Serial.print("ซักผ้าเสร็จแล้ว val = "); // พิมพ์ข้อมความส่งเข้าคอมพิวเตอร์ "val = "
    Serial.println(val); // พิมพ์ค่าของตัวแปร val
    machineRunning = false; // เปลี่ยนสถานะเครื่องเป็นเสร็จแล้ว
  }
  delay(1000); // 1 วินาที
}
