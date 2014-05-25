/*
	Boost converter
	Vin = 5v, Vout = 12v


	Plib:
	
	3 modes:
		Single compare.  OCx pin is low and you can drive it high once in some period of time

		Dual compare (pulse).
			goes high on a OCxR match and off on a OCxR match. You can have a single pulse
			or a continuous pulse
		
		Toggle.  OCx pin output is toggled every period of timer.  You get a square wave.
			Timer period is offtime
		
		PWM mode.  goes high when there's a timer period match.  goes low when OCxR = timerY.
			Goes high when there's another timer period match + loads OCxR from OCxRS to allow
			glitchless operation

		

*/
