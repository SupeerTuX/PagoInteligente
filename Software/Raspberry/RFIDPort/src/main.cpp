#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#include <util.h>
#include <cmdList.h>

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
  // LLave A y B de acceso a la tarjera RFID
  key.keyByte[0] = 0x84;
  key.keyByte[1] = 0x8F;
  key.keyByte[2] = 0x69;
  key.keyByte[3] = 0xBC;
  key.keyByte[4] = 0xF2;
  key.keyByte[5] = 0xEB;
}

void loop()
{
  delay(200);
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
  
  //Trajeta encontrada, leemos la tarjeta
  //comprobamos saldo e ID
  //Enviamos mensaje de tarjeta nueva encontrada
  switch (sendNewCard())
  {
  case true:
    // Envio OK
    cardProcces();
    break;

  case false:
    //Error RW
    Serial.println("AT+RW_ERROR");
    return;
    break;

  case ERROR_TIMEOUT:
    // Tiempo de respuesta consumido
    Serial.println("AT+CMD_TIMEOUT");
    return;
    break;

  default:
    // Error desoconocido
    Serial.println("AT+ERROR?");
    return;
    break;
  }



  //Cerramos operaciones RFID
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