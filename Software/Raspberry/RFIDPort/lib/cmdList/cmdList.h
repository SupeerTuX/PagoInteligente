/**
 * @file cmdList.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-07-23
 * 
 * @copyright Copyright (c) 2019
 * 
 * Lista de comandos 
 */

#include <Arduino.h>



/**
 * @brief Lee el id de la tarjeta 
 * 
 * @param id 
 * @return byte 
 */
byte leerID(char *id);

/**
 * @brief Lee el saldo de la tarjeta
 * 
 * @param saldo 
 * @return byte 
 */
byte leerSaldo(char *saldo);

/**
 * @brief Escribe un char array de 16 bytes en un sector de la tarjeta
 * 
 * @param dataBlock 
 * @param block 
 * @param trailerBlock 
 * @return byte 
 */
byte writeBlock(char *dataBlock, byte block, byte trailerBlock);

/**
 * @brief 
 * 
 * @param dataBlock 
 * @param block 
 * @param trailerBlock 
 * @return byte 
 */
byte readBlock(char *dataBlock, byte block, byte trailerBlock);
