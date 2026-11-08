#include "can_protocol.h"

static volatile VehicleData_t g_vehicle;

static uint16_t read_u16_be(const uint8_t *data)
{
    return (uint16_t)(((uint16_t)data[0] << 8) | (uint16_t)data[1]);
}

static int16_t read_i16_be(const uint8_t *data)
{
    uint16_t raw = read_u16_be(data);
    /* Conversion from 16-bit two's-complement representation. */
    if (raw <= 0x7FFFU)
    {
        return (int16_t)raw;
    }
    return (int16_t)((int32_t)raw - 65536L);
}

void CANProtocol_Init(void)
{
    g_vehicle.rpm = 0U;
    g_vehicle.speed_kmh = 0U;
    g_vehicle.temperature_c = 0;
    g_vehicle.battery_mv = 0U;
    g_vehicle.over_temperature = 0U;
    g_vehicle.transmitter_fault = 0U;
    g_vehicle.rpm_seen = 0U;
    g_vehicle.speed_seen = 0U;
    g_vehicle.temperature_seen = 0U;
    g_vehicle.battery_seen = 0U;
    g_vehicle.rpm_last_tick = 0U;
    g_vehicle.speed_last_tick = 0U;
    g_vehicle.temperature_last_tick = 0U;
    g_vehicle.battery_last_tick = 0U;
}

void CANProtocol_Process(uint32_t id, const uint8_t *data,
                         uint8_t length, uint32_t now)
{
    if ((data == 0) || (length < 2U))
    {
        return;
    }

    switch (id)
    {
        case CAN_ID_ENGINE_RPM:
            g_vehicle.rpm = read_u16_be(data);
            g_vehicle.rpm_last_tick = now;
            g_vehicle.rpm_seen = 1U;
            break;

        case CAN_ID_VEHICLE_SPEED:
            g_vehicle.speed_kmh = read_u16_be(data);
            g_vehicle.speed_last_tick = now;
            g_vehicle.speed_seen = 1U;
            break;

        case CAN_ID_ENGINE_TEMP:
            g_vehicle.temperature_c = read_i16_be(data);
            g_vehicle.temperature_last_tick = now;
            g_vehicle.temperature_seen = 1U;
            g_vehicle.over_temperature =
                (g_vehicle.temperature_c > VEHICLE_TEMP_LIMIT_C) ? 1U : 0U;
            break;

        case CAN_ID_BATTERY_MV:
            g_vehicle.battery_mv = read_u16_be(data);
            g_vehicle.battery_last_tick = now;
            g_vehicle.battery_seen = 1U;
            break;

        case CAN_ID_FAULT_STATUS:
            g_vehicle.over_temperature = (data[0] & 0x01U) ? 1U : 0U;
            g_vehicle.transmitter_fault = (data[0] & 0x02U) ? 1U : 0U;
            break;

        default:
            /* Unknown CAN identifier: intentionally ignored. */
            break;
    }
}

VehicleData_t CANProtocol_GetSnapshot(void)
{
    VehicleData_t snapshot;

    /*
     * The callback and main loop share data. Copy it briefly with interrupts
     * masked so a multi-field snapshot is less likely to be torn.
     */
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    snapshot.rpm = g_vehicle.rpm;
    snapshot.speed_kmh = g_vehicle.speed_kmh;
    snapshot.temperature_c = g_vehicle.temperature_c;
    snapshot.battery_mv = g_vehicle.battery_mv;
    snapshot.over_temperature = g_vehicle.over_temperature;
    snapshot.transmitter_fault = g_vehicle.transmitter_fault;
    snapshot.rpm_seen = g_vehicle.rpm_seen;
    snapshot.speed_seen = g_vehicle.speed_seen;
    snapshot.temperature_seen = g_vehicle.temperature_seen;
    snapshot.battery_seen = g_vehicle.battery_seen;
    snapshot.rpm_last_tick = g_vehicle.rpm_last_tick;
    snapshot.speed_last_tick = g_vehicle.speed_last_tick;
    snapshot.temperature_last_tick = g_vehicle.temperature_last_tick;
    snapshot.battery_last_tick = g_vehicle.battery_last_tick;

    if (primask == 0U)
    {
        __enable_irq();
    }
    return snapshot;
}

uint8_t CANProtocol_HasCommunicationTimeout(uint32_t now)
{
    VehicleData_t v = CANProtocol_GetSnapshot();

    if ((v.rpm_seen == 0U) || (v.speed_seen == 0U) ||
        (v.temperature_seen == 0U) || (v.battery_seen == 0U))
    {
        return 1U;
    }

    if (((uint32_t)(now - v.rpm_last_tick) > VEHICLE_TIMEOUT_MS) ||
        ((uint32_t)(now - v.speed_last_tick) > VEHICLE_TIMEOUT_MS) ||
        ((uint32_t)(now - v.temperature_last_tick) > VEHICLE_TIMEOUT_MS) ||
        ((uint32_t)(now - v.battery_last_tick) > VEHICLE_TIMEOUT_MS))
    {
        return 1U;
    }

    return 0U;
}
