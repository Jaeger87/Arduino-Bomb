#include "LedControl.h"
#include "types.h"

// Arduino Pin 7 to DIN, 6 to Clk, 5 to LOAD, no.of devices is 1 
LedControl lc=LedControl(7,6,5,1);


enum  stati {
  START,SETTIME,INFUNZIONE, GAMEOVER};

stati statoBomba = START; 

int porteFili[] = {
  512,256,128,64,32,16};

int numNote = 16;
int nota[] = {
  261, 528, 261, 528, 261, 528, 261, 528,  261, 528, 261, 528, 261, 528, 261, 528};

const byte rumor = 15;

const byte minFilo = 8;
const byte maxFilo = 13;


const byte buttonTime = 4;
const byte buttonStart = 3;
const byte buzzer = 2;
const byte sirena = 13;
const int tilt = A0;

int tiltStatus;
int filoGlitch = 512;
int filoBoom = 256;
int filoDisinnesco = 128;
int filoTempo = 64;
int filo30Secondi = 32;
byte minuti = 0;
byte secondi = 4;
byte decimiSecondi = 5;
int tempoDelay = 50;
boolean premuto = false;
boolean tempoTagliato = false;
boolean disinnescoTagliato = false;
boolean secondiTagliato = false;
boolean boomTagliato = false;
boolean glitchTagliato = false;
boolean esplosa = false;
boolean startUp = true;
boolean opzionale = true;
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
  
  inzializzaPorteFili();

for(int h = 0; h < 6; h++)
Serial.println(porteFili[h]);
  
  minuti = 0;
  secondi = 0;
  decimiSecondi = 0;
  decimiDaTogliere = 1;
  scriviTempo(minuti, secondi, decimiSecondi);

  tempoTagliato = false;
  disinnescoTagliato = false;
  secondiTagliato = false;
  boomTagliato = false;
  glitchTagliato = false;
  esplosa = false;
  premuto = false;
  opzionale = true;
  randomSeed(analogRead(0));

  pinMode(tilt, INPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);
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

      val = digitalRead(buttonStart);

      if (val == HIGH && minuti > 0)
      {
        int fili = checkFili();
        if (fili == 8)
        {
          writeError(fili);
          delay(15);
        }
        else
        {
          if(fili == 0)
            opzionale = false;
          settaFili();
Serial.print("tempo");
Serial.println(filoTempo);
Serial.print("glitch ");
Serial.println(filoGlitch); 
Serial.print("disinnesco ");
Serial.println(filoDisinnesco);     
      Serial.print("boom");    
            Serial.println(filoBoom);
      Serial.print("30 secondi");    
            Serial.println(filo30Secondi); 
          startUp = true;
          statoBomba = INFUNZIONE;
          tempoDelay = 100;
          tiltStatus = digitalRead(tilt);
          tone(buzzer, 770);
          delay(110);
          tone(buzzer, 870);
          delay(180);
          noTone(buzzer);
          delay(100);
        }
      }
      break; 
    }

  case INFUNZIONE:
    {
      delay(tempoDelay);
      noTone(buzzer);
      if(glitchTagliato)
        scriviGlitch();
      else
        scriviTempo(minuti, secondi, decimiSecondi); 
      if(minuti == 0 && secondi == 0 && decimiSecondi == 0 || startButtonIsLow())
      {
        esplosa = true;
        if(startButtonIsLow())
        {
          noob();
          startUp = false;
        }
        else
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

      if (startButtonIsLow() && startUp)
      {
        statoBomba = START;
        digitalWrite(sirena, LOW);
        tone(buzzer, 50);
        delay(120);
        noTone(buzzer);
        delay(300);
      }

      if (!startButtonIsLow())
      {
        startUp = true;
      }
      delay(15);
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


void scriviGlitch()
{
  lc.clearDisplay(0); 
  if (minuti >= 100)
  {
    byte centinaia = minuti/ 100;
    lc.setDigit(0,7,centinaia,false);
    minuti -= centinaia * 100;
  }
  lc.setDigit(0,6,int(random(0, 54)),false);
  lc.setDigit(0,5,int(random(0, 54)),false);
  lc.setDigit(0,3,int(random(0, 54)),false);
  lc.setDigit(0,2,int(random(0, 54)),false);
  lc.setDigit(0,0,int(random(0, 54)),false);
}

void aggiornaTempo()
{
  byte vecchiMinuti = minuti;
  decimiSecondi -= decimiDaTogliere;
  if(decimiSecondi > 10)
  {
    decimiSecondi = 9;
    secondi--;
    tone(buzzer, 900);
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
  if(opzionale)
  {
    Serial.println("OOOK");
    filoDisinnesco = assegnaFilo(5);
    filoTempo = assegnaFilo(4);
    filo30Secondi = assegnaFilo(3);
    filoBoom = assegnaFilo(2);
    filoGlitch = assegnaFilo(1);
  }
  else
  {
    filoTempo = assegnaFilo(4);
    filo30Secondi = assegnaFilo(3);
    filoDisinnesco = assegnaFilo(2);
    filoGlitch = assegnaFilo(1);
  }
}  

boolean startButtonIsLow()
{
  int val = digitalRead(buttonStart);
  return val == LOW;
}

int checkFili() //RIFARE
{
  int wiresValue = cleanRumor(analogRead(A7));
  if (wiresValue > 1007)
    return 1;
  return 8;
}

int assegnaFilo(int maxIndice)
{
  maxIndice++;
  int sizeArray = sizeof(porteFili) / sizeof(int);
  int indice = int(random(1, maxIndice));
  int portaScelta = porteFili[indice];
  int indiceUltimo = sizeArray - (sizeArray - maxIndice) -1;
  porteFili[indice] = porteFili[indiceUltimo];
  porteFili[indiceUltimo] = portaScelta;
  return portaScelta;
}




void inzializzaPorteFili()
{
  for(int i = 4; i < 10; i++)
      porteFili[i - 4] = (int)pow(2,i) + 1;
}





