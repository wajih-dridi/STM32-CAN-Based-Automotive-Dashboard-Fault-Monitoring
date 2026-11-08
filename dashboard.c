#include "main.h"
#include "dashboard.h"
#include "can_protocol.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

static uint32_t g_last_print_tick;

static void uart_write(const char *text)
{
    (void)HAL_UART_Transmit(&huart2, (uint8_t *)text,
                            (uint16_t)strlen(text), 100U);
}

void Dashboard_Init(void)
{
    CANProtocol_Init();
    g_last_print_tick = HAL_GetTick();

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);

    uart_write("\r\n=== STM32F407 CAN AUTOMOTIVE DASHBOARD ===\r\n");
    uart_write("CAN1: 500 kbit/s | UART2: 115200 8-N-1\r\n");
    uart_write("Waiting for vehicle telemetry...\r\n\r\n");
}

void Dashboard_OnCanFrame(uint32_t id, const uint8_t *data, uint8_t length)
{
    CANProtocol_Process(id, data, length, HAL_GetTick());
}

void Dashboard_Process(void)
{
    char line[220];
    uint32_t now = HAL_GetTick();

    if ((uint32_t)(now - g_last_print_tick) < 500U)
    {
        return;
    }
    g_last_print_tick = now;

    VehicleData_t v = CANProtocol_GetSnapshot();
    uint8_t timeout = CANProtocol_HasCommunicationTimeout(now);
    uint8_t fault = (uint8_t)(timeout || v.over_temperature || v.transmitter_fault);

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,
                      fault ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14,
                      fault ? GPIO_PIN_SET : GPIO_PIN_RESET);

    (void)snprintf(line, sizeof(line),
        "RPM:%u | Speed:%u km/h | Temp:%d C | Battery:%u.%03u V\r\n",
        (unsigned int)v.rpm,
        (unsigned int)v.speed_kmh,
        (int)v.temperature_c,
        (unsigned int)(v.battery_mv / 1000U),
        (unsigned int)(v.battery_mv % 1000U));
    uart_write(line);

    if (timeout)
    {
        uart_write("FAULT: CAN TELEMETRY TIMEOUT / DATA NOT RECEIVED\r\n");
    }
    if (v.over_temperature)
    {
        uart_write("FAULT: ENGINE OVER-TEMPERATURE (>105 C)\r\n");
    }
    if (v.transmitter_fault)
    {
        uart_write("FAULT: TRANSMITTER REPORTED A FAULT\r\n");
    }
    if (!fault)
    {
        uart_write("STATUS: NORMAL\r\n");
    }
    uart_write("----------------------------------------------\r\n");
}
