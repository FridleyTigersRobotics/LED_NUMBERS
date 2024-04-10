#include <FastLED.h>    // if not found, get it here http://fastled.io/

#define DEFAULT_ID 8
#define NUM_LEDS 64
#define NUM_LEDS_IN_2 ( 18 )
#define NUM_LEDS_IN_7 ( 10 )

#define LED_CONTROL_PIN 3
#define PATTERN_CHANGE_PIN 2

#define NUMDROPS 20

CRGB leds[NUM_LEDS];

// Pattern Functions
void StripeHorizontal(void);
void DotChase(void);
void Meter(void);
void Chant(void);
void RainbowFull(void);
void RainbowChase(void);
void ColorPick(void);
void ColorSplash(void);
void NumBounce(void);


void ( *PatternFunctions[] )( void ) = {
  StripeHorizontal,
  DotChase,
  Meter,
  Chant,
  RainbowFull,
  RainbowChase,
  ColorPick,
  ColorSplash,
  NumBounce
};
uint8_t numPatterns = sizeof( PatternFunctions ) / sizeof( PatternFunctions[0] );

CRGB GoldColor = CRGB(0xFF, 0xDF, 0x00);
CRGB ColorA = CRGB::White;
CRGB ColorB = GoldColor;

uint8_t  patternId = DEFAULT_ID;
uint16_t buttonCount = 0;


uint32_t patternHoldCount = 0;


// This function sets up the ledsand tells the controller about them
void setup() {
    pinMode(PATTERN_CHANGE_PIN, INPUT);
    Serial.begin(9600);//begin serial feedback on usb port
    // sanity check delay - allows reprogramming if accidently blowing power w/leds
    Serial.println("wait...");
    delay(2000);
    Serial.println("Starting");
    FastLED.addLeds<UCS1903, LED_CONTROL_PIN, BRG>(leds, NUM_LEDS);           // <- this part
    randomSeed(analogRead(A1));
}



void loop() {
  if( digitalRead( PATTERN_CHANGE_PIN ) )
  {
    if ( buttonCount == 2 )
    {
      patternId++;
      patternId %= numPatterns;


      Serial.print("patternId update: ");
      Serial.println(patternId);

      patternHoldCount = 0;
    }
    
    if ( buttonCount < 10 )
    {
      buttonCount++;
    }
  }
  else
  {
    buttonCount = 0;
  }

   PatternFunctions[patternId]();
   
   FastLED.show();
   delay( 10 );
   patternHoldCount++;
}







void StripeHorizontal(void)
{
    uint16_t delayAfterShow = analogRead( A0 );
   if ( patternHoldCount < delayAfterShow / 10 )
   {
      return;
   }
   else
   {
      patternHoldCount = 0;
   }
   
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) 
   {
      if ( whiteLed < NUM_LEDS_IN_7 )
      {
        leds[whiteLed] = ColorA;
      }
      else if ( whiteLed < NUM_LEDS_IN_7 + NUM_LEDS_IN_2 )
      {
        leds[whiteLed] = ColorB;
      }
      else if ( whiteLed < NUM_LEDS_IN_7 + NUM_LEDS_IN_2 + NUM_LEDS_IN_2 )
      {
        leds[whiteLed] = ColorA;
      }
      else if ( whiteLed < NUM_LEDS_IN_7 + NUM_LEDS_IN_2 + NUM_LEDS_IN_2 + NUM_LEDS_IN_2 )
      {
        leds[whiteLed] = ColorB;
      }
   }
  {
    CRGB temp = ColorA;
    ColorA = ColorB;
    ColorB = temp;
  }
}




void DotChase(void)
{
   static uint8_t ledIdx = 0;
   uint16_t delayAfterShow = analogRead( A0 );
   if ( patternHoldCount < ( delayAfterShow / 10 ) )
   {
      return;
   }
   else
   {
      patternHoldCount = 0;
   }
   
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) 
   {
      if ( whiteLed == ledIdx )
      {
        leds[whiteLed] = CRGB::White;
      }
      else
      {
        leds[whiteLed] = CRGB::Black;
      }
   }

   ledIdx++;
   ledIdx %= NUM_LEDS;
}

void Meter(void)
{
   if ( patternHoldCount < 1 )
   {
      return;
   }
   else
   {
      patternHoldCount = 0;
   }
   
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) 
   {
      if ( float(analogRead( A0 )) / 1024.0 > float(whiteLed) / float(NUM_LEDS) )
      {
        leds[whiteLed] = CRGB::White;
      }
      else
      {
        leds[whiteLed] = CRGB::Black;
      }
   }
}




void Chant(void)
{
  static uint8_t chantIdx2 = 0;
  uint8_t chantIdx = chantIdx2 / 2;
  uint16_t delayAfterShow = 0;
   if ( chantIdx2 % 2 == 0 )
   {
     if ( chantIdx < 4 )
     {
      delayAfterShow = 300;
     }
     else
     {
      delayAfterShow = 130;
     }
   }
   else
   {
      delayAfterShow = 30;
   }

   if ( patternHoldCount < (delayAfterShow / 10) )
   {
      return;
   }
   else
   {
      patternHoldCount = 0;
   }

   
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) 
   {
     if ( chantIdx2 % 2 == 1 )
     {
        leds[whiteLed] = CRGB::Black;
        continue;
     }

    
      if ( whiteLed < NUM_LEDS_IN_7 )
      {
        if( chantIdx == 3 )
        {
          leds[whiteLed] = CRGB::White;
        }
        else if ( chantIdx == 4 )
        {
          leds[whiteLed] = GoldColor;
        }
        else
        {
          leds[whiteLed] = CRGB::Black;
        }
        
      }
      else if ( whiteLed < NUM_LEDS_IN_7 + NUM_LEDS_IN_2 )
      {
        if( chantIdx == 2 )
        {
          leds[whiteLed] = CRGB::White;
        }
        else
        {
          leds[whiteLed] = CRGB::Black;
        }
      }
      else if ( whiteLed < NUM_LEDS_IN_7 + NUM_LEDS_IN_2 + NUM_LEDS_IN_2 )
      {
        if( chantIdx == 1 )
        {
          leds[whiteLed] = CRGB::White;
        }
        else
        {
          leds[whiteLed] = CRGB::Black;
        }
      }
      else if ( whiteLed < NUM_LEDS_IN_7 + NUM_LEDS_IN_2 + NUM_LEDS_IN_2 + NUM_LEDS_IN_2 )
      {
        if( chantIdx == 0 )
        {
          leds[whiteLed] = CRGB::White;
        }
        else
        {
          leds[whiteLed] = CRGB::Black;
        }
      }
   }
  {
    CRGB temp = ColorA;
    ColorA = ColorB;
    ColorB = temp;
  }


  chantIdx2++;
  chantIdx2 %= 10;
}





void RainbowFull(void)
{
   static uint8_t currentHue = 0;
   uint16_t delayAfterShow = analogRead( A0 );

   if ( patternHoldCount < (delayAfterShow / 10) )
   {
      return;
   }
   else
   {
      patternHoldCount = 0;
   }
   
   for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed = whiteLed + 1) 
   {
    leds[whiteLed].setHSV( currentHue, 255, 255 );
   }
   
   currentHue++;
}

void RainbowChase(void)
{
   static uint8_t stepcount = 0;
   uint16_t delayAfterShow = analogRead( A0 );

   if ( patternHoldCount < (delayAfterShow / 10) )
   {
      return;
   }
   else
   {
      patternHoldCount = 0;
      stepcount++;
   }
   
   for(int ledbank = 0; ledbank < NUM_LEDS; ledbank = ledbank + 1) 
   {
    leds[ledbank].setHSV( ledbank+stepcount, 255, 255 );
   }
}

void ColorPick(void)
{
   uint16_t colorHue = map(analogRead(A0),0,1023,0,255);
   for(int ledbank = 0; ledbank < NUM_LEDS; ledbank = ledbank + 1) 
   {
    leds[ledbank].setHSV( colorHue, 255, 255 );
   }
}

void ColorSplash(void)
{
   uint16_t delayAfterShow = analogRead( A0 );

  if ( patternHoldCount < (delayAfterShow / 10) )
  {
     return;
  }
  else
  {
    patternHoldCount = 0;
  }


  static uint16_t step = 1;
  static bool newsplash = true;
  static uint16_t splashtrack [NUMDROPS];
  uint16_t numsplash = analogRead(A0)/10;
  static uint16_t colorHue = random(255);

  if(newsplash==true){
    colorHue = random(255);
    for (int drops = 0; drops < NUMDROPS; drops = drops + 1)
    {
      splashtrack[drops] = random(NUM_LEDS);
      newsplash=false;
    }
  }
  bool showthis=false;
  if(newsplash==false)
  {
    for(int ledbank = 0; ledbank < NUM_LEDS; ledbank = ledbank + 1) 
    {
      for(int i=0; i<NUMDROPS; i = i + 1)
      {
        if(step==1)
        {
          if(splashtrack[i]==ledbank) showthis=true;
        }
        if(step==2)
        {
          if((splashtrack[i]-1==ledbank)||(splashtrack[i]+1==ledbank)||(splashtrack[i]==ledbank)) showthis=true;
        }
        if(step==3)
        {
          if((splashtrack[i]-2==ledbank)||(splashtrack[i]+2==ledbank)||(splashtrack[i]-1==ledbank)||(splashtrack[i]+1==ledbank)||(splashtrack[i]==ledbank)) showthis=true;
        }
       
      }
      if(showthis){
        leds[ledbank].setHSV( colorHue, 255, 255 );
        showthis=false;
      }
      else{
        leds[ledbank] = CRGB::Black;
      }

    }
  }
    step++;
    if(step==4)
      {
      step=1;
      newsplash=true;
     }
}


void NumBounce(void)
{
  static int stepsgn = -1;
  uint16_t delayAfterShow = analogRead( A0 );
  if ( patternHoldCount < (delayAfterShow / 10) )
  {
     return;
  }
  else
  {
    patternHoldCount = 0;
  }
  static uint16_t dispnum = 1;
  static uint16_t colorHue = random(255);
  if (dispnum == 1)
  {
    for(int ledstep = 0; ledstep < NUM_LEDS; ledstep++) 
    {
    leds[ledstep] = CRGB::Black;
    }
    colorHue = random(255);
    stepsgn=0-stepsgn;
  } 
  if (dispnum == 2)
  {
    for(int ledstep = 0; ledstep < NUM_LEDS; ledstep++) 
    {
      if ( ledstep < NUM_LEDS_IN_7 ) leds[ledstep].setHSV( colorHue, 255, 255 );
      else leds[ledstep] = CRGB::Black;
    }
  }
  if (dispnum == 3)
  {

    for(int ledstep = 0; ledstep < NUM_LEDS; ledstep++) 
    {
      if ( (ledstep > NUM_LEDS_IN_7) && (ledstep<NUM_LEDS_IN_7+NUM_LEDS_IN_2) ) leds[ledstep].setHSV( colorHue, 255, 255 );
      else leds[ledstep] = CRGB::Black;
    }
  }
  if (dispnum == 4)
  {
    for(int ledstep = 0; ledstep < NUM_LEDS; ledstep++) 
    {
      if ( (ledstep > NUM_LEDS_IN_7+NUM_LEDS_IN_2) && (ledstep<NUM_LEDS_IN_7+NUM_LEDS_IN_2*2) ) leds[ledstep].setHSV( colorHue, 255, 255 );
      else leds[ledstep] = CRGB::Black;
    } 
  }
  if (dispnum == 5)
  {
    for(int ledstep = 0; ledstep < NUM_LEDS; ledstep++) 
    {
      if ( (ledstep > NUM_LEDS_IN_7+NUM_LEDS_IN_2*2) && (ledstep<NUM_LEDS_IN_7+NUM_LEDS_IN_2*3) ) leds[ledstep].setHSV( colorHue, 255, 255 );
      else leds[ledstep] = CRGB::Black;
    }
  }
  if (dispnum == 6)
  {
    for(int ledstep = 0; ledstep < NUM_LEDS; ledstep++) 
    {
    leds[ledstep] = CRGB::Black;
    }
    colorHue = random(255);
    stepsgn=0-stepsgn;
  }
  dispnum+=stepsgn;
}