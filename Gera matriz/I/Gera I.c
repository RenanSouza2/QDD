#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <locale.h>

void I(int N)
{
    int e;
    e = pow(2,N);

    FILE *fp;
    char s[20];
    sprintf(s,"I%d.txt",N);
    fp = fopen(s,"w");
    fprintf(fp,"%d\n",N);
    for(int i=0; i<e; i++)
    {
        for(int j=0; j<e; j++)
        {
            if(i == j)
                fprintf(fp,"1 0 ");
            else
                fprintf(fp,"0 0 ");
        }
        fprintf(fp,"\n");
    }
    fclose(fp);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    int p;
    printf("0- Unico\n1- todos\n\np: ");
    scanf("%d",&p);

    int N;
    printf("N: ");
    scanf("%d",&N);

    int i;
    if(p == 0)
    {
        I(N);
    }
    if(p == 1)
    {
        for(i=1; i<=N; i++)
            I(i);
    }

    return 0;
}
