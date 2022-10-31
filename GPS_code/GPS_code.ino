#include <dht.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h> // grafik kütüphanesi
#include <Adafruit_ST7735.h> // donanım kütüphanesi
#include <SPI.h>
  
int sclk = 13;
int mosi = 11;
int cs = 10;
int dc = 9;
int rst = 8;


#define DHT11_PIN 4
dht DHT;

int RXPin = 2;
int TXPin = 3;
int GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

Adafruit_ST7735 tft = Adafruit_ST7735 (cs, dc, rst);
void setup()
{
  float hum = 0;
  float tem = 0;
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  tft.initR (INITR_BLACKTAB);
  tft.fillScreen (ST7735_BLACK);  // Ekranın tamamını siyaha boya
  tft.setTextColor(ST7735_RED);  // Yazı rengini kırmızı olarak ayarla
  tft.setTextSize(2);  // Yazı boyutu 1 olsun
  tft.setCursor (35, 6);  // Yazının başlangıç pikselinin koordinatları
  tft.print ("TOTAR");  // Ekrana yazdırılacak metin

}
void loop()
{
  tft.setTextSize(1);  
  // Bu taslak, yeni bir cümle her doğru kodlandığında bilgileri görüntüler
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();
  // 1000 milisaniye geçerse ve gelen karakter yoksa
  // yazılım seri portu üzerinden "GPS algılanmadı" hatası göster
  if (millis() > 1000 && gps.charsProcessed() < 10)
  {
    Serial.println("GPS algılanmadı");
    while(true);
  }
}

void displayInfo()
{
  int chk = DHT.read11(DHT11_PIN);
  float tem = DHT.temperature;  // Read temperature
  float hum = DHT.humidity; // Read humidity
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6); Serial.print(",");Serial.print(gps.location.lng(), 6);Serial.print(",");Serial.println(gps.satellites.value());//Koordinat Bilgisi
    // Nesne "beslendikten" sonra, herhangi bir veri alanının güncellenip güncellenmediğini görmek için onu sorgulayabilirsiniz
    // <----- Title ----->
    tft.fillScreen(ST7735_BLACK);
    tft.setTextColor(ST7735_WHITE);  // Yazı rengini kırmızı olarak ayarla
    tft.setTextSize(2);  // Yazı boyutu 2 olsun
    tft.fillRect(0,0,128,24,ST7735_RED);
    tft.setCursor (35, 5);
    tft.print ("TOTAR");
    // <----- Title ----->
  
    // <----- Coordinate LAT ----->
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(0,28,85,12,ST7735_YELLOW);
    tft.setCursor (3, 31);
    tft.print ("LAT="); tft.print (gps.location.lat(), 6);
    // <----- Coordinate LAT ----->
  
    // <----- Coordinate LON ----->
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(0,43,85,12,ST7735_YELLOW);
    tft.setCursor (3, 46);
    tft.print ("LON="); tft.print (gps.location.lng(), 6);
    // <----- Coordinate LON ----->
  
    
    // <----- SAT ----->
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(88,28,128,27,ST7735_WHITE);
    tft.setCursor (99, 34);
    tft.print ("SAT");
    tft.setCursor (105, 43);
    tft.print (gps.satellites.value());
    // <----- SAT ----->
  
    // <----- GPS Title ----->
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(0,58,128,15,ST7735_CYAN);
    tft.setCursor (25, 62);
    tft.print ("GY-NEO6MV2 GPS");
    // <----- GPS Title ----->
  
    // <----- DHT BG ----->
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(0,76,37,27,ST7735_WHITE);
  
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(40,76,128,12,ST7735_GREEN);
    
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(40,91,128,12,ST7735_GREEN);
    // <----- DHT BG ----->
  
    if (tem >= 0 & hum >= 20)
    {
      // <----- DHT HUM ----->
      tft.setCursor (46, 79);
      tft.print ("HUM= "); tft.print (hum); 
      tft.setCursor (120, 79);
      tft.print ("%");
      // <----- DHT HUM ----->
    
      // <----- DHT TEM ----->
      tft.setCursor (46, 94);
      tft.print ("TEM= "); tft.print (tem);
      tft.setCursor (114, 91);
      tft.print("o"); 
      tft.setCursor (120, 94);
      tft.print ("C");
      // <----- DHT TEM ----->
    }
    else
    {
      // <----- DHT HUM ----->
      tft.setCursor (46, 79);
      tft.print ("HUM= "); tft.print (0); 
      tft.setCursor (120, 79);
      tft.print ("%");
      // <----- DHT HUM ----->
    
      // <----- DHT TEM ----->
      tft.setCursor (46, 94);
      tft.print ("TEM= "); tft.print (0); tft.print (" ");
      tft.setCursor (114, 91);
      tft.print("o"); 
      tft.setCursor (120, 94);
      tft.print ("C");
      // <----- DHT TEM ----->
    }
  
    // <----- DHT Title ----->
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.fillRect(0,106,128,15,ST7735_CYAN);
    tft.setCursor (52, 110);
    tft.print ("DHT11");
    // <----- DHT Title ----->
   
  
    // <----- COUNTER ----->
    int a = 1;
    for(int i = 0; i < 13; i++)
    {
      tft.fillRect(a,153,5,5,ST7735_WHITE);
      a = a+10;
      delay(300);
    }
    // <----- COUNTER ----->

 }
 else
 {
  
   Serial.println("Konum: Müsait değil");//Konum bilgisi alınmadığını gösterir
   tft.setCursor (10, 26);
   tft.setTextColor(ST7735_GREEN);
   tft.print ("GPS KONUM ALAMIYOR");
   tft.setCursor (10, 36);
   tft.print ("LUTFEN ANTENI ACIK");
   tft.setCursor (25, 46);
   tft.print ("ALANA CIKARIN");
   
 }
 Serial.println();
 delay(1000);
}
