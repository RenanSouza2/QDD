#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
    FILE *fp;
    fp = fopen("H.txt","r");
    int N, e;
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

    fp = fopen("H.txt","w");
    fprintf(fp,"%d\n",N+1);
    for(i=0; i<2*e; i++)
    {
        for(j=0; j<4*e; j++)
            fprintf(fp,"%f ",m[i][j]);
        fprintf(fp,"\n");
    }
    fclose(fp);

    return 0;
}
