#include <Arduino.h>
#include <SPI.h>


#include "def.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Inicio de programa");
}

void loop()
{
  //Loop Principal
  //leerMensaje();  Se pueden leer mensajes sin que una tarjeta este activa
  cardProcces();

  delay(250);
}

/**
 * @brief Envia comando y >espera la respuesta
 * 
 * @param cmd comando a enviar, este array es usado para devolver
 * la respuesta
 * @return byte 
 */
String sendCommand(char *cmd)
{

  unsigned long timeout = millis();
  //Enviamos el comando
  Serial.println(cmd);

  //Esperamos por la respuesta
  while (1)
  {
    delay(10);

    if (Serial.available())
    {
      String data;
      data = Serial.readStringUntil('\n');
      return data;
    }

    //Timeout de 1 segundo
    if (millis() - timeout > 5000)
    {
      String data = "Timeout";
      return data;
    }
  }
}

/**
 * @brief llena con '\0' todo el array
 * 
 */
void clearCharArray(char *buffer)
{
  for (byte i = 0; i < strlen(buffer); i++)
  {
    buffer[i] = '\0';
  }
}

void cardProcces()
{
  while (1)
  {
    String msg = readMsg();
    if (msg.length() > 0)
    {
      Serial.print("CMD :[");
      Serial.print(msg);
      Serial.println("]");

      //Buscando el comando recibido
      Serial.println("Buscando Comando");
      searchCMD(msg);

    }
    delay(250);
  }
}

//Verifica si hay mensajes nuevos
String readMsg()
{
  String data = "";
  //Si hay mensajes entrantes
  if (Serial.available())
  {
    //Leemos el mensaje hasta encontran \n
    data = Serial.readStringUntil('\n');
    Serial.flush();
  }
  return data;
}

//Buscamos el comando, que se ha recibido en 
byte searchCMD(String msg)
{
  if(msg.equals("AT"))
  {
    Serial.println("Send OK");
  }
  else if (msg.equals("AT+ReadID"))
  {
    Serial.println("Send MVAXXX10");
  }
  else if (msg.equals("AT+ReadSaldo"))
  {
    Serial.println("Send Saldo:535");    
  }
  

}