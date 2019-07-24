/**
 * @file util.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-07-21
 * 
 * @copyright Copyright (c) 2019
 * 
 * Implementacion de funciones utilitarias
 */

#include "util.h"

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
 * @brief Espera la llegada de mensajes
 * 
 * @return Devuelve el string del mensaje
 */
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
  char buffer[18];

  /** ******************
   *    HANDSHAKE 
   *********************/
  if (msg.equals("AT"))
  {
    Serial.println("OK");
  }
  /** ******************
   *   LEER ID
   *********************/
  else if (msg.equals("AT+ReadID"))
  {
    if (leerID(buffer))
    {
      //Envio de respuesta ID
      Serial.print("ID[");
      Serial.print(buffer);
      Serial.println("]");
    }
  }
  /** ******************
   *    LEER SALDO
   *********************/
  else if (msg.equals("AT+ReadSaldo"))
  {
    if (leerSaldo(buffer))
    {
      //Envio de respuesta Saldo
      Serial.print("Saldo[");
      Serial.print(buffer);
      Serial.println("]");
    }
  }
  /** ******************
   * ESCRIBIR NUEVO SALDO
   *********************/
  else if (msg.indexOf("AT+NewSaldo") >= 0)
  {
    /* code */
  }

  //Default
  else
  {
    Serial.println("Error");
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

/**
 * @brief Funcion para proceso de de cobro con tarjeta RFID
 *          Maneja todo el proceso de cobro
 * 
 */
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

byte sendNewCard(void)
{
  char id[18];
  char saldo[18];
  char cmd[150];

  String data;

  id[0] = '\0';
  saldo[0] = '\0';
  cmd[0] = '\0';

  if (!leerID(id) || !leerSaldo(saldo))
  {
    //Error de lectura, enviamos el comando de error
    strcpy(cmd, "AT+ErrorLectura");
    data = sendCommand(cmd);
    //Salimos de la funcion
    return false;
  }

  //Formando el comando
  strcat(cmd, "AT+NewCard[");
  strcat(cmd, id);
  strcat(cmd, "][");
  strcat(cmd, saldo);
  strcat(cmd, "]");

  //Enviando comando
  data = sendCommand(cmd);

  //Buscabndo respuesta
  if (data.equals("OK"))
  {
    //Respuesta OK Encontrada
    return true;
  }
  else if (data.equals("Timeout"))
  {
    //Error timepo de espera consumido
    return ERROR_TIMEOUT;
  }
  else
  {
    //Error
    return false;
  }
}