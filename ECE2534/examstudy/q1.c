
register int keypadRead(void){
	int rows = PORTB;
	int cols = PORTC;
	int num = -1;
	while (rows >> 1)
		num++;
	while(cols >> 1)
		num += 8;
	return num;
}


