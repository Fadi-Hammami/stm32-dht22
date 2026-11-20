#include "dht22.h"

extern TIM_HandleTypeDef htim2;

void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);

    while (__HAL_TIM_GET_COUNTER(&htim2) < us);
}

static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DHT22_Start(void)
{
    Set_Pin_Output(GPIOA, GPIO_PIN_1);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

    HAL_Delay(2);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

    delay_us(30);

    Set_Pin_Input(GPIOA, GPIO_PIN_1);
}

uint8_t DHT22_Check_Response(void)
{
    uint8_t Response = 0;

    delay_us(40);

    if (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
    {
        delay_us(80);

        if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
        {
            Response = 1;
        }
    }

    while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));

    return Response;
}

uint8_t DHT22_Read(void)
{
    uint8_t i = 0, j;

    for (j = 0; j < 8; j++)
    {
        while (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));

        delay_us(40);

        if (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)))
        {
            i &= ~(1 << (7 - j));
        }
        else
        {
            i |= (1 << (7 - j));
        }

        while ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)));
    }

    return i;
}

float DHT22_GetTemperature(void)
{
	uint8_t Temp_byte1, Temp_byte2;

    float Temperature;

    DHT22_Start();
    DHT22_Check_Response();

    DHT22_Read();
    DHT22_Read();

    Temp_byte1 = DHT22_Read();
    Temp_byte2 = DHT22_Read();

    DHT22_Read();

    Temperature = ((Temp_byte1 << 8) | Temp_byte2);

    Temperature /= 10.0;

    return Temperature;
}
