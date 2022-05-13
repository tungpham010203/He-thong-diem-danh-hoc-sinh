
//#include <ArduinoJson.h>
#include<EEPROM.h>
#include "quanLy.h"
#include<WiFi.h>
//Các chân đã dùng:
//21 22 2 4 13 32 33 25 26 27 14 12 23 18 19
//Chuông : 16
//#include <SoftwareSerial.h>
String c = "";
#include <IOXhop_FirebaseESP32.h> //Thư viện cho ESP32
#define FIREBASE_HOST "quanlygioday-ab237.firebaseio.com" //Máy chủ Firebase
#define FIREBASE_AUTH "1ntE2kAqSA5OuMEomXoaaCNLZcrPypz8qJbtOp4L" //mã secret
/*
  #define WIFI_SSID "iPhone" //Tên WiFi
  #define WIFI_PASSWORD "12344321" //Pass WiFi
*/
#include<WiFiUdp.h>//khai báo thư viện để kết nối wifi theo chuẩn Udp
#include<NTPClient.h>//Khai báo thư viện dùng cho máy khách
WiFiUDP u;
NTPClient n(u, "2.asia.pool.ntp.org", 7 * 3600); //Lấy Time

//Thư viện LCD, KeyPad

//RFID:
#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".
#define SS_PIN 2
#define RST_PIN 4
MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec = 0;
unsigned long uidDecTemp; // hien thi so UID dang thap phan

bool coGiaoVien = false;
bool loadStudent = false;

int numberStudents = 0;
//-----------
String ID = "";

String lopDung = "";

String giaoVien = "";
int SiSo1 = 0;
int ssthe = 0;
String SiSo = "";
bool isEnter = false;
bool enableKey = true;

int trangThaiLCD = 1;// Dừng in
int Mode = 3;
bool danhChuong = false;
unsigned long timer1; //Lưu thời gian đánh chuông
int chuong = 26;
bool congTiet = false;
/*
   Mode = 1: Chưa có giáo viên, nhập ID
   Mode = 2: Có giáo viên rồi, Yêu cầu nhập Sĩ số
   Mode = 3: Có giáo viên rồi, nhập sĩ số rồi, Teaching

*/
int lastTiet = 0;
//-----------------------Khởi tạo tiết-------------------------------------------------
Time S1(0, 0);
Time E1(0, 0);

Time S2(0, 0);
Time E2(0, 0);

Time S3(0, 0);
Time E3(0, 0);

Time S4(0, 0);
Time E4(0, 0);

Time S5(0, 0);
Time E5(0, 0);
//----------------------Check tiết học--------------------------------------------------
int checkTietHoc(Time t)
{
  Time S0(7, 0);
  Time E0(7, 15);

  int tiet = -1;
  if (checkInTime(t, S0, E0) == true) tiet = 0;
  if (checkInTime(t, S1, E1) == true) tiet = 1;
  if (checkInTime(t, S2, E2) == true) tiet = 2;
  if (checkInTime(t, S3, E3) == true) tiet = 3;
  if (checkInTime(t, S4, E4) == true) tiet = 4;
  if (checkInTime(t, S5, E5) == true) tiet = 5;

  return tiet;
}
bool cmpTime(Time time1, Time time2)
{
  bool result = false;
  if (time1.Hours == time2.Hours && time1.Minutes == time2.Minutes)result = true;
  return result;
}

Time hieuThoiGian(Time t1, Time t2)
{
  Time hieu(0, 0);
  if (t1.Minutes < t2.Minutes)
  {
    hieu.Minutes = t1.Minutes + 60 - t2.Minutes;
    t1.Hours--;
  }
  else
  {
    hieu.Minutes = t1.Minutes - t2.Minutes;
  }

  hieu.Hours = t1.Hours - t2.Hours;
  return hieu;
}
//------JSONOBJECT----------
int buzzer = 12;
int value;
struct hocSinh
{
  int ID;
  String Name;
  bool Co_mat;
  String CardNumber;
  String Email;
  String Vi_tri;

};

hocSinh Students[50];

int manHinh = 1;


//EEPROM


//-------------------------------------------------

void docEEPROM(String &admin, String &pass)
{
  admin = "";
  pass = "";
  for (int i = 0; i < 32; i++)
  {
    if (EEPROM.read(i) != 0)admin += char(EEPROM.read(i)); //admin = admin + EEPROM.read(i);
  }
  for (int i = 32; i < 64; i++)
  {
    if (EEPROM.read(i) != 0)pass += char(EEPROM.read(i));
  }
}
void ghiEEPROM(String admin, String pass)
{
  Serial.println(admin);
  Serial.println(pass);
  EEPROM.begin(64);//Khởi tạo để bắt đầu quá trình ghi vào EEprom
  for (int i = 0; i < 64; i++)
  {
    EEPROM.write(i, 0);
  }
  for (int i = 0; i < admin.length(); i++)
  {
    EEPROM.write(i, (int)admin[i]);
  }
  for (int i = 32; i < (32 + pass.length()); i++)
  {
    EEPROM.write(i, (int)pass[i - 32]);
  }
  EEPROM.commit();//kết thúc ghi
  ESP.restart();//Khởi động lại esp
}
//-------------------------------------------------
void nextionSet(String object, int data)
{
  String command = String(object) + String("=") + data;
  Serial2.print(command);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}
void nextionSet(String object, String data)
{
  String command = String(object) + String("=\"") + data + String("\"");
  Serial2.print(command);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}
//-----------------------END------------------------------------------------------------
String getJson(String path)
{
  HTTPClient http;

  Serial.print("[HTTP] begin...\n");

  http.begin(String("https://") + String(FIREBASE_HOST) + String(path) + String(".json") + String("?auth=") + String(FIREBASE_AUTH)); //HTTP

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  unsigned long time1 = millis();
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      //JSON


      return payload;
    }
  }
}

String readUart()//chương trình con đọc tín hiệu ở cổng nối tiếp
{
  //Read from bluetooth and write to usb serial ( KHI CHƯA KẾT NỐI)
  if (Serial2.available()) //nếu có tín hiệu ở cổng Uart
  {

    String s = Serial2.readString(); //đọc giá trị dưới dạng chuỗi ở cổng nối tiếp rồi gán vào biến c
    Serial.println(s);
    return s;
  }
}

void setup() {
  pinMode(buzzer, OUTPUT); digitalWrite(buzzer, LOW);
  pinMode(chuong, OUTPUT);
  digitalWrite(chuong, HIGH);
  Serial2.begin(9600);
  Serial.begin(115200);
  nextionSet("page1.reset.val", 1);
  delay(200);
  EEPROM.begin(64);
  /*
    String usename = "";
    String password = "";
  */
  String USENAME = "";
  String PASSWORD = "";
  docEEPROM(USENAME, PASSWORD);
  Serial.println(USENAME);
  Serial.println(PASSWORD);
  WiFi.begin(USENAME.c_str(), PASSWORD.c_str()); //Kết nối WiFi
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    String l = "";
    if (Serial2.available() > 0)
    {
      l = Serial2.readString();

    }
    if (l == "pageWifi")
    {
      while (1)
      {
        String s = "";
        while (Serial2.available() > 0)
        {
          s = Serial2.readString();
        }
        Serial.println(s);
        String admin;
        String password;
        Serial.println(s.indexOf("@"));
        if (s != "")
        {
          for (int i = 0; i < s.indexOf("@"); i++)
          {
            admin += s[i];
          }
          for (int i = s.indexOf("@") + 1; i < s.length(); i++)
          {
            password += s[i];
          }
          Serial.println("Tach ra ne: ");
          Serial.println(admin);
          Serial.println(password);
          ghiEEPROM(admin, password);
          break;
        }

      }
    }

  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP()); //IP WiFi
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  n.begin();
  //RFID
  SPI.begin();
  mfrc522.PCD_Init();
  //---------------------------LOAD TIME-------------------------------------
  const size_t bufferTime = JSON_ARRAY_SIZE(6) + 15 * JSON_OBJECT_SIZE(2) + 680;
  DynamicJsonBuffer jsonBufferTime(bufferTime);
  Serial.println(getJson("/quanlygioday-ab237/Time"));
  JsonObject &Time = jsonBufferTime.parseObject(getJson("/quanlygioday-ab237/Time"));
  if (!Time.success()) {
    Serial.println("JSON parsing failed!");
  }
  S1.Hours = Time["1"]["Start"]["Hours"];
  S1.Minutes = Time["1"]["Start"]["Minutes"];

  S2.Hours = Time["2"]["Start"]["Hours"];
  S2.Minutes = Time["2"]["Start"]["Minutes"];

  S3.Hours = Time["3"]["Start"]["Hours"];
  S3.Minutes = Time["3"]["Start"]["Minutes"];

  S4.Hours = Time["4"]["Start"]["Hours"];
  S4.Minutes = Time["4"]["Start"]["Minutes"];

  S5.Hours = Time["5"]["Start"]["Hours"];
  S5.Minutes = Time["5"]["Start"]["Minutes"];
  //Het Tiet:
  Serial.println("Toi day");

  E1.Hours = Time["1"]["End"]["Hours"];
  E1.Minutes = Time["1"]["End"]["Minutes"];

  E2.Hours = Time["2"]["End"]["Hours"];
  E2.Minutes = Time["2"]["End"]["Minutes"];

  E3.Hours = Time["3"]["End"]["Hours"];
  E3.Minutes = Time["3"]["End"]["Minutes"];

  E4.Hours = Time["4"]["End"]["Hours"];
  E4.Minutes = Time["4"]["End"]["Minutes"];

  E5.Hours = Time["5"]["End"]["Hours"];
  E5.Minutes = Time["5"]["End"]["Minutes"];

  Serial.println(String(S1.Hours) + String(":") + String(S1.Minutes)) ;

  //----------------------------END------------------------------------------
  //Read Hoc Sinh:

  const size_t bufferSize = 4 * JSON_OBJECT_SIZE(4) + 390;
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(getJson("/quanlygioday-ab237/TKB/10A/Students"));

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
  }

  value = root["count"];
  Serial.println(value);
  hocSinh Student[value];

  Serial.println("Doc xong JSON");


  for (int i = 0; i < value; i++)
  {
    const char* Name = root[String(i + 1)]["Name"];
    String Name1(Name);
    Students[i].Name = Name1;

    const char* CardNumber =  root[String(i + 1)]["CardNumber"];
    String CardNumber1(CardNumber);
    Students[i].CardNumber = CardNumber1;

    Students[i].ID = i + 1;
    Students[i].Co_mat = root[String(i + 1)]["Co_mat"];

    const char* Email = root[String(i + 1)]["Email"];
    String Email1(Email);
    Students[i].Email = Email1;

    const char* vi_Tri = root[String(i + 1)]["Vi_tri"];
    String Vi_tri(vi_Tri);
    Students[i].Vi_tri = Vi_tri;


  }

  for (hocSinh x : Students)
  {
    if (x.Co_mat == true)nextionSet(String("page0.") + x.Vi_tri + ".bco", 2047);
  }

  Serial.println("Loi o day");
  //Serial.println(Student[0].Name);

  //Students = &Student[0];
  Serial.println(Students[0].Name);
  Serial.println(Students[0].Co_mat);
  //-------------------------------------------------------------------------
  giaoVien = Firebase.getString("/quanlygioday-ab237/TKB/10A/teacherOnClass");
  if (giaoVien != "")
  {
    manHinh = 1;
    int n = Firebase.getInt("/quanlygioday-ab237/TKB/10A/present");
    SiSo1 = n;
    ssthe = Firebase.getInt("/quanlygioday-ab237/TKB/10A/ssthe");
    coGiaoVien = false;
  }
  numberStudents = Firebase.getInt("/quanlygioday-ab237/TKB/10A/numberStudents");
}

int thoiGianKeuCoi = 0;
void loop()
{
  n.update();
  Time now(n.getHours(), n.getMinutes());
  int tietNow = checkTietHoc(now);

  if (tietNow != lastTiet)
  {
    nextionSet(String("page1.t5.txt"), String(tietNow));
    lastTiet = tietNow;
  }
  int Day = n.getDay() + 1;
  //Bắt sự kiện Enter cho mỗi Mode
  if (tietNow >= 1 && tietNow <= 5) //Nếu mà thời gian nằm trong tiết 1-5 và không có gv trong lớp
  {
    if ((hieuThoiGian(now, S1).Hours == 0 && hieuThoiGian(now, S1).Minutes == 5) || (hieuThoiGian(now, S2).Hours == 0 && hieuThoiGian(now, S2).Minutes == 5)
        || (hieuThoiGian(now, S3).Hours == 0 && hieuThoiGian(now, S3).Minutes == 5) || (hieuThoiGian(now, S4).Hours == 0 && hieuThoiGian(now, S4).Minutes == 5)
        || (hieuThoiGian(now, S5).Hours == 0 && hieuThoiGian(now, S5).Minutes == 5))
    {
      if (coGiaoVien == false)
      {
        if (thoiGianKeuCoi <= 200)
        {
          digitalWrite(buzzer, HIGH);
          delay(20);
          digitalWrite(buzzer, LOW);
          delay(20);
          thoiGianKeuCoi++;
        }
      }
    }
    if (cmpTime(now, S1) || cmpTime(now, S2) || cmpTime(now, S3) || cmpTime(now, S4) || cmpTime(now, S5)) //bắt đầu tiết
    {
      if (congTiet == true)
      {
        congTiet = false;
        nextionSet("page1.serial.txt", "page1");
        thoiGianKeuCoi = 0;
      }
    }
    if (cmpTime(now, E1) || cmpTime(now, E2) || cmpTime(now, E3) || cmpTime(now, E4) || cmpTime(now, E5))
    {
      if (congTiet == false)
      {

        const size_t bufferSize = 4 * JSON_OBJECT_SIZE(4) + 390;// tạo kik cỡ bộ nhớ để lưu trữ chuỗi json
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& root = jsonBuffer.parseObject(getJson("/quanlygioday-ab237/listTeacher"));

        int counter = root["Counter"]["count"];
        Serial.println(counter);

        for (int i = 1; i <= counter; i++)
        {
          const char* GV = root[String(i)]["NameInTKB"];
          String giaoVienCheck(GV);
          if (giaoVienCheck == giaoVien)
          {
            int soTietHienTai = root[String(i)]["demGioDay"]["10A"];
            Firebase.setInt(String("/quanlygioday-ab237/listTeacher/") + String(i) + String("/demGioDay") + String("/10A"), ++soTietHienTai);
            Firebase.setInt("/quanlygioday-ab237/TKB/10A/present", 0);
            Firebase.setString("/quanlygioday-ab237/TKB/10A/teacherOnClass", String(""));
            congTiet = true;
            coGiaoVien = false;
            break;
          }
        }
      }

      if (danhChuong == false)
      {
        danhChuong = true;
        timer1 = millis();
      }
      if (cmpTime(now, E5))
      {
        int cnt = Firebase.getInt("/quanlygioday-ab237/TKB/10A/Students/count");
        for (int i = 1; i <= cnt; i++)
        {
          Firebase.setBool(String("/quanlygioday-ab237/TKB/10A/Students/") + i + "/Co_mat", false);
        }
      }
    }
    if (manHinh == 1)
    {
      String s = "";
      if (Serial2.available() > 0)
      {
        s = Serial2.readString();
      }
      if (s == "page3") manHinh = 3;
      if (s == "pageWifi")manHinh = 4;
      //Serial.println("Chay vao man hinh 1");
      String giaoVienNow = "";
      //enableKey = false;
      docThe();
      if (uidDec != 0)
      {
        if (String(uidDec) != "2257196670")
        {
          const size_t bufferSize = 30000;
          DynamicJsonBuffer jsonBuffer(bufferSize);
          JsonObject& root = jsonBuffer.parseObject(getJson("/quanlygioday-ab237/listTeacher"));
          int counter = root["Counter"]["count"];

          for (int i = 1; i <= counter; i++)
          {
            const char* maTheGV = root[String(i)]["CardNumber"];
            String maTheGiaoVien(maTheGV);
            if (String(uidDec) == maTheGiaoVien)
            {
              const char* GV = root[String(i)]["NameInTKB"];
              String giaoVienN(GV);
              giaoVienNow = giaoVienN;
            }
          }
        }
        else giaoVienNow = "HieuTruong";
      }
      uidDec = 0;

      if (giaoVienNow != "") //Nếu tồn tại ID
      {
        String path1 = String("/quanlygioday-ab237/TKB/10A/Thứ ") + String(Day) + String("/Tiết ") + String(tietNow) + String("/Teacher");//Đường dẫn tới thứ và tiết
        String giaoVienDung = Firebase.getString(path1);
        Serial.println(giaoVienDung);//Giao vien cua lop 10A theo TKB hien tai
        Serial.println(giaoVienNow);
        if (giaoVienNow == giaoVienDung)
        {
          Firebase.setString("/quanlygioday-ab237/TKB/10A/teacherOnClass", giaoVienNow);
          if (Firebase.failed())return;
          giaoVien = giaoVienNow;
          coGiaoVien = true;
          nextionSet(String("serial.txt"), String("page2"));
          manHinh = 2;
          Serial.println("Nhay vao day");
        }
        else if (giaoVienNow == "HieuTruong")
        {
          nextionSet(String("serial.txt"), String("page2"));
          manHinh = 2;
        }
        else
        {
          String FindClass = String("/quanlygioday-ab237/TKB/10B/Thứ ") + String(Day) + String("/Tiết ") + String(tietNow) + String("/Teacher");
          String giaoVienFind = Firebase.getString(FindClass);
          Serial.println(String("Giao vien Find: ") + String(giaoVienFind));//Giao vien tim duoc o lop 10B tiet hien tai
          Serial.println(String("Giao vien Now: ") + String(giaoVienNow));//Giao vien vua quet
          if (giaoVienFind == giaoVienNow)
          {
            lopDung = "10B";
            nextionSet("lopDung.txt", String(lopDung));
            nextionSet("tmMsgLop.en", 1);
          }
          else
          {
            lopDung = "Khong co tiet";
            nextionSet("lopDung.txt", String(lopDung));
            nextionSet("tmMsgLop.en", 1);
          }

        }
      }
    }
    if (manHinh == 2)
    {
      String s = "";

      if (Serial2.available() > 0)
      {
        s = Serial2.readString(); //Serial.println (s);
      }
      if (s != "")
      {
        if (s == "page3")manHinh = 3;
        else
        {
          Firebase.setString("/quanlygioday-ab237/TKB/10A/present", s);
          manHinh = 1;
        }
      }
    }
    if (manHinh == 3)
    {
      String s = "";
      if (Serial2.available() > 0)
      {
        s = Serial2.readString();

      }

      if (s == "page1")manHinh = 1;
      else if (s.indexOf("kich") != -1)// gia tri, vi tri suat hien cua "kich" trong sau s
      {
        String cmd1 = "";
        int viTri = s.indexOf("kich");
        if (viTri != -1)
        {
          for (int i = viTri + 4; i < s.length(); i++)cmd1 += s[i];

          for (int i = 0; i < value; i++)
          {
            if (Students[i].Vi_tri == cmd1)
            {
              Serial.println(Students[i].Name);
              Firebase.setBool(String("/quanlygioday-ab237/TKB/10A/Students/") + Students[i].ID + "/Co_mat", false);
              Students[i].Co_mat = false;
              nextionSet(String(Students[i].Vi_tri) + String(".bco"), 63504);
            }

          }

        }
      }
      else if (s.indexOf("quenThe") != -1)
      {
        String cmd1 = "";
        int viTri = s.indexOf("quenThe");
        if (viTri != -1)
        {
          for (int i = viTri + 7; i < s.length(); i++)cmd1 += s[i];

          for (int i = 0; i < value; i++)
          {
            if (Students[i].Vi_tri == cmd1)
            {
              Serial.println(Students[i].Name);
              Firebase.setBool(String("/quanlygioday-ab237/TKB/10A/Students/") + Students[i].ID + "/Co_mat", true);
              Students[i].Co_mat = true;
              nextionSet(String(Students[i].Vi_tri) + String(".bco"), 2047);
            }

          }

        }
      }
      docThe();
      if (uidDec != 0)
      {

        for (int i = 0; i < value; i++)
        {
          /*
            Serial.println("chay vao day");
            Serial.println(String(Students[i].Co_mat));
            Serial.println(Students[i].Name);
            Serial.println(Students[i].CardNumber);
          */
          if (Students[i].Co_mat == true)continue;

          //Serial.println("Chay xuong day:");
          String CardNumber = Students[i].CardNumber;
          // Serial.print("Quet: ");
          // Serial.println(CardNumber);


          if (CardNumber == String(uidDec))
          {
            Serial.println("Trung Ma the");
            Students[i].Co_mat = true;
            Firebase.setBool(String("/quanlygioday-ab237/TKB/10A/Students/") + String(i + 1) + String("/Co_mat"), true);
            digitalWrite(buzzer, HIGH);
            delay(50);
            digitalWrite(buzzer, LOW);
            delay(50);
            digitalWrite(buzzer, HIGH);
            delay(50);
            digitalWrite(buzzer, LOW);
            nextionSet(String(Students[i].Vi_tri) + String(".bco"), 2047);
          }
        }
      }
      uidDec = 0;
    }
    if (manHinh == 4)
    {
      String s = "";
      if (Serial2.available() > 0)
      {
        s = Serial2.readString();
      }
      String admin;
      String password;

      for (int i = 0; i < s.indexOf("@"); i++)
      {
        admin[i] += s[i];
      }
      for (int i = s.indexOf("@") + 1; i < s.length(); i++)
      {
        password[i - s.indexOf("@") - 1] = s[i];
      }
      Serial.println(admin);
      Serial.println(password);
      ghiEEPROM(admin, password);
    }
  }
  else
  {
    nextionSet(String("serial.txt"), "pageRaChoi");
  }


  /*
    if (Serial2.available() > 0)
    {
    String cmd = Serial2.readString();
    String cmd1 = "";
    int viTri = cmd.indexOf("kich");
    if (viTri != -1)
    {
      for (int i = viTri + 4; i < cmd.length(); i++)cmd1 += cmd[i];
      Serial.println(cmd1);
    }

    }
  */

  if (danhChuong == true)
  {
    if (millis() - timer1 <= 5000)
    {
      digitalWrite(chuong, LOW);
      Serial.println(millis() - timer1);
    }
    else
    {
      digitalWrite(chuong, HIGH);
    }
  }

}

void docThe()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Doc the
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  uidDec = 0;
  digitalWrite(buzzer, HIGH);
  Serial.println("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");
  Serial.println("================================================");

  // Hien thi so UID cua the


  Serial.println("Card UID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  Serial.print("            [");
  Serial.print(uidDec);
  Serial.println("]");
  Serial.println("================================================");
  mfrc522.PICC_HaltA();
  digitalWrite(buzzer, LOW);

}
