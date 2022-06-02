NegativeShiftComplementTest{
  assert: value(10) value(3) value(2);
  assert: value(64) value(0) value(64);
  assert: value(1024) value(1) value(512);
}

LeftShiftValueTest{
  assert: value(256) value(4) value(4096);
  assert: value(3) value(2) value(12);
  assert: value(0) value(4) value(0);
  assert: value(2137) value(0) value(2137);
}

RightShiftValueTest{
  assert: value(256) value(4) value(16);
  assert: value(3) value(2) value(0);
  assert: value(10) value(2) value(2);
  assert: value(0) value(4) value(0);
  assert: value(2137) value(0) value(2137);
}

ShiftSelfTest{
  case(100): unsigned(1024) range(0,1024);
}
