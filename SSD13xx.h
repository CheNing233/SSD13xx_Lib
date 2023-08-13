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
 * @brief ��ʼ���ṹ��
 */
typedef struct
{
    /*
     * @brief ����ͨѶ
     */
    void (*StartComunication)();

    /*
     * @brief ���͵�λ�ֽں���
     */
    void (*SendByte)(uint8_t byte);

    /*
     * @brief ���ܵ�λ�ֽں���
     */
    void (*ReceiveByte)(uint8_t *byte_buf);

    /*
     * @brief ֹͣͨѶ
     */
    void (*StopComunication)();

} SSD13xx_InitTypeDef;


/*
 * @brief SSD13xx�豸���
 */
typedef SSD13xx_InitTypeDef * SSD13xx_handle_t;


/*
 * @brief SSD13xx������
 */
struct _SSD13xx_Class{
    /**
     * @brief ��ʼ��SSD13xx�豸
     *
     * @parameter SSD13xx_InitStructure SSD13xx�豸��ʼ�����ýṹ��
     * @parameter SSD13xx_Handle SSD13xx�豸���
     */
    void (*Init)(SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle);

    /**
     * @brief �����Ļ��ʾ����
     *
     * @parameter SSD13xx_Handle SSD13xx�豸���
     */
    void (*Clear)(SSD13xx_handle_t SSD13xx_Handle);

    /**
     * @brief ��ָ��λ�ô�ӡ��ʽ���ַ���
     *
     * @parameter SSD13xx_Handle SSD13xx�豸���
     * @parameter pos_x X������
     * @parameter pos_y Y������
     * @parameter ptr_format ��ʽ���ַ���
     * @parameter ... �ɱ�����б�
     */
    void (*PrintF)(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...);

    /**
     * @brief ��ָ��λ�ô�ӡ�����ַ�
     *
     * @parameter SSD13xx_Handle SSD13xx�豸���
     * @parameter pos_x X������
     * @parameter pos_y Y������
     * @parameter ch Ҫ��ӡ���ַ�
     */
    void (*PutChar)(SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, uint8_t ch);

    /**
     * @brief ���ù��λ��
     *
     * @parameter SSD13xx_Handle SSD13xx�豸���
     * @parameter Pos_X X������
     * @parameter Pos_Y Y������
     */
    void (*SetCursor)(SSD13xx_handle_t SSD13xx_Handle, uint8_t Pos_X, uint8_t Pos_Y);
};


/*
 * @brief SSD13xxȫ�ֶ���
 */
extern struct _SSD13xx_Class SSD13xx;

#ifdef __cplusplus
}
#endif

#endif
