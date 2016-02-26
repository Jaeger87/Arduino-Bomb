void boom()
{
  digitalWrite(sirena, HIGH);
  writeYouDead();
  for (int i = 0; i < numNote; i++)
  {
 //   tone(buzzer, nota[i]);
    delay(350);
    noTone(buzzer);
  }

  statoBomba = GAMEOVER;
}


void defused()
{
  writeDefused();
//  tone(buzzer, 700);
  delay(550);
  //tone(buzzer, 860);
  delay(550);
  //tone(buzzer, 900);
  delay(1580);
  noTone(buzzer);
  statoBomba = GAMEOVER;
}

void noob()
{
  digitalWrite(sirena, HIGH);
  writeYouNoob();
    for (int i = 0; i < numNote; i++)
  {
//    tone(buzzer, nota[i]);
    delay(350);
    noTone(buzzer);
  }

  statoBomba = GAMEOVER;
}
