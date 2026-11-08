#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <stdint.h>

void Dashboard_Init(void);
void Dashboard_Process(void);
void Dashboard_OnCanFrame(uint32_t id, const uint8_t *data, uint8_t length);

#endif
