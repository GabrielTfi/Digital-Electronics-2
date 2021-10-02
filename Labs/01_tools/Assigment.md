
# Lab 1: YOUR_FIRSTNAME FAMILYNAME

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/...](https://github.com/...)


### Blink example

1. What is the meaning of the following binary operators in C?
   * `|` : or
   * `&` : and
   * `^` : xor
   * `~` : complement
   * `<<` : shift left
   * `>>` : shift right

2. Complete truth table with operators: `|`, `&`, `^`, `~`

| **b** | **a** |**b or a** | **b and a** | **b xor a** | **not b** |
| :-: | :-: | :-: | :-: | :-: | :-: |
| 0 | 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 1 | 0 | 1 | 1 |
| 1 | 0 | 1 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 | 0 |


### Morse code

1. Listing of C code with syntax highlighting which repeats one "dot" and one "comma" (BTW, in Morse code it is letter `A`) on a LED:

```c
int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);
    
    
    // Infinite loop of the letter A in morse " ._ "
    // a dot corresponds to the LED on for a SHORT_DELAY
    // a comma corresponds to the LED on for two SHORT_DELAY
    // between a comma and a point, the LED is off for a SHORT_DELAY
    // between two letters, the LED is off for two SHORT_DELAY
    while (1)
    {
        PORTB = PORTB ^ (1<<LED_GREEN); // .
        _delay_ms(SHORT_DELAY);
        
        PORTB = PORTB & ~(1<<LED_GREEN); // pause between two caracters of a letter
        _delay_ms(SHORT_DELAY);
        
        PORTB = PORTB ^ (1<<LED_GREEN); // _
        _delay_ms(SHORT_DELAY*2);
        
        PORTB = PORTB & ~(1<<LED_GREEN); // pause between two letters
        _delay_ms(SHORT_DELAY*2);
        
        
    }
    // Will never reach this
    return 0;
}
```


2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![alt text](https://ibb.co/YhM33QQ)
