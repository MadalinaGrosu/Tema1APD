#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <omp.h>

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

// ./paralel nr_saptamani fsin fsout
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
	int i,j;
	int colors[Nc]; // numarul de senatori din fiecare partid
	initializeArray(colors,Nc,0);
	initializeMatrix(matrix);
	
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
	
	for (week = 1; week <= weeks; week++) 
	{	
		initializeArray(colors,Nc,0);
		//pentru fiecare senator determin noua lui culoare
		#pragma omp parallel for private(i,j) shared(colors,matrixWeekEnd) schedule(runtime)
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
					for (l = 0; l < N; l++)
						for (m = 0; m < N; m++)
						{
							int d = getMax(abs(i-l),abs(j-m));
							k = matrix[l][m];
							if (d != 0 && d <= dist[k])
								dist[k] = d;
						}
					
					for (k = 0; k < Nc; k++)
						if (dist[k] > max && dist[k] != N) 
						{
							max = dist[k];
							c_max = k;
						}
					matrixWeekEnd[i][j] = c_max;
					//actualizez numarul de senatori din partide
					#pragma omp critical 
						colors[c_max]++;	
				}
			// afisez numarul de senatori din fiecare partid
			int p;
			for (p = 0; p < Nc; p++)
				fprintf(fsout, "%d ", colors[p]);
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
