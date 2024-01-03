#ifndef _RTC_PCF8566
#define _RTC_PCF8566

#include <time.h>

#define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000
                 
#define RTC_PCF85263_ADDR           0x51

#define SECOND_ADDR                 0x02
#define MINUTE_ADDR                 0x03
#define HOUR_ADDR                   0x04
#define DAY_ADDR                    0x05
#define WEEK_ADDR                   0x06
#define MONTH_ADDR                  0x07
#define YEAR_ADDR                   0x08


#define WRITE_BIT                  I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN               0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS              0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                    0x0              /*!< I2C ack value */
#define NACK_VAL                   0x1              /*!< I2C nack value */


//struct tm RTC_get_time(void);
static esp_err_t i2c_master_init(void);
void i2c_scan();
static esp_err_t rtc_pcf_read_reg_nbyte(uint8_t i2c_reg, uint8_t* data_rd, size_t size);
esp_err_t rtc_pcf_write_reg_byte(uint8_t address, uint8_t regVal);

void WriteTimeOn(void);
void WriteTimeOff(void);
void rtc_pcf_set(struct tm value);
struct tm rtc_pcf_read(void);
void test_rtc(void);

#endif