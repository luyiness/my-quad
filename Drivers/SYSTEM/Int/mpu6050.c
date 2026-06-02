//
// Created by luyi on 2026/6/1.
//

#include "mpu6050.h"

#include "Com_debug.h"

void mpu6050_write_byte(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_W, reg,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

void mpu6050_read_byte(uint8_t reg, uint8_t *data)
{
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_R, reg,
                     I2C_MEMADD_SIZE_8BIT, data, 1, 1000);
}

void mpu6050_read_bytes(uint8_t reg, uint8_t *data, uint8_t len)
{
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_R, reg,
                     I2C_MEMADD_SIZE_8BIT, data, len, 1000);
}

void mpu6050_init(void)
{
    //1、初始化IIC

    //2、复位 MPU-6050
    mpu6050_write_byte(MPU_PWR_MGMT1_REG, 0x80);
    HAL_Delay(200);
    mpu6050_write_byte(MPU_PWR_MGMT1_REG, 0x00);

    //3、gyroscope full-scale range of ±250, ±500, ±1000； accelerometer full-scale range of ±2g, ±4g, ±8g, and ±16g
    mpu6050_write_byte(MPU_GYRO_CFG_REG, 0b11 << 3);
    mpu6050_write_byte(MPU_ACCEL_CFG_REG, 0x00);

    //4、其他参数：设置数字低通滤波器DLPF、设置陀螺仪采样率， 关闭中断、关闭 AUX IIC 接口、禁止 FIFO
    mpu6050_write_byte(MPU_CFG_REG, 0x01);
    mpu6050_write_byte(MPU_SAMPLE_RATE_REG, 0x01);

    mpu6050_write_byte(MPU_INT_EN_REG, 0x00);       //关闭中断
    mpu6050_write_byte(MPU_USER_CTRL_REG, 0x00);    //关闭FIFO和AUX IIC

    //5、配置系统时钟源，使能角速度传感器、加速度传感器
    mpu6050_write_byte(MPU_PWR_MGMT1_REG, 0x01);    //选择时钟
    mpu6050_write_byte(MPU_PWR_MGMT2_REG, 0x00);    //使能传感器

    //测试
    // uint8_t id;
    // mpu6050_read_byte(MPU_DEVICE_ID_REG, &id);
    // debug_printf("MPU6050 ID: %02X\n", id);     //“%X”表示输出16进制格式，“02”表示最小输出宽度=2、小于2的话补0
}


void read_gyroscope_data(gyroscope_data *gd)
{
    //Gyro
    uint8_t data[6] = {0};
    mpu6050_read_bytes(MPU_GYRO_XOUTH_REG, data, 6);    //从MPU_GYRO_XOUTH_REG读到MPU_GYRO_ZOUTL_REG，共6个registers
    gd->gyro.x = (int16_t)(data[0] << 8 | data[1]);     //(注意这里转换不转换都行，因为获取到的数据就是有正负的（最高位=1的表示负数）)
    gd->gyro.y = (int16_t)(data[2] << 8 | data[3]);
    gd->gyro.z = (int16_t)(data[4] << 8 | data[5]);

    //Accel
    uint8_t data2[6] = {0};
    mpu6050_read_bytes(MPU_ACCEL_XOUTH_REG, data2, 6);
    gd->accel.x = (int16_t)(data2[0] << 8 | data2[1]);
    gd->accel.y = (int16_t)(data2[2] << 8 | data2[3]);
    gd->accel.z = (int16_t)(data2[4] << 8 | data2[5]);
}