#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX 32000

int N;

// determina maximul a doua numere
int getMax(int a, int b) 
{
	return a >= b ? a : b;
}

void initializeMatrix(int matrix[N][N])
{
	int i,j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			matrix[i][j] = 0;
}

void initializeArray(int *v, int length, int value) 
{
	int i;
	for (i = 0; i < length; i++)
		v[i] = value;
}

// copiaza matricea sursa in matricea destinatie
void copyMatrix(int dest[N][N], int source[N][N]) 
{
	int i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			dest[i][j] = source[i][j];
}

void copyArray(int *dest, int *source, int length)
{
	int i;
	for (i = 0; i < length; i++)
		dest[i] = source[i];
}

// ./serial nr_saptamani fsin fsout
int main(int argc, char **argv)
{
	int Nc, weeks; 
	FILE *fsin, *fsout;
	
	fsin = fopen(argv[2], "r");
	if (!fsin)
	{
		printf("Eroare la deschiderea fisierului de intrare!\n");
		return -1;
	}
	
	fscanf(fsin, "%i\n%i", &N, &Nc);
	
	int matrix[N][N];
	initializeMatrix(matrix);
	int i,j;
	int colors[Nc]; // numarul de senatori din fiecare partid
	initializeArray(colors,Nc,0);
	
	for (i = 0; i < N; i++) 
		for (j = 0; j < N; j++)
		{
			fscanf(fsin, "%i ", &matrix[i][j]);
			colors[matrix[i][j]]++;
		}
	fclose(fsin);
	

	
	fsout = fopen(argv[3], "w");
	if (!fsout) 
	{
		printf("Eroare la deschiderea fisierului de iesire\n!");
		return -1;
	}
	
	int week;
	weeks = atoi(argv[1]);
	int matrixWeekEnd[N][N]; // matricea de la sfarsitul saptamanii
	initializeMatrix(matrixWeekEnd);
	int current = Nc;
	
	for (week = 1; week <= weeks; week++) {
		
	//	copyArray(colorsCopy,colors,Nc);
		initializeArray(colors,Nc,0);
		//pentru fiecare senator determin noua lui culoare
		for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
			{
				int k, l, m;
				int max = -1;
				int dist[Nc]; //vector de distante minime
				int c_max;
				
				initializeArray(dist,Nc, N);
				//pentru fiecare culoare determin distanta minima
				// de la senatorul (i,j) la un senator de culoare k
				int radius = 1; //raza
				int nr = 0; //distantele descoperite
				
				while (nr != current && radius != N)
				{
					int down,up,left,right;
					down = i - radius;
					up = i + radius;
					left = j - radius;
					right = j + radius;
					if (down < 0)
						down = 0;
					if (up > N - 1)
						up = N - 1;
					if (left < 0)
						left = 0;
					if (right > N - 1)
						right = N - 1;
					
					for (l = left; l <= right; l++)
					{
						k = matrix[down][l];
						int d = getMax(abs(i-down),abs(j-l));
						if (dist[k] == N && d != 0) 
						{
							dist[k] = d;
							nr++;
						}
						k = matrix[up][l];
						d = getMax(abs(i-up),abs(j-l));
						if (dist[k] == N && d != 0) 
						{
							dist[k] = d;
							nr++;
						}
					}
					for (m = down; m <= up; m++)
					{
						k = matrix[m][left];
						int d = getMax(abs(i-m),abs(j-left));
						if (dist[k] == N && d != 0) 
						{
							dist[k] = d;
							nr++;
						}
						k = matrix[m][right];
						d = getMax(abs(i-m),abs(j-right));
						if (dist[k] == N && d != 0) 
						{
							dist[k] = d;
							nr++;
						}	
					}
					radius++;
				}
				
				for (k = 0; k < Nc; k++)
					if (dist[k] > max && dist[k] != N) 
					{
						max = dist[k];
						c_max = k;
					}
				matrixWeekEnd[i][j] = c_max;
				//actualizez numarul de senatori din partide
				colors[c_max]++;	
			}
			// afisez numarul de senatori din fiecare partid
			int p;
			current = 0;
			for (p = 0; p < Nc; p++)
			{
				fprintf(fsout, "%d ", colors[p]);
				if (colors[p] != 0)
					current++;
			}
			fprintf(fsout, "\n");
			// actualizez configuratia pentru urmatoarea saptamana
			copyMatrix(matrix,matrixWeekEnd);
	}
	
	for (i = 0; i < N; i++) 
	{
		for (j = 0; j < N; j++)
			fprintf(fsout, "%d ", matrixWeekEnd[i][j]);
		fprintf(fsout, "\n");
	}
	
	fclose(fsout);
			
	return 0;
}
