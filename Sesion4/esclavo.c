#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <math.h>
#include <stdbool.h>

bool esPrimo(int n) {
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    int primer_intervalo = atoi(argv[1]);
    int segundo_intervalo = atoi(argv[2]);

    for (int i = primer_intervalo; i <= segundo_intervalo; i++) {
        if (esPrimo(i)) {
            printf("%d ", i);
        }
    }

    return 0;
}

