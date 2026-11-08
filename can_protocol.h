#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H

#include <stdint.h>

#define CAN_ID_ENGINE_RPM       0x100U
#define CAN_ID_VEHICLE_SPEED    0x101U
#define CAN_ID_ENGINE_TEMP      0x102U
#define CAN_ID_BATTERY_MV       0x103U
#define CAN_ID_FAULT_STATUS     0x200U

#define VEHICLE_TEMP_LIMIT_C    105
#define VEHICLE_TIMEOUT_MS      1000U

typedef struct
{
    uint16_t rpm;
    uint16_t speed_kmh;
    int16_t  temperature_c;
    uint16_t battery_mv;

    uint8_t  over_temperature;
    uint8_t  transmitter_fault;

    uint8_t  rpm_seen;
    uint8_t  speed_seen;
    uint8_t  temperature_seen;
    uint8_t  battery_seen;

    uint32_t rpm_last_tick;
    uint32_t speed_last_tick;
    uint32_t temperature_last_tick;
    uint32_t battery_last_tick;
} VehicleData_t;

void CANProtocol_Init(void);
void CANProtocol_Process(uint32_t id, const uint8_t *data,
                         uint8_t length, uint32_t now);
VehicleData_t CANProtocol_GetSnapshot(void);
uint8_t CANProtocol_HasCommunicationTimeout(uint32_t now);

#endif
