#include "LedControl.h"
#include "types.h"

// Arduino Pin 7 to DIN, 6 to Clk, 5 to LOAD, no.of devices is 1 
LedControl lc=LedControl(7,6,5,1);


enum  stati {
  START,SETTIME,INFUNZIONE, GAMEOVER};

stati statoBomba = START; 

int numNote = 16;
int nota[] = {
  261, 528, 261, 528, 261, 528, 261, 528,  261, 528, 261, 528, 261, 528, 261, 528};

const byte buttonTime = 9;
const byte buttonStart = 8;
const byte buzzer = 2;
byte filoDisinnesco = 10;
byte filoTempo = 11;
byte filo30Secondi = 12;
byte minuti = 0;
byte secondi = 4;
byte decimiSecondi = 5;
int tempoDelay = 50;
boolean premuto = false;
boolean tempoTagliato = false;
boolean disinnescoTagliato = false;
boolean secondiTagliato = false;
boolean esplosa = false;
byte decimiDaTogliere = 1;


void setup() 
{ 
  Serial.begin(9600);
  // Initialize the MAX7219 device 
  lc.shutdown(0,false);
  // Enable display 
  lc.setIntensity(0,10);
  // Set brightness level (0 is min, 15 is max) 
  lc.clearDisplay(0); 
  // Clear display register

  minuti = 0;
  secondi = 0;
  decimiSecondi = 0;
  decimiDaTogliere = 1;
  scriviTempo(minuti, secondi, decimiSecondi);

  tempoTagliato = false;
  disinnescoTagliato = false;
  secondiTagliato = false;
  esplosa = false;
  premuto = false;
  randomSeed(analogRead(0));
  settaFili();

  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
} 


void loop() 
{
  switch(statoBomba)
  {
  case START:
    {
      setup();
      statoBomba = SETTIME;
      break;
    }
  case SETTIME:
    {
      int val = digitalRead(buttonTime);

      if (val == HIGH)
      {
        if (!premuto)
        {
          if(minuti == 255)
            minuti++;
          minuti += 1;
          scriviTempo(minuti, secondi, decimiSecondi);
          premuto = true;
          tone(buzzer, 700);
          delay(50);
          noTone(buzzer);
        }
      }
      else 
        premuto = false;

      val = val = digitalRead(buttonStart);
      if (val == HIGH && minuti > 0)
      {
        statoBomba = INFUNZIONE;
        tempoDelay = 100;
        tone(buzzer, 770);
        delay(110);
        tone(buzzer, 870);
        delay(180);
        noTone(buzzer);
        delay(100);
      }
      break; 
    }

  case INFUNZIONE:
    {
      delay(tempoDelay);
      scriviTempo(minuti, secondi, decimiSecondi); 
      if(minuti == 0 && secondi == 0 && decimiSecondi == 0)
      {
        esplosa = true;
        boom();
      }
      if (!esplosa)
      {
        aggiornaTempo();
        leggiFili();
      }
      break;
    }

  case GAMEOVER:
    {
      Serial.println("dio cane");
      int val = digitalRead(buttonStart);
      if (val == HIGH)
      {
        statoBomba = START;
        tone(buzzer, 50);
        delay(120);
        noTone(buzzer);
        delay(300);
      }
      break;
    }


  }



} 

void scriviTempo(byte minuti, byte secondi, byte decimiSecondi)
{
  lc.clearDisplay(0); 
  if (minuti >= 100)
  {
    byte centinaia = minuti/ 100;
    lc.setDigit(0,7,centinaia,false);
    minuti -= centinaia * 100;
  }
  lc.setDigit(0,6,minuti/10,false);
  lc.setDigit(0,5,minuti%10,true);
  lc.setDigit(0,3,secondi/10,false);
  lc.setDigit(0,2,secondi%10,true);
  lc.setDigit(0,0,decimiSecondi,false);
}




void aggiornaTempo()
{
  byte vecchiMinuti = minuti;
  decimiSecondi -= decimiDaTogliere;
  if(decimiSecondi > 10)
  {
    decimiSecondi = 9;
    secondi--;
  }
  if(secondi > 61)
  {
    secondi = 59;
    minuti--; 
  }

  if (vecchiMinuti < minuti)
  {
    minuti = 0;
    secondi = 0;
    decimiSecondi = 0;
  }
}



void settaFili()
{
  filoDisinnesco = int(random(10, 14)); 
  filoTempo = int(random(10, 14)); 
  filo30Secondi = int(random(10, 14));
  while(filoDisinnesco == filoTempo)
  {
    filoTempo = int(random(10, 14));
  }
    while(filoTempo == filo30Secondi)
  {
    filo30Secondi = int(random(10, 14));
  }
  
}








