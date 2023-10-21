/*Begining of Auto generated code by Atmel studio */
/*
This code is not clean and not perfect, this is only 
a reference to extract ideas and adapte to your solution.
*/
void send_char(unsigned char a);
void send_data(unsigned char a);
#define LCD_in 8  // This is the pin number 8 on Arduino UNO
#define LCD_clk 9 // This is the pin number 9 on Arduino UNO
#define LCD_stb 10 // This is the pin number 10 on Arduino UNO
#define LCD_INH 11 // This is the pin number 11 on Arduino UNO

unsigned int shiftBit=0;
unsigned int nBitOnBlock=0; // Used to count number of bits and split to 8 bits... (number of byte)
unsigned int nByteOnBlock=0; 
unsigned int sequencyByte=0x00;
byte Aa,Ab,Ac,Ad,Ae,Af,Ag,Ah,Ai,Aj,Ak,Al,Am,An,Ao,Ap,Aq,Ar,As,At ;
byte blockBit =0x00;

// constants won't change. They're used here to set pin numbers:
//const int buttonPin = 2;  // the number of the pushbutton pin
const int ledPin = 12;    // the number of the external LED pin

#define BUTTON_PIN 2 //Att check wich pins accept interrupts... Uno is 2 & 3
volatile byte buttonReleased = false;

/*
#define BIN(x) \
( ((0x##x##L & 0x00000001L) ? 0x01 : 0) \
| ((0x##x##L & 0x00000010L) ? 0x02 : 0) \
| ((0x##x##L & 0x00000100L) ? 0x04 : 0) \
| ((0x##x##L & 0x00001000L) ? 0x08 : 0) \
| ((0x##x##L & 0x00010000L) ? 0x10 : 0) \
| ((0x##x##L & 0x00100000L) ? 0x20 : 0) \
| ((0x##x##L & 0x01000000L) ? 0x40 : 0) \
| ((0x##x##L & 0x10000000L) ? 0x80 : 0))
*/

//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.

//*************************************************//
void setup() {
  pinMode(LCD_clk, OUTPUT);
  pinMode(LCD_in, OUTPUT);
  pinMode(LCD_stb, OUTPUT);
  pinMode(LCD_INH, OUTPUT);//When this pin is Low, the display is forcibly turned off. (SG1~SG52, COM1~COM3 are set to Low)
                           //When /INH = Low: Serial data transfers can be performed when the display is forcibly off. 
  pinMode(13, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);

 attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonReleasedInterrupt,
                  FALLING);
  Serial.begin(115200);
  /*CS12  CS11 CS10 DESCRIPTION
  0        0     0  Timer/Counter1 Disabled 
  0        0     1  No Prescaling
  0        1     0  Clock / 8
  0        1     1  Clock / 64
  1        0     0  Clock / 256
  1        0     1  Clock / 1024
  1        1     0  External clock source on T1 pin, Clock on Falling edge
  1        1     1  External clock source on T1 pin, Clock on rising edge
 */
  
// Note: this counts is done to a Arduino 1 with Atmega 328... Is possible you need adjust
// a little the value 62499 upper or lower if the clock have a delay or advnce on hours.

  digitalWrite(LCD_stb, LOW);
  delay(5);
  digitalWrite(LCD_INH, HIGH);
  delay(5);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
 
}
void send_char(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  // the validation of data happen when clk go from LOW to HIGH.
  // This lines is because the clk have one advance in data, see datasheet of sn74HC595
  // case don't have this signal instead of "." will se "g"
  digitalWrite(LCD_stb, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  digitalWrite(LCD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(LCD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(LCD_stb, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  }
}
// I h've created 3 functions to send bit's, one with strobe, other without strobe and one with first byte with strobe followed by remaing bits.
void send_char_without(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(LCD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
}
void send_char_8bit_stb(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
 int i = -1;
  data=a;
  // This lines is because the clk have one advance in data, see datasheet of sn74HC595
  // case don't have this signal instead of "." will se "g"
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
   i++;
   digitalWrite(LCD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(1);
    if (i==7){
    //Serial.println(i);
    digitalWrite(LCD_stb, HIGH);
    delayMicroseconds(2);
    }
     
  }
}
void msgHiFolks(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00011100);  send_char_without(0B01100100); //  8:1    -16:9 
send_char_without(0B11000000);  send_char_without(0B00000001); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B10000000); // 40:33  -48:41  
send_char_without(0B10100011);  send_char_without(0B11000000); // 56:49  -64:57  
send_char_without(0B00110000);  send_char_without(0B01100110); // 72:65  -80:73 
send_char_without(0B00010000);  send_char_without(0B01110000); // 88:81  -96:89 
send_char_without(0B01100000);  send_char_without(0B00101000); // 104:97-112:105
send_char_without(0B01001110);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00000000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgWheel_1(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000001);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00000000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgWheel_2(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000010);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00000000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgWheel_16(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B10000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00000000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgWheel_17(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000001);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00000000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
//
void msgDraw_143(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B11000000); //136:129-144:137
send_char_without(0B00111111);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_144(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B10000000); //136:129-144:137
send_char_without(0B00111111);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_145(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00111111);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_146(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00111110);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_147(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00111100);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_148(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00111000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_149(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1    -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00110000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void msgDraw_150(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  8:1   -16:9 
send_char_without(0B00000000);  send_char_without(0B00000000); // 24:17  -32:25 
send_char_without(0B00000000);  send_char_without(0B00000000); // 40:33  -48:41  
send_char_without(0B00000000);  send_char_without(0B00000000); // 56:49  -64:57  
send_char_without(0B00000000);  send_char_without(0B00000000); // 72:65  -80:73 
send_char_without(0B00000000);  send_char_without(0B00000000); // 88:81  -96:89 
send_char_without(0B00000000);  send_char_without(0B00000000); // 104:97-112:105
send_char_without(0B00000000);  send_char_without(0B00000000); //120:113-128:121
send_char_without(0B00000000);  send_char_without(0B00000000); //136:129-144:137
send_char_without(0B00100000);  send_char_without(0B00000000); //152:145-160:153 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void segments(){
//Bit function: 
      digitalWrite(LCD_stb, LOW); //
      delayMicroseconds(1);
      send_char_8bit_stb(0B01000001); //(0x4B) firts 8 bits is address, every fixed as (0B01000010), see if clk finish LOW or HIGH Very important!
      delayMicroseconds(1);
      // 
          send_char_without(~Aa);  send_char_without(~Ab);  //   8:1     -16:9// 
          send_char_without(~Ac);  send_char_without(~Ad);  //  24:17    -32:25//
          send_char_without(~Ae);  send_char_without(~Af);  //  40:33    -48:41// 
          send_char_without(~Ag);  send_char_without(~Ah);  //  56:49    -64:57// 
          send_char_without(~Ai);  send_char_without(~Aj);  //  72:65    -80:73// 
          send_char_without(~Ak);  send_char_without(~Al);  //  88:81    -96:89//  
          send_char_without(~Am);  send_char_without(~An);  // 104:97  -112:105//  
          send_char_without(~Ao);  send_char_without(~Ap);  // 120:113 -128:121// 
          send_char_without(~Aq);  send_char_without(~Ar);  // 136:129 -144:137// 
          send_char_without(~As);  send_char_without(At=(~At & 0x0F));  // 152:145 -160:153//   
          //The next switch finalize the burst of bits -41:48//  
      delayMicroseconds(1);
      digitalWrite(LCD_stb, LOW); //                   
}
void searchOfSegments(){
// put your main code here, to run repeatedly:
  int group = 0x00;
  byte nBit =0x00;
  byte nMask = 0b00000001;
  unsigned int block =0;
  unsigned int nSeg=0x000;  // This variable need support more of 255 (600 segments)
  Serial.println();
  Serial.println("We start the test of segments!");
  
for( group=0; group<19; group++){   // Do until n bits 15*8 bits
        //for(int nBit=0; nBit<8; nBit++){
          for (nMask = 0b00000001; nMask>0; nMask <<= 1){
            Aa=0x00; Ab=0x00; Ac=0x00; Ad=0x00; Ae=0x00; Af=0x00; Ag=0x00; Ah=0x00; Ai=0x00; Aj=0x00;
            Ak=0x00; Al=0x00; Am=0x00; An=0x00; Ao=0x00; Ap=0x00; Aq=0x00; Ar=0x00; As=0x00; At=0x00;
                  switch (group){
                    case 0: Aa=nMask; break;
                    case 1: Ab=nMask; break;//atoi(to integer)
                    case 2: Ac=nMask; break;
                    case 3: Ad=nMask; break;
                    case 4: Ae=nMask; break;
                    case 5: Af=nMask; break;
                    case 6: Ag=nMask; break;//atoi(to integer)
                    case 7: Ah=nMask; break;
                    case 8: Ai=nMask; break;
                    case 9: Aj=nMask; break;
                    case 10: Ak=nMask; break;
                    case 11: Al=nMask; break;//atoi(to integer)
                    case 12: Am=nMask; break;
                    case 13: An=nMask; break;
                    case 14: Ao=nMask; break;
                    case 15: Ap=nMask; break;
                    case 16: Aq=nMask; break;//atoi(to integer)
                    case 17: Ar=nMask; break;
                    case 18: As=nMask; break;
                    case 19: At=nMask; break;
                  } 
           nSeg++;   
      //This start the control of button to allow continue teste! 
                      while(1){
                            if(!buttonReleased){
                              delay(200);
                            }
                            else{
                              delay(15);
                               buttonReleased = false;
                               break;
                               }
                         }
               delay(50);
            segments();
            Serial.print(nSeg, DEC); Serial.print(", group: "); Serial.print(group, DEC);Serial.print(", BlockBit: "); Serial.print(blockBit, HEX);Serial.print(", nMask: "); Serial.print(nMask, BIN);Serial.print("   \t");
            Serial.print(Ae, HEX);Serial.print(", ");Serial.print(Ad, HEX);Serial.print(", ");Serial.print(Ac, HEX);Serial.print(", ");Serial.print(Ab, HEX);Serial.print(", ");Serial.print(Aa, HEX); Serial.print("; ");
            Serial.print(Aj, HEX);Serial.print(", ");Serial.print(Ai, HEX);Serial.print(", ");Serial.print(Ah, HEX);Serial.print(", ");Serial.print(Ag, HEX);Serial.print(", ");Serial.print(Af, HEX); Serial.print("; ");
            Serial.print(Ao, HEX);Serial.print(", ");Serial.print(An, HEX);Serial.print(", ");Serial.print(Am, HEX);Serial.print(", ");Serial.print(Al, HEX);Serial.print(", ");Serial.print(Ak, HEX); Serial.print("; ");
            Serial.print(At, HEX);Serial.print(", ");Serial.print(As, HEX);Serial.print(", ");Serial.print(Ar, HEX);Serial.print(", ");Serial.print(Aq, HEX);Serial.print(", ");Serial.print(Ap, HEX); Serial.print("; ");
            
            Serial.println();
            delay (250);          
           }        
      }
  }
void allON(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111); 
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B00001111);
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void allOFF(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000); 
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000); 
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B01100000);
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void loop() {
   unsigned int t1 = 500;
  for (int s=0; s<5; s++){
    allON(); // All on
    delay(1000);
    allOFF(); // All off
    delay(500);
   }
  msgHiFolks();
//  delay(4000);
//  allOFF(); // All off
//            for(unsigned int s=0; s< 5; s++){
//               msgWheel_17();
//               delay(200);
//               msgWheel_16();
//               delay(200);
//               msgWheel_2();
//               delay(200);
//               msgWheel_1();
//               delay(200);
//            }
//    for(unsigned int s=0; s<5; s++){
//      msgDraw_150();
//      delay(t1);
//      msgDraw_149();
//      delay(t1);
//      msgDraw_148();
//      delay(t1);
//      msgDraw_147();
//      delay(t1);
//      msgDraw_146();
//      delay(t1);
//      msgDraw_145();
//      delay(t1);
//      msgDraw_144();
//      delay(t1);
//    }
//  
  searchOfSegments();
}
void buttonReleasedInterrupt() {
  buttonReleased = true; // This is the line of interrupt button to advance one step on the search of segments!
}
