#include "U8glib.h" //OLED
#include "HX711.h" //로드셀

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);
HX711 scale;

// pin list
// 로드셀 센서
int load_shell_dout = 7;
int load_shell_sck = 6;

// 초음파 센서
int ultrasonic_trig1 = 8;
int ultrasonic_echo1 = 9;
int ultrasonic_trig2 = 10;
int ultrasonic_echo2 = 11;
int ultrasonic_trig3 = 12;
int ultrasonic_echo3 = 13;

// 변수 리스트
String people = "loading";
float space = -1.0;
float load = -1.0;

void setup() {
    pinMode(load_shell_dout, INPUT);
    pinMode(load_shell_sck, OUTPUT);

    pinMode(ultrasonic_echo1, INPUT);
    pinMode(ultrasonic_echo2, INPUT);
    pinMode(ultrasonic_echo3, INPUT);
    pinMode(ultrasonic_trig1, OUTPUT);
    pinMode(ultrasonic_trig2, OUTPUT);
    pinMode(ultrasonic_trig3, OUTPUT);

    Serial.begin(9600);
    scale.begin(load_shell_dout, load_shell_sck);
}

void trigger_print_loop(){
    digitalWrite(ultrasonic_trig1, LOW);
    delayMicroseconds(2);
    digitalWrite(ultrasonic_trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonic_trig1, LOW);

    digitalWrite(ultrasonic_trig2, LOW);
    delayMicroseconds(2);
    digitalWrite(ultrasonic_trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonic_trig2, LOW);

    digitalWrite(ultrasonic_trig3, LOW);
    delayMicroseconds(2);
    digitalWrite(ultrasonic_trig3, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonic_trig3, LOW);
}

float trigger_loop(){
    float Length1 = pulseIn(ultrasonic_echo1, HIGH);
    float Length2 = pulseIn(ultrasonic_echo2, HIGH);
    float Length3 = pulseIn(ultrasonic_echo3, HIGH);

    float distance1 = ((float)(340*Length1)/10000) / 2;
    float distance2 = ((float)(340*Length2)/10000) / 2;
    float distance3 = ((float)(340*Length3)/10000) / 2;

    float tmp1 = -5.4*distance1+108;
    float tmp2 = -5.4*distance2+108;
    float tmp3 = -5.4*distance3+108;

    if (tmp1 > 100) tmp1 = 100;
    if (tmp2 > 100) tmp2 = 100;
    if (tmp3 > 100) tmp3 = 100;
    if (tmp1 < 0) tmp1 = 0;
    if (tmp2 < 0) tmp2 = 0;
    if (tmp3 < 0) tmp3 = 0;

    float avgtmp = (tmp1+tmp2+tmp3)/3;

    return avgtmp;
}

float loadShell_loop(){
  if (scale.is_ready()){
    long reading = scale.get_units(10);
    float weight = 0.0059649 * reading + 1202.135;

    if (weight < 1) weight = 0;
  
    return weight;
  }
}

String objectDetecting_loop(){
    String people = "";

    while (people == ""){
      Serial.println("yes");
      delay(1000);
      people = Serial.readStringUntil('\n');
    }

    return people;
}

void oled_print(){
    u8g.firstPage();
    do{
        u8g.setFont(u8g_font_7x14);
        u8g.setPrintPos(0,10);
        u8g.print("People: ");
        u8g.print(people);
        u8g.setPrintPos(0,30);
        u8g.print("Space: ");
        u8g.print(space);
        u8g.print("%");
        u8g.setPrintPos(0,50);
        u8g.print("Load: ");
        u8g.print(load);
        u8g.print("g");
    } while(u8g.nextPage());

    delay(1000);
}

void loop(){
    oled_print();
    space = trigger_loop();
    load = loadShell_loop();
    people = objectDetecting_loop();
}
