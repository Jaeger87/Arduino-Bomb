void leggiFili()
{
  int wireValue = cleanRumor(analogRead(A7));
  if(!tempoTagliato)
  {
    
    if (checkWire(wireValue, filoTempo))
    {
      Serial.println(wireValue);
      Serial.println("tempo");
      tempoTagliato = true;
      tempoDelay -= 60;
      decimiDaTogliere = 6;
      return;
    }
  }



  if (!secondiTagliato)
  {
    if (checkWire(wireValue, filo30Secondi))
    {
      Serial.println(wireValue);
      Serial.println("secondi");
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
    if (checkWire(wireValue, filoDisinnesco))
    {
            Serial.println(wireValue);
      Serial.println("disinnesco");
      disinnescoTagliato = true;
      defused();
      return;
    }
  }


  if(!glitchTagliato)
  {
    if (checkWire(wireValue, filoGlitch))
    {
            Serial.println(wireValue);
      Serial.println("glitch");
      glitchTagliato = true;
      return;
    }
  }

/*
  int valTilt = digitalRead(tilt);

  if (valTilt != tiltStatus)
  {
    boom();
    return;
  }
*/

  if(opzionale) 
    if (!boomTagliato)
    {

      if (checkWire(wireValue, filoBoom))
      {
                    Serial.println(wireValue);
      Serial.println("boom");
        boomTagliato = true;
        boom();
      }
    }

}




