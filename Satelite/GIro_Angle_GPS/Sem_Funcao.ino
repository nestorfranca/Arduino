//INCLUSÃO DE BIBLIOTECAS
#include "Wire.h"
#include <MPU6050_light.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

MPU6050 mpu(Wire);

/*DEFINIÇÃO DE PINOS DO ARDUINO. LEMBRE-SE: O PINO ONDE VOCÊ CONECTOU O TX SERÁ O RX AQUI NO CÓDIGO E VICE-VERSA. ISSO É POR CAUSA DA
  COMUNICAÇÃO SERIAL. PARA MAIS DETALHES, VOCÊ PODE CONSULTAR NOSSO BLOG*/
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

//CRIANDO UM O BJETO PARA COMUNICAR COM A BIBLIOTECA
TinyGPSPlus gps;

// CRIANDO UMA PORTA SERIAL gpsSerial PARA CONVERSAR COM MÓDULO
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // INICIA A SERIAL
  Serial.begin(9600);
  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");

  // INICIA A PORTA SERIAL DO SOFTWARE NO BAUD PADRÃO DO GPS, COMO DETERMINAMOS ACIMA:9600
  gpsSerial.begin(GPSBaud);

  Serial.print("Data da Execução: ");
  if (gps.encode(gpsSerial.read())) // MOSTRARÁ A DATA DA EXECUÇÃO
      displayInfo_DATE();

}

void loop()
{
  // TODA VEZ QUE FOR LIDA UMA NOVA SENTENÇA NMEA, CHAMAREMOS A FUNÇÃO displayInfo() PARA MOSTRAR OS DADOS NA TELA
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  //SE EM 5 SEGUNDOS NÃO FOR DETECTADA NENHUMA NOVA LEITURA PELO MÓDULO,SERÁ MOSTRADO ESTA MENSGEM DE ERRO.
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("Sinal GPS não detectado");
    while (true);
  }
  mpu.update();
    Serial.print(F("TEMPERATURE: "));Serial.println(mpu.getTemp());
    Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
    Serial.print("\tY: ");Serial.print(mpu.getAccY());
    Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
  
    Serial.print(F("GYRO\tX: "));Serial.print(mpu.getGyroX());
    Serial.print("\tY: ");Serial.print(mpu.getGyroY());
    Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());

    Serial.print("ANGLE X : ");Serial.print(mpu.getAngleX());
    Serial.print("\tZ : ");Serial.println(mpu.getAngleZ());
    Serial.print("\tY : ");Serial.print(mpu.getAngleY());

    Serial.println(F("=====================================================\n"));
  delay(1000);
}

void displayInfo()//FUNÇÃO RESPONSAVEL PELA LEITURA DOS DADOS
{
  if (gps.location.isValid())//SE A LOCALIZAÇÃO DO SINAL ENCONTRADO É VÁLIDA, ENTÃO
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);//IMPRIME NA SERIAL O VALOR DA LATIDUE LIDA
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);//IMPRIME NA SERIAL O VALOR DA LONGITUDE LIDA

  }
  else
  {
    Serial.println("Não detectamos a localização");//SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
  }

  Serial.print("Time: "); //LEITURA DA HORA PARA SER IMPRESSA NA SERIAL
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour() - 3); //AJUSTA O FUSO HORARIO PARA NOSSA REGIAO (FUSO DE SP 03:00, POR ISSO O -3 NO CÓDIGO) E IMPRIME NA SERIAL
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());//IMPRIME A INFORMAÇÃO DOS MINUTOS NA SERIAL
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());//IMPRIME A INFORMAÇÃO DOS SEGUNDOS NA SERIA
  }
  else
  {
    Serial.println("Não detectamos o horário atual");//SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
  }

  Serial.println();
  Serial.println();
  delay(1000);
}

void displayInfo_DATE()
{
  if (gps.date.isValid())//IMPRIME A DATA NA SERIAL
    {
      Serial.print(gps.date.day());//LEITURA DO DIA
      Serial.print("/");
      Serial.print(gps.date.month());//LEITURA DO MêS
      Serial.print("/");
      Serial.println(gps.date.year());//LEITURA DO ANO
    }
    else
    {
      Serial.println("Erro");//SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
    }
}
