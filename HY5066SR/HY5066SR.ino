#include "SoftwareSerial.h"

#define DEBUG 1

#define BK3266SR_52 0x52
#define BK3266SR_42 0x42
#define BK3266SR_23 0x23

#define BK3266SR_CMD_TYPE_ASWER  0x00  //answer
#define BK3266SR_CMD_TYPE_SEND_RECEIVER_DATA  0x01  //send data
#define BK3266SR_CMD_TYPE_OUTGOING_NUMBER  0x02  //Data is outgoing number
#define BK3266SR_CMD_TYPE_CALLER_NUMBER  0x03  //The data is the caller number
#define BK3266SR_CMD_TYPE_CURRENT_VOLUME 0x04  //Data is the current volume
#define BK3266SR_CMD_TYPE_SEND_TRANSMITTER_DATA 0x05  //Launch mode command
#define BK3266SR_CMD_TYPE_FOUND_DEVICE 0x06  /*The data is the found device A+B+C+D
      A[1] signal strength
      B[1] Equipment Index
      C[6] device address
      D[N] device name*/
#define BK3266SR_CMD_TYPE_LOCAL_ADDRESS  0x07  //The data is the local Bluetooth address
#define BK3266SR_CMD_TYPE_NAME 0x08  //The data is the local Bluetooth name
#define BK3266SR_CMD_TYPE_CONNECT_TO_NAME  0x09  //The data is the Bluetooth name of the connected device
#define BK3266SR_CMD_TYPE_SOFWARE_VERSION  0x0A  //Data is software version
#define BK3266SR_CMD_TYPE_SEND_TO_CONNECT_ADDRESS  0x0b  //mcu send back to connect address
#define BK3266SR_CMD_TYPE_CONNECT_TO_REMOTE_ADDRESS   0x0C  //  The transmitter connects to the Bluetooth address 
#define BK3266SR_CMD_TYPE_SEARCH_REMOTE_ADDRESS  0x0D  //  Bluetooth address searched by the transmitter

#define BK3266SR_CMD_ENTER_PAIRING_STATE  0x01  //  Enter the pairing state MCU-->BT  
#define BK3266SR_CMD_CONNECT_TO_LAST_DEVICE  0x02  //  Connect back to the last device MCU-->BT  
#define BK3266SR_CMD_DISCONNECT 0x03  //  Disconnect  MCU-->BT  
#define BK3266SR_CMD_PICKUP_CALL  0x04  //  Answer the phone  MCU-->BT  
#define BK3266SR_CMD_REJECT_CALL 0x05  //  Reject the call MCU-->BT  
#define BK3266SR_CMD_HANGUP_CALL 0x06  //  hang up the phone MCU-->BT  
#define BK3266SR_CMD_REDIAL_LAST 0x07 //  Call back the last outgoing call  MCU-->BT  
#define BK3266SR_CMD_GET_CURRENT_VOLUMEUME_UP 0x08  //  Voice+  MCU-->BT  
#define BK3266SR_CMD_GET_CURRENT_VOLUME_DOWN 0x09  //  sound-  MCU-->BT  
#define BK3266SR_CMD_CLEAR_MEMORY 0x0A  //  Clear the pairing list  MCU-->BT  
#define BK3266SR_CMD_PLAYPAUSE  0x0B  //  Pause playback  MCU-->BT  
#define BK3266SR_CMD_STOP 0x0C  //  Keep  MCU-->BT  
#define BK3266SR_CMD_NEXT 0x0D  //  next track  MCU-->BT  
#define BK3266SR_CMD_PREV 0x0E  //  previous piece  MCU-->BT  
#define BK3266SR_CMD_FORWARD  0x0F  //  Fast forward  MCU-->BT  
#define BK3266SR_CMD_REWIND 0x10  //  Rewind  MCU-->BT  
#define BK3266SR_CMD_DEVICE_ADDR  0x11  //  Query Bluetooth address MCU-->BT  
#define BK3266SR_CMD_DEVICE_NAME  0x12  //  Query Bluetooth name  MCU-->BT  
#define BK3266SR_CMD_PHONE_NAME 0x13  //  Query the name of the connected device, the link is successful and the command is valid MCU-->BT  
#define BK3266SR_CMD_GET_SW_VERSION  0x14  //  Query software version  MCU-->BT  
#define BK3266SR_CMD_GET_ACL_STATUS  0x15  //  Check if acl is linked  MCU-->BT  
#define BK3266SR_CMD_CALL_STATUS 0x16  //  Check call status MCU-->BT  
#define BK3266SR_CMD_SONG_STATUS 0x17  //  Check the status of the song  MCU-->BT  
//#define BK3266SR_CMD_V0 0x18  //  OPEN test 1K signal MCU-->BT
//#define BK3266SR_CMD_VS 0x19  //  STOP test 1K signal MCU-->BT
#define BK3266SR_CMD_GET_CURRENT_VOLUME  0x1A  //  Query current volume  MCU-->BT  
#define BK3266SR_CMD_TRANSMITTER_SEARCH 0x1B  //  Launch mode search device MCU-->BT  cmd 0x05
#define BK3266SR_CMD_TRANSMITTER_DISABLE  0x1C  //  Launch mode is off  MCU-->BT  cmd 0x05
//  Equipment index【0-6】  //  Launch mode to connect back to the device MCU-->BT  cmd 0x05
#define BK3266SR_CMD_RESET  0x1D  //  Reset MCU-->BT  Confirm the mode and check the IO port level before sending the command
//#define BK3266SR_CMD_TESTMODE 0x1E  //  Reconnect to the specified address 0x888888888888 MCU-->BT  For test mode
#define BK3266SR_CMD_ENTER_IDLE_MODE 0x1F  //  Enter idle mode power consumption 4MA MCU-->BT  CMD 0x01
//[CMD 0x0B]  DATA[0-6] //  The speaker address that MCU sends to BT needs to be linked MCU-->BT  For example, 52 42 23 09 0B 3DAB55FA58FC 9F 234252
#define BK3266SR_CMD_MUTE 0x20  //  Mute emission mode  MCU-->BT  
#define BK3266SR_CMD_UNMUTE 0x21  //  Transmit mode is not muted  MCU-->BT  
#define BK3266SR_CMD_CLEAR_KEY  0x22  //  CLEAR KEY MCU-->BT  
#define BK3266SR_CMD_REMOTE_ADDR  0x23  //  Query the address of the Bluetooth connection MCU-->BT  
#define BK3266SR_CMD_ENTER_TRANSMITTER 0x24  //  Enter launch mode MCU-->BT  The machine will automatically restart after sending the command (for the case of automatic startup)
#define BK3266SR_CMD_ENTER_RECEIVE_MODE  0x25  //  Enter receive mode  MCU-->BT  If it is already in the receiving mode, it returns to the receiving state, and the same is true for the transmitting mode
#define BK3266SR_CMD_SET_BLUETOOTH_NAME  0x29  //  Change Bluetooth name MCU-->BT  

#define BK3266SR_STATUS_OK 0x61  //  Answer OK
#define BK3266SR_STATUS_TIMEOUT 0x62  //  time out
#define BK3266SR_STATUS_PLAY 0x63  //  Play
#define BK3266SR_STATUS_ON 0x64  //  Boot up
#define BK3266SR_STATUS_LINK_CONNECTED 0x65  //  Link successfully
#define BK3266SR_STATUS_ACL_NOT_CONNECTED 0x66  //  ACL not connected query returns
#define BK3266SR_STATUS_ACL_CONNECTED 0x67  //  ACL connected query returns
#define BK3266SR_STATUS_NOT_CONNECTED 0x68  //  Not connected query returns
#define BK3266SR_STATUS_HFP_CONNECTED 0x69  //  HFP connected query returns
#define BK3266SR_STATUS_INCOMING_CALL_M2 0x70  //  Call back
#define BK3266SR_STATUS_OUTGOING_CALL 0x71  //  Call back
#define BK3266SR_STATUS_CALL_IN_PROGRESS 0x72  //  Query returns during the call
#define BK3266SR_STATUS_INCOMMING_CALL 0x73  //  Incoming call 
#define BK3266SR_STATUS_ID 0x74  //  Call 
#define BK3266SR_STATUS_TRANSMITTER 0x75  //  Launch mode
#define BK3266SR_STATUS_RECEIVER_MODE 0x76  //  Bluetooth mode
#define BK3266SR_STATUS_END 0x77 //  End of launch search
#define BK3266SR_STATUS_WC 0x78  //  Launch paired successfully
#define BK3266SR_STATUS_WD 0x79  //  Launch disconnect
#define BK3266SR_STATUS_IA 0X7A  //  Receive disconnect
#define BK3266SR_STATUS_IDLE 0x7B  //  Enter idle mode
#define BK3266SR_STATUS_MUTE 0x7C  //  MUTE amplifier
#define BK3266SR_STATUS_UNMUTE 0x7D  //  Turn on the amplifier

#define BK3266SR_RESPONCE_ACK 0x00  //  answer 
#define BK3266SR_RESPONCE_SEND 0x01  //  Return data 
#define BK3266SR_RESPONCE_PR 0x02  //  Connect number after outgoing call 
#define BK3266SR_RESPONCE_IR 0x03  //  Follow the number after the call 
#define BK3266SR_RESPONCE_VOL 0x04  //  Current volume
#define BK3266SR_RESPONCE_TWS 0x05  //  Launch command 
#define BK3266SR_RESPONCE_TB 0x06  //  Transmit data 
#define BK3266SR_RESPONCE_ADDR 0x07  //  Local Bluetooth address 
#define BK3266SR_RESPONCE_NAME 0x08  //  Local Bluetooth name 
#define BK3266SR_RESPONCE_PHONE_NAME 0x09  //  Connected device name 
#define BK3266SR_RESPONCE_VERSION 0x0A  //  Software version 
#define BK3266SR_RESPONCE_AUTADD 0x0B  //   
#define BK3266SR_RESPONCE_REMOTEADDR 0x0C  //  The transmitter connects to the Bluetooth address 
#define BK3266SR_RESPONCE_SEARCHREMOTEADDR 0x0D  //  Bluetooth address searched by the transmitter


SoftwareSerial btSerial(7, 6); //rxPin, txPin, inverse_logic

enum flowstate : uint8_t {
  START_52,
  START_42,
  START_23,
  PACKETSIZE,
  DATA,
  CRC,
  END_23,
  END_42,
  END_52
};

static flowstate state = START_52;

void setup() {
  Serial.begin(115200);
  btSerial.begin(9600);
}

void start() {
  btSerial.write(BK3266SR_52);
  btSerial.write(BK3266SR_42);
  btSerial.write(BK3266SR_23);
}

uint8_t senddata(uint8_t data);
uint8_t senddata(uint8_t data[]);
uint8_t dumpdata(uint8_t data[]);

uint8_t enterPairingState() {
  return senddata(BK3266SR_CMD_ENTER_PAIRING_STATE);
}
uint8_t connect() {
  return senddata(BK3266SR_CMD_CONNECT_TO_LAST_DEVICE);
}
uint8_t disconnect() {
  return senddata(BK3266SR_CMD_DISCONNECT);
}
uint8_t pickupCall() {
  return senddata(BK3266SR_CMD_PICKUP_CALL);
}
uint8_t rejectcall() {
  return senddata(BK3266SR_CMD_REJECT_CALL);
}
uint8_t hangUpCall() {
  return senddata(BK3266SR_CMD_HANGUP_CALL);
}
uint8_t redialLastCall() {
  return senddata(BK3266SR_CMD_REDIAL_LAST);
}
uint8_t volumeUp() {
  return senddata(BK3266SR_CMD_GET_CURRENT_VOLUMEUME_UP);
}
uint8_t volumeDown() {
  return senddata(BK3266SR_CMD_GET_CURRENT_VOLUME_DOWN);
}
uint8_t clearMemory() {
  return senddata(BK3266SR_CMD_CLEAR_MEMORY);
}
uint8_t playpause() {
  return senddata(BK3266SR_CMD_PLAYPAUSE);
}
uint8_t stop() {
  return senddata(BK3266SR_CMD_STOP);
}
uint8_t nextTrack() {
  return senddata(BK3266SR_CMD_NEXT);
}
uint8_t previousTrack() {
  return senddata(BK3266SR_CMD_PREV);
}
uint8_t forward() {
  return senddata(BK3266SR_CMD_FORWARD);
}
uint8_t rewind() {
  return senddata(BK3266SR_CMD_REWIND);
}
uint8_t getModuleName() {
  return senddata(BK3266SR_CMD_DEVICE_NAME);
}
uint8_t getModuleAddr() {
  return senddata(BK3266SR_CMD_DEVICE_ADDR);
}
uint8_t getPhoneName() {
  return senddata(BK3266SR_CMD_PHONE_NAME);
}
uint8_t version() {
  return senddata(BK3266SR_CMD_GET_SW_VERSION);
}
uint8_t isAclLinked() {
  return senddata(BK3266SR_CMD_GET_ACL_STATUS);
}
uint8_t callStatus() {
  return senddata(BK3266SR_CMD_CALL_STATUS);
}
uint8_t songStatus() {
  return senddata(BK3266SR_CMD_SONG_STATUS);
}
uint8_t getVolume() {
  return senddata(BK3266SR_CMD_GET_CURRENT_VOLUME);
}
uint8_t trasmitSearch() {
  return sendOtherData(BK3266SR_CMD_TYPE_SEND_TRANSMITTER_DATA, BK3266SR_CMD_TRANSMITTER_SEARCH);
}
uint8_t trasmitDisconnect() {
  return sendOtherData(BK3266SR_CMD_TYPE_SEND_TRANSMITTER_DATA, BK3266SR_CMD_TRANSMITTER_DISABLE);
}
uint8_t trasmitConnectToName(uint8_t data[]) {
  data[1] = BK3266SR_CMD_TYPE_SEND_TRANSMITTER_DATA;
  data[2] = BK3266SR_CMD_TYPE_CONNECT_TO_NAME;
  return senddata(data);
}
uint8_t reset() {
  return senddata(BK3266SR_CMD_RESET);
}
uint8_t idleMode() {
  return senddata(BK3266SR_CMD_ENTER_IDLE_MODE);
}
uint8_t mute() {
  return senddata(BK3266SR_CMD_MUTE);
}
uint8_t unmute() {
  return senddata(BK3266SR_CMD_UNMUTE);
}
uint8_t clearKey() {
  return senddata(BK3266SR_CMD_CLEAR_KEY);
}
uint8_t remoteAddr() {
  return senddata(BK3266SR_CMD_REMOTE_ADDR);
}
uint8_t transmit() {
  return senddata(BK3266SR_CMD_ENTER_TRANSMITTER);
}
uint8_t receiver() {
  return senddata(BK3266SR_CMD_ENTER_RECEIVE_MODE);
}

uint8_t setName(uint8_t data[]) {
  data[1] = BK3266SR_CMD_TYPE_SEND_RECEIVER_DATA;
  data[2] = BK3266SR_CMD_SET_BLUETOOTH_NAME;
  return senddata(data);
}
uint8_t call(uint8_t data[]) {
  return senddata(data);
}

uint8_t search() {
  return senddata(BK3266SR_CMD_TYPE_SEARCH_REMOTE_ADDRESS);
}

void end() {
  btSerial.write(BK3266SR_23);
  btSerial.write(BK3266SR_42);
  btSerial.write(BK3266SR_52);
}

void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    char c = Serial.read();
    switch (c) {
      case 'w': printHelp(); break;
      case '1': transmit(); break;
      case '2': receiver(); break;
      case '3': getPhoneName(); break;
      case '4': playpause(); break;
      case '5': search(); break;
      case '6': getModuleName(); break;
      case '7': getModuleAddr(); break;
      case '8': enterPairingState(); ; break;
      case '9': connect(); ; break;
      case '0': disconnect(); ; break;
      case 'a': pickupCall(); ; break;
      case 'A': rejectcall(); ; break;
      case 'b': hangUpCall(); ; break;
      case 'B': redialLastCall(); ; break;
      case 'c': volumeUp(); ; break;
      case 'C': volumeDown(); ; break;
      case 'd': stop(); break;
      case 'D': nextTrack(); break;
      case 'e': previousTrack(); break;
      case 'E': forward(); break;
      case 'f': rewind(); break;
      case 'F': version(); break;
      case 'g': isAclLinked(); break;
      case 'G': callStatus(); break;
      case 'h': songStatus(); break;
      case 'H': getVolume(); break;
      case 'i': trasmitSearch(); break;
      case 'I': trasmitDisconnect(); break;
      case 'j': reset(); break;
      case 'J': idleMode(); break;
      case 'k': mute(); break;
      case 'K': unmute(); break;
      case 'l': clearKey(); break;
      case 'L': remoteAddr(); break;
      case 'm': {
          uint8_t name[32];
          name[0] = 3; //0th=packetsize,1st=BK3266SR_CMD_TYPE_SEND_RECEIVER_DATA, 2nd = BK3266SR_CMD_SET_BLUETOOTH_NAME, 3th and up -> name
          uint8_t g;
          delay(500);//wait for incoming buffer to read serial data if any
          while (Serial.available() > 0) {
            delay(1);  //clear buffer
            Serial.read();
          }
          Serial.print(F("Enter name: "));
          while (Serial.available() == 0); //wait for user to enter data
          delay(500);//wait for incoming buffer to read serial data if any
          while (Serial.available()) {
            g = Serial.read(); //read serial
            if (g == '\n' || g == '\r') { //if it is /n /r
              continue;//skipp
            }
            name[name[0]++] = g; //we will store it
            Serial.write(g);//print back what we read
          }
          Serial.println();
          setName(name);
          delay(500);
          disconnect();
          delay(500);
          reset();
        }
        break;
      case 'M': {
          uint8_t number[16]; //packetsize,BK3266SR_CMD_TYPE_OUTGOING_NUMBER,+421 123 456 789 => 13
          number[0] = 1;
          number[number[0]++] = BK3266SR_CMD_TYPE_OUTGOING_NUMBER;
          uint8_t g;
          delay(500);//wait for incoming buffer to read serial data if any
          while (Serial.available() > 0) {
            delay(1);  //clear buffer
            Serial.read();
          }
          Serial.print(F("Enter phone number: "));
          while (Serial.available() == 0); //wait for user to enter data
          delay(500);//wait for incoming buffer to read serial data if any
          while (Serial.available()) {
            g = Serial.read(); //read serial
            if (g == '\n' || g == '\r') { //if it is /n /r
              continue;//skipp
            }
            number[number[0]++] = g; //we will store it
            Serial.write(g);//print back what we read
          }
          Serial.println();
          call(number);
        }
        break;
      case 'n': {
          uint8_t name[32];
          name[0] = 3; //0th=packetsize,1st=BK3266SR_CMD_TYPE_SEND_TRANSMITTER_DATA, 2nd = BK3266SR_CMD_TYPE_CONNECT_TO_NAME, 3th and up -> name
          uint8_t g;
          delay(500);//wait for incoming buffer to read serial data if any
          while (Serial.available() > 0) {
            delay(1);  //clear buffer
            Serial.read();
          }
          Serial.print(F("Enter name: "));
          while (Serial.available() == 0); //wait for user to enter data
          delay(500);//wait for incoming buffer to read serial data if any
          while (Serial.available()) {
            g = Serial.read(); //read serial
            if (g == '\n' || g == '\r') { //if it is /n /r
              continue;//skipp
            }
            name[name[0]++] = g; //we will store it
            Serial.write(g);//print back what we read
          }
          Serial.println();
          trasmitConnectToName(name);
        }
        break;
    }
  }
  btCheckResponce();
}

uint8_t btCheckResponce() {
  //delay(100);
  static uint8_t crc;
  static uint8_t packetbyte;
  static uint8_t data[32];
  static uint8_t timeout;
  timeout = 10;
  while (timeout > 0 && btSerial.available()) {
    //Serial.println("btCheckResponce");
    while (btSerial.available()) {
      uint8_t n = btSerial.read();
      //Serial.print(n, HEX); Serial.print("|");
      switch (state) {
        case START_52:
          if (n == 0x52) {
            //Serial.println();Serial.println();Serial.println();
            // Serial.print("start52: "); Serial.println(n, HEX);
            state = START_42;
          }
          else
            state = START_52;
          break;
        case START_42:
          if (n == 0x42) {
            // Serial.print("start42: "); Serial.println(n, HEX);
            state = START_23;
          }
          else
            state = START_52;
          break;
        case START_23:
          if (n == 0x23) {
            //Serial.print("start23: "); Serial.println(n, HEX);
            state = PACKETSIZE;
          }
          else
            state = START_52;
          break;
        case PACKETSIZE:

          packetbyte = 0;
          data[0] = n;
          state = DATA;
          // Serial.print("packetsize: "); Serial.print(n, HEX); Serial.print(" [packetbyte: "); Serial.print(packetbyte); Serial.println("]");
          break;
        case DATA:
          data[++packetbyte] = n;
          //Serial.print("data: "); Serial.print(n, HEX); Serial.print(" [packetbyte: "); Serial.print(packetbyte); Serial.println("]");
          if (packetbyte == data[0] - 1 ) //next
            state = END_23;
          break;
        case END_23:
          if (n == 0x23) {
            //Serial.print("end23: "); Serial.println(n, HEX);
            state = END_42;
          }
          break;
        case END_42:
          if (n == 0x42) {
            // Serial.print("end42: "); Serial.println(n, HEX);
            state = END_52;
            //Serial.println();
          }
          break;
        case END_52:
          if (n == 0x52) {
            //Serial.print("end52: "); Serial.println(n, HEX);
            state = START_52;
            //Serial.print("packetbyte: "); Serial.println(packetbyte);
            crc = 0;
            for (uint8_t i = 0; i < data[0] - 1; i++) {
              crc += data[i];
              //              Serial.println(crc,HEX);
              //                            if (DEBUG) {
              //                              Serial.print(data[i], HEX);
              //                              Serial.print("|");
              //                            }
            }
            //            Serial.println();
            //                        if (DEBUG) {
            //                          Serial.print(data[data[0] - 1], HEX);
            //                          Serial.print(" ");
            //                          Serial.println(crc,HEX);
            //                        }

            if (crc == data[data[0] - 1]) {
              // if (DEBUG) Serial.println(F(" [crc OK]"));
              if (DEBUG) Serial.print(decodeResponce(data[1]));
              if (DEBUG)
                switch (data[1]) {
                  case BK3266SR_RESPONCE_ACK:
                    break;
                  case BK3266SR_RESPONCE_PR:

                  case BK3266SR_RESPONCE_IR:

                  case BK3266SR_RESPONCE_VOL:
                    Serial.println(data[2]);
                    break;
                  case BK3266SR_RESPONCE_TWS:
                    for (uint8_t i = 0; i < data[0]; i++) {
                      Serial.print(data[i], HEX); Serial.print("|");
                    }

                    break;
                  case BK3266SR_CMD_TYPE_FOUND_DEVICE: //BK3266SR_RESPONCE_TB:
                    {
                      if (DEBUG) { //Found devices: [0] signal: 197 addr: [8D:64:CB:FA:58:FC] KOVOTEST
                        Serial.print(F("Found devices:  [")); Serial.print(data[3], DEC); Serial.print(F("] ")); //device index
                        for (uint8_t i = 10; i < data[0]-1; i++) {
                          Serial.write(data[i]);
                        }

                        Serial.print(F(" Signal level: ")); Serial.print(data[2]); // signal strength
                        Serial.print(F(" addr: ["));
                        for (uint8_t i = 4; i < 10; i++) {
                          Serial.print(data[i], HEX);
                          if (i != 9) Serial.print(F(":"));
                        }
                        Serial.println("]");
                      }
                    }
                    break;
                  case BK3266SR_RESPONCE_VERSION:
                    for (uint8_t i = 2; i < data[0]; i++) {
                      Serial.write(data[i]);
                    }
                    Serial.println();
                    break;
                  case BK3266SR_RESPONCE_AUTADD:

                  case BK3266SR_RESPONCE_REMOTEADDR:

                  case BK3266SR_RESPONCE_SEARCHREMOTEADDR:
                    for (uint8_t i = 0; i < data[0]; i++) {
                      Serial.print(data[i], HEX); Serial.print("|");
                    }
                    break;
                  case BK3266SR_RESPONCE_SEND:
                    {
                      Serial.println(decodeReceivedData(data[2]));
                    }
                    break;
                  case BK3266SR_RESPONCE_PHONE_NAME:
                    {
                      Serial.print(F("Phone name"));
                      for (uint8_t i = 1; i < data[0]-1; i++) {
                        if (DEBUG) {
                          Serial.write(data[i]);
                        }
                      }
                      Serial.println();
                    }
                    break;
                  case BK3266SR_RESPONCE_NAME:
                    {
                      Serial.print(F("Bluetooth name"));
                      for (uint8_t i = 1; i < data[0]-1; i++) {
                        if (DEBUG) {
                          Serial.write(data[i]);
                        }
                      }
                      Serial.println();
                    }
                    break;
                  case BK3266SR_RESPONCE_ADDR:
                    {
                      Serial.print(F("Bluetooth addr: "));
                      for (uint8_t i = 2; i < 8; i++) {
                        if (DEBUG) {
                          Serial.print(data[i], HEX);
                          if (i<7)Serial.print(":");
                        }
                      }
                      Serial.println();
                    }
                    break;
                }
              if (data[1] == BK3266SR_RESPONCE_ACK && data[2] == BK3266SR_STATUS_OK) {
                return true;
              }
            }
            //Serial.println();
          }
          break;
      }
    }
    delay(50);
    timeout -= 1;
  }
  return false;
}

uint8_t senddata(uint8_t data) {
  if (DEBUG) Serial.println(decodeCmd(data));
  start();
  static uint8_t crc;
  crc = 0x05;//0x04+BK3266SR_CMD_TYPE_SEND_RECEIVER_DATA=>0x01
  btSerial.write(0x04);
  btSerial.write(BK3266SR_CMD_TYPE_SEND_RECEIVER_DATA);
  btSerial.write(data);
  crc += data;
  btSerial.write(crc & 0xFF);
  end();
  return btCheckResponce();
}

uint8_t sendOtherData(uint8_t cmd, uint8_t data) {
  if (DEBUG) Serial.println(decodeCmd(data));
  start();
  static uint8_t crc;
  crc = 0x04;
  btSerial.write(0x04); //length
  btSerial.write(cmd);
  crc += cmd;
  btSerial.write(data);
  crc += data;
  btSerial.write(crc & 0xFF);
  end();
  return btCheckResponce();
}

uint8_t senddata(uint8_t data[]) {
  data[0]++;
  //if (DEBUG) Serial.println(decodeCmd(data[2]));
  start();
  static uint8_t crc;
  crc = 0;
  for (uint8_t i = 0; i < data[0] - 1; i++) { //count till crc
    btSerial.write(data[i]);
    if (DEBUG) {
      if (i < 2) {
        Serial.print(data[i], HEX); Serial.print("|");
      }
      if (i > 1) {
        Serial.write(data[i]); Serial.print("|");
      }
    }
    crc += data[i];
  }
  Serial.println(crc, HEX);
  btSerial.write(crc & 0xFF); //Serial.print(crc, HEX);
  end();
  return btCheckResponce();
}

uint8_t dumpdata(uint8_t data[]) {
  Serial.println("caling dumpdata--------------------------------");
  data[0]++;
  static uint8_t crc;
  crc = 0;
  for (uint8_t i = 0; i < data[0] - 1; i++) { //count till crc
    if (i == 0) {
      Serial.print(data[i]); Serial.print("|");
    }
    if (i == 1) {
      Serial.print(data[i], HEX); Serial.print("|");
    }
    if (i > 1) {
      Serial.write(data[i]); Serial.print("|");
    }
    crc += data[i];
  }
  Serial.print(" CRC: "); Serial.println(crc, HEX);
  Serial.println("caling dumpdata end--------------------------------");
}

String decodeResponce(uint8_t RSP) {
  switch (RSP) {
    case BK3266SR_RESPONCE_ACK: return F("ACK\n");
    case BK3266SR_RESPONCE_SEND: return F("");//return F("Return data");//RECEIVER data?
    case BK3266SR_RESPONCE_PR: return F("Connect number after outgoing call");
    case BK3266SR_RESPONCE_IR: return F("Follow the number after the call");
    case BK3266SR_RESPONCE_VOL: return F("Current volume: ");
    case BK3266SR_RESPONCE_TWS: return F("Launch command");
    case BK3266SR_RESPONCE_TB: return F("");//Transmit data");
    case BK3266SR_RESPONCE_ADDR: return F("Local Bluetooth address: ");
    case BK3266SR_RESPONCE_NAME: return F("Local Bluetooth name: ");
    case BK3266SR_RESPONCE_PHONE_NAME: return F("Connected device name: ");
    case BK3266SR_RESPONCE_VERSION: return F("Software version: ");
    case BK3266SR_RESPONCE_AUTADD: return F("AUTADD");
    case BK3266SR_RESPONCE_REMOTEADDR: return F("The transmitter connects to the Bluetooth address");
    case BK3266SR_RESPONCE_SEARCHREMOTEADDR: return F("Bluetooth address searched by the transmitter");
  }
}

String decodeReceivedData(uint8_t data) {
  switch (data) {
    case BK3266SR_STATUS_OK: return F("OK");
    case BK3266SR_STATUS_TIMEOUT: return F("Time out");
    case BK3266SR_STATUS_PLAY: return F("Play");
    case BK3266SR_STATUS_ON: return F("Boot up");
    case BK3266SR_STATUS_LINK_CONNECTED: return F("Link successfully");
    case BK3266SR_STATUS_ACL_NOT_CONNECTED: return F("ACL not connected");
    case BK3266SR_STATUS_ACL_CONNECTED: return F("ACL connected");
    case BK3266SR_STATUS_NOT_CONNECTED: return F("Not connected");
    case BK3266SR_STATUS_HFP_CONNECTED: return F("HFP connected");
    case BK3266SR_STATUS_INCOMING_CALL_M2: return F("Incoming call");
    case BK3266SR_STATUS_OUTGOING_CALL: return F("Outgoing call");
    case BK3266SR_STATUS_CALL_IN_PROGRESS: return F("Call in progress");
    case BK3266SR_STATUS_INCOMMING_CALL: return F("Incoming call");
    case BK3266SR_STATUS_ID: return F("Call");
    case BK3266SR_STATUS_TRANSMITTER: return F("Transmit mode");
    case BK3266SR_STATUS_RECEIVER_MODE: return F("Receiver mode");
    case BK3266SR_STATUS_END: return F("End of transmitter search");
    case BK3266SR_STATUS_WC: return F("Transmitter paired successfully");
    case BK3266SR_STATUS_WD: return F("Transmitter disconnected");
    case BK3266SR_STATUS_IA: return F("Receiver disconnected");
    case BK3266SR_STATUS_IDLE: return F("Enteridle mode");
    case BK3266SR_STATUS_MUTE: return F("MUTE amplifier");
    case BK3266SR_STATUS_UNMUTE: return F("Turn on the amplifier");
  }
}

String decodeCmdType(uint8_t cmd) {
  switch (cmd) {
    case BK3266SR_CMD_TYPE_ASWER: return F("answer");
    case BK3266SR_CMD_TYPE_SEND_RECEIVER_DATA: return F("send data");
    case BK3266SR_CMD_TYPE_OUTGOING_NUMBER: return F("Data is outgoing number");
    case BK3266SR_CMD_TYPE_CALLER_NUMBER: return F("The data is the caller number");
    case BK3266SR_CMD_TYPE_CURRENT_VOLUME: return F("Data is the current volume");
    case BK3266SR_CMD_TYPE_SEND_TRANSMITTER_DATA: return F("Launch mode command");
    case BK3266SR_CMD_TYPE_FOUND_DEVICE: return F("The data is the found device A+B+C+D");/*
      A[1] signal strength
      B[1] Equipment Index
      C[6] device address
      D[N] device name*/
    case BK3266SR_CMD_TYPE_LOCAL_ADDRESS: return F("The data is the local Bluetooth address");
    case BK3266SR_CMD_TYPE_NAME: return F("The data is the local Bluetooth name");
    case BK3266SR_CMD_TYPE_CONNECT_TO_NAME: return F("The data is the Bluetooth name of the connected device");
    case BK3266SR_CMD_TYPE_SOFWARE_VERSION: return F("Data is software version");
    case BK3266SR_CMD_TYPE_SEND_TO_CONNECT_ADDRESS: return F("mcu send back to connect address");
    case BK3266SR_CMD_TYPE_CONNECT_TO_REMOTE_ADDRESS: return F("The transmitter connects to the Bluetooth address");
    case BK3266SR_CMD_TYPE_SEARCH_REMOTE_ADDRESS: return F("Bluetooth address searched by the transmitter");
  }
}

String decodeCmd(uint8_t cmd) {
  switch (cmd) {
    case BK3266SR_CMD_ENTER_PAIRING_STATE: return F("Enter the pairing state");
    case BK3266SR_CMD_CONNECT_TO_LAST_DEVICE: return F("Connect back to the last device");
    case BK3266SR_CMD_DISCONNECT: return F("Disconnect");
    case BK3266SR_CMD_PICKUP_CALL: return F("Answer the call");
    case BK3266SR_CMD_REJECT_CALL: return F("Reject the call");
    case BK3266SR_CMD_HANGUP_CALL: return F("hang up the phone");
    case BK3266SR_CMD_REDIAL_LAST: return F("Call back the last outgoing call");
    case BK3266SR_CMD_GET_CURRENT_VOLUMEUME_UP: return F("Vol+");
    case BK3266SR_CMD_GET_CURRENT_VOLUME_DOWN: return F("Vol-");
    case BK3266SR_CMD_CLEAR_MEMORY: return F("Clear the pairing list");
    case BK3266SR_CMD_PLAYPAUSE: return F("Pause playback");
    case BK3266SR_CMD_STOP: return F("Stop");
    case BK3266SR_CMD_NEXT: return F("next");
    case BK3266SR_CMD_PREV: return F("previous");
    case BK3266SR_CMD_FORWARD: return F("Fast forward");
    case BK3266SR_CMD_REWIND: return F("Rewind");
    case BK3266SR_CMD_DEVICE_ADDR: return F("Query Bluetooth address");
    case BK3266SR_CMD_DEVICE_NAME: return F("Query Bluetooth name");
    case BK3266SR_CMD_PHONE_NAME: return F("Query the name of the connected device, the link is successful and the command is valid");
    case BK3266SR_CMD_GET_SW_VERSION: return F("Query software version");
    case BK3266SR_CMD_GET_ACL_STATUS: return F("Check if acl is linked");
    case BK3266SR_CMD_CALL_STATUS: return F("Check call status");
    case BK3266SR_CMD_SONG_STATUS: return F("Check the status of the song");
    //case BK3266SR_CMD_V0: return F("OPEN test 1K signal");
    //case BK3266SR_CMD_VS: return F("STOP test 1K signal");
    case BK3266SR_CMD_GET_CURRENT_VOLUME: return F("Query current volume");
    case BK3266SR_CMD_TRANSMITTER_SEARCH: return F(" Launch mode search device");// MCU-- > BT  cmd 0x05
    case BK3266SR_CMD_TRANSMITTER_DISABLE: return F("Launch mode is off");//MCU-->BT  cmd 0x05
    //  Equipment index【0-6】  //  Launch mode to connect back to the device MCU-->BT  cmd 0x05
    case BK3266SR_CMD_RESET: return F("Reset");// MCU-->BT  Confirm the mode and check the IO port level before sending the command
    //case BK3266SR_CMD_TESTMODE: return F("Reconnect to the specified address");// 0x888888888888 MCU-->BT  For test mode
    case BK3266SR_CMD_ENTER_IDLE_MODE: return F("Enter idle mode power consumption 4MA");// MCU-->BT  CMD 0x01
    //[CMD 0x0B]  DATA[0-6] //  The speaker address that MCU sends to BT needs to be linked MCU-->BT  For example, 52 42 23 09 0B 3DAB55FA58FC 9F 234252
    case BK3266SR_CMD_MUTE: return F("Mute emission mode");
    case BK3266SR_CMD_UNMUTE: return F("Transmit mode is not muted");
    case BK3266SR_CMD_CLEAR_KEY: return F("CLEAR KEY");
    case BK3266SR_CMD_REMOTE_ADDR: return F("Query the address of the Bluetooth connection");
    case BK3266SR_CMD_ENTER_TRANSMITTER: return F("Enter launch mode");
    case BK3266SR_CMD_ENTER_RECEIVE_MODE: return F("Enter receive mode");
    case BK3266SR_CMD_SET_BLUETOOTH_NAME: return F("Change Bluetooth name");
  }
}

void printHelp() {
  Serial.println(F("w: printHelp"));
  Serial.println(F("1: transmit"));
  Serial.println(F("2: receiver"));
  Serial.println(F("3: getPhoneName"));
  Serial.println(F("4: playpause"));
  Serial.println(F("5: search"));
  Serial.println(F("6: getModuleName"));
  Serial.println(F("7: getModuleAddr"));
  Serial.println(F("8: Enter pairing tate"));
  Serial.println(F("9: connect"));
  Serial.println(F("0: disconnect"));
  Serial.println(F("a: pickupCall"));
  Serial.println(F("A: rejectcall"));
  Serial.println(F("b: hangUpCall"));
  Serial.println(F("B: redialLastCall"));
  Serial.println(F("c: volumeUp"));
  Serial.println(F("C: volumeDown"));
  Serial.println(F("d: stop"));
  Serial.println(F("D: nextTrack"));
  Serial.println(F("e: previousTrack"));
  Serial.println(F("E: forward"));
  Serial.println(F("f: rewind"));
  Serial.println(F("F: version"));
  Serial.println(F("g: isAclLinked"));
  Serial.println(F("G: callStatus"));
  Serial.println(F("h: songStatus"));
  Serial.println(F("H: getVolume"));
  Serial.println(F("i: trasmitSearch"));
  Serial.println(F("I: trasmitDisconnect"));
  Serial.println(F("j: reset"));
  Serial.println(F("J: idleMode"));
  Serial.println(F("k: mute"));
  Serial.println(F("K: unmute"));
  Serial.println(F("l: clearKey"));
  Serial.println(F("L: remoteAddr"));
  Serial.println(F("m: setName"));
  Serial.println(F("M: call"));
  Serial.println(F("n: connect to name"));
}
