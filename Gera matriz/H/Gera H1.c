#include<stdio.h>
#include<math.h>

int main()
{
    FILE *fp;
    fp = fopen("H.txt","w");
    fprintf(fp,"1\n");

    float r;
    r = pow(2,-0.5);
    fprintf(fp,"%f 0 %f 0\n",r,r);
    fprintf(fp,"%f 0 %f 0",r,-r);
    fclose(fp);
    return 1;
}
