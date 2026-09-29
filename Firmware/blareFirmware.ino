#include <Adafruit_GFX.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h> 

#define TFT_SCLK 0 // labeled SCL on the screen
#define TFT_MOSI 1 // labeled SDA on the screen
#define TFT_RST 2
#define TFT_DC 3
#define TFT_CS 4
#define TFT_BL 5

#define BTN_SW1   6
#define BTN_SW2   7
#define BUZZER   8

bool backlightState = true; 
bool startupDone = false; 


// Fix setColRowStart() by exposing it via a subclass
class MyST7789 : public Adafruit_ST7789 {
public:
  MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
    : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
  void setOffsets(uint8_t col, uint8_t row) {
    _colstart = _colstart2 = col;
    _rowstart = _rowstart2 = row;
  }
};

MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// setup() runs ONCE when the board powers on
void setup() {
  Serial.begin(115200); // lets the board talk to your computer

  pinMode(TFT_BL, OUTPUT); // Set the backlight pin mode, or just wire it to 3.3V
  digitalWrite(TFT_BL, LOW); // Turns the backlight ON, for some reason this screen is active Low, so setting it to LOW is really HIGH

  pinMode(BTN_SW1, INPUT_PULLUP);
  pinMode(BTN_SW2, INPUT_PULLUP);

  pinMode(TFT_BL, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(TFT_BL, HIGH);

  tft.init(76, 284); // Our panel size (portrait)
  tft.setOffsets(82, 18); // Offsets for the weird resolution
  tft.invertDisplay(false); // Invert the colors (This display is flipped from normal)
  tft.setRotation(1); // Landscape, if it's upside down use 3!
  Serial.println("TFT Initialized!");

  tft.fillScreen(ST77XX_BLACK); // clear the screen

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0,0); // Where the text is drawn, 0,0 is top left
  tft.print(42); // Show whatever you want! Draws from the top left of the text/number/shape 
}

// loop() runs OVER and OVER, forever
void loop() {
  Serial.println("Loop Initalized");
  if (!startupDone) {
    tft.print("Welcome to the Alarm Clock");
    delay(1000);
    tft.print("My favorite color is Orange!");
    startupDone = true; 
  }
  if (digitalRead(BTN_SW1) == LOW) {
    tone(BUZZER, 1000, 100);
    tft.print("SW1 PRESSED!");
    delay(2000);
  }

  if (digitalRead(BTN_SW2) == LOW) {
    backlightState = !backlightState;
    digitalWrite(TFT_BL, backlightState ? HIGH : LOW);
    
    tone(BUZZER, 1500, 50);
    delay(2000);
  }

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
}

