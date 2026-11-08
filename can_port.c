#include "main.h"
#include "can.h"
#include "can_port.h"
#include "dashboard.h"

HAL_StatusTypeDef CAN_Port_Start(void)
{
    CAN_FilterTypeDef filter = {0};

    /* Accept all frames into FIFO0 for this learning project.
       CANProtocol_Process() filters by application identifier. */
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &filter) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_CAN_ActivateNotification(&hcan1,
            CAN_IT_RX_FIFO0_MSG_PENDING |
            CAN_IT_ERROR |
            CAN_IT_BUSOFF) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef CAN_Port_Send(uint16_t standard_id,
                                const uint8_t *data, uint8_t length)
{
    CAN_TxHeaderTypeDef header = {0};
    uint32_t mailbox = 0U;
    uint8_t payload[8] = {0};

    if ((data == 0) || (length > 8U) || (standard_id > 0x7FFU))
    {
        return HAL_ERROR;
    }

    for (uint8_t i = 0U; i < length; i++)
    {
        payload[i] = data[i];
    }

    header.StdId = standard_id;
    header.ExtId = 0U;
    header.IDE = CAN_ID_STD;
    header.RTR = CAN_RTR_DATA;
    header.DLC = length;
    header.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&hcan1, &header, payload, &mailbox);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef header = {0};
    uint8_t data[8] = {0};

    if (hcan->Instance != CAN1)
    {
        return;
    }

    while (HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0U)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
        {
            break;
        }

        if ((header.IDE == CAN_ID_STD) && (header.RTR == CAN_RTR_DATA))
        {
            Dashboard_OnCanFrame(header.StdId, data, header.DLC);
        }
    }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
    /* Add diagnostic logging here when extending the project.
       Avoid blocking UART prints inside an interrupt callback. */
    (void)hcan;
}
