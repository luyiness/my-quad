//
// Created by luyi on 2026/5/25.
//

#include "../include/SI24R1.h"

// 地址 => 接收方和发送方保持一致
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x07, 0x0E, 0x01}; // 定义一个静态发送地址

//静态方法，用spi读写（交换）一个字节
static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t result = 0;
	taskENTER_CRITICAL();	//进入临界区
	HAL_SPI_TransmitReceive(&hspi1, &byte, &result, 1, 1000);
	taskEXIT_CRITICAL();	//
	return (result);
}

//SPI发送一个Byte。
////主要用于写寄存器，分两步：1、主机先发一次cmd=001xxxxx 从机发回状态寄存器的值，2、写数据
uint8_t Int_SI24R1_Write_Byte(uint8_t cmd, uint8_t value)
{
	uint8_t status;

	SI24R1_CS_L;
	status = SPI_RW(cmd);	//1、
	SPI_RW(value);		//2、
	SI24R1_CS_H;

	return (status);
}

////SPI发送多个Bytes
uint8_t Int_SI24R1_Write_Bytes(uint8_t cmd, const uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status, byte_ctr;

	SI24R1_CS_L;
	status = SPI_RW(cmd);
	for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
		SPI_RW(*pBuf++);
	SI24R1_CS_H;

	return (status);
}

//SPI读
uint8_t Int_SI24R1_Read_Byte(uint8_t cmd)
{
	uint8_t value;

	SI24R1_CS_L;
	SPI_RW(cmd);
	value = SPI_RW(0);
	SI24R1_CS_H;

	return (value);
}

uint8_t Int_SI24R1_Read_Bytes(uint8_t cmd, uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status, byte_ctr;

	SI24R1_CS_L;
	status = SPI_RW(cmd);
	for (byte_ctr = 0; byte_ctr < bytes; byte_ctr++)
		pBuf[byte_ctr] = SPI_RW(0); // 读取数据，低字节在前
	SI24R1_CS_H;

	return (status);
}

//SI24R1接收模式的初始化
void Int_SI24R1_RX_Mode(void)
{
	SI24R1_CE_L;
	Int_SI24R1_Write_Bytes(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 接收设备接收通道0使用和发送设备相同的发送地址
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + EN_AA, 0x01);							  // 使能接收通道0自动应答
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + EN_RXADDR, 0x01);						  // 使能接收通道0
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + RF_CH, CHANNEL);						  // 选择射频通道
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);			  // 接收通道0选择和发送通道相同有效数据宽度
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + RF_SETUP, 0x0f);						  // 数据传输率2Mbps，发射功率7dBm
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + CONFIG, 0x0f);						  // CRC使能，16位CRC校验，上电，接收模式
	// Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + STATUS, 0xff);						  // 清除所有的中断标志位
	SI24R1_CE_H; // 拉高CE启动接收设备
}

//SI24R1发送模式的初始化
void Int_SI24R1_TX_Mode(void)
{
	SI24R1_CE_L;
	Int_SI24R1_Write_Bytes(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	  // 写入发送地址
	Int_SI24R1_Write_Bytes(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 为了应答接收设备，接收通道0地址和发送地址相同

	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + EN_AA, 0x01);		 // 使能接收通道0自动应答
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + EN_RXADDR, 0x01);	 // 使能接收通道0
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + SETUP_RETR, 0x0a); // 自动重发延时等待250us+86us，自动重发10次
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + RF_CH, CHANNEL);	 // 选择射频通道
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + RF_SETUP, 0x0f);	 // 数据传输率2Mbps，发射功率7dBm
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + CONFIG, 0x0e);	 // CRC使能，16位CRC校验，上电
	SI24R1_CE_H;										 // SI24R1_CE_H;
}

//*************************************************************

//2.4G读负载数据
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;
	state = Int_SI24R1_Read_Byte(STATUS);			  // 读取状态寄存器的值
	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + STATUS, state); // 清除RX_DS中断标志：RX_DR、TX_DS、MAX_RT位，=1时表示中断发生，往里写1清除此位

	if (state & RX_DR) // 接收到数据
	{
		Int_SI24R1_Read_Bytes(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); // 读取数据；  读命令RD_RX_PLOAD
		Int_SI24R1_Write_Byte(FLUSH_RX, 0xff);					   // 清除RX FIFO寄存器
		return 0;
	}
	return 1; // 没收到任何数据
}

//2.4G发负载数据
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state = 0;
	SI24R1_CE_L;												// CE拉低，使能SI24R1配置
	Int_SI24R1_Write_Bytes(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // 写数据到TX FIFO,32个字节
	SI24R1_CE_H;												// CE置高，使能发送

	// while (IRQ == 1)
	// 	;						// 等待发送完成；没用中断 所以注掉，用下面的while()

	//等待中断标志，直到发送成功或失败：也就是TX_DS=1或MAX_RT=1
	while (state & (TX_DS | MAX_RT))
	{
		state = Int_SI24R1_Read_Byte(STATUS); // 读状态寄存器
	}

	Int_SI24R1_Write_Byte(SI24R1_WRITE_REG + STATUS, state); // 清除TX_DS或MAX_RT中断标志

	if (state & MAX_RT) // 达到最大重发次数
	{
		Int_SI24R1_Write_Byte(FLUSH_TX, 0xff); // 清除TX FIFO寄存器
		return 1;
	}
	if (state & TX_DS) // 发送完成
	{
		return 0;
	}

	return 1; // 发送失败
}

//*************************************************************
//SI24R1的初始化
void Int_SI24R1_Init(void)
{
	//1、初始化spi

	//2、上电后的延时，要>100ms
	vTaskDelay(200);

	//3、校验一下spi的写入读取
	//写入ARQ的地址
	Int_SI24R1_Write_Bytes(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);		//向寄存器TX_ADDR写入静态发送地址

	//读取寄存器TX_ADDR
	uint8_t rx_buff[TX_ADR_WIDTH] = {0};
	Int_SI24R1_Read_Bytes(SI24R1_READ_REG + TX_ADDR, rx_buff, TX_ADR_WIDTH);

	//判断
	for (uint8_t i = 0; i < TX_ADR_WIDTH; i++)
	{
		if (rx_buff[i] != TX_ADDRESS[i])
		{
			debug_printfln("spi写入读取错误");
			return;
		}
	}

	//4、进入RX mode
	Int_SI24R1_RX_Mode();
}