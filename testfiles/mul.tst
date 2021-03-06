
DivisionValueTest{
  assert: value(21) value(6) value(3);
  assert: value(99) value(10) value(9);
  assert: value(5) value(2) value(2);
  assert: value(4) value(2) value(2);
  assert: value(777) value(21) value(37);
  assert: value(100) value(11) value(9);
}

DivisionFloorTest{
  case(1000): unsigned(4) unsigned(2);
}

NastyDivisionFloorTest{
  case(100) : unsigned(8) unsigned(8);
  case(100) : unsigned(4) unsigned(8);
  case(100) : unsigned(8) unsigned(4);
  case(100) : unsigned(32) unsigned(32);
}

MultiplicationDivisionSelfTest{
  assert: binary(01111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111) binary(11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111);
  case(1) : unsigned(1024) unsigned(1024);
  case(10) : unsigned(128) unsigned(128);
  case(10) : unsigned(64) unsigned(64);
  case(10) : unsigned(32) unsigned(64);
  case(10) : unsigned(64) unsigned(32);
  case(10) : unsigned(64) unsigned(16);
  case(10) : unsigned(16) unsigned(64);
  case(10) : unsigned(4) unsigned(64);
  case(10) : unsigned(64) unsigned(4);
  case(10) : unsigned(128) unsigned(2);
  case(10) : unsigned(2) unsigned(128);
  case(10) : unsigned(1) unsigned(1);
  case(100) : unsigned(32) unsigned(32);
}

SmallModuloValueTest{
  assert: value(871584379632187569843216329857319875631874) value(432652) value(269338);
}

