#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define pi 3.1415926535

int main()
{
    FILE *fp;
    fp = fopen("V.txt","w");
    long N1, N2, i;
    printf("N1: ");
    scanf("%d",&N1);
    N2 = 1;
    for(i=0; i<N1; i++)
        N2 *= 2;
    fprintf(fp,"%d %d\n%d\n",N1,N2,N2);
    float re, im, amp;
    amp = pow(2,-(N1/(2.0)));
    for(i=0; i<N2; i++)
    {
        im = 2*pi*i/N2;

        re = amp*cos(im);
        im = amp*sin(im);
        fprintf(fp,"%f %f ",re,im);
    }
    fprintf(fp,"\n");
    for(i=0; i<N2; i++)
        fprintf(fp,"%d ",i);
}
