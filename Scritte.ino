void writeDefused()
{
  lc.clearDisplay(0); 
  lc.setDigit(0,6,13,false);//d
  lc.setDigit(0,5,14,false);//e
  lc.setDigit(0,4,15,false);//f
  lc.setDigit(0,3,17,false);//u
  lc.setDigit(0,2,5,false);//s
  lc.setDigit(0,1,14,false);//e
  lc.setDigit(0,0,13,false);//d
}

void writeYouDead()
{
  lc.clearDisplay(0); 
  lc.setDigit(0,7,16,false);//Y
  lc.setDigit(0,6,0,false);//o
  lc.setDigit(0,5,17,false);//u
  lc.setDigit(0,3,13,false);//d
  lc.setDigit(0,2,14,false);//e
  lc.setDigit(0,1,10,false);//a
  lc.setDigit(0,0,13,false);//d
}
