#include "tm1637.h"

static const unsigned char seg_data[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static void TM1637_Delay(void)
{
    unsigned int i;
    for (i = 0; i < 10; i++)
    {
        __NOP();
    }
}

static void TM1637_Start(void)
{
    TM1637_CLK_HIGH();
    TM1637_DIO_HIGH();
    TM1637_Delay();
    TM1637_DIO_LOW();
    TM1637_Delay();
    TM1637_CLK_LOW();
}

static void TM1637_Stop(void)
{
    TM1637_CLK_LOW();
    TM1637_Delay();
    TM1637_DIO_LOW();
    TM1637_Delay();
    TM1637_CLK_HIGH();
    TM1637_Delay();
    TM1637_DIO_HIGH();
    TM1637_Delay();
}

static unsigned char TM1637_WriteByte(unsigned char data)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        TM1637_CLK_LOW();
        if (data & 0x01)
            TM1637_DIO_HIGH();
        else
            TM1637_DIO_LOW();
        TM1637_Delay();
        TM1637_CLK_HIGH();
        TM1637_Delay();
        data >>= 1;
    }

    TM1637_CLK_LOW();
    TM1637_DIO_HIGH();
    TM1637_Delay();
    TM1637_CLK_HIGH();
    TM1637_Delay();

    unsigned char ack = TM1637_DIO_READ();
    TM1637_CLK_LOW();
    TM1637_Delay();

    return ack;
}

void TM1637_Init(void)
{
    TM1637_CLK_HIGH();
    TM1637_DIO_HIGH();
    TM1637_Delay();

    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_DATA);
    TM1637_Stop();

    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_DISPLAY);
    TM1637_Stop();

    TM1637_Clear();
}

void TM1637_DisplayNum(unsigned int num, unsigned char leading_zero)
{
    unsigned char digits[4];
    unsigned char i;
    unsigned char show = 0;

    if (num > 9999) num = 9999;

    digits[0] = (num / 1000) % 10;
    digits[1] = (num / 100) % 10;
    digits[2] = (num / 10) % 10;
    digits[3] = num % 10;

    if (leading_zero) show = 1;

    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_ADDR);

    for (i = 0; i < 4; i++)
    {
        if (digits[i] != 0 || show || i == 3)
        {
            show = 1;
            TM1637_WriteByte(seg_data[digits[i]]);
        }
        else
        {
            TM1637_WriteByte(0x00);
        }
    }

    TM1637_Stop();
}

void TM1637_DisplayTemp(unsigned char temp)
{
    unsigned char digits[4];
    digits[0] = (temp / 10) % 10;
    digits[1] = temp % 10;
    digits[2] = 0xFF;
    digits[3] = 0xFF;

    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_ADDR);

    TM1637_WriteByte(seg_data[digits[0]]);
    TM1637_WriteByte(seg_data[digits[1]] | 0x80);
    TM1637_WriteByte(0x39);
    TM1637_WriteByte(0x00);

    TM1637_Stop();
}

void TM1637_DisplayHighScore(unsigned int score)
{
    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_ADDR);

    TM1637_WriteByte(0x76);
    TM1637_WriteByte(0x06);
    TM1637_WriteByte(0x40);
    TM1637_WriteByte(0x40);

    TM1637_Stop();

    if (score > 9999) score = 9999;

    unsigned char d0 = (score / 1000) % 10;
    unsigned char d1 = (score / 100) % 10;
    unsigned char d2 = (score / 10) % 10;
    unsigned char d3 = score % 10;

    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_ADDR);

    TM1637_WriteByte(seg_data[d0]);
    TM1637_WriteByte(seg_data[d1]);
    TM1637_WriteByte(seg_data[d2]);
    TM1637_WriteByte(seg_data[d3]);

    TM1637_Stop();
}

void TM1637_SetBrightness(unsigned char brightness)
{
    if (brightness > 7) brightness = 7;
    TM1637_Start();
    TM1637_WriteByte(0x88 | brightness);
    TM1637_Stop();
}

void TM1637_Blink(unsigned char on)
{
    TM1637_Start();
    if (on)
        TM1637_WriteByte(0x89);
    else
        TM1637_WriteByte(TM1637_CMD_DISPLAY);
    TM1637_Stop();
}

void TM1637_Clear(void)
{
    unsigned char i;
    TM1637_Start();
    TM1637_WriteByte(TM1637_CMD_ADDR);
    for (i = 0; i < 4; i++)
    {
        TM1637_WriteByte(0x00);
    }
    TM1637_Stop();
}
