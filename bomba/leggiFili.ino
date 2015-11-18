void leggiFili()
{
  if(!tempoTagliato)
  {
    int val = digitalRead(filoTempo);
    if (val > 0)
    {
      tempoTagliato = true;
      tempoDelay -= 60;
      decimiDaTogliere = 6;
      return;
    }
  }



  if (!secondiTagliato)
  {
    int val = digitalRead(filo30Secondi);
    if (val > 0)
    {
      secondiTagliato = true;
      if (minuti > 0 || secondi > 30)
      {
        minuti = 0;
        secondi = 30;
        return;
      }
    }
  }

  if(!disinnescoTagliato)
  {
    int val = digitalRead(filoDisinnesco);
    if (val > 0)
    {
      disinnescoTagliato = true;
      defused();
      return;
    }
  }


  if(!glitchTagliato)
  {
    int val = digitalRead(filoGlitch);
    if (val > 0)
    {
      glitchTagliato = true;
      return;
    }
  }


  int valTilt = digitalRead(tilt);

  if (valTilt != tiltStatus)
  {
    boom();
    return;
  }


  if(opzionale) 
    if (!boomTagliato)
    {
      int val = digitalRead(filoBoom);
      if (val > 0)
      {
        boomTagliato = true;
        boom();
      }
    }

}




