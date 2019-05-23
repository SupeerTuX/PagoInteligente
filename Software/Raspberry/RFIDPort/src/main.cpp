#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#include "def.h"

#define RST_PIN 9
#define SS_PIN 10
//intancia de la clase MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN); // Creamos una instancia MFRC522
MFRC522::MIFARE_Key key;          //Llave usada como KEY_A y KEY_B

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("Inicio de programa");

  // Init SPI
  SPI.begin();

  // Init MFCR
  mfrc522.PCD_Init();

  /*
  key.keyByte[0] = 0xAA;
  key.keyByte[1] = 0xBB;
  key.keyByte[2] = 0xCC;
  key.keyByte[3] = 0xDD;
  key.keyByte[4] = 0xEE;
  key.keyByte[5] = 0xFF;
  */

  key.keyByte[0] = 0x84;
  key.keyByte[1] = 0x8F;
  key.keyByte[2] = 0x69;
  key.keyByte[3] = 0xBC;
  key.keyByte[4] = 0xF2;
  key.keyByte[5] = 0xEB;
}

void loop()
{

  //Revisar  por mensajes
  readMsg();
  delay(100);
  //Buscando nuevas tarjetas
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Seleccionamos la tarjeta
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  //Enviamos comando de nueva tarjeta encontrada con

  if (!sendNewCard())
  {
    //Avisar del error timeout
    return;
  }
  waitForCMD();

  //Cerramos operaciones de RFID
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

//Funcion para leer un bloque de la tarjeta RFID
byte readBlock(char *dataBlock, byte block, byte trailerBlock)
{

  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);
  // Authenticate using key A
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    //Serial.print(F("Auth() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  // Read data from the block
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK)
  {
    //Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  for (byte i = 0; i < 16; i++)
  {
    dataBlock[i] = buffer[i];
  }
  return true;
}

//Escribe un bloque en la tarjeta RFID
byte writeBlock(char *dataBlock, byte block, byte trailerBlock)
{

  MFRC522::StatusCode status;
  // Authenticate using key A
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    //Serial.print(F("Auth() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  // Escribiendo la nueva clave en los ultimo block del sector
  //dump_byte_array((byte *)strPrint, 16); Serial.println();
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(block, (byte *)dataBlock, 16);
  if (status != MFRC522::STATUS_OK)
  {
    //Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else
  {
    return true;
  }
}

//Verifica si hay mensajes nuevos
void readMsg()
{
  String data = "";
  if (Serial.available())
  {
    data = Serial.readStringUntil('\n');

    //Buscamos el substring del comando
    if (data.equals("AT"))
    {
      //Retornamos OK y salimos de la funcion
      Serial.println("OK");
      return;
    }
    else if (data.equals("AT+ReadID"))
    {
      //Leemos el id de la tarjeta
      char id[18];
      id[0] = '\0';
      leerID(id);
      //enviamos como respuesta el id
      Serial.print("ID: ");
      Serial.println(id);
      Serial.println("OK");
    }

    else if (data.equals("AT+ReadSaldo"))
    {
      //Leemos el id de la tarjeta
      char saldo[18];
      saldo[0] = '\0';
      leerSaldo(saldo);
      //enviamos como respuesta el id
      Serial.println(saldo);
      Serial.println("OK");
    }

    else
    {
      Serial.println(data.indexOf("AT"));
    }
  }
}

//Funcion para leer saldo de la tarjeta
byte leerSaldo(char *saldo)
{
  if (readBlock(saldo, BLOCK_SALDO, TBLOCK_SALDO))
  {
    Serial.print(F("Data: "));
    Serial.println(saldo);
    return true;
  }
  else
  {
    Serial.println(F("Error al leer SALDO"));
    return false;
  }
}

//Funcion para leer saldo de la tarjeta
byte leerID(char *id)
{
  if (readBlock(id, BLOCK_ID, TBLOCK_ID))
  {
    //Serial.print(F("Data: "));
    //Serial.println(id);
    return true;
  }
  else
  {
    //Serial.println(F("Error al leer ID"));
    return false;
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

  data = sendCommand(cmd);
  if (data.equals("OK"))
  {
    return true;
  }
  else
  {
    return false;
  }

  //Respuesta recibida
  //Serial.print("Respuesta: ");
  //Serial.println(data);
}

/**
 * @brief Envia comando y espera la respuesta
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
 * @brief Espera recibir un comando en un determinado timeout
 * 
 */
void waitForCMD(void)
{
  while (1)
  {
    String data;
    if (Serial.available())
    {
      data = Serial.readStringUntil('\n');
      if (data.indexOf("AT+NewSaldo") >= 0)
      {
        //Decodificamos el saldo que ha enviado
        byte inicio = 0;
        byte fin = 0;

        inicio = data.indexOf('[');
        fin = data.indexOf(']');

        //Buscamos error
        if (inicio == -1 || fin == -1)
        {
          //Error de formato
          //Enviamos el comando de reenvio
          Serial.println(F("AT+ReSend"));
          return;
        }

        inicio++;
        fin++;
        char saldo[18];
        clearCharArray(saldo);
        String buffer = data.substring(inicio, fin);
        buffer.toCharArray(saldo, buffer.length());
        //Serial.print(F("CMD: "));
        //Serial.println(saldo);

        //Ecribir Saldo actual en la tarjeta
        if (writeBlock(saldo, BLOCK_SALDO, TBLOCK_SALDO))
        {
          //Escritura realizada correctamente
          Serial.println(F("OK"));
        }
        //Error de escritura
        else
        {
          //Enviamos el comando de error
          Serial.println(F("AT+ErrorEscritura"));
        }

        return;
      }
    }
  }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
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