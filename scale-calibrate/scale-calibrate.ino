/*******************************************************************************
* Name:           Скетч калибровки весов на HX711
* Target Devices: Arduino / STM32
* Tool versions:  Arduino 1.6.5
*
* При запуске весы должны быть пустые. После появления надписи "You have 10 seconds..."
* необходимо на весы поместить груз известной массы. Желательно от килограмма и больше
* (в допустимых пределах тензодатчиков). После замера будет выведено значение, которое
* необходимо разделить на массу груза в граммах. Полученное значение и будет калибро-
* вочным коэффициентом, которое необходимо ввести в основной исходный код весов.
* 
*******************************************************************************/

#include "HX711.h"

// Пины подключения HX711
const int LOADCELL_DOUT_PIN = PA6;
const int LOADCELL_SCK_PIN = PA7;

HX711 scale;

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println("Initializing the scale...");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_gain(); // установка усиления на значение по-умолчанию (128)
  scale.set_scale(); // установка шкалы по-умолчанию (без калибровки)
  scale.tare();	// сброс шкалы на 0
  Serial.println("You have 10 seconds...");
  delay(10000);
  Serial.println("Measurement. Wait, please...");
  float m = scale.get_units(10);
  Serial.print("Result: ");
  Serial.println(m, 1);
}

void loop() {
  
}
