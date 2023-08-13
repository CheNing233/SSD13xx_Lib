/**
 ******************************************************************************
 * @file    SSD13xx.c
 * @brief   Library for controlling SSD13xx OLED display controller
 * @version 1.0.0
 * @date    2023-08-14
 * @author  xChenNing
 * @license MIT License
 ******************************************************************************
 */

// INCLUDE
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "SSD13xx_BaseFonts.h"
#include "SSD13xx.h"

// CMD LIST
#define _SSD13XX_CMD_ADDRESS 0x78                    // SSD13xx �豸��ַ

#define _SSD13XX_CMD_DATA_FLAG 0x40                  // ���ݱ�־λ
#define _SSD13XX_CMD_COMMAND_FLAG 0x00               // �����־λ

#define _SSD13XX_CMD_DISPLAY_ON 0xAF                 // ����ʾ
#define _SSD13XX_CMD_DISPLAY_OFF 0xAE                // �ر���ʾ
#define _SSD13XX_CMD_DISPLAY_OFFSET 0xD3             // ������ʾƫ��
#define _SSD13XX_CMD_ENTIRE_DISPLAY_ON 0xA4          // ��ȫ����ʾ
#define _SSD13XX_CMD_INVERSE_OFF 0xA6                // ȡ����ת��ʾ

#define _SSD13XX_CMD_DCLK_SET 0xD5                   // ����ʱ�ӷ�Ƶ��
#define _SSD13XX_CMD_MUX_RATIO_SET 0xA8              // ���ö�·���ñ���
#define _SSD13XX_CMD_COM_CFG_SET 0xDA                // ���� COM ����
#define _SSD13XX_CMD_CONTRAST_RATIO_SET 0x81         // ���öԱȶ�
#define _SSD13XX_CMD_PRE_CHARGING_PERIOD_SET 0xD9    // ����Ԥ�������
#define _SSD13XX_CMD_VCOMH_DESELECT_LEVEL_SET 0xDB   // ���� VCOMH ��ѡ��ѹ
#define _SSD13XX_CMD_CHARGING_PUMP_SET 0x8D          // ���ó���

#define _SSD13XX_CMD_LEFT_RIGHT_UPRIGHT 0xA1         // ��ֱ�����ҵ���ת
#define _SSD13XX_CMD_LEFT_RIGHT_INVERT 0xA0          // ��ֱ����������ʾ
#define _SSD13XX_CMD_UP_DOWN_UPRIGHT 0xC8            // ˮƽ�����µ��Ϸ�ת
#define _SSD13XX_CMD_UP_DOWN_INVERT 0xC0             // ˮƽ����������ʾ


// FUNCTIONS
static void _Init(SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle);
static void _PutChar(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, uint8_t ch);
static void _PrintF(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...);
static void _Clear(SSD13xx_handle_t SSD13xx_Handle);
static void _SetCursor(SSD13xx_handle_t SSD13xx_Handle, uint8_t Pos_X, uint8_t Pos_Y);
static inline void _WriteCommand(SSD13xx_handle_t SSD13xx_Handle, uint8_t Command);
static inline void _WriteData(SSD13xx_handle_t SSD13xx_Handle, uint8_t Data);

// SSD13xx�ඨ��
struct _SSD13xx_Class SSD13xx = {
        .Init = _Init,
        .Clear = _Clear,
        .PrintF = _PrintF,
        .PutChar = _PutChar,
        .SetCursor = _SetCursor
};

void _Init(SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle){
    // �������
    SSD13xx_handle_t ptr = (SSD13xx_handle_t)malloc(sizeof(SSD13xx_InitTypeDef));

    // ��������
    memcpy(ptr, SSD13xx_InitStructure, sizeof(SSD13xx_InitTypeDef));

    // ��ʼ���豸
    _WriteCommand(ptr, _SSD13XX_CMD_DISPLAY_OFF);

    _WriteCommand(ptr, _SSD13XX_CMD_DCLK_SET);
    _WriteCommand(ptr, 0x80);

    _WriteCommand(ptr, _SSD13XX_CMD_MUX_RATIO_SET);
    _WriteCommand(ptr, 0x3F);

    _WriteCommand(ptr, _SSD13XX_CMD_DISPLAY_OFFSET);
    _WriteCommand(ptr, 0x00);

    _WriteCommand(ptr, 0x40);

    _WriteCommand(ptr, _SSD13XX_CMD_LEFT_RIGHT_UPRIGHT);
    _WriteCommand(ptr, _SSD13XX_CMD_UP_DOWN_UPRIGHT);

    _WriteCommand(ptr, _SSD13XX_CMD_COM_CFG_SET);
    _WriteCommand(ptr, 0x12);

    _WriteCommand(ptr, _SSD13XX_CMD_CONTRAST_RATIO_SET);
    _WriteCommand(ptr, 0xCF);

    _WriteCommand(ptr, _SSD13XX_CMD_PRE_CHARGING_PERIOD_SET);
    _WriteCommand(ptr, 0xF1);

    _WriteCommand(ptr, _SSD13XX_CMD_VCOMH_DESELECT_LEVEL_SET);
    _WriteCommand(ptr, 0x30);

    _WriteCommand(ptr, _SSD13XX_CMD_ENTIRE_DISPLAY_ON);

    _WriteCommand(ptr, _SSD13XX_CMD_INVERSE_OFF);

    _WriteCommand(ptr, _SSD13XX_CMD_CHARGING_PUMP_SET);
    _WriteCommand(ptr, 0x14);

    _WriteCommand(ptr, _SSD13XX_CMD_DISPLAY_ON);

    _Clear(ptr);
    // ���ؾ��
    *SSD13xx_Handle = ptr;
}

void _WriteCommand(SSD13xx_handle_t SSD13xx_Handle, uint8_t Command){
    SSD13xx_Handle->StartComunication();

    // Address
    SSD13xx_Handle->SendByte(_SSD13XX_CMD_ADDRESS);
    // Command Flag
    SSD13xx_Handle->SendByte(_SSD13XX_CMD_COMMAND_FLAG);

    SSD13xx_Handle->SendByte(Command);
    SSD13xx_Handle->StopComunication();
}

void _WriteData(SSD13xx_handle_t SSD13xx_Handle, uint8_t Data){
    SSD13xx_Handle->StartComunication();

    // Address
    SSD13xx_Handle->SendByte(_SSD13XX_CMD_ADDRESS);
    // Data Flag
    SSD13xx_Handle->SendByte(_SSD13XX_CMD_DATA_FLAG);

    SSD13xx_Handle->SendByte(Data);
    SSD13xx_Handle->StopComunication();
}

void _SetCursor(SSD13xx_handle_t SSD13xx_Handle, uint8_t Pos_X, uint8_t Pos_Y){
    //����Yλ��
    _WriteCommand(SSD13xx_Handle, 0xB0 | Pos_Y);
    //����Xλ�ø�4λ
    _WriteCommand(SSD13xx_Handle, 0x10 | ((Pos_X & 0xF0) >> 4));
    //����Xλ�õ�4λ
    _WriteCommand(SSD13xx_Handle, 0x00 | (Pos_X & 0x0F));
}

void _Clear(SSD13xx_handle_t SSD13xx_Handle){
    uint8_t pos_x, pos_y;
    for (pos_y = 0; pos_y < 8; pos_y++)
    {
        _SetCursor(SSD13xx_Handle, 0, pos_y);
        for(pos_x = 0; pos_x < 128; pos_x++)
        {
            _WriteData(SSD13xx_Handle, 0x00);
        }
    }
}

void _PutChar(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, uint8_t ch){
    uint8_t i;
    //���ù��λ�����ϰ벿��
    _SetCursor(SSD13xx_Handle, (pos_x) * 8, (pos_y) * 2);
    //��ʾ�ϰ벿������
    for (i = 0; i < 8; i++)
    {
        _WriteData(SSD13xx_Handle, BaseFonts_8x16[ch - ' '][i]);
    }

    //���ù��λ�����°벿��
    _SetCursor(SSD13xx_Handle, (pos_x) * 8, (pos_y) * 2 + 1);
    //��ʾ�°벿������
    for (i = 0; i < 8; i++)
    {
        _WriteData(SSD13xx_Handle, BaseFonts_8x16[ch - ' '][i + 8]);
    }
}

void _PrintF(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...){
    // ����������
    char buf[256];
    // ��ʽ���ַ���
    va_list args;
    va_start(args, ptr_format);
    vsnprintf(buf, sizeof(buf), ptr_format, args);
    va_end(args);
    // ����ַ�
    uint8_t i,len = strlen(buf);
    for(i = 0; i < len; i++){
        _PutChar(SSD13xx_Handle, pos_x + i, pos_y, buf[i]);
    }
}
