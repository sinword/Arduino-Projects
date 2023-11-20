#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);

//序列串列埠 - 定義通訊格式資料
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00

# define ACTIVATED LOW

int buttonPause = 3;
//int buttonPrevious = 4;
//int buttonNext = 5;
int buttonVolume = A0;
boolean isPlaying = false;

void setup () {
  Serial.begin(115200);
  pinMode(buttonPause, INPUT);
  digitalWrite(buttonPause, HIGH);
  //pinMode(buttonNext, INPUT);
  //digitalWrite(buttonNext,HIGH);
  //pinMode(buttonPrevious, INPUT);
  //digitalWrite(buttonPrevious,HIGH);

  Serial.println("Entry...");
  mySerial.begin (9600); //透過串列埠傳送給mini mp3 player 模組的指令，必須將baud rate通訊速度設定在9600
  delay(1000);
  playFirst(); //初始化 Mini MP3 Player模組
  isPlaying = false;
}

void loop () {
  if (digitalRead(buttonPause) == ACTIVATED)  //暫停播放按鍵
  {
    if (isPlaying)
    {
      pause();
      isPlaying = false;
    } else
    {
      isPlaying = true;
      play();
    }
  }

  /*if (digitalRead(buttonNext) == ACTIVATED) //切換下一首按鍵
    {

     if(isPlaying)
     {
       playNext();
     }
    }

    if (digitalRead(buttonPrevious) == ACTIVATED)  //切換前一首按鍵
    {
     if(isPlaying)
     {
       playPrevious();
     }
    }*/
    int Volume = analogRead(buttonVolume) / 34.1; //分母數字可調小，限制最大音量
    //  34.1 的數字由來：1023 / 30 = 34.1
    setVolume(Volume);  //設定指定音量. From 0 to 30

    delay(300);
    }

    void playFirst()
    {
    execute_CMD(0x0F, 0, 0); //指定資料夾播放（須自己設定）
    delay(500);
    setVolume(5);
    delay(500);
    execute_CMD(0x11,0,1);  //循環播放
    delay(500);
    }

    void pause()
    {
    execute_CMD(0x0E,0,0);  //執行暫停播放命令
    }

    void play()
    {
    execute_CMD(0x0D,0,1);  //執行播放音樂命令
    delay(100);
    execute_CMD(0x11,0,1);  //循環播放
    delay(100);
    }

    /*void playNext()
    {
    execute_CMD(0x01,0,1);  //執行播放下一首命令
    delay(100);
    execute_CMD(0x11,0,1);  //循環播放
    delay(100);
    }

    void playPrevious()
    {
    execute_CMD(0x02,0,1);  //執行播放前一首命令
    delay(100);
    execute_CMD(0x11,0,1);  //循環播放
    delay(100);
    }*/

    void setVolume(int volume)
    {
    execute_CMD(0x06, 0, volume); // 設定指定音量 (0x00~0x30)
    }

    // 執行命令和傳入的參數
    void execute_CMD(byte CMD, byte Par1, byte Par2) {

    // 計算checksum (2 bytes)
    //word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);

    // 建立命令
    byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    Par1, Par2, End_Byte};

    //送出命令給模組
    for (byte k=0; k<10; k++){
     mySerial.write( Command_line[k]);
    }
    }
