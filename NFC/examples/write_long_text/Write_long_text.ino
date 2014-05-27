/**
 * Author:     Alberto Gil Tesa
 * WebSite:    http://giltesa.com
 * License:    CC BY-NC-SA 3.0
 *
 * Description:
 *   ARDUINO SHIELD NFC PN532 ISP
 *
 *   Ejemplo de uso de la shield NFC con chip PN532:
 *   En este otro ejemplo mediante los metodos "readAllMemory" y "writeAllMemory" añadidos por mi
 *   se puede leer o escribir en la memoria de forma consecutiva sin tener que preocuparnos por los
 *   bloques de autenticacion.
 *   Podemos almacenar los datos que queramos siempre y cuando sean arrays de bytes.
 *   Eso quiere decir que se pueden almacenar cadenas o estructuras de datos Union.
 *
 *   En concreto este ejemplo guarda un parrafo de texto de 447 caracteres/bytes.
 */


#include <PN532.h>

// Shield NFC:
#define SCK  13
#define MOSI 11
#define SS   10
#define MISO 12
PN532 nfc(SCK, MISO, MOSI, SS);


byte texto[] = "El hardware consiste en una placa con un microcontrolador Atmel AVR y puertos de entrada/salida. Los microcontroladores mas usados son el Atmega168, Atmega328, Atmega1280, ATmega8 por su sencillez y bajo coste que permiten el desarrollo de multiples disenios. Por otro lado el software consiste en un entorno de desarrollo que implementa el lenguaje de programacion Processing/Wiring y el cargador de arranque (boot loader) que corre en la placa.";
byte textoTag[sizeof(texto)];
boolean escrito = false;


void setup()
{
   Serial.begin(9600);
   nfc.begin();
   nfc.SAMConfig();
}


void loop()
{
   uint32_t id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

   if( id > 0 )
   {
      // Escribe por primera vez el texto en la memoria EEPROM de la Tag:
      if( !escrito )
      {
         if( !nfc.writeAllMemory( id , texto, sizeof(texto) ) )
            Serial.println("Error de escritura");
         else
         {
            escrito = true;
            Serial.println("Escritura realizada correctamente");
         }
      }


      // Le el contenido de la memoria EEPROM:
      if( escrito )
      {
         if( ! nfc.readAllMemory(id , textoTag , sizeof(textoTag)) )
            Serial.println("Error de lectura");
         else
         {
            for( int i=0 ; i<sizeof(textoTag) ; i++ )
               Serial.print( (char)textoTag[i] );

            Serial.println("\n");
            delay(2000);
         }
      }

   }

}
