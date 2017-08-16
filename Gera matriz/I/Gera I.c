#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main()
{
    int N;
    printf("N: ");
    scanf("%d",&N);

    int e;
    e = pow(2,N);

    FILE *fp;
    fp = fopen("I.txt","w");
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

    return 0;
}
