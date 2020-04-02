#include <M5StickC.h>

#define SENSOR  36
#define RESET 2
long currentMillis = 0;
long previousMillis = 0;
int interval = 100;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

float s2 = 0;
float s1 = 30;
float max1 = 37;
float min1 = 0;
float y = 0;

int timmer = 0;
int xpoint = 0;
int ypoint = 0;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}


// the setup routine runs once when M5StickC starts up
void setup(){
  // Initialize the M5StickC object
  M5.begin();
  Serial.begin(115200);
 
  pinMode(SENSOR, INPUT);
  pinMode(RESET, INPUT);
  M5.Lcd.setRotation(3);
  M5.Lcd.setCursor(45, 30);
  M5.Lcd.setTextSize(1);


  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);

  // LCD display
  
  /*
   // text print
  M5.Lcd.fillScreen(BLACK);
  
  M5.Lcd.setTextColor(WHITE);
  
  M5.Lcd.printf("Display Test!");

   */
}

// the loop routine runs over and over again forever
void loop() {


  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    //Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    //Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    //Serial.print("Output Liquid Quantity: ");
    //Serial.print(totalMilliLitres);
    //Serial.print("mL / ");
    //Serial.println(totalMilliLitres / 1000);
    //Serial.print(SENSOR);
    //Serial.println("L");
    //M5.Lcd.setCursor(50, 60);
    //M5.Lcd.print(flowRate);
    //M5.Lcd.setCursor(45, 45);
    //M5.Lcd.print(totalMilliLitres / 1000);
    y = (((s1 - s2)/(max1 - min1))*(flowRate - min1) + s2);
    M5.Lcd.drawCircle(35, 40, 30, RED); 
    M5.Lcd.drawLine(73, 75, 155, 75, WHITE); //Xaxis line
    M5.Lcd.drawLine(73, 30, 73, 75, WHITE); //Yaxis line
    M5.Lcd.fillCircle(35, 40, y, RED);
    //M5.Lcd.drawPixel(int16_t x, int16_t y, BLUE);
    ypoint = int(flowRate);
    /*
    while( xpoint < 155 && ypoint < 75){
       
    }
    */
    //M5.Lcd.drawPixel(xpoint + 73, 75 - ypoint, GREEN);
    M5.Lcd.drawLine(int(xpoint + 74), int(74 - ypoint), int(xpoint + 73), int(75 - ypoint), GREEN); 
      // delay(200);
       xpoint = xpoint + 1;
/*
    if (xpoint == 155){
      
    }
*/
    if (xpoint == 100){
      M5.Lcd.fillRect(74, 30, 85, 45, BLACK);
      xpoint = 0;
      ypoint = 0;
    }
    

      
    if (y >= 30){
      M5.Lcd.fillCircle(35, 40, y, GREEN);
    }
    delay(90);
    M5.Lcd.fillCircle(35, 40, y, BLACK);
    if (y >= 30){
      M5.Lcd.fillCircle(35, 40, y, GREEN);
      delay(500);
      M5.Lcd.fillCircle(35, 40, y, BLACK);
    }
    M5.Lcd.setCursor(68, 10);
    M5.Lcd.setTextSize(1);
    M5.Lcd.print("FRate");
    M5.Lcd.setCursor(68, 20);
    M5.Lcd.print("Total");
    M5.Lcd.setCursor(105, 10);
    M5.Lcd.setTextSize(1);
    M5.Lcd.print(flowRate);
    M5.Lcd.setCursor(105, 20);
    M5.Lcd.print(totalMilliLitres / 1000);
    M5.Lcd.setCursor(140, 10);
    M5.Lcd.print("L/m");
    M5.Lcd.setCursor(140, 20);
    M5.Lcd.print("ml");
    if (digitalRead(RESET) == 1){
      totalMilliLitres =0;
      M5.Lcd.setCursor(105, 20);
      M5.Lcd.print(totalMilliLitres/1000);
    }
    
  }
}
