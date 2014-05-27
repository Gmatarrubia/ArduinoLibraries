/**
 * Author:     Alberto Gil Tesa
 * WebSite:    http://giltesa.com
 * License:    CC BY-NC-SA 3.0
 *
 * Description:
 *   ARDUINO SHIELD NFC PN532 ISP
 *
 *   Ejemplo de uso de la shield NFC con chip PN532:
 *   Contabiliza el numero de veces que se lee la llave.
 *   Con cada lectura se suma 1 al contador de la EEPROM de
 *   la llave y se imprime el resultado por el monitor serial.
 */


#include <PN532.h>

// Shield NFC:
#define SCK        13
#define MOSI       11
#define SS         10
#define MISO       12


PN532 nfc(SCK, MISO, MOSI, SS);


void setup()
{
   Serial.begin(9600);
   nfc.begin();
   nfc.SAMConfig();
}


void loop()
{
   // Contador es un array de bytes de tamaño 1, es necesario crear la variable de este modo ya que el metodo que escribe la informacion espera recibir un puntero hacia el dato.
   byte contador[1];

   // Es la clave de acceso necesaria para acceder a cada sector de la memoria:
   byte keys[]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


   uint32_t id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

   // Si se obtuvo un ID...
   if( id > 0 )
   {
      // Antes de hacer uso de la memoria es necesario autenticarse con la clave de acceso, para ello se le pasa como parametro:
      // - El tipo de tarjeta, pudiendo elegir entre 1 o 2.
      // - El ID de dispositivo NFC al que queremos acceder a su memoria.
      // - La direccion de memoria del bloque en el que queremos autenticarnos, no se le indica el bloque que contiene la clave, si no al que se quiere acceder. El solo se apaña para encontrar la clave correspondiente al bloque.
      // - El tipo de clave, se puede elegir entre KEY_A o KEY_B.
      // - La clave de arriba.
      if( nfc.authenticateBlock(1, id ,0x08, KEY_A, keys) )
      {
         // Una vez autenticados se lee en el bloque 0x08, 1 byte de memoria.
         if( nfc.readMemoryBlock(1, 0x08, contador) )
         {
            // Cada 255 lecturas de la tarjeta se resetea el contador:
            if( contador[0] >= 255)
               contador[0] = 0;

            // Se suma 1 al contador cada vez que se hace una lectura con la Shield:
            contador[0] += 1;

            Serial.print("Contador= "); Serial.println(contador[0]);

            // Se escribe el nuevo valor del contador en el bloque 0x08:
            nfc.writeMemoryBlock(1, 0x08, contador);
         }
      }
      else
      {
         Serial.println("Clave incorrecta o error de lectura");
      }

   }

}
