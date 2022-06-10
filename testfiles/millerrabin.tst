TestMillerRabinValue{
  !assert: value(10) value(2);
  assert: value(7) value(2);
  assert: value(7) value(3);
  assert: value(7) value(4);
  assert: value(101) value(50);

  assert: value(25) value(7);
  !assert: value(25) value(8);
  assert: value(158997432850433) value(2137);
  assert: value(158997432850433) value(531945);
  assert: value(158997432850433) value(9385329);
  assert: value(158997432850433) value(4);
  !assert: value(2851263695) value(2873016484);
}
