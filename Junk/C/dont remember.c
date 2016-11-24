

int main ()
{
	unsigned long long x = 1;
	unsigned long long y[8];
	
		bool j = false;
	do {
		x++;
		
		y[0] = (93LL                  % x - 1);
		y[1] = (23901LL               % x - 2);
		y[2] = (6118749LL             % x - 3);
		y[3] = (1566399837LL          % x - 4);
		y[4] = (400998358365LL        % x - 5);
		y[5] = (102655579741533LL     % x - 6);
		y[6] = (26279828413832541LL   % x - 7);
		y[7] = (6727636073941130589LL % x - 8);
		j = (y[0] == 0);
		for(int i = 1; i != 8; i++) {
			j = j && (y[i] == 0);
		}
	} while(!j);


	return x;
}