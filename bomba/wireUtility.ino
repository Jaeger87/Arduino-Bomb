int cleanRumor(int value)
{
    return value - (value & rumor);
}


bool checkWire(int wiresValue, int wire)
{
  int val = wiresValue & wire;
  return !(val > 0);
}

