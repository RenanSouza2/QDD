#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
    unsigned short N;
    unsigned long long n;
    printf("N: ");
    scanf("%hu",&N);
    printf("\nn: ");
    scanf("%llu",&n);

    char nome[20];
    nome[0] = '\0';
    sprintf(nome,"D_%hu_%llu.txt",N,n);

    FILE *fp;
    fp = fopen(nome,"w");
    fprintf(fp,"%hu\n",N);

    unsigned long long MAX, i;
    MAX = pow(2,N);
    for(i=0; i<MAX; i++)
    {
        if(i == n)
            fprintf(fp,"1 0 ");
        else
            fprintf(fp,"0 0 ");

    }
    fclose(fp);
}
