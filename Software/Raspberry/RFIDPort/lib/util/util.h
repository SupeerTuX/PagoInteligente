/**
 * @file util.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-07-21
 * 
 * @copyright Copyright (c) 2019
 * 
 * Prototipos de funciones utilitarias
 * 
 */

#include <Arduino.h>
#include "cmdList.h"

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

//Definimos un error TimeOut
#define ERROR_TIMEOUT 2
#define ERROR_RW 3

/**
 * @brief Limpia el char array
 * 
 * @param char Array
 */
void clearCharArray(char *buffer);

/**
 * @brief Espera la llegada de mensajes
 * 
 * @return Devuelve el string del mensaje
 */
String readMsg(void);

/**
 * @brief Busca el comando enviado entre la lista de
 *        comandos validos y ejecuta el comando
 * 
 * @param cmd Comando recivido
 * @return byte true = Comando ejecutado | false = comando invalido
 */
byte searchCMD(String cmd);

/**
 * @brief Funcion para proceso de de cobro con tarjeta RFID
 *          Maneja todo el proceso de cobro
 * 
 */
void cardProcces();

/**
 * @brief Envia un el comando pasado como argumento
 * 
 * @param cmd 
 * @return String 
 */
String sendCommand(char *cmd);

/**
 * @brief  Envia el comando AT Avisando "Tarjeta nueva encontrada"
 *         dentro del mensaje envia el saldo y el id "AT+NewCard[id][saldo]"
 * 
 * @return false = Error RW | true = Envio OK | ERROR_TIMEOUT = Sin respuesta
 */
byte sendNewCard(void);

/**
 * @brief Funcion de ayuda para formater ala salida en hexadecimal
 * 
 * @param buffer 
 * @param bufferSize 
 */
void dump_byte_array(byte *buffer, byte bufferSize);
