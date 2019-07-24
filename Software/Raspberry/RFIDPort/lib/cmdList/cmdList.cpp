/**
 * @file cmdList.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-07-23
 * 
 * @copyright Copyright (c) 2019
 * 
 * Lista de comandos usada por la funcion
 * byte searchCMD(String msg)
 */
#include <util.h>
#include "cmdList.h"


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