 <a name="preparation"></a>
## Preparation tasks (done before the lab at home)

Use schematic of the [LCD keypad shield](../../Docs/arduino_shield.pdf) and find out the connection of LCD display. What data and control signals are used? What is the meaning of these signals?

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

   | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | Gnd | identifies read or write operation |
   | E | PB1 | a trigger signal whose edge synchronizes the communication on the bus |
   | D[3:0] | not connected | Data bus line |
   | D[7:4] | PD4 to PD7 | Data bus line |

What is the ASCII table? What are the codes/values for uppercase letters `A` to `Z`, lowercase letters `a` to `z`, and numbers `0` to `9` in this table?

   | **Char** | **Decimal** | **Hexadecimal** |
   | :-: | :-: | :-: |
   | `A` | 65 | 0x41 |
   | `B` | 66 | 0x42 |
   | ... | +1 | +1 |
   | `a` | 97 | 0x61 |
   | `b` | 98 | 0x62 |
   | ... | +1 | +1 |
   | `0` | 48 | 0x30 |
   | `1` | 49 | 0x31 |
   | ... | +1 | +1 |

<a name="part1"></a>
