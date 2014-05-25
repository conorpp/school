/*
    Analog to digital converters

    V_ref high , V_ref low 
    analog in
    digital output
    
    D = (Vin-Vref_low) / (Vref_high - Vref_low)  *  2^n
      = % of full scale
    span = Vref_high - Vref_low
    resolution = span/2^n = voltage associated with a 1-bit change in D
    
    # of resolution steps
        Vin = Vref_low + D*resolution
        
    weighted sum
        Vin = Vref_low + [MSB (signed)]span/2 + [MSB-1]span/4 + ... [LSB]span/2^n
    
    Fraction representing % of full scale (left justified)
        0110
        assume it's like 0.0110 = 2^-n
        D = %full_scale * 2^n
        
    e.g.
        3 bit converter
        Vref_low = 0, Vref_high=2v
        so resolution = 1/4 = 2/2^3
            the amount of voltage needed to change for a bit to changed
        ________________
        | D   |  Vin    |
        -----------------
        |000  |   0     |
        |001  |   .25   |
        |010  |   .5    |
        |011  |   .75   |
        |100  |   1     |
        |101  |   1.25  |
        |110  |   1.5   |
        |111  |   1.75  |
        |111  |   2     |
        ----------------|

        
    To start sampling:
    
        Set SAMP bit
        ASAM options - starts sampling as soon as last conversion is done
    
    Four conversion start options
        1. Clear the SAMP bit to                end sampling and start conversion
        2. Timer 3 periodocially sample & convert           "
        3. Active transition on the INT0 pin                "
        4. Internal counter counts down to zero             "
    
    You have to tell a pin if it's A or D
        AD1PCFG register
            select whether pins AN- to AN1S are digital (1) or analog (0)
            will be analog by default because it has a safer range.

    T_AD (tad) - period of the A to D
            
    HW:
        ISR - read AD and put in global var
        Main - setup timer to initiate the ADC every 10 ms
                the ADC triggers an interrupt
        Loop - display voltage
        

*/


OpenADC10();
SetChanADC10();
ConfigIntADC10();



