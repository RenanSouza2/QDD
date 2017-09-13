#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define pi 3.1415926535

int main()
{
    int N;
    printf("N: ");
    scanf("%d",&N);

    int e;
    e = (int)pow(2,N);

    float theta;
    theta = 2*pi/e;

    FILE *fp;
    char s[20];
    float theta2, re, im, amp;
    sprintf(s,"V%d.txt",N);
    fp = fopen(s,"w");
    fprintf(fp,"%d\n",N);
    amp = pow(2,-0.5*N);
    for(int i=0; i<e; i++)
    {
        theta2 = i*theta;
        re = amp*cos(theta2);
        im = amp*sin(theta2);
        fprintf(fp,"%f %f ",re,im);
    }
    fclose(fp);
    return 0;
}
