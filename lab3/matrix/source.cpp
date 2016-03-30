#include <stdio.h>
#include <algorithm>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <sys/socket.h>


const int MAX_N = 1000;


int n;
int res[MAX_N];

void worker(int p) {
	int n, begin, size;
	read(p, &n, sizeof(int));
	read(p, &begin, sizeof(int));
	read(p, &size, sizeof(int));

	int* v = new int[n];
	int* m = new int[n * size];
	int* res = new int[size];
	read(p, v, sizeof(int) * n);
	read(p, m, sizeof(int) * n * size);

    for (int i = 0; i < size; i++) {
    	res[i] = 0;
    	for (int j = 0; j < n; j++) {
    		res[i] += v[j] * m[i * n + j];
    	}
    }

    write(p, &begin, sizeof(int));
    write(p, &size, sizeof(int));
    write(p, res, sizeof(int) * size);

	exit(1);
}

void err(const char * msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("usage: %s k matrix_file res_file\n", argv[0]);
        exit(1);
    }

    int k = atoi(argv[1]);
    if (k < 1) {
        printf("k must be gt 0\n");
        exit(1);
    }

    int sv[k][2];

    for(int i = 0; i < k; i++){
        int* m = sv[i];
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, m) == -1) {
            err("socketpair");
        }
    }

    for (int i = 0; i < k; i++) {
    	fflush(stdout);
    	pid_t pid = fork();
    	if (pid == -1) {
    		err("fork");
    	}
    	if (pid == 0) {
    		worker(sv[i][1]);
    	}
    }

    FILE * f = fopen(argv[2], "r");
       if (f == NULL)
           err("File error");

	int n;
	fscanf(f, "%d", &n);

	int* v = new int[n];
	for (int i = 0; i < n; ++i)
		fscanf(f, "%d", &v[i]);

	int* m = new int[n * n];
    for (int i = 0; i < n; ++i) {

        for (int j = 0; j < n; ++j)
            fscanf(f, "%d", &m[i * n + j]);
    }
    int* res = new int[n];


    int other = n % k;
    int each = n / k;
    int begin = 0;
    for (int i = 0; i < k; i++) {
    	int size = each + ((other--) > 0? 1 : 0);
    	write(sv[i][0], &n, sizeof(int));
    	write(sv[i][0], &begin, sizeof(int));
    	write(sv[i][0], &size, sizeof(int));
    	write(sv[i][0], v, sizeof(int) * n);
    	write(sv[i][0], &m[begin * n], sizeof(int) * size * n);
    	begin += size;
    }

    printf("m x v = \n");
        for (int i = 0; i < n; i++) {
        	for (int j = 0; j < n; j++) {
        		printf("%d ", m[i * n + j]);
        	}
        	printf("  -  %d\n", v[i]);
        }
        printf("\n");

    for (int i = 0; i < k; i++) {
    	int b, s;

        read(sv[i][0], &b, sizeof(int));
        read(sv[i][0], &s, sizeof(int));
        int* r = new int[s];
        read(sv[i][0], r, sizeof(int) * s);

        for (int j = 0; j < s; j++) {
        	res[b + j] = r[j];
        }
    }

	printf("r = ");
	for (int i = 0;i < n; i++)
		printf("%d ", res[i]);
	printf("\n");

	FILE * out = fopen(argv[3], "w");
	if (out == NULL) {
		err("file out err");
	}

	for (int i = 0; i < n; i++)
		fprintf(out, "%d\n", res[i]);
    return 0;
}
