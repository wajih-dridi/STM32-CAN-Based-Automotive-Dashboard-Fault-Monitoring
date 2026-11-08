#ifndef CAN_PORT_H
#define CAN_PORT_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef CAN_Port_Start(void);
HAL_StatusTypeDef CAN_Port_Send(uint16_t standard_id,
                                const uint8_t *data, uint8_t length);

#endif
