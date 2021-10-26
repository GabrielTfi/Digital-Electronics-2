# Lab 5: YOUR_FIRSTNAME FAMILYNAME

Link to your `Digital-electronics-2` GitHub repository:

[https://github.com/your-github-account/repository-name/lab_name](https://github.com/...)


### 7-segment library

1. In your words, describe the difference between Common Cathode and Common Anode 7-segment display.
   * CC SSD : The Common Cathode SSD is active high.
   * CA SSD : The Common Anode SSD is active low.

2. Code listing with syntax highlighting of two interrupt service routines (`TIMER1_OVF_vect`, `TIMER0_OVF_vect`) from counter application with at least two digits, ie. values from 00 to 59:

```c
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Increment counter value from 00 to 59.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    cnt0++;
	if(cnt0 > 9){
		cnt0 = 0;
		cnt1++;
		
        if (cnt1 > 5)				// for assignment
		{
			cnt1 = 0;				// for assignment
		}
	}
}
```

```c
/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt
 * Purpose:  Display tens and units of a counter at SSD.
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{
    static uint8_t pos = 0;

    if (pos == 0){
		SEG_update_shift_regs(cnt0, 0);
	}
	
    else if (pos == 1) {
		SEG_update_shift_regs(cnt1, 1);
	}
	
    pos++;
	
    if(pos > 1)
	{
		pos = 0;
	}
}
```

3. Flowchart figure for function `SEG_clk_2us()` which generates one clock period on `SEG_CLK` pin with a duration of 2&nbsp;us. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![Flowchart of SEG_clk_2us](https://github.com/GabrielTfi/Digital-Electronics-2/blob/main/Labs/05-segments/flowchart_CLK.PNG)


### Kitchen alarm

Consider a kitchen alarm with a 7-segment display, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![kitchen alarm circuit](https://github.com/GabrielTfi/Digital-Electronics-2/blob/main/Labs/05-segments/kitchen_alarm.PNG)
