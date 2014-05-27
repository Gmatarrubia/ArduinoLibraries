/**
 * Author:     Alberto Gil Tesa
 * WebSite:    http://giltesa.com
 * License:    CC BY-NC-SA 3.0
 *
 * Description:
 *   ARDUINO SHIELD NFC PN532 ISP
 *
 *   Ejemplo de uso de la shield NFC con chip PN532.
 *   Enciende o apaga un LED conectado al pin "LED" cada vez que se lee la tarjeta
 *   indicada en la constante "ID_TARJETA"
 *
 *   Nota: No puede usarse el led del Arduino ya que ese pin se usa para la Shield.
 */


#include <PN532.h>

#define SCK         13
#define MOSI        11
#define SS          10
#define MISO        12

#define LED         8
#define ID_TARJETA  2864817847

PN532 nfc(SCK, MISO, MOSI, SS);


void setup()
{
   Serial.begin(9600);
   nfc.begin();
   nfc.SAMConfig();

   pinMode(LED, OUTPUT);

   Serial.println("Placa preparada para leer!");
}


void loop()
{
   uint32_t id = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A);

   // Si el ID leido coincide con el de la constante "ID_TARJETA"...
   if( id == ID_TARJETA )
   {
      digitalWrite( LED , !digitalRead(LED) ); // Si el led esta encendido lo apaga, si esta apagado lo enciende.
      delay(1000);
   }
}
