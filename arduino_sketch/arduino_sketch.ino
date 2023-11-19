// #include "U8glib.h" //OLED
// #include "HX711.h" //로드셀

// U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

//인원 수, 공간 포화도, 하중
// int people = 0;
// float space = 0;
// float load = 0;

// //로드셀 센서 1번의 DOUT, SCK
// int dout1 = 7;
// int sck1 = 6;

// HX711 scale;

// //초음파 센서 코드
// //초음파 센서 1번의 트리거, 에코
// int trig1 = 8;
// int echo1 = 9;

// //초음파 센서 2번의 트리거, 에코
// int trig2 = 10;
// int echo2 = 11;

// //초음파 센서 3번의 트리거, 에코
// int trig3 = 12;
// int echo3 = 13;

// void pin(int tr, int ech) { //pinMode 함수 묶음
//   pinMode(tr,OUTPUT);
//   pinMode(ech,INPUT);
// }

// void digiWrite(int tr) //digitalWrite 함수 묶음
// {
//   digitalWrite(tr,LOW);
//   delay(2);
//   digitalWrite(tr,HIGH);
//   delay(10);
//   digitalWrite(tr,LOW);
// }

// float circLength(int ech) //그냥 센서값 받아오는 계산
// {
//   float Length;
//   Length = pulseIn(ech,HIGH);
//   return Length;
// }

// float circSat(float len,int ech) // 포화도 계산 함수
// {
//   //일단 거리 계산 알고리즘 적용 -> cm단위 거리 계산
//   float distance, tmp;
//   distance = ((float)(340*len)/10000) / 2;

//   /*
//   20cm일 때 포화도 0
//   16cm일 때 포화도 30
//   8cm일 때 포화도 65
//   나오도록 선형 방정식 계산
//   */
//   tmp = -5.4*distance+108;

//   //포화도가 100을 초과하면 안되므로 100 넘어가면 100으로 보정
//   if (tmp>100)
//   {
//     tmp = 100;
//   }

//   //포화도가 0 미만일 수 없으므로 0 미만이면 0으로 보정
//   else if (tmp<0)
//   {
//     tmp = 0;
//   }

//   return tmp; //포화도 리턴.
// }

void printSat(int n, float s){ //포화도 출력 함수(매개변수로 초음파 센서 순서, 포화도 값 전달.)
  Serial.print(n);
  Serial.print("번 초음파 센서 공간 포화도:");
  Serial.print(s);
  Serial.println(" %");
  Serial.println();
}

// float avgSat(float s1, float s2, float s3) //값 세 개 평균 내는 함수
// {
//   float tmp = (s1+s2+s3)/3;
//   return tmp;
// }

//여기서부터 실제 실행 코드
// void setup() {
  // put your setup code here, to run once:
  //pin(trig1,echo1);
  //pin(trig2,echo2);

  // Serial.begin(9600);
  // scale.begin(dout1, sck1); //로드셀1
// }

void loop() {

  // digiWrite(trig1);
  // digiWrite(trig2);
  // digiWrite(trig3);

  //센서값 저장.
  // float Length1 = circLength(echo1);
  // float Length2 = circLength(echo2);
  // float Length3 = circLength(echo3);

  // //1번, 2번 센서값 출력
  // Serial.print("1번: ");
  // Serial.println(Length1);
  // Serial.println();
  
  
  // Serial.print("2번: ");
  // Serial.println(Length2);
  // Serial.println();

  // Serial.print("3번: ");
  // Serial.println(Length3);
  // Serial.println();
  
  
  // //1번,2번,3번 포화도 각각 계산
  // float sat1 = circSat(Length1, echo1);
  // float sat2 = circSat(Length2, echo2);
  // float sat3 = circSat(Length3, echo3);

  //1번, 2번 포화도 각각 출력
  /*
  printSat(1,sat1);
  printSat(2,sat2);
  printSat
  */

  // space = avgSat(sat1,sat2,sat3);
  /*
  Serial.print("전체 포화도: ");
  Serial.print(space);
  Serial.println(" %");
  */

  //로드셀
   if (scale.is_ready()) {
    long reading = scale.get_units(10);
    float weight;
    Serial.print("HX711 reading: ");//0-202000,170-168700
    Serial.println(reading);
    weight=0.0059649 * reading + 1202.135;
    if(weight<1){
      weight=0;
    }
    Serial.print("무게:");
    Serial.println(weight);
    load = weight;  //OLED에 출력 될 무게 값
  } else {
    Serial.println("HX711 not found.");
  }

  //OLED
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
    //u8g.drawStr(40,44,(str)result);
  }while(u8g.nextPage());
  delay(1000);
}