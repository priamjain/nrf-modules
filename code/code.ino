

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(9,10);                    // nRF24L01(+) radio attached using Getting Started board 
#define pir 8
RF24Network network(radio);          // Network uses that radio

const uint16_t first_node = 01;        // Address of our node in Octal format
const uint16_t second_node = 02;       // Address of the other node in Octal format
const uint16_t zero_node = 00;
const long interval = 10000;
unsigned long prevmillis = 0;
struct payload_t { // Structure of our payload
int motion=0;
int count=0;
};
payload_t pdnode ={};
payload_t pdrx ={};
void setup(void)
{
  Serial.begin(115200);
  pinMode(pir,INPUT);
  Serial.println("Zero Node");
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ zero_node);
}

void loop() {
  network.update();// Check the network regularly

  unsigned long curmillis = millis();

  if(curmillis-prevmillis == interval)
  {
   // senddata(pdnode);
    prevmillis=curmillis; 
  }
while ( network.available())
{     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out
    network.read(header,&pdrx,sizeof(pdrx));
    //Serial.print(pdrx.count);
    Serial.println(pdrx.motion);
    pdrx.count=pdrx.count+1;
    //senddata(pdrx);
    delay(500);
}
//my name is priam
/*
pdnode.motion = digitalRead(pir);
if(digitalRead(pir))
{
   pdnode.count=0;
  //senddata(pdnode);
}
*/
}
void senddata(struct payload_t x)
{
      Serial.print("Sending...");
    RF24NetworkHeader header(/*to node*/ first_node);
    bool ok = network.write(header,&x,sizeof(x));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
}
