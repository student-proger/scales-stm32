/*******************************************************************************
* Name:           Скетч весов на HX711 и STM32
* Target Devices: Arduino / STM32F103C8T6
* Tool versions:  Arduino 1.6.5
*
*******************************************************************************/

#include "HX711.h"
#include <TM74HC595Display.h>

//************** СТРОКИ КОНФИГУРАЦИИ ********************
// Пины подключения HX711
#define LOADCELL_DOUT_PIN PA6
#define LOADCELL_SCK_PIN PA7
// Пины подключения дисплея
#define SCLK_PIN PA3
#define RCLK_PIN PA4
#define DIO_PIN PA5
// Пины подключения зуммера
#define BUZZER_PLUS_PIN PA10
#define BUZZER_MINUS_PIN PA12
//*******************************************************

HX711 scale;
TM74HC595Display disp(SCLK_PIN, RCLK_PIN, DIO_PIN);
unsigned long dispIsrTimer = 0;
unsigned long updateWeightTime = 0;

/*******************************************************************************
* Function Name  : setup
* Description    : Инициализация устройств
*******************************************************************************/

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(24.029740721); // значение калибровочного коэффициента (см. справку)
  scale.tare(); // сброс шкалы на 0
}

/*******************************************************************************
* Function Name  : loop
* Description    : Главный цикл
*******************************************************************************/

void loop()
{
  // Каждые 200 миллисекунд ...
  if (millis() - updateWeightTime > 200)
  {
    // делаем замер, получаем массу в граммах
    float w = scale.get_units();
    Serial.println(w, 1);
    disp.clear();
    if (w > 0)
    {
      // делим на тысячу (получаем кг) и выводим на дисплей
      disp.float_dot(w / 1000, 1);
    }
    else
    {
      disp.float_dot(0.0, 1);
    }
    updateWeightTime = millis();
  }

  disp_isr();
}

/*******************************************************************************
* Function Name  : disp_isr
* Description    : Динамическая индикация дисплея. Каждые 100 мкс дёргаем функцию
*                  библиотеки дисплея для корректного вывода.
*******************************************************************************/

void disp_isr()
{
  if (micros() - dispIsrTimer > 100)
  {
    disp.timerIsr();
    dispIsrTimer = micros();
  }
}
