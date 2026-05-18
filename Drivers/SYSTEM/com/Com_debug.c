#include "../include/Com_debug.h"

void Debug_Init(void)
{
    // ��ʼ������
    // MX_USART2_UART_Init();
}


int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

