#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define eps 5e-5
#define pi 3.1415926535

int mem = 0, memMax = 0;
FILE *fm;
unsigned short print = 1;


struct QDD
{
    struct no *n;
    struct lista *l;
};

struct no
{
    struct lista *l;
    unsigned short nivel, tipo;
    float re, im;
    struct no *el, *th;
};

struct lista
{
    struct lista *l;
    struct no *n;
};

struct apply
{
    struct no    *n, *n1, *n2;
    struct apply *a, *a1, *a2;
};



typedef struct QDD QDD;

typedef struct no no;

typedef struct lista lista;

typedef struct apply apply;

typedef unsigned short Short;



void aumenta_memoria(int m)
{
    mem += m;
    if(print)
    fprintf(fm,"\nMemUP: %d\t\t\t %d",mem,m);
    if(memMax<mem)
    {
        memMax = mem;
        if(print)
        fprintf(fm,"\t\tMemMax");
    }
}

void diminui_memoria(int m)
{
    mem -= m;
    if(mem<0)
    {
        printf("\n\nERRO MEM");
        exit(EXIT_FAILURE);
    }
    if(print)
    fprintf(fm,"\n\tMemDOWN: %d\t\t%d",mem,-m);
}



QDD* cria_QDD()
{
    QDD *Q;
    Q = malloc(sizeof(QDD));
    if(Q == NULL)
    {
        printf("\n\nERRO QDD");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(sizeof(QDD));
    Q->n = NULL;
    Q->l = NULL;
    return Q;
}

no* cria_no(Short tipo, Short nivel, float re, float im)
{
    no* n;
    n = malloc(sizeof(no));
    if(n == NULL)
    {
        printf("\n\nERRO NO");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(sizeof(no));
    n->l = NULL;
    n->tipo = tipo;
    n->nivel = nivel;
    n->re = re;
    n->im = im;
    n->el = NULL;
    n->th = NULL;
    return n;
}

no* cria_no_vazio()
{
    no* n;
    n = cria_no(0,0,0,0);
    return n;
}

lista* cria_no_lista()
{
    lista *l;
    l = malloc(sizeof(lista));
    if(l == NULL)
    {
        printf("\n\nERRO LISTA");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(sizeof(lista));
    if(l == NULL)
        exit(0);
    l->l = NULL;
    l->n = NULL;
    return l;
}

apply* cria_apply()
{
    apply *a;
    a = malloc(sizeof(apply));

    a->n = NULL;
    a->n1 = NULL;
    a->n2 = NULL;

    a->a = NULL;
    a->a1 = NULL;
    a->a2 = NULL;

    return a;
}



void libera_no_QDD(QDD *Q)
{
    diminui_memoria(sizeof(QDD));
    free(Q);
}

void libera_no(no *n)
{
    diminui_memoria(sizeof(no));
    free(n);
}

void libera_no_lista(lista *l)
{
    diminui_memoria(sizeof(lista));
    free(l);
}

void libera_lista(lista *l)
{
    lista *lc;
    while(l != NULL)
    {
        lc = l->l;
        libera_no_lista(l);
        l = lc;
    }
}



void mostra_lista(lista *l)
{
    lista *lc;
    Short ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        printf("\tLigacao %u: %d\n",ligacao,lc->n);
        ligacao++;
    }
}

void fmostra_lista(FILE *fp, lista *l)
{
    lista *lc;
    Short ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        fprintf(fp,"\tLigacao %u: %d\n",ligacao,lc->n);
        ligacao++;
    }
}

void mostra_no(no *n)
{
    printf("\nEndereco: %d\n",n);
    if(n->l != NULL)
    {
        printf("Ligacoes anteriores:\n");
        mostra_lista(n->l);
    }
    printf("Tipo");
    switch(n->tipo)
    {
        case 0:
        printf(": Numero  %d\n",n->nivel);
        printf("%f %f \n",n->re,n->im);
        break;

        case 1:
        printf("/nivel: V%d\n",n->nivel);
        break;

        case 2:
        printf("/nivel: R%d\n",n->nivel);
        break;

        case 3:
        printf("/nivel: C%d\n",n->nivel);
        break;
    }
    if((n->el != NULL)||(n->th != NULL))
    {
        printf("Ligacoes posteriores\n");
        printf("\telse: %d\n",n->el);
        printf("\tThen: %d\n",n->th);
    }
    printf("\n");
}

void fmostra_no(FILE *fp, no *n)
{
    fprintf(fp,"\nEndereco: %d\n",n);
    if(n->l != NULL)
    {
        fprintf(fp,"Ligacoes anteriores:\n");
        fmostra_lista(fp,n->l);
    }
    fprintf(fp,"Tipo");
    switch(n->tipo)
    {
        case 0:
        fprintf(fp,": Numero\n");
        fprintf(fp,"%f %f \n",n->re,n->im);
        break;

        case 1:
        fprintf(fp,"/nivel: V%d\n",n->nivel);
        break;

        case 2:
        fprintf(fp,"/nivel: R%d\n",n->nivel);
        break;

        case 3:
        fprintf(fp,"/nivel: C%d\n",n->nivel);
        break;
    }
    if((n->el != NULL)||(n->th != NULL))
    {
        fprintf(fp,"Ligacoes posteriores\n");
        fprintf(fp,"\telse: %d\n",n->el);
        fprintf(fp,"\tThen: %d\n",n->th);
    }
}

void mostra_lista_com_no(lista *l)
{
    lista *lc;
    Short ligacao = 0;
    lc = l;
    printf("\n");
    for(lc = l; lc != NULL; lc = lc->l)
    {
        printf("\tLigacao %u: %d\n",ligacao,lc->n);
        mostra_no(lc->n);
        ligacao++;
    }
}

void fmostra_lista_com_no(FILE *fp, lista *l)
{
    lista *lc;
    Short ligacao = 0;
    lc = l;
    fprintf(fp,"\n");
    for(lc = l; lc != NULL; lc = lc->l)
    {
        fprintf(fp,"\n\tLigacao %u: %d\n",ligacao,lc->n);
        fmostra_no(fp,lc->n);
        ligacao++;
    }
}

void mostra_arvore_ineficiente(no *n)
{
    if(n == NULL)
        return;
    mostra_no(n);
    mostra_arvore_ineficiente(n->el);
    mostra_arvore_ineficiente(n->th);
}

lista* enlista_QDD(QDD *Q)
{
    lista *l, *la, *lc, *lf;
    no *n;

    l = cria_no_lista();
    la = cria_no_lista();
    la->n = Q->n;
    lf = l;

    while(la != NULL)
    {
        n = la->n;
        for(lc = l; lc != NULL; lc = lc->l)
            if(lc->n == n)
                break;
        if(lc == NULL)
        {
            lf->l = cria_no_lista();
            lf = lf->l;
            lf->n = n;

            la->n = n->th;
            lc = cria_no_lista();
            lc->n = n->el;
            lc->l = la;
            la = lc;
        }
        else
        {
            lc = la->l;
            libera_no_lista(la);
            la = lc;
        }
    }
    l->n = Q->n->l->n;
    return l;
}

void mostra_QDD(QDD *Q)
{
    lista *l;
    l = enlista_QDD(Q);
    mostra_lista_com_no(l);
    printf("\n");
    mostra_lista(Q->l);
    libera_lista(l);
}

void fmostra_QDD(FILE *fp,QDD *Q)
{

    lista *l;
    l = enlista_QDD(Q);
    fmostra_lista_com_no(fp,l);
    fprintf(fp,"\n");
    fmostra_lista(fp,Q->l);
    libera_lista(l);
}

void fmostra_QDD_sozinho(QDD *Q, char *nome)
{
    FILE *fp;
    fp = fopen(nome,"w");
    fmostra_QDD(fp,Q);
    fclose(fp);
}



void conecta_UM(no *n1, no *n2, Short lado)
{
    lista *l;

    if(lado)
        n1->th = n2;
    else
        n1->el = n2;

    l = cria_no_lista();
    l->n = n1;
    l->l = n2->l;
    n2->l = l;
}

void conecta_DOIS(no *n, no *el, no *th)
{
    conecta_UM(n,el,0);
    conecta_UM(n,th,1);
}

Short desconecta_UM(no *n1, no *n2)
{
    lista *l, *lc, *laux;
    Short lado;
    if(n1->el == n2)
    {
        n1->el = NULL;
        lado = 0;
    }
    else
    {
        n1->th = NULL;
        lado = 1;
    }

    l = cria_no_lista();
    l->l = n2->l;
    for(lc = l; lc->l != NULL; lc = lc->l)
    {
        if(lc->l->n == n1)
        {
            laux = lc->l;
            lc->l = laux->l;
            libera_no_lista(laux);
            break;
        }
    }
    n2->l = l->l;
    libera_no_lista(l);
    return lado;
}

void desconecta_DOIS(no *n)
{
    desconecta_UM(n,n->el);
    desconecta_UM(n,n->th);
}

void transfere_conexao(no *n1, no *n2)
{
    no *n;
    Short lado;
    while(n2->l != NULL)
    {
        n = n2->l->n;
        lado = desconecta_UM(n,n2);
        conecta_UM(n,n1,lado);
    }
}



void libera_QDD(QDD *Q)
{
    lista *l, *lc;
    l = enlista_QDD(Q);
    no *n1, *n2;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        n1 = lc->n;
        while(n1->l != NULL)
        {
            n2 = n1->l->n;
            desconecta_UM(n2,n1);
        }
    }

    for(lc = l; lc != NULL; lc = lc->l)
        libera_no(lc->n);

    libera_lista(l);
    libera_lista(Q->l);

    libera_no_QDD(Q);
}



no* copia_no(no *n1)
{
    no *n2;
    n2 = cria_no(n1->tipo,n1->nivel,n1->re,n1->im);
    return n2;
}

Short compara_no(no *n1, no *n2)
{
    float re, im;
    re = (n1->re)-(n2->re);
    im = (n1->im)-(n2->im);
    if(re<-eps)
        return 0;
    if(re>eps)
        return 0;
    if(im<-eps)
        return 0;
    if(im>eps)
        return 0;
    return 1;
}

Short compara_apply(apply *a1, apply *a2)
{
    if(a1->n1 == a2->n1)
    if(a1->n2 == a2->n2)
        return 1;
    return 0;
}

no* produto_complexo(no *n1, no *n2)
{
    no *n;
    float re, im;
    re = (n1->re)*(n2->re)-(n1->im)*(n2->im);
    im = (n1->re)*(n2->im)+(n1->im)*(n2->re);
    n = cria_no(0,0,re,im);
    return n;
}

no* produto_complexo_conjugado(no *n1, no *n2)
{
    no *n;
    float re, im;
    re = (n1->re)*(n2->re)+(n1->im)*(n2->im);
    im = (n1->re)*(n2->im)-(n1->im)*(n2->re);
    n = cria_no(0,0,re,im);
    return n;
}



lista* copia_lista(lista *l1)
{
    lista *l2, *lc, *lc2;
    l2 = cria_no_lista();
    lc2 = l2;
    for(lc = l1; lc != NULL; lc = lc->l)
    {
        lc2->l = cria_no_lista();
        lc2 = lc2->l;
        lc2->n = lc->n;
    }
    return l2;
}

lista* copia_lista_sem_cabeca(lista *l1)
{
    lista *l2, *laux;
    l2 = copia_lista(l1);
    laux = l2;
    l2 = laux->l;
    libera_no_lista(laux);
    return l2;
}

void reduz_lista(lista *l)
{
    no *n1, *n2;
    lista *lc1, *lc2, *laux;
    lc1 = l;
    for(lc1 = l; lc1->l != NULL; lc1 = lc1->l)
    {
        n1 = lc1->n;
        lc2 = lc1;
        while(lc2->l != NULL)
        {
            n2 = lc2->l->n;
            if(compara_no(n1,n2))
            {
                transfere_conexao(n1,n2);
                laux = lc2->l;
                lc2->l = laux->l;
                libera_no_lista(laux);
                libera_no(n2);
            }
            else
                lc2 = lc2->l;
        }
        if(lc1->l == NULL)
            break;
    }
}



void completa_QDD(no *n, Short r, Short c, Short exp, Short **M, lista **L)
{
    Short ind1, ind2;
    if((n->tipo == 3)&&(exp == 1))
    {
        ind1 = M[r][c];
        ind2 = M[r][c+1];
        conecta_DOIS(n,L[ind1]->n,L[ind2]->n);
    }
    else
    {
        if(n->tipo == 2)
        {
            completa_QDD(n->el,r,c,exp,M,L);
            completa_QDD(n->th,r+exp,c,exp,M,L);
        }
        if(n->tipo == 3)
        {
            completa_QDD(n->el,r,c,exp/2,M,L);
            completa_QDD(n->th,r,c+exp,exp/2,M,L);
        }
    }
}

QDD* le_matriz(char *nome)
{
    Short i, j, k;

    FILE *fp;
    fp = fopen(nome,"r");

    Short N1, N2, N3;
    fscanf(fp,"%hu %hu\n%hu\n",&N1, &N2, &N3);


    lista **L;
    float re, im;
    L = malloc(N3*sizeof(lista*));
    if(L == NULL)
    {
        printf("\n\nERRO L");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(N3*sizeof(lista*));
    for(i=0; i<N3; i++)
    {
        L[i] = cria_no_lista();
        fscanf(fp,"%f %f",&re, &im);
        L[i]->n = cria_no(0,N1,re,im);
    }
    fscanf(fp,"\n");
    for(i=0; i<N3-1; i++)
        L[i]->l = L[i+1];

    Short **M;
    M = malloc(N2*sizeof(Short*));
    if(M == NULL)
    {
        printf("\n\nERRO M");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(N2*sizeof(Short*));
    for(i=0; i<N2; i++)
    {
        M[i] = malloc(N2*sizeof(Short));
        if(M[i] == NULL)
        {
            printf("\n\nERRO M[%d]",i);
            exit(EXIT_FAILURE);
        }
        aumenta_memoria(N2*sizeof(Short));
        for(j=0; j<N2; j++)
            fscanf(fp,"%hu",&M[i][j]);
    }

    no **N;
    N = malloc((N2*N2-1)*sizeof(no*));
    if(N == NULL)
    {
        printf("\n\nERRO N");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria((N2*N2-1)*sizeof(no*));

    Short exp, ind;
    exp = 1;
    ind = 0;
    for(i=0; i<N1; i++)
    {
        for(j=2; j<=3; j++)
        {
            for(k=0; k<exp; k++)
            {
                N[ind] = cria_no(j,i,0,0);
                ind++;
            }
            exp *= 2;
        }
    }

    for(i=0; i<(N2*N2-1)/2; i++)
        conecta_DOIS(N[i],N[2*i+1],N[2*i+2]);

    completa_QDD(N[0],0,0,N2/2,M,L);

    QDD *Q;
    Q = cria_QDD();
    Q->n = cria_no_vazio();
    conecta_UM(Q->n,N[0],1);
    Q->n = N[0];
    Q->l = L[0];

    free(N);
    diminui_memoria((N2*N2-1)*sizeof(no*));
    free(L);
    diminui_memoria(N3*sizeof(lista*));
    for(i=0; i<N2; i++)
    {
        free(M[i]);
        diminui_memoria(N2*sizeof(Short));
    }
    free(M);
    diminui_memoria(N2*sizeof(Short*));
    free(fp);

    return Q;
}

void reduz_QDD(QDD *Q)
{
    no *n1, *n2, *nc;
    lista *l, *laux, *lnc1, *lnc2, *lr, *lrc, *lf;
    Short mudou, saida, del, regra;
    l = copia_lista(Q->l);
    while(l->l != NULL)
    {
        nc = l->l->n;
        del = 1;
        do
        {
            mudou = 0;
            for(regra = 2; regra >= 1; regra--)
            {
                do
                {
                    saida = 0;
                    for(lnc1 = nc->l; lnc1->l != NULL; lnc1 = lnc1->l)
                    {
                        n1 = lnc1->n;
                        for(lnc2 = lnc1->l; lnc2 != NULL; lnc2 = lnc2->l)
                        {
                            n2 = lnc2->n;
                            switch(regra)
                            {
                                case 1:
                                if(n1 == n2)
                                {
                                    desconecta_DOIS(n2);
                                    transfere_conexao(nc,n2);
                                    libera_no(n2);

                                    mudou = 1;
                                    del = 0;
                                    saida = 1;
                                }
                                break;

                                case 2:
                                if(n1        != n2       )
                                if(n1->tipo  == n2->tipo )
                                if(n1->nivel == n2->nivel)
                                if(n1->el    == n2->el   )
                                if(n1->th    == n2->th   )
                                {
                                    lr = cria_no_lista();
                                    lr->n = n1;
                                    lrc = cria_no_lista();
                                    lrc->n = n2;
                                    lr->l = lrc;

                                    while(lnc2->l != NULL)
                                    {
                                        lnc2 = lnc2->l;
                                        n2 = lnc2->n;

                                        if(n1        != n2       )
                                        if(n1->tipo  == n2->tipo )
                                        if(n1->nivel == n2->nivel)
                                        if(n1->el    == n2->el   )
                                        if(n1->th    == n2->th   )
                                        {
                                            if(n1->el == n1->th)
                                            {
                                                for(lrc = lr; lrc->l != NULL; lrc = lrc->l)
                                                    if(lrc->l->n == n2)
                                                        break;
                                                if(lrc->l != NULL)
                                                    continue;
                                            }
                                            lrc->l = cria_no_lista();
                                            lrc = lrc->l;
                                            lrc->n = n2;
                                        }
                                    }

                                    while(lr->l != NULL)
                                    {
                                        lrc = lr->l;
                                        n2 = lrc->n;

                                        desconecta_DOIS(n2);
                                        transfere_conexao(n1,n2);
                                        libera_no(n2);

                                        lr->l = lrc->l;
                                        libera_no_lista(lrc);
                                    }
                                    libera_no_lista(lrc);

                                    if(n1->el != n1->th)
                                    {
                                        for(lf = l; lf->l != NULL; lf = lf->l);
                                        lf->l = cria_no_lista();
                                        lf = lf->l;
                                        lf->n = n1;
                                    }

                                    mudou = 1;
                                    saida = 1;
                                }
                                break;
                            }
                            if(saida)
                                break;
                        }
                        if(saida)
                            break;
                    }
                }
                while(saida);
            }
        }
        while(mudou);

        if(del)
        {
            laux = l->l;
            libera_no_lista(l);
            l = laux;
        }
        else
        {
            laux = l->l;
            l->l = laux->l;
            laux->l = NULL;

            for(lf = l; lf->l != NULL; lf = lf->l);
            lf->l = laux;
        }
    }
    libera_no_lista(l);
}

QDD* copia_QDD(QDD *Q1)
{
    lista *l1, *l2, *lc1a, *lc2a, *lc1b, *lc2b;
    no *n1, *n2, *nf, *nt1, *nt2;

    l1 = enlista_QDD(Q1);
    l2 = copia_lista_sem_cabeca(l1);
    for(lc2a = l2; lc2a != NULL; lc2a = lc2a->l)
        lc2a->n = copia_no(lc2a->n);

    lc1a = l1;
    lc2a = l2;
    do
    {
        n1 = lc1a->n;
        n2 = lc2a->n;

        if(n1->el != NULL)
        {
            nf = n1->el;

            lc1b = l1;
            lc2b = l2;
            do
            {
                nt1 = lc1b->n;
                nt2 = lc2b->n;

                lc1b = lc1b->l;
                lc2b = lc2b->l;
            }
            while(nt1 != nf);

            conecta_UM(n2,nt2,0);
        }

        if(n1->th != NULL)
        {
            nf = n1->th;

            lc1b = l1;
            lc2b = l2;
            do
            {
                nt1 = lc1b->n;
                nt2 = lc2b->n;

                lc1b = lc1b->l;
                lc2b = lc2b->l;
            }
            while(nt1 != nf);

            conecta_UM(n2,nt2,1);
        }

        lc1a = lc1a->l;
        lc2a = lc2a->l;
    }
    while(lc1a != NULL);

    QDD *Q2;
    Q2 = cria_QDD();
    Q2->n = l2->l->n;

    lc2a = l2;
    while(lc2a->l != NULL)
    {
        lc2b = lc2a->l;
        if(lc2b->n->tipo == 0)
            lc2a = lc2b;
        else
        {
            lc2a->l = lc2b->l;
            libera_no_lista(lc2b);
        }
    }

    Q2->l = l2->l;
    libera_no_lista(l2);
    libera_lista(l1);

    return Q2;
}

QDD* produto_tensorial(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    Short nqbit;
    lista *l;

    Q = copia_QDD(Q1);
    reduz_QDD(Q);
    nqbit = Q->l->n->nivel;
    l = Q->l;
    Q->l = NULL;

    QDD *Q2a;
    lista *l2, *lc;
    Q2a = copia_QDD(Q2);
    reduz_QDD(Q2a);
    l2 = enlista_QDD(Q2a);
    for(lc = l2; lc != NULL; lc = lc->l)
        (lc->n->nivel) += nqbit;
    libera_lista(l2);

    nqbit+=Q2->l->n->nivel;

    QDD *Q2b;
    no *n1, *n2, *naux, *n0;
    n0 = cria_no_vazio();
    while(l != NULL)
    {
        n1 = l->n;
        if(compara_no(n0,n1))
        {
            n1->nivel = nqbit;
            l2 = l->l;
            l->l = Q->l;
            Q->l = l;
            l = l2;
        }
        else
        {
            Q2b = copia_QDD(Q2a);
            for(lc = Q2b->l; lc != NULL; lc = lc->l)
            {
                n2 = lc->n;
                naux = produto_complexo(n1,n2);
                naux->nivel = nqbit;
                transfere_conexao(naux,n2);
                lc->n = naux;
                libera_no(n2);
            }

            n2 = Q2b->n;
            naux = Q2b->n->l->n;
            desconecta_UM(naux,n2);
            libera_no(naux);

            transfere_conexao(n2,n1);
            libera_no(n1);

            lc = Q2b->l;
            while(lc->l != NULL)
                lc = lc->l;
            lc->l = Q->l;
            Q->l = Q2b->l;
            libera_no_QDD(Q2b);

            lc = l->l;
            libera_no_lista(l);
            l = lc;
        }
    }

    libera_no(n0);
    libera_QDD(Q2a);

    reduz_lista(Q->l);
    reduz_QDD(Q);

    return Q;
}

QDD* potencia_tensorial(QDD *Q, Short i)
{
    QDD *Qs, *Qaux;
    Short j;

    Qs = copia_QDD(Q);
    for(j=1; j<i; j++)
    {
        Qaux = produto_tensorial(Qs,Q);
        reduz_QDD(Qaux);
        libera_QDD(Qs);
        Qs = Qaux;
    }
    return Qs;
}

void produto_por_escalar(QDD *Q, float re, float im)
{
    no *n;
    n = cria_no(0,0,re,im);
    printf("NO ESCALAR");
    mostra_no(n);

    lista *l;
    no *n1, *naux;
    for(l = Q->l; l != NULL; l = l->l)
    {
        n1 = l->n;
        naux = produto_complexo(n1,n);
        transfere_conexao(naux,n1);
        l->n = naux;
        libera_no(n1);
    }

    libera_no(n);
}


QDD* soma_matriz(QDD *Q1, QDD *Q2)
{
    apply *a, *ac, *a1, *a2;
    a = cria_apply();
    a->n1 = Q1->n;
    a->n2 = Q2->n;
    ac = a;

    no *n, *n1, *n2;
    Short caso;
    while(ac != NULL)
    {
        n1 = ac->n1;
        n2 = ac->n2;
    }
}



QDD* I()
{
    QDD *Q;
    Q = le_matriz("I1.txt");
    reduz_QDD(Q);
    return Q;
}

QDD* I_n(Short i)
{
    QDD *Q1, *Q2;

    Q1 = I();
    Q2 = potencia_tensorial(Q1,i);
    libera_QDD(Q1);

    return Q2;
}

QDD* H()
{
    QDD *Q;
    Q = le_matriz("H1.txt");
    reduz_QDD(Q);
    return Q;
}

QDD* H_n(Short i)
{
    QDD *Q1, *Q2;

    Q1 = H();
    Q2 = potencia_tensorial(Q1,i);
    libera_QDD(Q1);

    return Q2;
}



void inicia_relatorio_memoria(Short i)
{
    print = i;
    if(print)
    fm = fopen("relatorio_memoria.txt","w");
}

void finaliza_relatorio_memoria()
{
    printf("\n\nMemMax  : %d",memMax);
    printf("\nMemFinal: %d",mem);
    if(print)
    {
        fprintf(fm,"\n\nMemMax: %d",memMax);
        fclose(fm);
    }
}



int main()
{
    inicia_relatorio_memoria(0);
    /***********************************/

    QDD *Q;
    Q = le_matriz("QFT8.txt");
    reduz_QDD(Q);
    libera_QDD(Q);

    /***********************************/
    finaliza_relatorio_memoria();
    return 0;
}
