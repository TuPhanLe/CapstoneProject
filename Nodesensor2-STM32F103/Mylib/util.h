/******************************************************************************

 @file  util.h

 @brief Declaration of utility functions common to TIMAC Applications.

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
#ifndef UTIL_H
#define UTIL_H

/******************************************************************************
 Includes
 *****************************************************************************/
#include <stdint.h>
#include "time.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*!
 \defgroup UtilMisc Utility and Miscellaneous
 <BR>
 Miscellaneous conversion functions.
 <BR>
 */

/******************************************************************************
 Constants and definitions
 *****************************************************************************/
 #define MAX_NUMBER(x,y) (((x) >= (y)) ? (x) : (y))
/* takes a byte out of a uint32 : var - uint32,  ByteNum - byte to take out (0 - 3) */
#define BREAK_UINT32( var, ByteNum ) \
          (uint8_t)((uint32_t)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32_t)((uint32_t)((Byte0) & 0x00FF) \
          + ((uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((uint32_t)((Byte2) & 0x00FF) << 16) \
          + ((uint32_t)((Byte3) & 0x00FF) << 24)))
		  
#define BUILD_INT32(Byte0, Byte1, Byte2, Byte3) \
          ((int)((int)((Byte0) & 0x00FF) \
          + ((int)((Byte1) & 0x00FF) << 8) \
          + ((int)((Byte2) & 0x00FF) << 16) \
          + ((int)((Byte3) & 0x00FF) << 24)))
			  
#define BUILD_UINT24(Byte0, Byte1, Byte2) \
          ((uint32_t)((uint32_t)((Byte0) & 0x00FF) \
          + ((uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((uint32_t)((Byte2) & 0x00FF) << 16)))		  

#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))
		  
#define BUILD_INT16(loByte, hiByte) \
          ((int16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
          ((uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)

// Write the 32bit value of 'val' in little endian format to the buffer pointed 
// to by pBuf, and increment pBuf by 4
#define UINT32_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
	do { \
		*(pBuf)++ = ((((uint32_t)(val)) >>  0) & 0xFF); \
		*(pBuf)++ = ((((uint32_t)(val)) >>  8) & 0xFF); \
		*(pBuf)++ = ((((uint32_t)(val)) >> 16) & 0xFF); \
		*(pBuf)++ = ((((uint32_t)(val)) >> 24) & 0xFF); \
	} while (0)
	
#define INT32_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
	do { \
		*(pBuf)++ = ((((int)(val)) >>  0) & 0xFF); \
		*(pBuf)++ = ((((int)(val)) >>  8) & 0xFF); \
		*(pBuf)++ = ((((int)(val)) >> 16) & 0xFF); \
		*(pBuf)++ = ((((int)(val)) >> 24) & 0xFF); \
	} while (0)

// Return the 32bit little-endian formatted value pointed to by pBuf, and increment pBuf by 4
#define BUF_TO_UINT32_LITTLE_ENDIAN(pBuf) (((pBuf) += 4), BUILD_UINT32((pBuf)[-4], (pBuf)[-3], (pBuf)[-2], (pBuf)[-1])) 

// Write the 24bit value of 'val' in little endian format to the buffer pointed 
// to by pBuf, and increment pBuf by 4
#define UINT24_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
	do { \
		*(pBuf)++ = ((((uint32_t)(val)) >>  0) & 0xFF); \
		*(pBuf)++ = ((((uint32_t)(val)) >>  8) & 0xFF); \
		*(pBuf)++ = ((((uint32_t)(val)) >> 16) & 0xFF); \
	} while (0)
	
#define INT24_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
	do { \
		*(pBuf)++ = ((((int)(val)) >>  0) & 0xFF); \
		*(pBuf)++ = ((((int)(val)) >>  8) & 0xFF); \
		*(pBuf)++ = ((((int)(val)) >> 16) & 0xFF); \
	} while (0)

#define UINT16_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
	do { \
		*(pBuf)++ = ((((uint32_t)(val)) >>  0) & 0xFF); \
		*(pBuf)++ = ((((uint32_t)(val)) >>  8) & 0xFF); \
	} while (0)
	
#define INT16_TO_BUF_LITTLE_ENDIAN(pBuf,val) \
	do { \
		*(pBuf)++ = ((((int)(val)) >>  0) & 0xFF); \
		*(pBuf)++ = ((((int)(val)) >>  8) & 0xFF); \
	} while (0)
// Return the 24bit little-endian formatted value pointed to by pBuf, and increment pBuf by 4
#define BUF_TO_UINT24_LITTLE_ENDIAN(pBuf) (((pBuf) += 3), BUILD_UINT24(((pBuf)[-3], (pBuf)[-2], (pBuf)[-1]))
// Return the 32bit little-endian formatted value pointed to by pBuf, and increment pBuf by 4
#define BUF_TO_INT32_LITTLE_ENDIAN(pBuf) (((pBuf) += 4), BUILD_INT32((pBuf)[-4], (pBuf)[-3], (pBuf)[-2], (pBuf)[-1])) 

/******************************************************************************
 Function Prototypes
 *****************************************************************************/

/*!
 * @brief      Converts from a uint16 to ascii hex string.
 *             The # will be exactly 4 hex digits (e.g. 0x0000 or 0x1E3F).
 *             NULL terminates the string.
 *
 * @param      u - Number to be converted
 * @param      string - pointer to coverted string
 */
extern void Util_uint16toa(uint16_t u, char *string);

/*!
 * @brief      Convert a 16bit number to ASCII
 *
 * @param      num - number to convert
 * @param      buf - buffer to write ASCII
 * @param      radix - base to convert to (ie. 10 or 16)
 */
extern void Util_itoa(uint16_t num, uint8_t *buf, uint8_t radix);

/*!
 * @brief      Convert a long unsigned int to a string.
 *
 * @param      l - long to convert
 * @param      buf - buffer to convert to
 * @param      radix - 10 dec, 16 hex
 *
 * @return     pointer to buffer
 */
extern unsigned char *Util_ltoa(uint32_t l, uint8_t *buf, uint8_t radix);

/*!
 * @brief      Get the high byte of a uint16_t variable
 *
 * @param      a - uint16_t variable
 *
 * @return     high byte
 */
extern uint8_t Util_hiUint16(uint16_t a);

/*!
 * @brief      Get the low byte of a uint16_t variable
 *
 * @param      a - uint16_t variable
 *
 * @return     low byte
 */
extern uint8_t Util_loUint16(uint16_t a);

/*!
 * @brief      Build a uint16_t out of 2 uint8_t variables
 *
 * @param      loByte - low byte
 * @param      hiByte - high byte
 *
 * @return     combined uint16_t
 */
extern uint16_t Util_buildUint16(uint8_t loByte, uint8_t hiByte);

/*!
 * @brief      Build a uint32_t out of 4 uint8_t variables
 *
 * @param      byte0 - byte - 0
 * @param      byte1 - byte - 1
 * @param      byte2 - byte - 2
 * @param      byte3 - byte - 3
 *
 * @return     combined uint32_t
 */
extern uint32_t Util_buildUint32(uint8_t byte0, uint8_t byte1, uint8_t byte2,
                                 uint8_t byte3);

/*!
 * @brief      Pulls 1 uint8_t out of a uint32_t
 *
 * @param      var - uint32_t variable
 * @param      byteNum - what byte to pull out (0-3)
 *
 * @return     uint8_t
 */
extern uint8_t Util_breakUint32(uint32_t var, int byteNum);

/*!
 * @brief      Build a uint16_t from a uint8_t array
 *
 * @param      pArray - pointer to uint8_t array
 *
 * @return     combined uint16_t
 */

extern uint16_t Util_parseUint16(uint8_t *pArray);

/*!
 * @brief      Build a uint32_t from a uint8_t array
 *
 * @param      pArray - pointer to uint8_t array
 *
 * @return     combined uint32_t
 */
extern uint32_t Util_parseUint32(uint8_t *pArray);

/*!
 * @brief      Break and buffer a uint16 value - LSB first
 *
 * @param      pBuf - ptr to next available buffer location
 * @param      val  - 16-bit value to break/buffer
 *
 * @return     pBuf - ptr to next available buffer location
 */
extern uint8_t *Util_bufferUint16(uint8_t *pBuf, uint16_t val);

/*!
 * @brief      Break and buffer a uint32 value - LSB first
 *
 * @param      pBuf - ptr to next available buffer location
 * @param      val  - 32-bit value to break/buffer
 *
 * @return     pBuf - ptr to next available buffer location
 */
extern uint8_t *Util_bufferUint32(uint8_t *pBuf, uint32_t val);


/*!
 * @brief       Utility function to copy the extended address
 *
 * @param       pSrcAddr - pointer to source from which to be copied
 * @param       pDstAddr - pointer to destination to copy to
 */
extern void Util_copyExtAddr(void *pSrcAddr, void *pDstAddr);

/*show hex buffer*/
extern void show_hex(uint8_t *buf, uint16_t len);

/*crc16 util*/
uint16_t crc16Calc(uint8_t *bytes, uint32_t len, uint16_t init_crc);

int splitStr(char *Src, char *Delim, char *Des, int size);

int SplitnData(char *datin, char tok1, int tok1_n, char tok2, int size, char *datout);

uint32_t round_number(uint32_t n, uint32_t modulo);

uint8_t UINT8_Ascii_To_Hex(uint8_t hi, uint8_t lo);

float string_to_float(char *str);

void convert_cord_to_hex(float kd, float vd, uint8_t *kd_buf, uint8_t *vd_buf);

void convert_hex_to_cord(uint8_t *kd_buf, uint8_t *vd_buf, float *kd, float *vd);

bool is_valid_ip(char *ip_str);

//int DaysInMonth(int Month, int Year); 

//void GMT2LocalTime(struct tm GMTtime, struct tm *LocalTime, uint8_t timezone);

void rvereseArray(uint8_t *arr, int start, int end);

uint32_t String_to_Hex(const char *str, uint16_t len, uint8_t *hex_out);

void text_print(char const* p_label, char const * p_text, size_t len);
void hex_text_print(char const* p_label, char const * p_text, size_t len);

void convert_ascii_to_hex(char *str, uint16_t len, uint8_t *pbuf);
void convert_hex_to_ascii(uint8_t *pHex, uint16_t len, char *pAscii);

uint8_t hex_to_bcd(uint8_t hex_val);
uint8_t dec_to_bcd(uint8_t num);
uint8_t bcd_to_dec(uint8_t bcd);
/*! @} end group UtilMisc */
//void RED_TEXT(void);
//void YELLOW_TEXT(void);
//void BLUE_TEXT(void);
//void GREEN_TEXT(void);
//void NORMAL_TEXT(void);
#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */
