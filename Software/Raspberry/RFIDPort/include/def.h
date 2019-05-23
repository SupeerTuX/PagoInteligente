#include <Arduino.h>

//Definiciones de MFRC522
#define SECTOR_ID 0 //Sector donde se almacena el ID
#define BLOCK_ID 1  //Bloque del sector donde se almacena el ID
#define TBLOCK_ID 3 //Bloque del sector NOMBRE usado para autenticacion

#define SECTOR_SALDO 2  //Sector donde se almacena el Saldo
#define BLOCK_SALDO 8   //Bloque del sector donde se almacena el Saldo
#define TBLOCK_SALDO 11 //Bloque del sector SALDO usado para autenticacion

#define BLOCK_MASTER 60  //Block donde esta la clave maestra
#define TBLOCK_MASTER 63 //Bloque del sector MASTER usado para autenticacion

//  \n New line ascii 10
//  \r Retorno  ascii 13
/**
 * @brief verifffffica si hay mensajes nuevos
 * 
 */
void readMsg(void);

/**
 * @brief Funcion de ayuda para formater ala salida en hexadecimal
 * 
 * @param buffer 
 * @param bufferSize 
 */
void dump_byte_array(byte *buffer, byte bufferSize);

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
 * @brief 
 * 
 */
byte sendNewCard(void);

/**
 * @brief 
 * 
 * @param cmd 
 * @return String 
 */
String sendCommand(char *cmd);

/**
 * @brief Espera recibir un comando en un determinado timeout
 * 
 */
void waitForCMD(void);

/**
 * @brief llena con '\0' todo el array
 * 
 */
void clearCharArray(char *buffer);