/* Get tilt angles on X and Y, and rotation angle on Z
 * Angles are given in degrees
 * 
 * License: MIT
 */

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire); // declarando e inicianmdo o objeto MPU6050
/*Um objeto chamado 'mpu' do tipo MPU6050 é criado, e ele é inicializado 
 usando a comunicação I2C através do objeto 'Wire'. */
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();  // Inicializa a biblioteca Wire para a comunicação I2C.
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
}
/*Essa parte calcula uma compensação para o giroscopio e acelerometro
Pede para nao mover o MPu6050 durante a execução*/

void loop() {
  mpu.update();     // Atualiza os dados do MPU6050.
  
  if((millis()-timer)>1000){ // print data every 10ms
  Serial.print("X : ");
  Serial.print(mpu.getAngleX());  // Obtém e imprime o ângulo de inclinação no eixo X.
  Serial.print("\tY : ");
  Serial.print(mpu.getAngleY());  // Obtém e imprime o ângulo de inclinação no eixo Y.
  Serial.print("\tZ : ");
  Serial.println(mpu.getAngleZ());  // Obtém e imprime o ângulo de rotação no eixo Z.
  timer = millis();   // Reinicia o temporizador para a próxima leitura.
  }
}
