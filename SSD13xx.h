/**
 ******************************************************************************
 * @file    SSD13xx.h
 * @brief   Library for controlling SSD13xx OLED display controller
 * @version 1.0.0
 * @date    2023-08-14
 * @author  xChenNing
 * @license MIT License
 ******************************************************************************
 */

#ifndef __SSD13xx_H__
#define __SSD13xx_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdarg.h"

/*
 * @brief 初始化结构体
 */
typedef struct
{
    /*
     * @brief 启动通讯
     */
    void (*StartComunication)();

    /*
     * @brief 发送单位字节函数
     */
    void (*SendByte)(uint8_t byte);

    /*
     * @brief 接受单位字节函数
     */
    void (*ReceiveByte)(uint8_t *byte_buf);

    /*
     * @brief 停止通讯
     */
    void (*StopComunication)();

} SSD13xx_InitTypeDef;


/*
 * @brief SSD13xx设备句柄
 */
typedef SSD13xx_InitTypeDef * SSD13xx_handle_t;


/*
 * @brief SSD13xx类声明
 */
struct _SSD13xx_Class{
    /**
     * @brief 初始化SSD13xx设备
     *
     * @parameter SSD13xx_InitStructure SSD13xx设备初始化配置结构体
     * @parameter SSD13xx_Handle SSD13xx设备句柄
     */
    void (*Init)(SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle);

    /**
     * @brief 清空屏幕显示内容
     *
     * @parameter SSD13xx_Handle SSD13xx设备句柄
     */
    void (*Clear)(SSD13xx_handle_t SSD13xx_Handle);

    /**
     * @brief 在指定位置打印格式化字符串
     *
     * @parameter SSD13xx_Handle SSD13xx设备句柄
     * @parameter pos_x X轴坐标
     * @parameter pos_y Y轴坐标
     * @parameter ptr_format 格式化字符串
     * @parameter ... 可变参数列表
     */
    void (*PrintF)(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...);

    /**
     * @brief 在指定位置打印单个字符
     *
     * @parameter SSD13xx_Handle SSD13xx设备句柄
     * @parameter pos_x X轴坐标
     * @parameter pos_y Y轴坐标
     * @parameter ch 要打印的字符
     */
    void (*PutChar)(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, uint8_t ch);

    /**
     * @brief 设置光标位置
     *
     * @parameter SSD13xx_Handle SSD13xx设备句柄
     * @parameter Pos_X X轴坐标
     * @parameter Pos_Y Y轴坐标
     */
    void (*SetCursor)(SSD13xx_handle_t SSD13xx_Handle, uint8_t Pos_X, uint8_t Pos_Y);
};


/*
 * @brief SSD13xx全局对象
 */
extern struct _SSD13xx_Class SSD13xx;

#ifdef __cplusplus
}
#endif

#endif
