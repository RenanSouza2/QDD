#include<stdio.h>
#include<stdlib.h>

int main()
{
    FILE *fp;
    fp = fopen("H.txt","r");

    int i, j;

    int N1, N2, N3;
    fscanf(fp,"%d %d\n%d\n",&N1,&N2,&N3);
    float m[4];
    for(i=0; i<4; i++)
    {
        fscanf(fp,"%f ",&m[i]);
        m[i] *= 0.70710678118654752440084436210485;
    }
    unsigned short **M;
    M = malloc(N2*sizeof(unsigned short*));
    for(i=0; i<N2; i++)
    {
        M[i] = malloc(N2*sizeof(unsigned short));
        for(j=0; j<N2; j++)
            fscanf(fp,"%hu",&M[i][j]);
    }
    fclose(fp);

    fp = fopen("H.txt","w");
    fprintf(fp,"%d %d\n%d\n",N1+1,2*N2,N3);
    for(i=0; i<4; i++)
        fprintf(fp,"%f ",m[i]);
    for(i=0; i<N2; i++)
    {
        fprintf(fp,"\n");
        for(j=0; j<N2; j++)
            fprintf(fp,"%hu ",M[i][j]);
        for(j=0; j<N2; j++)
            fprintf(fp,"%hu ",M[i][j]);
    }
    for(i=0; i<N2; i++)
    {
        fprintf(fp,"\n");
        for(j=0; j<N2; j++)
            fprintf(fp,"%hu ",M[i][j]);
        for(j=0; j<N2; j++)
            fprintf(fp,"%hu ",1-M[i][j]);
    }
    fclose(fp);

    return 0;
}
