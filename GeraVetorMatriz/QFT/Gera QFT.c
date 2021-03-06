#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <locale.h>

void QFT(int N)
{
    setlocale(LC_ALL, "Portuguese");

    int e;
    e = (int)pow(2,N);

    float theta;
    theta = 2*3.1415926535897932384626433832795/e;

    FILE *fp;
    char s[20];
    sprintf(s,"QFT%d.txt",N);
    fp = fopen(s,"w");
    fprintf(fp,"%d\n",N);

    float re, im, theta2, amp;
    amp = pow(2,-0.5*N);
    for(int i=0; i<e; i++)
    {
        for(int j=0; j<e; j++)
        {
            theta2 = i*j*theta;
            re = amp*cos(theta2);
            im = amp*sin(theta2);
            fprintf(fp,"%f %f ",re,im);
        }
        fprintf(fp,"\n");
    }
}

int main()
{
    int p;
    printf("0- Unico\n1- todos\n\np: ");
    scanf("%d",&p);

    int N;
    printf("N: ");
    scanf("%d",&N);

    int i;
    if(p == 0)
    {
        QFT(N);
    }
    if(p == 1)
    {
        for(i=1; i<=N; i++)
            QFT(i);
    }


    return 0;
}
