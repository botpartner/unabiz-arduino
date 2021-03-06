//  Send sample SIGFOX messages with UnaBiz UnaShield Arduino Shield.
//  This sketch includes diagnostics functions in the UnaShield.
//  For a simpler sample sketch, see examples/send-light-level.
#include "SIGFOX.h"

//  IMPORTANT: Check these settings with UnaBiz to use the SIGFOX library correctly.
static const String device = "g88pi";  //  Set this to your device name if you're using UnaBiz Emulator.
static const bool useEmulator = false;  //  Set to true if using UnaBiz Emulator.
static const bool echo = true;  //  Set to true if the SIGFOX library should display the executed commands.
static const Country country = COUNTRY_SG;  //  Set this to your country to configure the SIGFOX transmission frequencies.
static UnaShieldV2S transceiver(country, useEmulator, device, echo);  //  Uncomment this for UnaBiz UnaShield V2S Dev Kit
// static UnaShieldV1 transceiver(country, useEmulator, device, echo);  //  Uncomment this for UnaBiz UnaShield V1 Dev Kit

void setup() {  //  Will be called only once.
  //  Initialize console so we can see debug messages (9600 bits per second).
  Serial.begin(9600);  Serial.println(F("Running setup..."));  
  //  Check whether the SIGFOX module is functioning.
  if (!transceiver.begin()) stop(F("Unable to init SIGFOX module, may be missing"));  //  Will never return.

  //  Send a raw 12-byte message payload to SIGFOX.  In the loop() function we will use the Message class, which sends structured messages.
  transceiver.sendMessage("0102030405060708090a0b0c");
  //  Delay 10 seconds before sending next message.
  Serial.println("Waiting 10 seconds...");
  delay(10000);
}

void loop() {  //  Will be called repeatedly.
  //  Send message counter, temperature and voltage as a SIGFOX message, up to 10 times.
  static int counter = 0, successCount = 0, failCount = 0;  //  Count messages sent and failed.
  Serial.print(F("\nRunning loop #")); Serial.println(counter);

  //  Get temperature and voltage of the SIGFOX module.
  float temperature;  float voltage;
  transceiver.getTemperature(temperature);
  transceiver.getVoltage(voltage);

  //  Convert the numeric counter, temperature and voltage into a compact message with binary fields.
  Message msg(transceiver);  //  Will contain the structured sensor data.
  msg.addField("ctr", counter);  //  4 bytes for the counter.
  msg.addField("tmp", temperature);  //  4 bytes for the temperature.
  msg.addField("vlt", voltage);  //  4 bytes for the voltage.
  //  Total 12 bytes out of 12 bytes used.

  //  Send the message.
  if (msg.send()) {
    successCount++;  //  If successful, count the message sent successfully.
  } else {
    failCount++;  //  If failed, count the message that could not be sent.
  }
  counter++;

  //  Send only 10 messages.
  if (counter >= 10) {
    //  If more than 10 times, display the results and hang here forever.
    stop(String(F("Messages sent successfully: ")) + successCount +
                   F(", failed: ") + failCount);  //  Will never return.
  }

  //  Delay 10 seconds before sending next message.
  Serial.println("Waiting 10 seconds...");
  delay(10000);
}

/* Expected output for UnaShield V2S:

Running setup...
 - Disabling emulation mode...
 - Checking emulation mode (expecting 0)...
 - Getting SIGFOX ID...
 - Wisol.sendBuffer: AT$I=10

>> AT$I=10
<< 002BEDB40x0d
 - Wisol.sendBuffer: response: 002BEDB4
 - Wisol.sendBuffer: AT$I=11

>> AT$I=11
<< C53D6575A5DAA5890x0d
 - Wisol.sendBuffer: response: C53D6575A5DAA589
 - Wisol.getID: returned id=002BEDB4, pac=C53D6575A5DAA589
 - SIGFOX ID = 002BEDB4
 - PAC = C53D6575A5DAA589
 - Wisol.setFrequencySG
 - Set frequency result = OK
 - Getting frequency (expecting 3)...
 - Frequency (expecting 3) = 52
 - Wisol.sendMessage: 002BEDB4,0102030405060708090a0b0c
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,0 
 - Wisol.sendBuffer: response: 1,0
 - Wisol.sendBuffer: AT$RC

>> AT$RC
<< OK0x0d
 - Wisol.sendBuffer: response: OK
 - Wisol.sendBuffer: AT$SF=0102030405060708090a0b0c

>> AT$SF=0102030405060708090a0b0c
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #0
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 280 
 - Wisol.sendBuffer: response: 280
 - Wisol.getTemperature: returned 28.00
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=0
 - Message.addField: tmp=280.0
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e0000b051180194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,3 
 - Wisol.sendBuffer: response: 1,3
 - Wisol.sendBuffer: AT$SF=920e0000b051180194592100

>> AT$SF=920e0000b051180194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #1
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 282 
 - Wisol.sendBuffer: response: 282
 - Wisol.getTemperature: returned 28.20
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=1
 - Message.addField: tmp=282.2
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e0a00b0511a0194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,0 
 - Wisol.sendBuffer: response: 1,0
 - Wisol.sendBuffer: AT$RC

>> AT$RC
<< OK0x0d
 - Wisol.sendBuffer: response: OK
 - Wisol.sendBuffer: AT$SF=920e0a00b0511a0194592100

>> AT$SF=920e0a00b0511a0194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #2
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 282 
 - Wisol.sendBuffer: response: 282
 - Wisol.getTemperature: returned 28.20
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=2
 - Message.addField: tmp=282.2
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e1400b0511a0194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,3 
 - Wisol.sendBuffer: response: 1,3
 - Wisol.sendBuffer: AT$SF=920e1400b0511a0194592100

>> AT$SF=920e1400b0511a0194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #3
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 280 
 - Wisol.sendBuffer: response: 280
 - Wisol.getTemperature: returned 28.00
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=3
 - Message.addField: tmp=280.0
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e1e00b051180194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,0 
 - Wisol.sendBuffer: response: 1,0
 - Wisol.sendBuffer: AT$RC

>> AT$RC
<< OK0x0d
 - Wisol.sendBuffer: response: OK
 - Wisol.sendBuffer: AT$SF=920e1e00b051180194592100

>> AT$SF=920e1e00b051180194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #4
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 285 
 - Wisol.sendBuffer: response: 285
 - Wisol.getTemperature: returned 28.50
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=4
 - Message.addField: tmp=285.5
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e2800b0511d0194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,3 
 - Wisol.sendBuffer: response: 1,3
 - Wisol.sendBuffer: AT$SF=920e2800b0511d0194592100

>> AT$SF=920e2800b0511d0194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #5
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 278 
 - Wisol.sendBuffer: response: 278
 - Wisol.getTemperature: returned 27.80
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=5
 - Message.addField: tmp=278.8
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e3200b051160194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,0 
 - Wisol.sendBuffer: response: 1,0
 - Wisol.sendBuffer: AT$RC

>> AT$RC
<< OK0x0d
 - Wisol.sendBuffer: response: OK
 - Wisol.sendBuffer: AT$SF=920e3200b051160194592100

>> AT$SF=920e3200b051160194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #6
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 278 
 - Wisol.sendBuffer: response: 278
 - Wisol.getTemperature: returned 27.80
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=6
 - Message.addField: tmp=278.8
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e3c00b051160194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,3 
 - Wisol.sendBuffer: response: 1,3
 - Wisol.sendBuffer: AT$SF=920e3c00b051160194592100

>> AT$SF=920e3c00b051160194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #7
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 278 
 - Wisol.sendBuffer: response: 278
 - Wisol.getTemperature: returned 27.80
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=7
 - Message.addField: tmp=278.8
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e4600b051160194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,0 
 - Wisol.sendBuffer: response: 1,0
 - Wisol.sendBuffer: AT$RC

>> AT$RC
<< OK0x0d
 - Wisol.sendBuffer: response: OK
 - Wisol.sendBuffer: AT$SF=920e4600b051160194592100

>> AT$SF=920e4600b051160194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #8
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 278 
 - Wisol.sendBuffer: response: 278
 - Wisol.getTemperature: returned 27.80
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=8
 - Message.addField: tmp=278.8
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e5000b051160194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,3 
 - Wisol.sendBuffer: response: 1,3
 - Wisol.sendBuffer: AT$SF=920e5000b051160194592100

>> AT$SF=920e5000b051160194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
Waiting 10 seconds...

Running loop #9
 - Wisol.sendBuffer: AT$T?

>> AT$T?
<< 282 
 - Wisol.sendBuffer: response: 282
 - Wisol.getTemperature: returned 28.20
 - Wisol.sendBuffer: AT$V?

>> AT$V?
<< 33000x0d
 - Wisol.sendBuffer: response: 3300
 - Wisol.getVoltage: returned 3.30
 - Message.addField: ctr=9
 - Message.addField: tmp=282.2
 - Message.addField: vlt=33.3
 - Wisol.sendMessage: 002BEDB4,920e5a00b0511a0194592100
Warning: Should wait 10 mins before sending the next message
 - Wisol.sendBuffer: AT$GI?

>> AT$GI?
 
<< 1,0 
 - Wisol.sendBuffer: response: 1,0
 - Wisol.sendBuffer: AT$RC

>> AT$RC
<< OK0x0d
 - Wisol.sendBuffer: response: OK
 - Wisol.sendBuffer: AT$SF=920e5a00b0511a0194592100

>> AT$SF=920e5a00b0511a0194592100
 
<< OK0x0d
 - Wisol.sendBuffer: response: OK
OK
STOPSTOPSTOP: Messages sent successfully: 10, failed: 0

*/

