__kernel void matvec_mult(__global char* buf){
	int i = get_global_id(0)*10;
	buf[i  ] = 'H';
	buf[i+1] = 'E';
	buf[i+2] = 'L';
	buf[i+3] = 'L';
	buf[i+4] = 'O';
	buf[i+5] = 'W';
	buf[i+6] = 'O';
	buf[i+7] = 'R';
	buf[i+8] = 'L';
	buf[i+9] = 'D';
}
