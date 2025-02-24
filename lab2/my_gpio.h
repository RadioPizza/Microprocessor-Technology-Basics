// my_gpio v1
#ifndef MY_GPIO_H
#define MY_GPIO_H

void delay(unsigned int ms);
void delay10Microseconds(unsigned int us);
void pinMode(char port, int pin, int mode);
void digitalWrite(char port, int pin, int value);

#endif // MY_GPIO_H
