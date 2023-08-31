//Incluindo Bibliotecas
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; //I2C
#define address 0x1E //0011110b, I2C 7bit address of HMC5883

void setup() {
  //Iniciando a comunicação serial
  Serial.begin(9600);
  // Imprimindo Mensagem de teste no Monitor Serial
  Serial.println(F("BMP280 teste"));

  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  
  if (!bmp.begin(0x76)) { /*Definindo o endereço I2C como 0x76. Mudar, se necessário, para (0x77)*/
    
    //Imprime mensagem de erro no caso de endereço invalido ou não localizado. Modifique o valor 
    Serial.println(F(" Não foi possível encontrar um sensor BMP280 válido, verifique a fiação ou "
                      "tente outro endereço!"));
    while (1) delay(10);
  }
}

void loop() {
    int x,y,z; //triple axis data

    //Imprimindo os valores de Temperatura
    Serial.print(F("Temperatura = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    //Imprimindo os valores de Pressão
    Serial.print(F("Pressão = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    //Imprimindo os valores de Altitude Aproximada
    Serial.print(F("Altitude Aprox = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Ajustar a pressão de nível do mar de acordo com o local!*/
    Serial.print(" m");

    //Tempo de espera de 1 segundo
    Serial.println();
    delay(500);
    
    //Tell the HMC5883L where to begin reading data
    Wire.beginTransmission(address);
    Wire.write(0x03); //select register 3, X MSB register
    Wire.endTransmission();
    
   
   //Read data from each axis, 2 registers per axis
    Wire.requestFrom(address, 6);
    if(6<=Wire.available()){
      x = Wire.read()<<8; //X msb
      x |= Wire.read(); //X lsb
      z = Wire.read()<<8; //Z msb
      z |= Wire.read(); //Z lsb
      y = Wire.read()<<8; //Y msb
      y |= Wire.read(); //Y lsb
    }
    
    //Print out values of each axis
    Serial.print("x: ");
    Serial.print(x);
    Serial.print("  y: ");
    Serial.print(y);
    Serial.print("  z: ");
    Serial.println(z);

    Serial.println();      
    delay(1000);
}
