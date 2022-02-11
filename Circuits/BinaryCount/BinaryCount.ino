/* Gabriel Tyler
 * Light up an array of 8 LEDS to count from 0 to 255 in binary
 */

// the pins range from 9 to 2
const int start = 9; 
// wait 50ms before incrementing
const int wait = 50; 

// called in void loop(), loops through each LED and sets it on HIGH (255) or LOW (0)
void SetLeds(int x) 
{  
    // for each LED
    for (int i = 0; i < 8; ++i) 
    {    
        // bit magic
        unsigned int mask = 1 << i;    
        if (mask & x) 
        {      
            // set the corresponding LED on high
            analogWrite(start - i, 255);    
        } 
        else 
        {      
            // set the corresponding LED on low
            analogWrite(start - i, 0);    
        }  
    }
} 

void setup() 
{  
    // start the Serial terminal: press CTRL+SHIFT+M to open
    // sets the bit-rate for the Serial terminal to 9600 bits/second (don't worry about it)
    Serial.begin(9600);
} 

void loop() 
{  
    // the maximum value of an 8-bit unsigned integer is 255, loop from 0 to 255
    // light up the LEDS corresponding to each number
    for (int i = 0; i < 256; ++i) 
    {    
        // light the LEDS to be the binary representation of "i"
        SetLeds(i);    
        // wait how many milliseconds
        delay(wait);  
    }
}
