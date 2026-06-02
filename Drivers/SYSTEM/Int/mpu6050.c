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

    calibrate_gyroscope_data();
}

static Gyroscope_Data gd_calibration_data;     //偏移量，用于校准

void mpu6050_read_gyro_data(Gyro_Data *gd)
{
    //Gyro
    uint8_t data[6] = {0};
    mpu6050_read_bytes(MPU_GYRO_XOUTH_REG, data, 6);    //从MPU_GYRO_XOUTH_REG读到MPU_GYRO_ZOUTL_REG，共6个registers
    gd->x = (int16_t)(data[0] << 8 | data[1]);     //(注意这里转换不转换都行，因为获取到的数据就是有正负的（最高位=1的表示负数）)
    gd->y = (int16_t)(data[2] << 8 | data[3]);
    gd->z = (int16_t)(data[4] << 8 | data[5]);
}

void mpu6050_read_accel_data(Accel_Data *ga)
{
    //Accel
    uint8_t data[6] = {0};
    mpu6050_read_bytes(MPU_ACCEL_XOUTH_REG, data, 6);
    ga->x = (int16_t)(data[0] << 8 | data[1]);
    ga->y = (int16_t)(data[2] << 8 | data[3]);
    ga->z = (int16_t)(data[4] << 8 | data[5]);
}

void read_gyroscope_data(Gyroscope_Data *gd)
{
    mpu6050_read_gyro_data(&gd->gyro);
    mpu6050_read_accel_data(&gd->accel);


    //calibration
    gd->gyro.x -= gd_calibration_data.gyro.x;
    gd->gyro.y -= gd_calibration_data.gyro.y;
    gd->gyro.z -= gd_calibration_data.gyro.z;
    gd->accel.x -= gd_calibration_data.accel.x;
    gd->accel.y -= gd_calibration_data.accel.y;
    gd->accel.z -= gd_calibration_data.accel.z;
}


//零偏校准 （只在初始化中调用一次）
//偏移量获取：飞机启动后 判断飞机的连续100次采集的gyroscope数据稳定（波动量<100），然后计算偏移量=100次的平均值
void calibrate_gyroscope_data(void)
{
    //1、判断飞机的连续100次采集的gyroscope数据稳定（波动量<100）
    uint8_t count = 0;
    Gyro_Data last_gyro;
    Gyro_Data current_gyro;
    mpu6050_read_gyro_data(&last_gyro);
    while (count < 100)
    {
        HAL_Delay(2);       //防止采集间隔太小
        mpu6050_read_gyro_data(&current_gyro);
        if (ABS(last_gyro.x - current_gyro.x) < 100 &&
            ABS(last_gyro.y - current_gyro.y) < 100 &&
            ABS(last_gyro.z - current_gyro.z) < 100)
        {
            count++;
        }else
        {
            count=0;
        }
        last_gyro = current_gyro;     //结构体变量相互赋值、就相当于其中每个属性赋值
    }

    //2、计算100次采集的gyro、accel的平均值 作为偏移量
    int32_t array[6] = {0};
    Gyroscope_Data gd_temp = {0};
    for (uint8_t i = 0; i < 100; i++)
    {
        HAL_Delay(2);       //防止采集间隔太小
        mpu6050_read_gyro_data(&gd_temp.gyro);
        mpu6050_read_accel_data(&gd_temp.accel);
        array[0] += gd_temp.gyro.x;
        array[1] += gd_temp.gyro.y;
        array[2] += gd_temp.gyro.z;
        array[3] += gd_temp.accel.x;
        array[4] += gd_temp.accel.y;
        array[5] += gd_temp.accel.z;
    }
    gd_calibration_data.gyro.x = (int16_t)(array[0] / 100);
    gd_calibration_data.gyro.y = (int16_t)(array[1] / 100);
    gd_calibration_data.gyro.z = (int16_t)(array[2] / 100);
    gd_calibration_data.accel.x = (int16_t)(array[3] / 100);
    gd_calibration_data.accel.y = (int16_t)(array[4] / 100);
    gd_calibration_data.accel.z = (int16_t)(array[5] / 100);
    gd_calibration_data.accel.z -= 16384;       //gravity （意思就是accel.z校准成16384）
}