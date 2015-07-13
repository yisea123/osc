#ifndef __NORMAL_H
#define __NORMAL_H

void Normal_Init(void);
void TimeSel(unsigned char Type);
void CH1GainSel(unsigned char Type);
void CH2GainSel(unsigned char Type);

#define SampleFinish  (GPIOE->IDR & (1 << 1))
#define SRAMData GPIOB->IDR
#define CPLDReset(x, y) {TimeSel(x); GPIOC->BRR = (1 << 3); Delay(0xFF); TimeSel(y); GPIOC->BSRR = (1 << 3);}

#define MCURead_H GPIOE->BSRR = 0x0001
#define MCURead_L GPIOE->BRR  = 0x0001

#define MCUReadFre_H  GPIOA->BSRR = (1 << 8)
#define MCUReadFre_L  GPIOA->BRR =  (1 << 8)

#define SetLED1  GPIOC->BSRR = (1 << 4)
#define ClrLED1  GPIOC->BRR  = (1 << 4)
#define SetLED2  GPIOC->BSRR = (1 << 5)
#define ClrLED2  GPIOC->BRR  = (1 << 5)
#endif
