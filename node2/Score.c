void Score_Init()
{

}

void Score_Update()
{

  // Read hte value converted by the ADC
  uint8_t value = ADC_Read();

  if (value < 10)
  {
    score_stack--;
  }
  // If the ball is not more on the bottom of the box increase the counter
  if (value > 10 && score_stack <= 50)
  {
    score_stack++;
  }
  // If the ball stayed at the bottom of the box for 50 cycle it0s GOAL
  if (score_stack == 0)
  {
    printf("%s\n\r", "GOAL");
    // Move to the communication Controller
    cli();
    setIDH(0x55);
    setData(0x00);
    CAN_Trasmission();
    sei();
    // Restart the count
    score_stack = 50;
  }
}
