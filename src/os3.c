#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <malloc.h>

pthread_mutex_t mutex;

typedef struct {
	int **matrix;
	int size;
} T_MS;


void* determinant(void *npt) {
	T_MS* tmp = (T_MS*) npt;
	int i,j;
	double det = 0;
	pthread_t *array = malloc(sizeof(pthread_t) * tmp->size);
	T_MS   *mtarr = malloc(sizeof(T_MS)   * tmp->size);

	if (tmp->size == 1) {
		det = tmp->matrix[0][0];
	} else if (tmp->size == 2) {
		det = tmp->matrix[0][0] * tmp->matrix[1][1] - tmp->matrix[0][1] * tmp->matrix[1][0];
	} else {

		for (i = 0; i < tmp->size; ++i) {
			mtarr[i].matrix = (int **)malloc(sizeof(int *) * tmp->size);
			mtarr[i].size = tmp->size - 1;

			for (j = 0; j < tmp->size - 1; ++j) {
				if (j < i)
					mtarr[i].matrix[j] = tmp->matrix[j];
				else
					mtarr[i].matrix[j] = tmp->matrix[j + 1];
			}

			pthread_create(&array[i], NULL, determinant, mtarr + i);

		}

		for (i = 0; i < tmp->size; ++i) {
			void *res;
			for (j = 0; j < tmp->size - 1; ++j) {
			}
			pthread_join(array[i], &res);
			double x = *(double *)&res;
			det += (-1 + 2 * !(i % 2)) * x * tmp->matrix[i][tmp->size - 1];


			double answer = *(double*)&det;
			free(mtarr[i].matrix);
		}


	}
	free(mtarr);
	free(array);

	void* ans = *(void **)&det;
	return ans;
}


int main(int argc, char const *argv[]) {
	srand(time(NULL));
	int **matrix;
	int n = 0;
	int a;

	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	printf("Insert the demention of matrix:\n");
	scanf("%d", &n);

	matrix = (int**)malloc(n * sizeof(int*));

	for (int i=0; i<n; ++i)
		matrix[i] = (int*)malloc(n * sizeof(int));


	printf("Insert matrix:\n");


	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			matrix[i][j]=0+rand()%5-1+1;
			//matrix[i][j] = i;
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}


	T_MS* npt = (T_MS*)malloc(sizeof(T_MS));
	npt->matrix = matrix;
	npt->size = n;

	void *det;
	pthread_mutex_init(&mutex, NULL);
	pthread_create(&tid, NULL, determinant, npt);
	pthread_join(tid, &det);

	double answer = *(double*)&det;
	printf("Det is: %f\n", answer);

	for (int i = 0; i < n; ++i)
		free(matrix[i]);
	free(matrix);
	free(npt);

	return 0;
}
