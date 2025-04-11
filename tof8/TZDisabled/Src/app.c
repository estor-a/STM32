#include "main.h"

// JOYSTICK MANAGEMENT




const IO_Pad_t UserKeyPin_ANALOG =      { PD_11, { .Mode = IO_ANALOG, .Pull = IO_NOPULL, .Speed = IO_SPEED_01 } }; // as digital push-in button, can also be used as analog keyboard...
const IO_Pad_t UserKeyPin_EXTI =        { PD_11, { .Mode = IO_INPUT, .Pull = IO_NOPULL, .Speed = IO_SPEED_01 } }; // as digital push-in button, can also be used as analog keyboard...

UserKey_t UserKey = {
  .pPin = (IO_Pad_t*)&UserKeyPin_ANALOG,
  .Pressed = 0,
  .PressedCount_50ms = 0,
};


void Joy_Init(void);
void Joy_Init(void)
{
  
}




const uint16_t rock_lsbs[] = { 0x46, 0x38B, 0x6ab, 0xa0c, 0xd28, 0xfff }; // ADC 12 bit modes right aligned
const uint16_t rock_limits[] = { (0+0x38B)/2, (0x38B+0x6ab)/2, (0x6ab+0xa0c)/2, (0xa0c+0xd28)/2, (0xd28+0xfff)/2};
//old enum { ROCK_SEL, ROCK_UP, ROCK_LEFT, ROCK_RIGHT, ROCK_DOWN, ROCK_UNPRESSED,  } rock_keys_t;
//typedef enum { ROCK_SEL, ROCK_LEFT, ROCK_DOWN, ROCK_RIGHT, ROCK_UP, ROCK_UNPRESSED } rock_keys_t; // keysequence in increasing voltage

void ActivateRockSwitch(void) {

	//IO_PadInit((IO_Pad_t*)&UserRockSwitchAnalog); // this should be done after any PA4 GPIO configuration, or in the main loop
}

uint8_t RockGetKey(void) {

	uint16_t lsb = Analog.ADCs[1].Normal_Lsb[11];
	uint8_t i;
	for(i = 0; i<countof(rock_limits); i++) {
		if(lsb<=rock_limits[i])
			return i;
	}

	return ROCK_UNPRESSED;
}


void Joy_50ms(void)
{
  UserKey.codebefore = UserKey.codenow;
  UserKey.codenow = RockGetKey();//==ROCK_SEL)
    
  //-------------------  
  if(UserKey.codebefore == UserKey.codenow) {
    UserKey.PressedCount_50ms = min2(UserKey.PressedCount_50ms + 1,200);
    if(UserKey.PressedCount_50ms == 4)
      UserKey.Pressed |= 1;
    else
      if(UserKey.PressedCount_50ms == 100)
        UserKey.Pressed_5sec = 1;
    
  }else{
    UserKey.PressedCount_50ms = 0;
  }
}








