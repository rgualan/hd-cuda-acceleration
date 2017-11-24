#include<stdio.h>

int main()
{
	int i,j,k;
	float matrix[125];
	int counter = 0;

	for(k=0; k<5; k++){
		for(j=0; j<5; j++){
			for(i=0; i<5; i++){
				//matrix[i+j*5+k*25] = counter;
				matrix[i+j*5+k*25] = j;
				counter++;
			}
		}
	}

	for(k=0; k<5; k++){
		for(j=0; j<5; j++){
			for(i=0; i<5; i++){
				printf("%f\t", matrix[i+j*5+k*25]);
			}
		}
	}
	
	FILE *f = fopen("outc.txt", "wb");
	fwrite(matrix, sizeof(float), sizeof(matrix), f);
	fclose(f);

	return 0;
}
