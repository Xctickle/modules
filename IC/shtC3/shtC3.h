#ifndef _SHTC3_H_
#define _SHTC3_H_



#include "modules.h"
#include "SoftwareI2C.h"
#include "delay.h"


//-- Enumerations --------------------------------------------------------------
// Error codes
typedef enum{
  NO_ERROR       = 0x00, // no error
  ACK_ERROR      = 0x01, // no acknowledgment error
  CHECKSUM_ERROR = 0x02 // checksum mismatch error
}etError;


//==============================================================================
void SHTC3_Init(void);
//==============================================================================
// Initializes the I2C bus for communication with the sensor.
//------------------------------------------------------------------------------

//==============================================================================
etError SHTC3_GetId(uint16_t *id);
//==============================================================================
// Gets the ID from the sensor.
//------------------------------------------------------------------------------
// input:  *id          pointer to a integer, where the id will be stored
//
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      CHECKSUM_ERROR = checksum mismatch
//                      NO_ERROR       = no error

//==============================================================================
etError SHTC3_GetTempAndHumi(float *temp, float *humi);
//==============================================================================
// Gets the temperature [°C] and the humidity [%RH].
//------------------------------------------------------------------------------
// input:  *temp        pointer to a floating point value, where the calculated
//                      temperature will be stored
//         *humi        pointer to a floating point value, where the calculated
//                      humidity will be stored
//
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      CHECKSUM_ERROR = checksum mismatch
//                      NO_ERROR       = no error
//
// remark: If you use this function, then the sensor blocks the I2C-bus with
//         clock stretching during the measurement.

//==============================================================================
etError SHTC3_GetTempAndHumiPolling(float *temp, float *humi);
//==============================================================================
// Gets the temperature [°C] and the humidity [%RH]. This function polls every
// 1ms until measurement is ready.
//------------------------------------------------------------------------------
// input:  *temp        pointer to a floating point value, where the calculated
//                      temperature will be stored
//         *humi        pointer to a floating point value, where the calculated
//                      humidity will be stored
//
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      CHECKSUM_ERROR = checksum mismatch
//                      NO_ERROR       = no error

etError SHTC3_Sleep(void);
etError SHTC3_Wakeup(void);

//==============================================================================
etError SHTC3_SoftReset(void);
//==============================================================================
// Calls the soft reset mechanism that forces the sensor into a well-defined
// state without removing the power supply.
//------------------------------------------------------------------------------
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      NO_ERROR       = no error


#endif
