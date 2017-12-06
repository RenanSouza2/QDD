#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <locale.h>

void H(int N)
{
    FILE *fp, *fp1;
    if(N == 1)
    {
        fp = fopen("H.txt","w");
        fp1 = fopen("H1.txt","w");
        fprintf(fp,"1\n");
        fprintf(fp1,"1\n");

        float r;
        r = pow(2,-0.5);
        fprintf(fp,"%f 0 %f 0\n",r,r);
        fprintf(fp,"%f 0 %f 0",r,-r);
        fclose(fp);
        fprintf(fp1,"%f 0 %f 0\n",r,r);
        fprintf(fp1,"%f 0 %f 0",r,-r);
        fclose(fp1);
        return;
    }

    fp = fopen("H.txt","r");
    if(fp == NULL)
    {
        printf("\n\nERRO| NAO ABRIU H.TXT");
        exit(EXIT_FAILURE);
    }

    int e;
    fscanf(fp,"%d",&N);
    e = (int)pow(2,N);

    int i, j;
    float **m;
    m = malloc(2*e*sizeof(float*));
    for(i=0; i<2*e; i++)
        m[i] = malloc(4*e*sizeof(float));

    float p;
    p = pow(2,-0.5);
    for(i=0; i<e; i++)
    {
        for(j=0; j<2*e; j++)
        {
            fscanf(fp,"%f",&m[i][j]);
            m[i][j] *= p;

            m[i][j+2*e]   =  m[i][j];
            m[i+e][j]     =  m[i][j];
            m[i+e][j+2*e] = -m[i][j];
        }
    }
    fclose(fp);


    char nome[20];
    nome[0] = '\0';
    sprintf(nome,"H%d.txt",N+1);

    fp = fopen("H.txt","w");
    fp1 = fopen(nome,"w");
    fprintf(fp,"%d\n",N+1);
    fprintf(fp1,"%d\n",N+1);
    for(i=0; i<2*e; i++)
    {
        for(j=0; j<4*e; j++)
        {
            fprintf(fp,"%f ",m[i][j]);
            fprintf(fp1,"%f ",m[i][j]);
        }
        fprintf(fp,"\n");
        fprintf(fp1,"\n");
    }
    fclose(fp);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    int N;
    printf("N: ");
    scanf("%d",&N);

    int i;
    for(i=1; i<=N; i++)
        H(i);

    return 0;
}
