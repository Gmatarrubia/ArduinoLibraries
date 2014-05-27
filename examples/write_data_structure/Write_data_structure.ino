/**
 * Author:     Alberto Gil Tesa
 * WebSite:    http://giltesa.com
 * License:    CC BY-NC-SA 3.0
 *
 * Description:
 *   ARDUINO SHIELD NFC PN532 ISP
 *
 *   Ejemplo de uso de la shield NFC con chip PN532.
 *   Escribe en la memoria EEPROM de la llave NFC
 *   la configuracion de colores de un LED RGB
 */


struct DATOS{
   byte rojo;
   byte verde;
   byte azul;
};

union MEMORIA{
   DATOS color;
   byte  b[sizeof(DATOS)];
}
rgb;


#include <PN532.h>

// Shield NFC:
#define SCK        13
#define MOSI       11
#define SS         10
#define MISO       12
boolean escrito = false;


PN532 nfc(SCK, MISO, MOSI, SS);


void setup()
{
   Serial.begin(9600);
   nfc.begin();
   nfc.SAMConfig();
}


void loop()
{
   uint32_t id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

   if( id > 0  &&  !escrito )
   {
      // Segun el color del Tag (del plastico) deberemos de descomentar la linea que se corresponda
      // para que tenga coherencia con la configuracion que almacena:

      rgb.color.rojo  = 0;   rgb.color.verde = 255; rgb.color.azul  = 0;   // Lima
      // rgb.color.rojo  = 0;   rgb.color.verde = 0;   rgb.color.azul  = 255; // Azul
      // rgb.color.rojo  = 204; rgb.color.verde = 0;   rgb.color.azul  = 0;   // Rojo
      // rgb.color.rojo  = 160; rgb.color.verde = 255; rgb.color.azul  = 0;   // Amarillo
      // rgb.color.rojo  = 255; rgb.color.verde = 255; rgb.color.azul  = 255; // Blanco
      // rgb.color.rojo  = 0;   rgb.color.verde = 0;   rgb.color.azul  = 0;   // GRIS == OFF

      if( !nfc.writeAllMemory( id , rgb.b, sizeof(rgb.b) ) )
         Serial.println("Error de escritura");
      else
      {
         escrito = true;
         Serial.println("Escritura realizada correctamente");
      }

   }
}
