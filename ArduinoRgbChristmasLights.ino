// ATTENTION: IN ADDITION TO MAKING USE OF ADAFRUIT NEOPIXEL LIBRARY, THIS PROGRAM
//            CONTAINS PARTS TAKEN FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE.
//            THESE PARTS OF THE SOURCE CODE ARE CLEARLY MARKED AS SUCH, AND THEY ARE NOT
//            COVERED BY MIT LICENSE, LIKE THE REST OF THE SOURCE CODE, WRITTEN BY ME,
//            ALEXEI KIREEV. FOR LICENSING, CONTACT ADAFRUIT.
//
// As of today, January 8th, 2019, Adafruit NeoPixel library is licensed under GNU Lesser
// General Public License version 3, which allows distribution of Combined Works, as long as
// the Combined Work:
//     * is licensed under terms do not restrict modification of the portions of 
//       Adaruit NeoPixel library contained in the Combined Work and reverse engineering
//       for debugging such modifications;
//       -- Code written by me is licensed under MIT License, which is more permissive than
//          GPL and LGPL, so, check.
//     * acknowlegdes the use of Adafruit NeoPixel library;
//       -- Check.
//     * is accompanied with a copy of GNU GPL and GNU LGPL (see files GPL3.txt and LGPL3.txt);
//       -- Check.
//     * conveys the Minimal Corresponding Source under the terms of GNU LGPL, and the
//       Corresponding Application Code in a form suitable for, and under terms that permit,
//       the user to recombine or relink the Application with a modified version of
//       the Linked Version to produce a modified Combined Work, in the  manner specified by
//       section 6 of the GNU GPL for conveying Corresponding Source;
//       -- Quite a mouthful, but I believe I do satisfy this requirement by releasing all my
//          code under MIT License, which is in fact more permissive than either GPL or LGPL,
//          so, check.
//     * provides installation information (see README.md)
//       -- Check.
//
// Because of this, I, Alexei Kireev, author of ArduinoRgbChristmasLights do believe that I
// am allowed to use portions of Adafruit NeoPixel library strandtest example in
// ArduinoRgbChristmasLights.
// If you are the Adafruit NeoPixel library rights holder and you disagree with that, please
// contact me by emailing to: durobotdev@gmail.com

/*
 * MIT License

 * Copyright (c) 2019 Alexei Kireev

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// ----- BEGIN CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
// ----- END CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----

// Uncomment if you want to see debug output in
// the Serial Monitor window. Program size and RAM usage will increase.
//#define DEBUG

// Uncomment if you want wave data tables to be stored
// in PROGMEM (FLASH) rather than RAM.
// USE_PROGMEM helps save RAM, but is slower.
// Since I'm not RAM-limited on ATmega328 or even on
// ATmega168, I don't care for PROGMEM.
// Could be useful with other MCUs with less RAM, like ATtiny (?),
// although the progam is too large to fit into ATtiny.
// Removal of some modes could reduce the size though.
//#define USE_PROGMEM

#define PIN 5 // The pin used to control LED pixels
#define NUM_PIXELS 100 // 50 + 50
#define DANGLING_ANALOG_PIN A7 // A dangling (unconnected) analog in pin used initialize RNG

// Mode constants.
// Define mode duration, fade out / fade in duration between modes,
// settings internal to each mode
#define MODE_CALM_WAVES_DUR 30000
#define MODE_CALM_WAVES_FADEIN 1000
#define MODE_WAVES_MAX_WAVES 3 // Max number of waves, also used in MODE_FAST_WAVES
#define MODE_WAVES_MAX_SPARKLES 10 // Max number of sparkles, also used in MODE_FAST_WAVES
#define MODE_CALM_WAVES_WAVE_DICE 101 // Less means greater chance
#define MODE_CALM_WAVES_SPARKLE_DICE 121 // Less means greater chance
#define MODE_CALM_WAVES_SPARKLE_TIME_TO_LIVE 60
#define MODE_CALM_WAVES_BELL_STEP_DUR 40 // BELL wave step duration, less means faster wave
#define MODE_CALM_WAVES_SAWTOOTH_SMOOTH_FADE_STEP_DUR 30 // SAWTOOTH_SMOOTH_FADEe wave step duration, less means faster wave
#define MODE_CALM_WAVES_BELL_RIPPLES_STEP_DUR 35 // BELL_RIPPLES wave step duration, less means faster wave
// No FADEOUT for MODE_CALM_WAVES or FADEIN for MODE_FAST_WAVES,
// since the former blends into the latter seamlessly
//
#define MODE_FAST_WAVES_DUR 30000
#define MODE_FAST_WAVES_WAVE_DICE 41 // Less means greater chance
#define MODE_FAST_WAVES_SPARKLE_DICE 11 // Less means greater chance
#define MODE_FAST_WAVES_SPARKLE_TIME_TO_LIVE 10
#define MODE_FAST_WAVES_BELL_STEP_DUR 30 // BELL wave step duration, less means faster wave
#define MODE_FAST_WAVES_SAWTOOTH_SMOOTH_FADE_STEP_DUR 10 // SAWTOOTH_SMOOTH_FADE wave step duration, less means faster wave
#define MODE_FAST_WAVES_BELL_RIPPLES_STEP_DUR 25 // BELL_RIPPLES wave step duration, less means faster wave
#define MODE_FAST_WAVES_FADEOUT 1000
//
#define MODE_RAINBOW_DUR 30000
#define MODE_RAINBOW_FADEIN 1000
#define MODE_RAINBOW_FADEOUT 1000
//
#define MODE_CHESS_1_DUR 30000
#define MODE_CHESS_1_DIVISIONS 10
#define MODE_CHESS_1_COLOR_FADE_MILLIS 1000
#define MODE_CHESS_1_PIXELS_PER_COLOR (NUM_PIXELS / MODE_CHESS_1_DIVISIONS)
#define MODE_CHESS_1_FADEIN 1000
#define MODE_CHESS_1_FADEOUT 1000
//
#define MODE_MARQUEE_1_DUR 30000
#define MODE_MARQUEE_1_DIVISIONS 10
#define MODE_MARQUEE_1_STEP_MILLIS 40
#define MODE_MARQUEE_1_COLORS_DUR 2000
#define MODE_MARQUEE_1_COL_XFADE_DUR 200
#define MODE_MARQUEE_1_FADEIN 1000
#define MODE_MARQUEE_1_FADEOUT 1000
//
#define MODE_THEATER_CHASE_DUR 30000
#define MODE_THEATER_CHASE_STEP_DUR 120
#define MODE_THEATER_CHASE_FADEIN 1000
#define MODE_THEATER_CHASE_FADEOUT 1000

typedef enum
{
    MODE_CALM_WAVES,
    MODE_FAST_WAVES,
    MODE_RAINBOW,      // ----- THIS MODE IS ADOPTED FROM ADAFRUIT NEOPIXELS STRANDTEST EXAMPLE -----
    MODE_CHESS_1,
    MODE_MARQUEE_1,
    MODE_THEATER_CHASE // ----- THIS MODE IS ADOPTED FROM ADAFRUIT NEOPIXELS STRANDTEST EXAMPLE -----
} Mode;
Mode g_mode = MODE_CALM_WAVES;

// Generic color type used
// by all modes / functions
typedef struct
{
    uint8_t r, g, b;
} Color;

// Current millis() value, taken at the start of current frame
// (current call to loop()), which means it stays the same for all
// functions called from loop() or functions called from them, etc.
unsigned long g_frameTime = 0L;
// Same as above, but previous value. Not terribly useful, but
// necessary..
unsigned long g_prevFrameTime = 0L;
// ..to calculate this, which is time difference between current
// frame (call to loop()) and the previous one.
unsigned long g_frameTimeDelta = 0L;
// Time when the current mode started
unsigned long g_modeStartMillis = 0L;
// The position step counter used by some modes (so they share this variable)
uint16_t g_modeStepCounterMillis = 0;
// Color duration counter used by some modes (so they share this variable)
uint16_t g_modeColorDurCounterMillis = 0;
// Color crossfade counter used by some modes (so they share this variable)
uint16_t g_modeColorXfadeCounterMillis = 0;

// --- MODE_CALM_WAVES / MODE_FAST_WAVES ---
// - Wave falloff tables -
typedef enum
{
    BELL = 0,
    SAWTOOTH_SMOOTH_FADE,
    BELL_RIPPLES
}
FalloffType; // Wave falloff type (index)

#define MAX_FALLOFF_VALUES 21 // Increase this value if you need longer waves
typedef struct
{
    uint8_t numValues; // How many values (how many lights) in this wave type, must be <= MAX_FALLOFF_VALUES
    uint8_t values[MAX_FALLOFF_VALUES]; // Falloff coefficient values. 255 is max brightness, 0 is off.
} Falloff;


#ifdef USE_PROGMEM

// These macros provide a universal way (whether we use PROGMEM or not)
// to access falloff arrays
#define FALLOFF_NUM_VALUES(fType) ((uint8_t)pgm_read_byte( &g_waveFalloffs[fType].numValues ))
#define FALLOFF_VALUE(fType, idx) ((uint8_t)pgm_read_byte( &g_waveFalloffs[fType].values[idx] ))
const PROGMEM Falloff g_waveFalloffs[] =

#else

// These macros provide a universal way (whether we use PROGMEM or not)
// to access falloff arrays
#define FALLOFF_NUM_VALUES(fType) (g_waveFalloffs[fType].numValues)
#define FALLOFF_VALUE(fType, idx) (g_waveFalloffs[fType].values[idx])
const Falloff g_waveFalloffs[] =

#endif
// ---!!!--- IMPORTANT: number of values must be less or equal to MAX_FALLOFF_VALUES ---!!!---
{   // Wave falloff coefficient values are uint8_t. 255 is max brightness, 0 is off.
    {21, {5, 10, 20, 38, 77, 133, 184, 217, 240, 252, 255, 252, 240, 217, 184, 133, 77, 38, 20, 10, 5}}, // BELL
    {12, {3, 6, 10, 18, 28, 41, 56, 82, 115, 158, 204, 255}},                                            // SAWTOOTH_SMOOTH_FADE
    {21, {5, 10, 36, 26, 107, 82, 184, 148, 240, 173, 255, 173, 240, 148, 184, 82, 107, 26, 36, 10, 5}}  // BELL_RIPPLES is a jagged bell curve
};
// - Wave falloff tables end -

// - Waves -
typedef struct
{
    bool active; // false means this slot is free
    int16_t pos; // Position (LED index) of wave start (pixel with MIN index). CAN be < 0 (tail still visible).
    int16_t step; // Current speed, can be positive, zero or negative
    uint16_t posStepBuf; // Milliseconds buffer, pos is updated by step when it overflows (becomes > posStepMillis)
    uint16_t posStepMillis; // How many millis before each pos step

    int16_t ttl; // Currently remaining time to live (in milliseconds), updated

    Color color; // Wave color (more precisely, wave apex color)
    FalloffType falloffType; // Index of falloff lookup table
} LedWave;

// How many waves simultaneously
LedWave g_waves[MODE_WAVES_MAX_WAVES];
uint8_t g_waveCount = 0; // How many waves currently present
unsigned long g_lastWaveGenMillis = 0;
// - Waves end -

// - Sparkles -
typedef struct
{
    bool active;
    uint8_t r, g, b;
    uint16_t pos;
    unsigned long birthMillis;
    uint16_t ttlMillis;
}
Sparkle;

Sparkle g_sparkles[MODE_WAVES_MAX_SPARKLES];
uint16_t g_sparkleCount = 0;
// - Sparkles end -
// --- MODE_CALM_WAVES / MODE_FAST_WAVES end ---

// --- MODE_CHESS_1 ---
Color g_chessColors[MODE_CHESS_1_DIVISIONS];
// We surely could use a single boolean to indicate even / odd positions
// going up / down (brigtness), but this way we can potentially implement
// other effects in the future.
bool g_chessColorsGoingUp[MODE_CHESS_1_DIVISIONS];
// --- MODE_CHESS_1 end ---

// --- MODE_MARQUEE_1 ---
Color g_marquee1Colors[3]; // Color 2 is intermediate between 0 and 1
Color g_marquee1NextColors[3]; // Color 2 is intermediate between 0 and 1
uint16_t g_marquee1StartPos = 0;
// --- MODE_MARQUEE_1 end ----

// ----- BEGIN CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_RGB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.
// ----- END CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----

void setup() // Contains code from Adafruit NeoPixel strandtest example
{
    // ----- BEGIN CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    #if defined (__AVR_ATtiny85__)
        if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
    #endif
    // End of trinket special code
    // ----- END CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----

#ifdef DEBUG
    Serial.begin(9600);
    while(!Serial); // Wait for serial port to connect. Needed for native USB.
#endif
    pinMode(DANGLING_ANALOG_PIN, INPUT);
    randomSeed(analogRead(DANGLING_ANALOG_PIN)); // Randomize using noise from unconnected analog pin
    clearWaves();
    clearSparkles();

    // ----- BEGIN CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    // ----- END CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
}

void loop()
{
    g_prevFrameTime = g_frameTime;
    g_frameTime = millis();
    g_frameTimeDelta = g_frameTime - g_prevFrameTime;
    unsigned long modeMillis = g_frameTime - g_modeStartMillis;

    switch(g_mode)
    {
    case MODE_CALM_WAVES:
        if(modeMillis < MODE_CALM_WAVES_FADEIN) // Fade in
            strip.setBrightness((modeMillis << 8) / MODE_CALM_WAVES_FADEIN);
        else
            if(modeMillis >= MODE_CALM_WAVES_DUR) // Next mode?
            {
                g_modeStartMillis = g_frameTime;
                g_mode = MODE_FAST_WAVES;
                //clearWaves(); // Don't remove existing waves
                //clearSparkles(); // or sparkles, this is a seamless transition.
                break;
            }
            else
                strip.setBrightness(0xFF);

        if(g_waveCount < MODE_WAVES_MAX_WAVES && random(MODE_CALM_WAVES_WAVE_DICE) == 0)
            newWave();
        if(g_sparkleCount < MODE_WAVES_MAX_SPARKLES && random(MODE_CALM_WAVES_SPARKLE_DICE) == 0)
            newSparkle();

        strip.clear();
        processWaves();
        processSparkles();
        strip.show();
        break;

    case MODE_FAST_WAVES:
        if(modeMillis >= MODE_FAST_WAVES_DUR) // Next mode?
        {
            g_modeStartMillis = g_frameTime;
            g_mode = MODE_RAINBOW;
            break;
        }
        else
            if(modeMillis >= MODE_FAST_WAVES_DUR - MODE_FAST_WAVES_FADEOUT) // Fade out
                strip.setBrightness(((MODE_FAST_WAVES_DUR - modeMillis) << 8) / MODE_FAST_WAVES_FADEOUT);
            else 
                strip.setBrightness(0xFF);

        if(g_waveCount < MODE_WAVES_MAX_WAVES && random(MODE_FAST_WAVES_WAVE_DICE) == 0)
            newWave();
        if(g_sparkleCount < MODE_WAVES_MAX_SPARKLES && random(MODE_FAST_WAVES_SPARKLE_DICE) == 0)
            newSparkle();

        strip.clear();
        processWaves();
        processSparkles();
        strip.show();
        break;

    // ----- THIS MODE IS ADOPTED FROM ADAFRUIT NEOPIXELS STRANDTEST EXAMPLE -----
    case MODE_RAINBOW:
        if(modeSwitch(MODE_CHESS_1, modeMillis, MODE_RAINBOW_DUR,
                      MODE_RAINBOW_FADEIN, MODE_RAINBOW_FADEIN))
        {
            initChess1();
            break;
        }
        rainbowMode(8);
        break;

    case MODE_CHESS_1:
        if(modeSwitch(MODE_MARQUEE_1, modeMillis, MODE_CHESS_1_DUR,
                      MODE_CHESS_1_FADEIN, MODE_CHESS_1_FADEOUT))
        {
            initMarquee1();
            break;
        }

        processChess1();
        break;

    case MODE_MARQUEE_1:
        if(modeSwitch(MODE_THEATER_CHASE, modeMillis, MODE_MARQUEE_1_DUR,
                      MODE_MARQUEE_1_FADEIN, MODE_MARQUEE_1_FADEOUT))
            break;

        processMarquee1();
        break;

    // ----- THIS MODE IS ADOPTED FROM ADAFRUIT NEOPIXELS STRANDTEST EXAMPLE -----
    case MODE_THEATER_CHASE:
        if(modeSwitch(MODE_CALM_WAVES, modeMillis, MODE_THEATER_CHASE_DUR,
                      MODE_THEATER_CHASE_FADEIN, MODE_THEATER_CHASE_FADEOUT))
        {
            clearWaves();
            clearSparkles();
            break;
        }
        theaterChaseRainbowMode(MODE_THEATER_CHASE_STEP_DUR);
        break;
    }
}

// Processes fade in / out / mode switching logic.
// Returns true if mode has been switched, so no further processing of current
// mode is to be carried out.
bool modeSwitch(Mode nextMode, unsigned long modeMillis, unsigned long modeDurMillis,
                unsigned long modeFadeinMillis, unsigned long modeFadeoutMillis)
{
    if(modeMillis < modeFadeinMillis) // Fade in
        strip.setBrightness((modeMillis << 8) / modeFadeinMillis);
    else
        if(modeMillis >= modeDurMillis) // Next mode?
        {
            g_modeStartMillis = g_frameTime;
            g_mode = nextMode;
            randomSeed(analogRead(DANGLING_ANALOG_PIN)); // Randomize using noise from unconnected analog pin
            return true; // Mode has been switched, don't process it anymore
        }
        else
            if(modeMillis >= modeDurMillis - modeFadeoutMillis) // Fade out
                strip.setBrightness(((modeDurMillis - modeMillis) << 8) / modeFadeoutMillis);
            else
                strip.setBrightness(0xFF); // Max brightness (stored internally as 0 by the lib).
    return false; // No mode switch performed
}

// Generate a nice bright juicy color.
// Returns the new color in Color structure
// provided by the caller (pCol).
// If contrasting parameter is true, generates a color
// contrasting (loosely) to that in pCol, otherwise color in pCol is
// ignored.
// If contrasting is true and pCol is white, black is generated.
// If whiteIsOk is false, white is off the table
// (only proper "colors" are generated).
void randomColor(Color* pCol, bool contrasting, bool whiteIsOk)
{
    // Skip for white, as "contrasting to red" would be invoked every time
    if(contrasting && (pCol->r != 255 || pCol->g != 255 || pCol->b == 255))
    {
        if(pCol->r == 255) // contrasting to red
        {
            pCol->r = random(13);
            switch(random(2))
            {
            case 0: // Green is the main color
                pCol->g = 255;
                if(random(1) == 0) // Blue as secondary..
                    pCol->b = random(128, 225);
                else // ..or none
                    pCol->b = random(13);
                return;
            case 1: // Blue is the main color
                pCol->b = 255;
                if(random(1) == 0) // Green as secondary..
                    pCol->g = random(128, 225);
                else // ..or none
                    pCol->g = random(13);
                return;
            }
        }
        else if(pCol->g == 255) // contrasting to green
        {
            pCol->g = random(13);
            switch(random(2))
            {
            case 0: // Red is the main color
                pCol->r = 255;
                if(random(1) == 0) // Blue as secondary..
                    pCol->b = random(128, 225);
                else // ..or none
                    pCol->b = random(13);
                return;
            case 1: // Blue is the main color
                pCol->b = 255;
                if(random(1) == 0) // Red as secondary..
                    pCol->b = random(128, 225);
                else // ..or none
                    pCol->b = random(13);
                return;
            }
        }
        else if(pCol->b == 255) // contrasting to blue
        {
            pCol->b = random(13);
            switch(random(2))
            {
            case 0: // Red is the main color
                pCol->r = 255;
                if(random(1) == 0) // Green as secondary..
                    pCol->g = random(128, 225);
                else // ..or none
                    pCol->g = random(13);
                return;
            case 1: // Green is the main color
                pCol->g = 255;
                if(random(1) == 0) // Red as secondary..
                    pCol->r = random(128, 225);
                else // ..or none
                    pCol->r = random(13);
                return;
            }
        }
    }
    
    // Just a random color
    unsigned long dice = random((whiteIsOk) ? 10 : 9);
    switch(dice)
    {
    case 0: // Red
        pCol->r = 255; // Red is the main color
        pCol->g = random(128, 225); // Green is the secondary color
        pCol->b = random(13);
        break;
    case 1: // Red
        pCol->r = 255; // Red is the main color
        pCol->g = random(13);
        pCol->b = random(128, 225); // Blue is the secondary color
        break;
    case 2: // Red
        pCol->r = 255; // Red is the main color, no secondary color
        pCol->g = random(33);
        pCol->b = random(33);
        break;
    case 3: // Green
        pCol->r = random(128, 225); // Red is the secondary color
        pCol->g = 255; // Green is the main color
        pCol->b = random(13);
        break;
    case 4: // Green
        pCol->r = random(13);
        pCol->g = 255; // Green is the main color
        pCol->b = random(128, 225); // Blue is the secondary color
        break;
    case 5: // Green
        pCol->r = random(33);
        pCol->g = 255; // Green is the main color, no secondary color
        pCol->b = random(33);
        break;
    case 6: // Blue
        pCol->r = random(128, 225); // Red is the secondary color
        pCol->g = random(13);
        pCol->b = 255; // Blue is the main color
        break;
    case 7: // Blue
        pCol->r = random(13);
        pCol->g = random(128, 225); // Green is the secondary color
        pCol->b = 255; // Blue is the main color
        break;
    case 8: // Blue
        pCol->r = random(33);
        pCol->g = random(33);
        pCol->b = 255; // Blue is the main color, no secondary color
        break;
     case 9: // White
        pCol->r = pCol->g = pCol->b = 255;
#ifdef DEBUG
        Serial.print(F("White generated, whiteIsOk = "));
        Serial.println(whiteIsOk ? F("true") : F("false"));
#endif
        break;        
    }
}

// Returns color between those pointed to by pCol1 and pCol2
// in the Color structure pIntermedCol points to
void intermediateColor(Color* pCol1, Color* pCol2, Color* pIntermedCol)
{
    pIntermedCol->r = ((uint16_t)(pCol1->r) + pCol2->r) >> 1;
    pIntermedCol->g = ((uint16_t)(pCol1->g) + pCol2->g) >> 1;
    pIntermedCol->b = ((uint16_t)(pCol1->b) + pCol2->b) >> 1;
}

// --- MODE_CALM_WAVES / MODE_FAST_WAVES functions ---
void clearWaves()
{
    for(int i = 0; i < MODE_WAVES_MAX_WAVES; i++)
        g_waves[i].active = false;

    g_waveCount = 0;
}

void newWave()
{
    for(int i = 0; i < MODE_WAVES_MAX_WAVES; i++)
        if(!g_waves[i].active)
        {
            g_waves[i].active = true;
            // Wave type and speed (inverse of speed, how many ms per step)
            FalloffType wType; // BELL / SAWTOOTH_SMOOTH_FADE / BELL_RIPPLES
            unsigned long dice = random(8); // I wanted less chance of BELL_RIPPLES waves
            if(dice <= 3) // 4/9 chance of BELL
            {
                wType = BELL;
                g_waves[i].posStepMillis = (g_mode == MODE_FAST_WAVES) ? MODE_FAST_WAVES_BELL_STEP_DUR : MODE_CALM_WAVES_BELL_STEP_DUR;
            }
            else if(dice <= 7) // 4/9 chance of SAWTOOTH_SMOOTH_FADE
            {
                wType = SAWTOOTH_SMOOTH_FADE;
                g_waves[i].posStepMillis = (g_mode == MODE_FAST_WAVES) ?
                    MODE_FAST_WAVES_SAWTOOTH_SMOOTH_FADE_STEP_DUR : MODE_CALM_WAVES_SAWTOOTH_SMOOTH_FADE_STEP_DUR;
                
            }
            else // 1/9 chance of BELL_RIPPLES
            {
                wType = BELL_RIPPLES;
                g_waves[i].posStepMillis = (g_mode == MODE_FAST_WAVES) ?
                    MODE_FAST_WAVES_BELL_RIPPLES_STEP_DUR : MODE_CALM_WAVES_BELL_RIPPLES_STEP_DUR;
            }
            g_waves[i].falloffType = wType;

            randomColor(&(g_waves[i].color), false, false);

            // Going up or down?
            if(random(2) == 0)
            {
                g_waves[i].pos = -FALLOFF_NUM_VALUES(wType) + 1; // +1 to stick out into the LED strip a bit
                g_waves[i].step = 1;  
            }
            else
            {
                g_waves[i].pos = NUM_PIXELS - 1; // -1 to stick out into the LED strip a bit
                g_waves[i].step = -1;
            }
            g_waves[i].posStepBuf = 0;
            g_waveCount++;

#ifdef DEBUG
            Serial.print(F("+++ New wave: step = "));
            Serial.print(g_waves[i].step);
            Serial.print(F("; posStepMillis = "));
            Serial.print(g_waves[i].posStepMillis);
            Serial.print(F("; R = "));
            Serial.print(g_waves[i].color.r);
            Serial.print(F(", G = "));
            Serial.print(g_waves[i].color.g);
            Serial.print(F(", B = "));
            Serial.println(g_waves[i].color.b);
#endif
            break;
        }
}

void processWaves()
{
    for(int i = 0; i < MODE_WAVES_MAX_WAVES; i++)
        if(g_waves[i].active)
        {
            // --- Apply wave color ---
            // As waves can move in either direction, we
            // must apply falloff coefficients in reverse order
            // when a wave is going backwards (step < 0)
            uint8_t falloffIdx = 0;
            int8_t falloffIdxStep = 0;
            if(g_waves[i].step > 0)
            {
                falloffIdx = 0;
                falloffIdxStep = 1;
            }
            else
            {
                falloffIdx = FALLOFF_NUM_VALUES(g_waves[i].falloffType) - 1;
                falloffIdxStep = -1;
            }

            int endIdx = g_waves[i].pos + FALLOFF_NUM_VALUES(g_waves[i].falloffType);
            for(int pixIdx = g_waves[i].pos; pixIdx < endIdx; pixIdx++, falloffIdx += falloffIdxStep) 
                if(pixIdx >= 0 && pixIdx < NUM_PIXELS)
                {
                    uint32_t pixColor = strip.getPixelColor(pixIdx);
                    // Add 1 to convert coefficient range from 0-255 to 1-256.
                    // When setting pixel color below, we multiply wave color by the
                    // coefficient [1-256], then divide it by 256 by shifting it 8 bits right.
                    // Thus, when coefficient value is 255 we first multiply wave color by (255 + 1),
                    // then divide it by 256 and the color remains unchanged. Any coefficient values
                    // below 255 have the same effect on the color as multiplication by values in
                    // 0.0 - 1.0 range.
                    uint16_t falloffCoeff = FALLOFF_VALUE(g_waves[i].falloffType, falloffIdx) + 1;
                    if(pixColor == 0) // No other wave to blend with
                        strip.setPixelColor(pixIdx,
                                            (g_waves[i].color.r * falloffCoeff) >> 8,
                                            (g_waves[i].color.g * falloffCoeff) >> 8,
                                            (g_waves[i].color.b * falloffCoeff) >> 8);
                    else // Blend with existing color
                    {
                        uint16_t newR = ((uint16_t)(g_waves[i].color.r * falloffCoeff) >> 8) + (uint16_t)(pixColor >> 16);
                        uint16_t newG = ((uint16_t)(g_waves[i].color.g * falloffCoeff) >> 8) + (uint16_t)((pixColor >> 8) & 0x000F);
                        uint16_t newB = ((uint16_t)(g_waves[i].color.b * falloffCoeff) >> 8) + (uint16_t)(pixColor & 0x000F);
                        // Clamp at 0xFF (maximum value for uint8_t)
                        if(newR > 0xFF) newR = 0xFF;
                        if(newG > 0xFF) newG = 0xFF;
                        if(newB > 0xFF) newB = 0xFF;
                        strip.setPixelColor(pixIdx, newR, newG, newB);
                    }
                }
            // Move the wave
            g_waves[i].posStepBuf += g_frameTimeDelta;
            if(g_waves[i].posStepBuf >= g_waves[i].posStepMillis)
            {
                g_waves[i].posStepBuf = 0;
                g_waves[i].pos += g_waves[i].step;
            }

            // Has the wave disappeared from the strip?
            if((g_waves[i].step > 0 && g_waves[i].pos >= (int16_t)NUM_PIXELS) ||
                (g_waves[i].step < 0 && g_waves[i].pos <= -FALLOFF_NUM_VALUES(g_waves[i].falloffType)))
            {
                g_waves[i].active = false;
                g_waveCount--;

#ifdef DEBUG
                Serial.print(F("--- Wave removed: step = "));
                Serial.print(g_waves[i].step);
                Serial.print(F("; posStepMillis = "));
                Serial.print(g_waves[i].posStepMillis);
                Serial.print(F("; R = "));
                Serial.print(g_waves[i].color.r);
                Serial.print(F(", G = "));
                Serial.print(g_waves[i].color.g);
                Serial.print(F(", B = "));
                Serial.println(g_waves[i].color.b);

                Serial.print(F("--- pos = "));
                Serial.print(g_waves[i].pos);
                Serial.print(F(", numPixels = "));
                Serial.print(NUM_PIXELS);
                Serial.print(F(", -numValues = "));
                //Serial.println(-g_waveFalloffs[g_waves[i].falloffType].numValues);
                Serial.println(-FALLOFF_NUM_VALUES(g_waves[i].falloffType));
#endif
            }
        }
}

void clearSparkles()
{
    for(int i = 0; i < MODE_WAVES_MAX_SPARKLES; i++)
        g_sparkles[i].active = false;

    g_sparkleCount = 0;
}

void newSparkle()
{
    for(int i = 0; i < MODE_WAVES_MAX_SPARKLES; i++)
        if(!g_sparkles[i].active)
        {
            g_sparkles[i].active = true;
            switch(random(2))
            {
            case 0:
                g_sparkles[i].r = g_sparkles[i].g = g_sparkles[i].b = 255;
                break;
            case 1:
                g_sparkles[i].r = g_sparkles[i].g = 255;
                g_sparkles[i].b = 0;
                break;
            }
            g_sparkles[i].pos = random(NUM_PIXELS - 1);
            g_sparkles[i].birthMillis = g_frameTime;
            g_sparkles[i].ttlMillis = (g_mode == MODE_CALM_WAVES) ? MODE_CALM_WAVES_SPARKLE_TIME_TO_LIVE : MODE_FAST_WAVES_SPARKLE_TIME_TO_LIVE;
            g_sparkleCount++; 
            break;
        }
}

void processSparkles()
{
    for(int i = 0; i < MODE_WAVES_MAX_SPARKLES; i++)
        if(g_sparkles[i].active)
        {
            // Apply sparkle color
            strip.setPixelColor(g_sparkles[i].pos,
                                g_sparkles[i].r,
                                g_sparkles[i].g,
                                g_sparkles[i].b);
            // Simple / naive update (fade) of sparkle color
            g_sparkles[i].r -= 10;
            g_sparkles[i].g -= 10;
            g_sparkles[i].b -= 10;
            // Is it time for this sparkle to go?
            if(g_frameTime - g_sparkles[i].birthMillis > g_sparkles[i].ttlMillis)
            {
                g_sparkles[i].active = false;
                g_sparkleCount--;
            }
        }
}
// --- MODE_CALM_WAVES / MODE_FAST_WAVES functions end ---

// --- MODE_RAINBOW functions ---
// ----- THIS MODE IS ADOPTED FROM ADAFRUIT NEOPIXELS STRANDTEST EXAMPLE -----
// ----- BEGIN CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
void rainbowMode(uint8_t wait)
{
    uint16_t i;
    static uint16_t j = 0;
    
    for(i = 0; i < NUM_PIXELS; i++)
        strip.setPixelColor(i, Wheel((i + j) & 255));

    strip.show();
    delay(wait);

    if(++j > 255)
        j = 0;
}
// --- MODE_RAINBOW functions end ---

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);

    if(WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
// ----- END CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----

// --- MODE_CHESS_1 ---
void initChess1()
{
    g_modeStepCounterMillis = 0;
    for(int i = 0; i < MODE_CHESS_1_DIVISIONS; i++)
    {
        g_chessColorsGoingUp[i] = i & 1;
        randomColor(&(g_chessColors[i]), false, false);
    }
}

void processChess1()
{
    uint8_t upFadeCoeff = (uint32_t)g_modeStepCounterMillis * 255 / MODE_CHESS_1_COLOR_FADE_MILLIS;
    uint8_t dnFadeCoeff = 255 - upFadeCoeff; // Precalculate to save time
    for(int i = 0, colIdx = 0, colPixIdx = 0; i < NUM_PIXELS; i++)
    {
        if(g_chessColorsGoingUp[colIdx])
            strip.setPixelColor(i,
                                (uint16_t)g_chessColors[colIdx].r * upFadeCoeff / 255,
                                (uint16_t)g_chessColors[colIdx].g * upFadeCoeff / 255,
                                (uint16_t)g_chessColors[colIdx].b * upFadeCoeff / 255);
        else
            strip.setPixelColor(i,
                                (uint16_t)g_chessColors[colIdx].r * dnFadeCoeff / 255,
                                (uint16_t)g_chessColors[colIdx].g * dnFadeCoeff / 255,
                                (uint16_t)g_chessColors[colIdx].b * dnFadeCoeff / 255);
        if(++colPixIdx >= MODE_CHESS_1_PIXELS_PER_COLOR)
        {
            colPixIdx = 0;
            colIdx++;
        }
    }

    g_modeStepCounterMillis += g_frameTimeDelta;
    if(g_modeStepCounterMillis >= MODE_CHESS_1_COLOR_FADE_MILLIS)
    {
        // Time to generate new colors for positions previously going down
        // and switch up / down positions
        g_modeStepCounterMillis = 0; // Reset color update timer
        for(int i = 0; i < MODE_CHESS_1_DIVISIONS; i++)
        {
            if(!g_chessColorsGoingUp[i])
                randomColor(&(g_chessColors[i]), false, false);
            g_chessColorsGoingUp[i] = !g_chessColorsGoingUp[i];
        }
    }
    strip.show();
}
// ---- MODE_CHESS_1 end ---

// --- MODE_MARQUEE_1 functions ---
void initMarquee1()
{
    g_modeStepCounterMillis = 0;
    // -- Initialize colors --
    // to be used now
    randomColor(&(g_marquee1Colors[0]), false, true);
    g_marquee1Colors[1] = g_marquee1Colors[0];
    randomColor(&(g_marquee1Colors[1]), true, true);
    intermediateColor(&(g_marquee1Colors[0]), &(g_marquee1Colors[1]), &(g_marquee1Colors[2]));
    // to be used next
    marquee1GenNextColors();
}

void marquee1GenNextColors()
{
    randomColor(&(g_marquee1NextColors[0]), false, true);
    g_marquee1NextColors[1] = g_marquee1NextColors[0];
    randomColor(&(g_marquee1NextColors[1]), true, true);
    intermediateColor(&(g_marquee1NextColors[0]), &(g_marquee1NextColors[1]), &(g_marquee1NextColors[2]));
}

void processMarquee1()
{
    // Cross-fade coefficient, between current colors and next set of colors.
    // 0 is 100% current colors, 255 is 100% next colors
    uint8_t xfadeCoeff = 0;
    uint8_t xfadeReciprCoeff = 0; // 255 - xfadeCoeff
    if(g_modeColorDurCounterMillis >= MODE_MARQUEE_1_COLORS_DUR - MODE_MARQUEE_1_COL_XFADE_DUR)
    {
        xfadeCoeff = ((uint32_t)g_modeColorDurCounterMillis - (MODE_MARQUEE_1_COLORS_DUR - MODE_MARQUEE_1_COL_XFADE_DUR) + 1) * 255 /
            MODE_MARQUEE_1_COL_XFADE_DUR;
        xfadeReciprCoeff = 255 - xfadeCoeff; // Precalculate to save some time
    }
    
    int pxlInCol = 0; // Index of pixel within its color
    int clrIdx = 0; // Color index (0 or 1)
    for(int i = g_marquee1StartPos; i < NUM_PIXELS; i++)
    {
        // Last pixel of each color is halfway between colors 0 and 1.
        // Helps avoid stair effect.
        int effectiveClrIdx = (pxlInCol == NUM_PIXELS / MODE_MARQUEE_1_DIVISIONS - 1) ? 2 : clrIdx;
        if(xfadeCoeff > 0) // Time to fade current colors into the next set
            strip.setPixelColor(i,
                                (g_marquee1Colors[effectiveClrIdx].r * xfadeReciprCoeff + g_marquee1NextColors[effectiveClrIdx].r * xfadeCoeff) / 255,
                                (g_marquee1Colors[effectiveClrIdx].g * xfadeReciprCoeff + g_marquee1NextColors[effectiveClrIdx].g * xfadeCoeff) / 255,
                                (g_marquee1Colors[effectiveClrIdx].b * xfadeReciprCoeff + g_marquee1NextColors[effectiveClrIdx].b * xfadeCoeff) / 255);
        else
            strip.setPixelColor(i,
                                g_marquee1Colors[effectiveClrIdx].r,
                                g_marquee1Colors[effectiveClrIdx].g,
                                g_marquee1Colors[effectiveClrIdx].b);

        if(++pxlInCol >= NUM_PIXELS / MODE_MARQUEE_1_DIVISIONS) // Switch to the next stripe?
        {
            pxlInCol = 0;
            clrIdx = (clrIdx == 0) ? 1 : 0;
        }
    }
    //
    for(int i = 0; i < g_marquee1StartPos; i++)
    {
        // Last pixel of each color is halfway between colors 0 and 1.
        // Helps avoid stair effect.
        int effectiveClrIdx = (pxlInCol == NUM_PIXELS / MODE_MARQUEE_1_DIVISIONS - 1) ? 2 : clrIdx;
        if(xfadeCoeff > 0) // Time to fade current colors into the next set
            strip.setPixelColor(i,
                                (g_marquee1Colors[effectiveClrIdx].r * xfadeReciprCoeff + g_marquee1NextColors[effectiveClrIdx].r * xfadeCoeff) / 255,
                                (g_marquee1Colors[effectiveClrIdx].g * xfadeReciprCoeff + g_marquee1NextColors[effectiveClrIdx].g * xfadeCoeff) / 255,
                                (g_marquee1Colors[effectiveClrIdx].b * xfadeReciprCoeff + g_marquee1NextColors[effectiveClrIdx].b * xfadeCoeff) / 255);
        else
            strip.setPixelColor(i,
                                g_marquee1Colors[effectiveClrIdx].r,
                                g_marquee1Colors[effectiveClrIdx].g,
                                g_marquee1Colors[effectiveClrIdx].b);

        if(++pxlInCol >= NUM_PIXELS / MODE_MARQUEE_1_DIVISIONS) // Switch to the next stripe?
        {
            pxlInCol = 0;
            clrIdx = (clrIdx == 0) ? 1 : 0;
        }
    }

    g_modeStepCounterMillis += g_frameTimeDelta;
    if(g_modeStepCounterMillis >= MODE_MARQUEE_1_STEP_MILLIS)
    {
        g_modeStepCounterMillis = 0;
        if(++g_marquee1StartPos >= NUM_PIXELS)
            g_marquee1StartPos = 0;
    }

    g_modeColorDurCounterMillis += g_frameTimeDelta;
    if(g_modeColorDurCounterMillis >= MODE_MARQUEE_1_COLORS_DUR)
    {
        g_modeColorDurCounterMillis = 0;
        // Switch to next colors completely
        g_marquee1Colors[0] = g_marquee1NextColors[0];
        g_marquee1Colors[1] = g_marquee1NextColors[1];
        g_marquee1Colors[2] = g_marquee1NextColors[2];
        // Generate new next colors
        marquee1GenNextColors();
    }

    strip.show();
}
// --- MODE_MARQUEE_1 functions end ---

// --- MODE_THEATER_CHASE functions ---
// ----- THIS MODE IS ADOPTED FROM ADAFRUIT NEOPIXELS STRANDTEST EXAMPLE -----
// ----- BEGIN CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
// Theatre-style crawling lights with rainbow effect
void theaterChaseRainbowMode(uint8_t wait)
{
    static int j = 0;
    for(int q = 0; q < 3; q++)
    {
        for(uint16_t i = 0; i < NUM_PIXELS; i = i + 3)
            strip.setPixelColor(i + q, Wheel((i+j) % 255)); // turn every third pixel on
        
        strip.show();
        delay(wait);
        
        for(uint16_t i = 0; i < NUM_PIXELS; i = i + 3)
            strip.setPixelColor(i + q, 0); // turn every third pixel off
    }

    if(++j > 255) // cycle all 256 colors in the wheel
        j = 0;
}
// ----- END CODE FROM ADAFRUIT NEOPIXEL STRANDTEST EXAMPLE -----
// --- MODE_THEATER_CHASE functions end ---
