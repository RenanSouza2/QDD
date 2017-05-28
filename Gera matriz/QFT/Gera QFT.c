#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define pi 3.1415926535

int main()
{
    int i, j;
    int N1, N2, N3;

    printf("N1: ");
    scanf("%d",&N1);
    N2 = 1;
    for(i=0; i<N1; i++)
        N2 *= 2;
    N3 = N2;

    FILE *fp;
    fp = fopen("QFT.txt","w");
    fprintf(fp,"%d %d\n%d\n",N1,N2,N3);

    float re, im;
    for(i=0; i<N2; i++)
    {
        re = (cos(2*pi*i/N2))/(sqrt(N2));
        im = (sin(2*pi*i/N2))/(sqrt(N2));
        fprintf(fp,"%f %f ",re,im);
    }

    for(i=0; i<N2; i++)
    {
        fprintf(fp,"\n");
        for(j=0; j<N2; j++)
            fprintf(fp,"%d ",(i*j)%N2);
    }

    return 0;
}
