#include "ultrasonic.h"

static float last_distance = 999.0f;
static unsigned int read_count = 0;

static void Delay_us(unsigned int us)
{
    unsigned int i;
    for (i = 0; i < us * 8; i++)
    {
        __NOP();
    }
}

void Ultrasonic_Init(void)
{
    TRIG_OFF();
}

float Ultrasonic_GetDistance(void)
{
    unsigned int timeout;
    unsigned int pulse_width;

    TRIG_OFF();
    Delay_us(2);
    TRIG_ON();
    Delay_us(10);
    TRIG_OFF();

    timeout = 30000;
    while (ECHO_READ() == GPIO_PIN_RESET && timeout > 0)
    {
        timeout--;
    }
    if (timeout == 0) return 999.0f;

    pulse_width = 0;
    timeout = 30000;
    while (ECHO_READ() == GPIO_PIN_SET && timeout > 0)
    {
        pulse_width++;
        Delay_us(1);
        timeout--;
    }
    if (timeout == 0) return 999.0f;

    last_distance = (float)pulse_width * 0.017f;
    return last_distance;
}

unsigned char Ultrasonic_IsNear(void)
{
    read_count++;
    if (read_count < ULTRASONIC_INTERVAL) return 0;
    read_count = 0;

    float dist = Ultrasonic_GetDistance();
    return (dist > 0.1f && dist < (float)DISTANCE_JUMP_THRESHOLD) ? 1 : 0;
}
