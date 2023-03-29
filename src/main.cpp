#include <Arduino.h>
#include <M5Unified.h> // M5Unified���C�u�������v���O�����Ŏg�p�\�ɂ��܂��B

#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Wii Nunchuck");

#include <Wire.h>
#include <nunchuck_funcs.h>

int loop_cnt=0;

byte accx,accy,zbut,cbut,joyx,joyy;

#include <FastLED.h>   // FastLED(RGB LED)�̃��C�u�������g�p�\�ɂ��܂��B

// RGB LED�̐����w��(M5Atom Matrix�Ȃ�25)
#define NUM_LEDS 1
// RGB LED��DATA PIN���w��
#define LED_DATA_PIN 27

// �O���[�o���ϐ��i�v���O�����S�̂Ŏg�p����ϐ��̒�`�����܂��B�j
uint32_t count;
CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  auto cfg = M5.config();
  M5.begin(cfg);

  bleKeyboard.begin();


  Wire.begin(26,32);                // join i2c bus as master
  nunchuck_init(); // send the initilization handshake

  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);   // RGB LED�������ݒ�
  FastLED.setBrightness(20);                               // ���邳��ݒ�i20�ȏ�͔M�ŉ���\������B�j
  leds[0] = CRGB::Red; 
  FastLED.show(); 
  delay(30);
}


void loop() {
  // put your main code here, to run repeatedly:
  M5.update();

//  leds[0] = CRGB::Red; 
//  FastLED.show(); 
//  delay(30);

  nunchuck_get_data();

  accx  = nunchuck_accelx(); // ranges from approx 70 - 182
  accy  = nunchuck_accely(); // ranges from approx 65 - 173
  zbut = nunchuck_zbutton();
  cbut = nunchuck_cbutton();
  joyx = nunchuck_joyx();
  joyy = nunchuck_joyy();

  int x = (joyx & 0xf0);
  int y = (joyy & 0xf0);
    
  Serial.print("accx: "); Serial.print((byte)accx,HEX);
  Serial.print("\taccy: "); Serial.print((byte)accy,HEX);
  Serial.print("\tzbut: "); Serial.print((byte)zbut,HEX);
  Serial.print("\tcbut: "); Serial.print((byte)cbut,HEX);
  Serial.print("\tjoyx: "); Serial.print((byte)joyx,HEX);
  Serial.print("\tjoyy: "); Serial.print((byte)joyy,HEX);

  if(x <= 0x30){
    Serial.print("\tleft");
  }
  else if(x >= 0xd0){
    Serial.print("\tright");
  }
  else if(y <= 0x30){
    Serial.print("\tdown");
  }
  else if(y >= 0xd0){
    Serial.print("\tup");
  }
  else {
    Serial.print("\tcenter");
  }
  Serial.println("\t");


  if(bleKeyboard.isConnected()) {

    if(x <= 0x30){
      bleKeyboard.write(KEY_LEFT_ARROW);
    }
    else if(x >= 0xd0){
      bleKeyboard.write(KEY_RIGHT_ARROW);
    }
    else if(y <= 0x30){
      bleKeyboard.write(KEY_DOWN_ARROW);
    }
    else if(y >= 0xd0){
      bleKeyboard.write(KEY_UP_ARROW);
    }
    else if(zbut){
      bleKeyboard.write(KEY_RETURN);
    }
    else if(cbut){
      bleKeyboard.write(KEY_ESC);
    }
    else{
    }
  }

  delay(200);
}