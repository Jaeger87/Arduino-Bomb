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
    }
  }

  if(!disinnescoTagliato)
  {
    int val = digitalRead(filoDisinnesco);
    if (val > 0)
    {
      disinnescoTagliato = true;
      defused();
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
      }
    }
  }

}


