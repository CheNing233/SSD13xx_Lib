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
#define _SSD13XX_CMD_ADDRESS 0x78                    // SSD13xx 设备地址

#define _SSD13XX_CMD_DATA_FLAG 0x40                  // 数据标志位
#define _SSD13XX_CMD_COMMAND_FLAG 0x00               // 命令标志位

#define _SSD13XX_CMD_DISPLAY_ON 0xAF                 // 打开显示
#define _SSD13XX_CMD_DISPLAY_OFF 0xAE                // 关闭显示
#define _SSD13XX_CMD_DISPLAY_OFFSET 0xD3             // 设置显示偏移
#define _SSD13XX_CMD_ENTIRE_DISPLAY_ON 0xA4          // 打开全屏显示
#define _SSD13XX_CMD_INVERSE_OFF 0xA6                // 取消反转显示

#define _SSD13XX_CMD_DCLK_SET 0xD5                   // 设置时钟分频器
#define _SSD13XX_CMD_MUX_RATIO_SET 0xA8              // 设置多路复用比率
#define _SSD13XX_CMD_COM_CFG_SET 0xDA                // 设置 COM 配置
#define _SSD13XX_CMD_CONTRAST_RATIO_SET 0x81         // 设置对比度
#define _SSD13XX_CMD_PRE_CHARGING_PERIOD_SET 0xD9    // 设置预充电周期
#define _SSD13XX_CMD_VCOMH_DESELECT_LEVEL_SET 0xDB   // 设置 VCOMH 反选电压
#define _SSD13XX_CMD_CHARGING_PUMP_SET 0x8D          // 设置充电泵

#define _SSD13XX_CMD_LEFT_RIGHT_UPRIGHT 0xA1         // 垂直方向右到左反转
#define _SSD13XX_CMD_LEFT_RIGHT_INVERT 0xA0          // 垂直方向正常显示
#define _SSD13XX_CMD_UP_DOWN_UPRIGHT 0xC8            // 水平方向下到上反转
#define _SSD13XX_CMD_UP_DOWN_INVERT 0xC0             // 水平方向正常显示


// FUNCTIONS
static void _Init(SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle);
static void _PutChar(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, uint8_t ch);
static void _PrintF(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...);
static void _Clear(SSD13xx_handle_t SSD13xx_Handle);
static void _SetCursor(SSD13xx_handle_t SSD13xx_Handle, uint8_t Pos_X, uint8_t Pos_Y);
static inline void _WriteCommand(SSD13xx_handle_t SSD13xx_Handle, uint8_t Command);
static inline void _WriteData(SSD13xx_handle_t SSD13xx_Handle, uint8_t Data);

// SSD13xx类定义
struct _SSD13xx_Class SSD13xx = {
        .Init = _Init,
        .Clear = _Clear,
        .PrintF = _PrintF,
        .PutChar = _PutChar,
        .SetCursor = _SetCursor
};

void _Init(SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle){
    // 创建句柄
    SSD13xx_handle_t ptr = (SSD13xx_handle_t)malloc(sizeof(SSD13xx_InitTypeDef));

    // 复制配置
    memcpy(ptr, SSD13xx_InitStructure, sizeof(SSD13xx_InitTypeDef));

    // 初始化设备
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
    // 返回句柄
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
    //设置Y位置
    _WriteCommand(SSD13xx_Handle, 0xB0 | Pos_Y);
    //设置X位置高4位
    _WriteCommand(SSD13xx_Handle, 0x10 | ((Pos_X & 0xF0) >> 4));
    //设置X位置低4位
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
    //设置光标位置在上半部分
    _SetCursor(SSD13xx_Handle, (pos_x) * 8, (pos_y) * 2);
    //显示上半部分内容
    for (i = 0; i < 8; i++)
    {
        _WriteData(SSD13xx_Handle, BaseFonts_8x16[ch - ' '][i]);
    }

    //设置光标位置在下半部分
    _SetCursor(SSD13xx_Handle, (pos_x) * 8, (pos_y) * 2 + 1);
    //显示下半部分内容
    for (i = 0; i < 8; i++)
    {
        _WriteData(SSD13xx_Handle, BaseFonts_8x16[ch - ' '][i + 8]);
    }
}

void _PrintF(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...){
    // 建立缓存区
    char buf[256];
    // 格式化字符串
    va_list args;
    va_start(args, ptr_format);
    vsnprintf(buf, sizeof(buf), ptr_format, args);
    va_end(args);
    // 输出字符
    uint8_t i,len = strlen(buf);
    for(i = 0; i < len; i++){
        _PutChar(SSD13xx_Handle, pos_x + i, pos_y, buf[i]);
    }
}
