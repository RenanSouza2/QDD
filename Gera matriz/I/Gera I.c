#include<stdio.h>
#include<stdlib.h>

int main()
{
    int i, j;
    int N1, N2, N3;

    printf("N1: ");
    scanf("%d",&N1);
    N2 = 1;
    for(i=0; i<N1; i++)
        N2 *= 2;
    N3 = 2;

    FILE *fp;
    fp = fopen("I.txt","w");
    fprintf(fp,"%d %d\n%d\n0 0 1 0",N1,N2,N3);
    for(i=0; i<N2; i++)
    {
        fprintf(fp,"\n");
        for(j=0; j<N2; j++)
        {
            if(i==j)
                fprintf(fp,"1 ");
            else
                fprintf(fp,"0 ");
        }
    }

    return 0;
}
