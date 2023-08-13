# SSD13xx_Lib

A simple driver library suitable for SSD1306/SSD1315, providing `printf(format_str, ...)` function for visual debugging

## Feature

Only 4...

- [x] .Init (SSD13xx_InitTypeDef *SSD13xx_InitStructure, SSD13xx_handle_t *SSD13xx_Handle);
- [x] .PrintF (SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, const char* ptr_format, ...);
- [x] .PutChar (SSD13xx_handle_t SSD13xx_Handle, uint8_t pos_x, uint8_t pos_y, uint8_t ch);
- [x] .Clear (SSD13xx_handle_t SSD13xx_Handle);
- [x] .SetCursor (SSD13xx_handle_t SSD13xx_Handle, uint8_t Pos_X, uint8_t Pos_Y);

## Usage

### First: Provide communication related methods

You must first implement three functions: "communication start", "send a single byte", and "end communication", as well as the optional "accept a single byte" function

```C
/*
 * Based on the CH573F platform
 */
#define IIC_SDA GPIO_Pin_12
#define IIC_SCL GPIO_Pin_13
#define IIC_VCC GPIO_Pin_14
#define IIC_GND GPIO_Pin_15

void IIC_Init(){
    GPIOA_SetBits(IIC_SDA);
    GPIOA_SetBits(IIC_SCL);
    GPIOA_SetBits(IIC_VCC);
    GPIOA_SetBits(IIC_GND);

    GPIOA_ModeCfg(IIC_GND, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(IIC_VCC, GPIO_ModeIN_Floating);
    GPIOA_ModeCfg(IIC_SDA, GPIO_ModeOut_PP_5mA);
    GPIOA_ModeCfg(IIC_SCL, GPIO_ModeOut_PP_5mA);


    GPIOA_SetBits(IIC_SCL);
    GPIOA_SetBits(IIC_SDA);

    DelayMs(1000);
}

void IIC_Start(){
    GPIOA_SetBits(IIC_SDA);
    GPIOA_SetBits(IIC_SCL);
    GPIOA_ResetBits(IIC_SDA);
    GPIOA_ResetBits(IIC_SCL);
}

void IIC_Stop(){
    GPIOA_ResetBits(IIC_SDA);
    GPIOA_SetBits(IIC_SCL);
    GPIOA_SetBits(IIC_SDA);
}

void IIC_SendByte(uint8_t byte){
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if((byte & (0x80 >> i)) > 0){
            GPIOA_SetBits(IIC_SDA);
        }
        else{
            GPIOA_ResetBits(IIC_SDA);
        }

        GPIOA_SetBits(IIC_SCL);
        GPIOA_ResetBits(IIC_SCL);
    }
    GPIOA_SetBits(IIC_SCL);
    GPIOA_ResetBits(IIC_SCL);
}
```

### Second: Initialize device

```C
IIC_Init(); // Remember to initialize your communication method

SSD13xx_InitTypeDef SSD13xx_InitStructure;
SSD13xx_handle_t OLED_Handle;

SSD13xx_InitStructure.StartComunication = &IIC_Start;
SSD13xx_InitStructure.StopComunication = &IIC_Stop;
SSD13xx_InitStructure.SendByte = &IIC_SendByte;

SSD13xx.Init(&SSD13xx_InitStructure, &OLED_Handle);
```

### Third: Easy to use

Using an IDE with syntax prompts, type SSD13xx and use `.` to introduce the available functions

```C
SSD13xx.PrintF(OLED_Handle, 0, 0, "xChenNing");

uint8_t i = 233;
SSD13xx.PrintF(OLED_Handle, 0, 1, "i = %d", i);
```