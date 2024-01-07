/******************************************************************************

 @file  util.c

 @brief Utility functions commonly used by TIMAC applications

 Group: WCS LPC
 Target Device: CC13xx

 ******************************************************************************
 
 Copyright (c) 2016-2017, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 Release Name: simplelink_cc13x0_sdk_1_60_00_
 Release Date: 2017-12-20 13:05:49
 *****************************************************************************/

/******************************************************************************
 Includes
 *****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "time.h"
#include "util.h"


/******************************************************************************
 Typedefs
 *****************************************************************************/
#define UTIL_SADDR_EXT_LEN  8

/******************************************************************************
 Public Functions
 *****************************************************************************/

/*!
 Converts from a uint16 to ascii hex string.

 Public function defined in util.h
 */
void Util_uint16toa(uint16_t u, char *string)
{
    if(string == NULL)
    {
        return;
    }

    /* add preceding zeros */
    if(u < 0x1000)
    {
        *string++ = '0';
    }

    if(u < 0x0100)
    {
        *string++ = '0';
    }

    if(u < 0x0010)
    {
        *string++ = '0';
    }

    Util_ltoa( (unsigned long)u, (unsigned char *)string, 16 );
}

/*!
 Convert a 16bit number to ASCII

 Public function defined in util.h
 */
void Util_itoa(uint16_t num, uint8_t *buf, uint8_t radix)
{
    char c, i;
    uint8_t *p, rst[5];

    p = rst;
    for(i = 0; i < 5; i++, p++)
    {
        c = num % radix;  /* Isolate a digit */
        *p = c + ( (c < 10) ? '0' : '7' );  /* Convert to Ascii */
        num /= radix;
        if(!num)
        {
            break;
        }
    }

    for(c = 0; c <= i; c++)
    {
        *buf++ = *p--;  /* Reverse character order */
    }

    *buf = '\0';
}

/*!
 Convert a long unsigned int to a string.

 Public function defined in util.h
 */
//unsigned char *Util_ltoa(uint32_t l, uint8_t *buf, uint8_t radix)
//{
//#if defined (__GNUC__)
//    return( (char *)ltoa(l, buf, radix) );
//#else
//    unsigned char tmp1[10] = "", tmp2[10] = "", tmp3[10] = "";
//    unsigned short num1, num2, num3;
//    unsigned char i;
//
//    buf[0] = '\0';
//
//    if(radix == 10)
//    {
//        num1 = l % 10000;
//        num2 = (l / 10000) % 10000;
//        num3 = (unsigned short)(l / 100000000);
//
//        if(num3)
//        {
//            Util_itoa(num3, tmp3, 10);
//        }
//        if(num2)
//        {
//            Util_itoa(num2, tmp2, 10);
//        }
//        if(num1)
//        {
//            Util_itoa(num1, tmp1, 10);
//        }
//
//        if(num3)
//        {
//            strcpy( (char *)buf, (char const *)tmp3 );
//            for(i = 0; i < 4 - strlen( (char const *)tmp2 ); i++)
//            {
//                strcat( (char *)buf, "0" );
//            }
//        }
//        strcat( (char *)buf, (char const *)tmp2 );
//        if(num3 || num2)
//        {
//            for(i = 0; i < 4 - strlen( (char const *)tmp1 ); i++)
//            {
//                strcat( (char *)buf, "0" );
//            }
//        }
//        strcat( (char *)buf, (char const *)tmp1 );
//        if(!num3 && !num2 && !num1)
//        {
//            strcpy( (char *)buf, "0" );
//        }
//    }
//    else if(radix == 16)
//    {
//        num1 = l & 0x0000FFFF;
//        num2 = l >> 16;
//
//        if(num2)
//        {
//            Util_itoa(num2, tmp2, 16);
//        }
//        if(num1)
//        {
//            Util_itoa(num1, tmp1, 16);
//        }
//
//        if(num2)
//        {
//            strcpy( (char *)buf, (char const *)tmp2 );
//            for(i = 0; i < 4 - strlen( (char const *)tmp1 ); i++)
//            {
//                strcat( (char *)buf, "0" );
//            }
//        }
//        strcat( (char *)buf, (char const *)tmp1 );
//        if(!num2 && !num1)
//        {
//            strcpy( (char *)buf, "0" );
//        }
//    }
//    else
//    {
//        return(NULL);
//    }
//
//    return(buf);
//#endif
//}

/*!
 Get the high byte of a uint16_t variable

 Public function defined in util.h
 */
uint8_t Util_hiUint16(uint16_t a)
{
    return((a >> 8) & 0xFF);
}

/*!
 Get the low byte of a uint16_t variable

 Public function defined in util.h
 */
uint8_t Util_loUint16(uint16_t a)
{
    return((a) & 0xFF);
}

/*!
 Build a uint16_t out of 2 uint8_t variables

 Public function defined in util.h
 */
uint16_t Util_buildUint16(uint8_t loByte, uint8_t hiByte)
{
    return((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)));
}

/*!
 Build a uint32_t out of 4 uint8_t variables

 Public function defined in util.h
 */
uint32_t Util_buildUint32(uint8_t byte0, uint8_t byte1, uint8_t byte2,
                            uint8_t byte3)
{
    return((uint32_t)((uint32_t)((byte0) & 0x00FF) +
                     ((uint32_t)((byte1) & 0x00FF) << 8) +
                     ((uint32_t)((byte2) & 0x00FF) << 16) +
                     ((uint32_t)((byte3) & 0x00FF) << 24)));
}

/*!
 Pull 1 uint8_t out of a uint32_t

 Public function defined in util.h
 */
uint8_t Util_breakUint32(uint32_t var, int byteNum)
{
    return(uint8_t)((uint32_t)(((var) >> ((byteNum) * 8)) & 0x00FF));
}

/*!
 Build a uint16_t from a uint8_t array

 Public function defined in util.h
 */
uint16_t Util_parseUint16(uint8_t *pArray)
{
    return(Util_buildUint16(pArray[0], pArray[1]));
}

/*!
 Build a uint32_t from a uint8_t array

 Public function defined in util.h
 */
uint32_t Util_parseUint32(uint8_t *pArray)
{
    return(Util_buildUint32(pArray[0], pArray[1], pArray[2], pArray[3]));
}

/*!
 Break and buffer a uint16_t value - LSB first

 Public function defined in util.h
 */
uint8_t *Util_bufferUint16(uint8_t *pBuf, uint16_t val)
{
    *pBuf++ = Util_loUint16(val);
    *pBuf++ = Util_hiUint16(val);

    return(pBuf);
}

/*!
 Break and buffer a uint32_t value - LSB first

 Public function defined in util.h
 */
uint8_t *Util_bufferUint32(uint8_t *pBuf, uint32_t val)
{
    *pBuf++ = Util_breakUint32(val, 0);
    *pBuf++ = Util_breakUint32(val, 1);
    *pBuf++ = Util_breakUint32(val, 2);
    *pBuf++ = Util_breakUint32(val, 3);

    return(pBuf);
}
/*Show buffer hex*/
void show_hex(uint8_t *buf, uint16_t len)
{
	uint16_t i;
	
	for(i = 0 ; i < len; i++)
		printf("%02x ", buf[i]);
	printf("\r\n");
}

void text_print(char const* p_label, char const * p_text, size_t len)
{
 
    printf("----%s (len: %u) ----\r\n", p_label, len);
//    NRF_LOG_FLUSH();
    for(size_t i = 0; i < len; i++)
    {
        printf("%c", p_text[i]);
        //NRF_LOG_FLUSH();
    }
    printf("\r\n");
    printf("---- %s end ----\r\n\r\n", p_label);
  //  NRF_LOG_FLUSH();
	 
}

void hex_text_print(char const* p_label, char const * p_text, size_t len)
{
	 
    printf("---- %s (len: %u) ----\r\n", p_label, len);
    //NRF_LOG_FLUSH();

    // Handle partial line (left)
    for (size_t i = 0; i < len; i++)
    {
        if (((i & 0xF) == 0) && (i > 0))
        {
            printf("\r\n");
           // NRF_LOG_FLUSH();
        }

        printf("%02x ", p_text[i]);
        //NRF_LOG_FLUSH();
    }
    printf("\r\n=====================\r\n");
    //printf("---- %s end ----\r\n\r\n", p_label);
    //NRF_LOG_FLUSH();
	 
}
/*crc16 calc*/

uint16_t crc16Calc(uint8_t *bytes, uint32_t len, uint16_t init_crc)
{
    #define POLY 0x1021
    uint32_t loop;
    //uint16_t init_crc = 0x00;

    for (loop = 0; loop < len; loop++) {
        for (int i = 0; i < 8; i++) {
            bool bit = ((bytes[loop] >> (7 - i) & 1) == 1);
            bool c15 = ((init_crc >> 15 & 1) == 1);
            init_crc <<= 1;
            if (c15 ^ bit) {
                init_crc ^= POLY;
            }
        }
    }
    init_crc &= 0xFFFF;

    return init_crc;
}
/*--------------------------------------------------------------------------------------
* int splitStr(char *Src, char *Delim, char *Des, int size)
 * Descriptions: Copy Src to Des from Src[0] to Delim 
 *				 
 * Return: number of char copied
 *-------------------------------------------------------------------------------------*/
int splitStr(char *Src, char *Delim, char *Des, int size)
{
	int i=0;
	char ch;

	while ( (ch=Src[i]) != Delim[0] && Src[i] !='\0')
	{
		Des[i] = ch;
		if (ch=='\0' || i>=size) break;
		i++;
	}
	Des[i] = '\0';

	return i;
}
/*****************************************************************************
 *  Function: SplitnData(...)
 * 	Desc: get the content between token_begin & token_end from data_in 
 *	string, then copy to data_out string
 * 	Return: strlen(datout)
 *****************************************************************************/
int SplitnData(char *datin, char tok1, int tok1_n, char tok2, int size, char *datout)
{
    int i = 0;
    int j = 0;
    int pos = 0;
    int mark = 0;

    datout[j] = 0;//NULL
    while((datin[i]!=0) && (j<size-1))
    {
        if(mark == 1)
        {
            if(tok2 == datin[i]){
                datout[j] = 0;//NULL
                return j;
            }
            datout[j++] = datin[i];
        }
        if(tok1 == datin[i]) pos++;
        if(pos == tok1_n)
        {
            mark = 1;
        }
        i++;
    }
    datout[j]=0;
    return j;
} 

/*round nearest number n mod modulo*/
uint32_t round_number(uint32_t n, uint32_t modulo)
{
	uint32_t num;
	num = (n - n % modulo) + modulo;
	//printf("num: %d\r\n", num);
	return num;
}
//'0'..'9', 'a'..'f', and 'A'..'F'
uint8_t ascii_to_hex(char chr)
{
	if( (chr >= '0') && (chr <= '9'))
		return (chr - '0');
	else if((chr >= 'a') && (chr <= 'f'))
		return (chr - 'a' + 10);
	else if((chr >= 'A') && (chr <= 'F'))
		return (chr - 'A' + 10);
	return 0;
}

uint8_t UINT8_Ascii_To_Hex(uint8_t hi, uint8_t lo)
{
	
	return 16*ascii_to_hex(hi) + ascii_to_hex(lo);
}
//convert string to float number
float string_to_float(char *str)
{
    //char inputstr[] = "1046.70346";
    float result= 0.0f;
    int len = strlen(str)-1;
    int dotpos=0;
    int n;

    //printf("len: %d - %d\r\n", sizeof(str), len);
    if (str[0]=='-'||str[0]=='+')   //Added line to check sign.If the number is signed,
        n=1;                 //set n to position 1.
    else                     //(number is not signed)
        n=0;                 //set n to position 0.
/*If the number was signed,then we set n to 1,so that we start with inputstr[1],and at the end if the number was negative we will multiply by -1.*/
    for (; n < len; ++n)         //n is already set to the position of the fisrt number.
        if (str[n] == '.')
            dotpos = len - n  - 1;
        else
            result = result * 10.0f + (str[n]-'0');
    while (dotpos--)
        result /= 10.0f;
    if (str[0]=='-')  //If inputstr[] is "negative",
        result*=(-1);      //multiply the number by -1,making it also negative.
    printf("result: %f\r\n",result);
    return result;
}
//sprintf(paras.kd, "%s", "1046.70346");
//sprintf(paras.vd, "%s", "10643.6671");
//10.778391 --> 1046.70346
//106.727785 --> 10643.6671
void convert_cord_to_hex(float kd, float vd, uint8_t *kd_buf, uint8_t *vd_buf)
{
    int32_t kd_tmp = kd*100000;
    int32_t vd_tmp = vd*100000;

//    printf("%d -- %d\r\n", kd_tmp, vd_tmp);
//    printf("%x -- %x\r\n", kd_tmp, vd_tmp);
//    kd_buf[0] = kd_tmp & 0xFF;
//    kd_buf[1] = (kd_tmp >> 8)& 0xFF;
//    kd_buf[2] = (kd_tmp >> 16) & 0xFF;
//    kd_buf[3] = (kd_tmp >> 24) & 0xFF;
	INT32_TO_BUF_LITTLE_ENDIAN(kd_buf, kd_tmp);

//    vd_buf[0] = vd_tmp & 0xFF;
//    vd_buf[1] = (vd_tmp >> 8)& 0xFF;
//    vd_buf[2] = (vd_tmp >> 16) & 0xFF;
//    vd_buf[3] = (vd_tmp >> 24) & 0xFF;
	INT32_TO_BUF_LITTLE_ENDIAN(vd_buf, vd_tmp);
}

void convert_hex_to_cord(uint8_t *kd_buf, uint8_t *vd_buf, float *kd, float *vd)
{
    uint32_t kd_tmp;
    uint32_t vd_tmp;



    //printf("%x -- %x -- %x -- %x\r\n", kd_buf[3] , kd_buf[2], kd_buf[1], kd_buf[0]);
    //kd_tmp = ((uint32_t)kd_buf[3] << 24) | ((uint32_t)kd_buf[2] << 16) | ((uint32_t)kd_buf[1] << 8) | kd_buf[0];
    kd_tmp = Util_parseUint32(kd_buf);
//    vd_tmp = ((uint32_t)vd_buf[3] << 24) | ((uint32_t)vd_buf[2] << 16) | ((uint32_t)vd_buf[1] << 8) | vd_buf[0];
	vd_tmp = Util_parseUint32(vd_buf);

    *kd = (float)kd_tmp/100000;
    *vd = (float)vd_tmp/100000;

    //printf("=====>%d -- %f\r\n", kd_tmp, *kd);
}

#if 0
bool is_valid_ip(char *ip_str)
{
    unsigned int n1,n2,n3,n4;

    if(sscanf(ip_str,"%u.%u.%u.%u", &n1, &n2, &n3, &n4) != 4) return false;

    //printf("n1: %d<=========\r\n", n1);
    if((n1 != 0) && (n1 <= 255) && (n2 <= 255) && (n3 <= 255) && (n4 <= 255)) {
        char buf[64];
        sprintf(buf,"%u.%u.%u.%u",n1,n2,n3,n4);
        if(strcmp(buf,ip_str)) return false;
        return true;
    }
    return false;
}
#endif

// int DaysInMonth(int Month, int Year) 
// {
//     int number_of_days;

//     switch (Month) 
//     {
//         case 1: case 3: case 5: case 7:
//         case 8: case 10:case 12:
//             number_of_days = 31;
//             break;
//         case 4: case 6: case 9: case 11:
//             number_of_days = 30;
//             break;
//         case 2:
//             if ((Year % 4) == 0)
//                 number_of_days = 29;
//             else number_of_days = 28;
//             break;
//     }
// //    printf("Year: %d <=============\r\n", Year);
// //    printf("RET: %d <=============\r\n", ret);
//     return number_of_days;
// }

// void GMT2LocalTime(struct tm GMTtime, struct tm *LocalTime, uint8_t timezone) 
// {
//     GMTtime.tm_mon += 1;
//     GMTtime.tm_year += 1900;

//     *LocalTime = GMTtime;

//     LocalTime->tm_hour = GMTtime.tm_hour + timezone;
// //    printf("LocalTime->tm_hour: %d <====\r\n", LocalTime->tm_hour);
//     if (LocalTime->tm_hour > 23) 
//     {
//         LocalTime->tm_hour = LocalTime->tm_hour - 24;
//         LocalTime->tm_mday = LocalTime->tm_mday + 1;

//         if (LocalTime->tm_mday > DaysInMonth(LocalTime->tm_mon, GMTtime.tm_year)) 
//         {
//             LocalTime->tm_mday -= DaysInMonth(LocalTime->tm_mon, GMTtime.tm_year);
//             LocalTime->tm_mon += 1;
//         }
//         if (LocalTime->tm_mon > 12) 
//         {
//             LocalTime->tm_mon = 1;
//             LocalTime->tm_year += 1;
//         }
//     }
 
//     LocalTime->tm_mon -= 1;
//     LocalTime->tm_year -= 1900;
// //    printf("LocalTime->tm_mon: %d <============\r\n", LocalTime->tm_mon);
    
// }


/* Function to reverse arr[] from start to end*/
void rvereseArray(uint8_t *arr, int start, int end)
{
    uint8_t temp;
    while (start < end)
    {
        temp = arr[start];   
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }   
}  

uint32_t String_to_Hex(const char *str, uint16_t len, uint8_t *hex_out) 
{
    //int len = strlen(str);
    int hex_len = len / 2;  // Two characters represent one hex value
    unsigned char *hex = (unsigned char *)malloc(hex_len);

    for (int i = 0; i < hex_len; i++) {
        sscanf(&str[i * 2], "%2hhx", &hex[i]);
    }

    for (int i = 0; i < hex_len; i++) {
        printf("0x%02X ", hex[i]);
    }

    memcpy(hex_out, hex, hex_len);

    free(hex);
    return hex_len;
}

void convert_ascii_to_hex(char *str, uint16_t len, uint8_t *pbuf)
{
    for (size_t i = 0; i < len; i++)
    {
        *(pbuf++) =  *(str++) ; 
    }
}

void convert_hex_to_ascii(uint8_t *pHex, uint16_t len, char *pAscii)
{
    for (size_t i = 0; i < len; i++)
    {
        *(pAscii++) = (char)*(pHex++);
    }
    
}

uint8_t hex_to_bcd(uint8_t hex_val)
{
    return (((hex_val & 0xf0) >> 4 )*10 + (hex_val & 0x0f));
}

uint8_t dec_to_bcd(uint8_t num)
{
    return ((num / 10 * 16) + (num % 10));
}

uint8_t bcd_to_dec(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}



