/*

TO DO:
  caps lock key
  map reset key for ctrl-apple-reset

*/



/*
        XO      X1      X2      X3  |   X4      X5      X6      X7
------------------------------------+--------------------------------
YO      ESC     TAB     A       Z   |   /       )       *       ESC
                                    |
Y1      1!      Q       D       X   |   DOWN    UP      LEFT    RIGHT
                                    |
Y2      20      W       S       C   |   0       4       8       (
                                    |
Y3      34      E       H       V   |   1       5       9       -
                                    |
Y4      4$      R       F       B   |   2       6       .       RETURN
                                    |
Y5      6"      Y       G       N   |   3       7       +       ,
                                    +----------------------------------
Y6      5%      T       J       M       \|      `~      RETURN  DELETE

Y7      7&      U       K       ,<      +=       P      UP       DOWN

Y8      8*      I       ;:      .>      0)       [{     SPACE   LEFT

Y9      9(      O       L       /?      -_       ]}      '"      RIGHT



IIe  Col/Row  
1     Y0        
2     Y1	
3     +5V       
4     Y2	
5     SW1/SAPL*  
6     Y3	
7     SW0/OAPL*  
8     Y4	
9     CAPLOCK*  
10     Y5	
11     CNTL*    
12     Y8	
13     GND      
14     X0	
15     RESET*   
16     X2     
17     X7     
18     X1	
19     X5      
20     X3	
21     X4      
22     Y9	
23     Y6      
24     SHFT*	
25     Y7      
26     X6

*  RESET is a switch between CTL (11) and pin 15
  L and R shift keys share pin 24, switched to ground
  CONTROL switched to ground
  CAPLOCK switched to ground
  pins 5 and 7 are grounded via 470Ω resistors
  Apple keys connect pin 3 (5v) to pins 5 & 7 



cd74hc4067  == ATMega pins
mux enable = 0
s0 = 3
s1 = 4
s2 = 2
s3 = 1

to J1
0 = y0
1
2
3
4
5
6
7
8
9 = y9



4051 == ATMega pins
enable = GND (always enabled)
s0 = 5
s1 = 6
s2 = 7

to J1
0 = x0
1
2
3
4
5
6
7 = x7


Solid Apple = AVR 9
Open Apple = AVR 8
shift = AVR A0 / 14
CTL = AVR A1 / 15


*/
    
  int ENABLE_PIN =   0;
  int S0_PIN = 3;
  int S1_PIN = 4;
  int S2_PIN = 2;
  int S3_PIN = 1;

// 4051 pins
  int S0_4051 = 5;
  int S1_4051 = 6;
  int S2_4051 = 7;

  int SHIFT_PIN = 14;
  int CONTROL_PIN = 15;
  int OPEN_APPLE_PIN = 8;
  int CLOSED_APPLE_PIN = 16;
  int CAPS_LOCK_PIN = 17;
  int RESET_PIN = 19;

  int r0;
  int r1;
  int r2;

  int c0;
  int c1;
  int c2;
  int c3;
  
  int count= 0;


const int COLUMNS = 8;
const int ROWS = 10;

int KEYS_ARRAY[ROWS][COLUMNS] = {
{      0x29,      0x2B,      0x04,      0x1D,      0x54,       0x27,       0x55,     0x29   },
                                    
{      0x1E,      0x14,      0x07,      0x1B,      0x51,       0x52,       0x50,     0x4F   },
                                    
{      0x1F,      0x1a,      0x16,      0x06,      0x62,       0x5C,       0x60,     0x26   },
                                    
{      0x20,      0x08,      0x0B,      0x19,      0x59,       0x5D,       0x61,     0x56   },
                                    
{      0x21,      0x15,      0x09,      0x05,      0x5A,       0x5E,       0x63,     0x58   },
                                    
{      0x23,      0x1C,      0x0A,      0x11,      0x5B,       0x5F,       0x57,     0x85   },

{      0x22,      0x17,      0x0D,      0x10,      0x31,       0x35,       0x28,     0x2A   },
  
{      0x24,      0x18,      0x0E,      0x36,      0x2E,       0x13,       0x52,     0x51   },

{      0x25,      0x0C,      0x33,      0x37,      0x27,       0x2F,       0x2C,     0x50   },

{      0x26,      0x12,      0x0F,      0x38,      0x2D,       0x30,       0x34,     0x4F   }
};



/*

	0x00	Reserved (no event indicated)
	0x01	Keyboard ErrorRollOver
	0x02	Keyboard POSTFail
	0x03	Keyboard ErrorUndefined
	0x04	Keyboard a and A
	0x05	Keyboard b and B
	0x06	Keyboard c and C
	0x07	Keyboard d and D
	0x08	Keyboard e and E
	0x09	Keyboard f and F
	0x0A	Keyboard g and G
	0x0B	Keyboard h and H
	0x0C	Keyboard i and I
	0x0D	Keyboard j and J
	0x0E	Keyboard k and K
	0x0F	Keyboard l and L
	0x10	Keyboard m and M
	0x11	Keyboard n and N
	0x12	Keyboard o and O
	0x13	Keyboard p and P
	0x14	Keyboard q and Q
	0x15	Keyboard r and R
	0x16	Keyboard s and S
	0x17	Keyboard t and T
	0x18	Keyboard u and U
	0x19	Keyboard v and V
	0x1A	Keyboard w and W
	0x1B	Keyboard x and X
	0x1C	Keyboard y and Y
	0x1D	Keyboard z and Z
	0x1E	Keyboard 1 and !
	0x1F	Keyboard 2 and @
	0x20	Keyboard 3 and #
	0x21	Keyboard 4 and $
	0x22	Keyboard 5 and %
	0x23	Keyboard 6 and ^
	0x24	Keyboard 7 and &
	0x25	Keyboard 8 and *
	0x26	Keyboard 9 and (
	0x27	Keyboard 0 and )
	0x28	Keyboard Return (ENTER)
	0x29	Keyboard ESCAPE
	0x2A	Keyboard DELETE (Backspace)
	0x2B	Keyboard Tab
	0x2C	Keyboard Spacebar
	0x2D	Keyboard - and (underscore)
	0x2E	Keyboard = and +
	0x2F	Keyboard [ and {
	0x30	Keyboard ] and }
	0x31	Keyboard \ and |
	0x32	Keyboard Non-US # and ~
	0x33	Keyboard ; and :
	0x34	Keyboard ' and "
	0x35	Keyboard Grave Accent and Tilde
	0x36	Keyboard, and <
	0x37	Keyboard . and >
	0x38	Keyboard / and ?
	0x39	Keyboard Caps Lock
	0x3A	Keyboard F1
	0x3B	Keyboard F2
	0x3C	Keyboard F3
	0x3D	Keyboard F4
	0x3E	Keyboard F5
	0x3F	Keyboard F6
	0x40	Keyboard F7
	0x41	Keyboard F8
	0x42	Keyboard F9
	0x43	Keyboard F10
	0x44	Keyboard F11
	0x45	Keyboard F12
	0x46	Keyboard PrintScreen
	0x47	Keyboard Scroll Lock
	0x48	Keyboard Pause
	0x49	Keyboard Insert
	0x4A	Keyboard Home
	0x4B	Keyboard PageUp
	0x4C	Keyboard Delete Forward
	0x4D	Keyboard End
	0x4E	Keyboard PageDown
	0x4F	Keyboard RightArrow
	0x50	Keyboard LeftArrow
	0x51	Keyboard DownArrow
	0x52	Keyboard UpArrow
	0x53	Keypad Num Lock and Clear
	0x54	Keypad /
	0x55	Keypad *
	0x56	Keypad -
	0x57	Keypad +
	0x58	Keypad ENTER
	0x59	Keypad 1 and End
	0x5A	Keypad 2 and Down Arrow
	0x5B	Keypad 3 and PageDn
	0x5C	Keypad 4 and Left Arrow
	0x5D	Keypad 5
	0x5E	Keypad 6 and Right Arrow
	0x5F	Keypad 7 and Home
	0x60	Keypad 8 and Up Arrow
	0x61	Keypad 9 and PageUp
	0x62	Keypad 0 and Insert
	0x63	Keypad . and Delete
	0x64	Keyboard Non-US \ and |
	0x65	Keyboard Application
	0x66	Keyboard Power
	0x67	Keypad =
	0x68	Keyboard F13
	0x69	Keyboard F14
	0x6A	Keyboard F15
	0x6B	Keyboard F16
	0x6C	Keyboard F17
	0x6D	Keyboard F18
	0x6E	Keyboard F19
	0x6F	Keyboard F20
	0x70	Keyboard F21
	0x71	Keyboard F22
	0x72	Keyboard F23
	0x73	Keyboard F24
	0x74	Keyboard Execute
	0x75	Keyboard Help
	0x76	Keyboard Menu
	0x77	Keyboard Select
	0x78	Keyboard Stop
	0x79	Keyboard Again
	0x7A	Keyboard Undo
	0x7B	Keyboard Cut
	0x7C	Keyboard Copy
	0x7D	Keyboard Paste
	0x7E	Keyboard Find
	0x7F	Keyboard Mute
	0x80	Keyboard Volume Up
	0x81	Keyboard Volume Down
	0x82	Keyboard Locking Caps Lock
	0x83	Keyboard Locking Num Lock
	0x84	Keyboard Locking Scroll Lock
	0x85	Keypad Comma
	0x86	Keypad Equal Sign
	0x87	Keyboard International1
	0x88	Keyboard International2
	0x89	Keyboard International3
	0x8A	Keyboard International4
	0x8B	Keyboard International5
	0x8C	Keyboard International6
	0x8D	Keyboard International7
	0x8E	Keyboard International8
	0x8F	Keyboard International9
	0x90	Keyboard LANG1
	0x91	Keyboard LANG2
	0x92	Keyboard LANG3
	0x93	Keyboard LANG4
	0x94	Keyboard LANG5
	0x95	Keyboard LANG6
	0x96	Keyboard LANG7
	0x97	Keyboard LANG8
	0x98	Keyboard LANG9
	0x99	Keyboard Alternate Erase
	0x9A	Keyboard SysReq/Attention
	0x9B	Keyboard Cancel
	0x9C	Keyboard Clear
	0x9D	Keyboard Prior
	0x9E	Keyboard Return
	0x9F	Keyboard Separator
	0xA0	Keyboard Out
	0xA1	Keyboard Oper
	0xA2	Keyboard Clear/Again
	0xA3	Keyboard CrSel/Props
	0xA4	Keyboard ExSel
	0xE0	Keyboard LeftControl
	0xE1	Keyboard LeftShift
	0xE2	Keyboard LeftAlt
	0xE3	Keyboard Left GUI
	0xE4	Keyboard RightControl
	0xE5	Keyboard RightShift
	0xE6	Keyboard RightAlt
	0xE7	Keyboard Right GUI


*/



#include <avr/pgmspace.h>

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <address.h>
#include <hidboot.h>

#include <printhex.h>
#include <message.h>
#include <hexdump.h>
#include <parsetools.h>

class KbdRptParser : public KeyboardReportParser
{
        void PrintKey(uint8_t mod, uint8_t key);
        
protected:
	virtual void OnKeyDown	(uint8_t mod, uint8_t key);
	virtual void OnKeyUp	(uint8_t mod, uint8_t key);
	virtual void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)	
{
    MODIFIERKEYS mod;
    *((uint8_t*)&mod) = m;
/* 
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
    Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
    Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
    Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");
    
    Serial.print(" >");
    PrintHex<uint8_t>(key);
    Serial.print("< ");

    Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
    Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
    Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
    Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
    
    
    */

// needs to send APPLE keys as key presses, not just as modifiers. 

  if((mod.bmLeftAlt  == 1) || (mod.bmLeftGUI  == 1) ) {
      digitalWrite(OPEN_APPLE_PIN, HIGH);
  } else {
      digitalWrite(OPEN_APPLE_PIN, LOW);
  }


  if((mod.bmRightAlt  == 1) || (mod.bmRightGUI  == 1) ) {
      digitalWrite(CLOSED_APPLE_PIN, HIGH);
  } else {
      digitalWrite(CLOSED_APPLE_PIN, LOW);
  }



  if((mod.bmLeftShift  == 1) || (mod.bmRightShift  == 1) ) {
      digitalWrite(SHIFT_PIN, LOW);
  } else {
      digitalWrite(SHIFT_PIN, HIGH);
  }

  if((mod.bmRightCtrl  == 1) || (mod.bmLeftCtrl  == 1) ) {
      digitalWrite(CONTROL_PIN, LOW);
  } else {
      digitalWrite(CONTROL_PIN, HIGH);
  }

};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)	
{

    digitalWrite(S0_4051, LOW);
    digitalWrite(S1_4051, LOW);
    digitalWrite(S2_4051, LOW);


//  Serial.print("DN ");
    PrintKey(mod, key);
    uint8_t c = OemToAscii(mod, key);
    
    if (c)
        OnKeyPressed(c);
        
        
    
int SEARCH_COLUMN = -1;
int SEARCH_ROW = -1;

for (int row=0; row<ROWS; row++) {
    
    for (int column=0; column<COLUMNS; column++) {
       if (key == KEYS_ARRAY[row][column]) {
         SEARCH_COLUMN = row;
         SEARCH_ROW = column;
         break;
       }
    }        
}

/*
Serial.print( "SEARCH_ROW = " );
 Serial.println(      SEARCH_ROW );
        
 Serial.print( "SEARCH_COLUMN = " );
 Serial.println(      SEARCH_COLUMN );
   
*/

//select the column bits  
    c0 = bitRead(SEARCH_COLUMN,0);        
    c1 = bitRead(SEARCH_COLUMN,1);       
    c2 = bitRead(SEARCH_COLUMN,2);   
    c3 = bitRead(SEARCH_COLUMN,3);     

// set the column signals to match selected column
    digitalWrite(S0_PIN, c0);
    digitalWrite(S1_PIN, c1);
    digitalWrite(S2_PIN, c2);
    digitalWrite(S3_PIN, c3);

//select the row bits  
    r0 = bitRead(SEARCH_ROW,0);        
    r1 = bitRead(SEARCH_ROW,1);       
    r2 = bitRead(SEARCH_ROW,2);   

// set the column signals to match selected column
    digitalWrite(S0_4051, r0);
    digitalWrite(S1_4051, r1);
    digitalWrite(S2_4051, r2);

// enable the common IO
  digitalWrite(ENABLE_PIN, LOW);

     
     
        
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)	
{
  
 // disable the common IO 
  digitalWrite(ENABLE_PIN, HIGH);

//    Serial.print("UP ");
    PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)	
{
//    Serial.print("ASCII: ");
 //   Serial.println((char)key);
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    Keyboard(&Usb);

uint32_t next_time;

KbdRptParser Prs;

void setup()
{
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);
  pinMode(SHIFT_PIN, OUTPUT);
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(OPEN_APPLE_PIN, OUTPUT);
  pinMode(CLOSED_APPLE_PIN, OUTPUT);
  pinMode(CAPS_LOCK_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(S0_4051, OUTPUT);
  pinMode(S1_4051, OUTPUT);
  pinMode(S2_4051, OUTPUT);
  
    
  digitalWrite(ENABLE_PIN, HIGH); // write low to enable, high to disable
  digitalWrite(S0_PIN, LOW);
  digitalWrite(S1_PIN, LOW);
  digitalWrite(S2_PIN, LOW);
  digitalWrite(S3_PIN, LOW);

  digitalWrite(CAPS_LOCK_PIN, HIGH);
  digitalWrite(RESET_PIN, HIGH);
  
  digitalWrite(OPEN_APPLE_PIN, LOW);
  digitalWrite(CLOSED_APPLE_PIN, LOW);

// modifier keys - HIGH == not pressed. LOW == pressed.
digitalWrite(SHIFT_PIN, HIGH);
digitalWrite(CONTROL_PIN, HIGH);



    digitalWrite(S0_4051, LOW);
    digitalWrite(S1_4051, LOW);
    digitalWrite(S2_4051, LOW);


  
//    Serial.begin( 115200 );
//    Serial.println("Start");

    if (Usb.Init() == -1)
//        Serial.println("OSC did not start.");
      
    delay( 200 );
  
    next_time = millis() + 5000;
  
    Keyboard.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
    Usb.Task();
    
   
}

