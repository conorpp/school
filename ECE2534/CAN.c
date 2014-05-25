
/*
CAN Bus
	Controller area network
	serial protocol
	speeds to 1Mbit
	originated with automotive applications
		Connects sensors to ECUs
		reduce wiring to windows, etc.
	multi-master with prioritization, error checking, and fault isolation

	probability of undetected corrupted messages is < (message error rate) * 4.7e-11

	key idea:
		bus can have two states:
			dominant
			recessive
		if one tx send d and the other sends r,
			the d wins always.
	differential bus
		0=dominant
		1=recessive
		CANH (CAN high)
		CANL (CAN Low)
		bus uses two wires CANH, CANL
			if both are same than it's a 1 (recessive)
			if they are different 0 (dominant)

		CPU ->  CAN controller -> UART -> CAN transceiver->CANH/CANL->CAN transceiver->CAN controller-> CPM

			the transcievers can handle rough environments with spikes and w/e so that's why
	Async

	Dataline
		Start of Frame (SOF) -> Arbitration field-> control field -> data field-> CRC field-> Ack field
			-> End of Frame (EOF) -> InterFrame Space

		SOF - single 'd' bit
			for syncronization
			can only start transmitting when bus is idle

		ArbitrationField - 11 bit message identifier
				   1 bit RTR field
			CAN nodes do not have ID's - messages do.
			RTR - d = data 
			      r = request for data
		Control field
			r1 r0                       |x x x x 
			reserved bits sent as 'd'   |# of data bytes in message 0-8
		CRC (cyclic redundancy code)
			15 bits long - based on all previous bits sent
			detects any odd number of errors up to 5 randomly distributed errors
			burst error of length < 15 (??)
			there's one reserved bit at the end (to make 16 bits)
		ACK slot
			xmtr sends R
			and receives D if message correctly validly received			
		EOF
			7 R bits
		Intermission
			3 R bits
		Interframe
			any number of R bits (anybody can transmit now)

	Bit stuffing
		frame segments from SOF to CRC are bit stuffed
		every 5 consecutive bits are appended with a complementary bit
		to help sync on the CAN

	Error Handling (5 types)
		bit error - xmtr sees a bus value different from the value its transmitting
		stuff error - if you have 6 of the same bits
		CRC - CRC received and calculated are different
		form error - fixed field is recieved wrong
		ack error - xmtr does not see a D in the ack slot
		

	Node 1
		d d d r -> gets off
	Node 2 
		d d d d -> keeps going
	Bus
		d d d d

	lowest numbered messages have priority

	if a node detects an error it sends an error flag
	of 6 D bits even if only one node sees an error 
	if will cause all other nodes to see a stuff error (globalization of error)

	if only one saw error, that will cause other nodes to throw another error frame
	causing a superposition of error frames
	
	so error frame is delimited by 8 R bits
	and followed by 3 interframe bits (R)

	Error incrementing rules (abridged version):
	
		Rx err0r: REC++		(receive error counter reg)
		Tx err0r: TEC++
		
		Rx detects D bit as first bit after error flag: REX+=8

		Tx error += 8

		Tx | Rx success as Xx : Xx--
 
	Error states
		Error active - transmit active (dominant) error flag
		Error passive - transmit passive (recessive) error flag
			8 'r' bits beyound intermission before initiating transmission
		Bus Off - disconnected

	if REC > 127 or TEC > 127
		error active -> error passive
	if TEC <= 127 and REC <= 127
		error passive -> error active
	if TEC > 255
		Bus off
		(only software reset can reconnect.)

	CAN bit timing
		bit time is divided into 4 segments
		bit -> (--sync--prop--phase1--phase2--)
					  ^^^sample
			sync is 1 T_Q
			prop is 1-8 T_Q
			phase1 is 1-8 T_Q
			phase2 is ?
		CAN clock is derived from system clock by a prescaler
		CAN clock period = T_Q = time quantum


E.g.
	Find the prescale, Tq per segment, RJW to meet the spec
		[ RJW is resync jump width ]
		sysclock = 42.5 MHz
		Bitrate = 500kbps
		Tcur delay (Rx + Tx) = 235 ns
		Bus length 5m
		prop delay 5ns/m
		the prescale can go 1-64
	Solution>>
	Period of bit, Tbit = 2us = 1/bitrate = (1/sysclock) * Prescale * N
		85 = Prescale*N
		5 * 17 = prescale*N
			N = number of time quantum per bit
		time quanta , tq = (2/17) us = 0.118

		trip time = 2(235 + 5*5) ns [multiply by two for both ways] = 0.52 us	
		

		segments: [we have 17 time quanta]
			S = 1 tq
			prop = 5 + 1
				prop >= trip time >= 5 tq
			ps1 = 5
			ps2 = 5
				ps1 = mas(Info processing time, PS1)
	
		RJW = min(4, PS1)
		
		RJW = 4


The CAN transciever writes tx/rx to a protocol engine which handles the rx and tx 
FIFO buffers for the microcontroller so it's easy for microcontroller

	can have up to 32 fifos
	reside in RAM and requires energy
	must be either Rx or Tx
	a Rx FIFO is associated with an acceptance filter
		(0-31) which specifies the messages ID that 
		will be accepted
	Each FIFO can have it's own message ID it's associated with
	You can filter by the message ID and set the filter mask






*/






int main(){

	return 0;
}
























