/**
 * Author:     Alberto Gil Tesa
 * WebSite:    http://giltesa.com
 * License:    CC BY-NC-SA 3.0
 *
 * Description:
 *   ARDUINO SHIELD NFC PN532 ISP
 *
 *   Ejemplo de uso de la shield NFC con chip PN532.
 *   Lee en la memoria EEPROM de la llave NFC
 *   la configuracion de colores de un LED RGB
 *   para encenderlo segun corresponda.
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

// Pines LED RGB:
#define LED_AZUL    2
#define LED_VERDE   3
#define LED_ROJO    4


PN532 nfc(SCK, MISO, MOSI, SS);


void setup()
{
   Serial.begin(9600);
   nfc.begin();
   nfc.SAMConfig();

   pinMode( LED_AZUL  , OUTPUT );
   pinMode( LED_VERDE , OUTPUT );
   pinMode( LED_ROJO  , OUTPUT );
}


void loop()
{
   uint32_t id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

   if( id > 0 )
   {
      if( !nfc.readAllMemory(id , rgb.b , sizeof(rgb.b)) )
         Serial.println("Error de lectura");
      else
      {
         analogWrite( LED_ROJO  , rgb.color.rojo  );
         analogWrite( LED_VERDE , rgb.color.verde );
         analogWrite( LED_AZUL  , rgb.color.azul  );
         delay(1000);
      }
   }
}
