#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define Inicio 0
#define Meio 1
#define Fim 2

#define V 0
#define R 1
#define C 2

#define El 1
#define Th 2



FILE *fm;
unsigned long mem = 0, memMax = 0, iQ = 0, ii = 0, iM = 0, ifi = 0, il = 0, ia = 0, ic = 0, is = 0;
unsigned short print, nqbit;
float eps;



/** Structs  **/

struct QDD
{
    struct no *n;
    unsigned short nqbit;
    struct lista *l;
};

struct no
{
    unsigned short tipo;
    struct lista *l;
    union atributos
    {
        struct inicio
        {
            struct no *n;
        }i;

        struct meio
        {
            unsigned short classe, nivel;
            struct no *el, *th;
        }m;

        struct fim
        {
            float re, im;
        }f;
    }at;
};

struct lista
{
    struct lista *l;
    struct no *n;
};



/** Typedefs e definitions  **/

typedef struct QDD   QDD;
typedef struct no    no;
typedef struct lista lista;

typedef unsigned short Short;
typedef unsigned long Long;



/** ERRO**/

void ERRO(char *s)
{
    printf("\n\nERRO %s",s);
    if(print)
        fprintf(fm,"\n\nERRO %s",s);
    exit(EXIT_FAILURE);
}



/** Memoria  **/

void aumenta_memoria(Long m)
{
    mem += m;
    if(print)
    fprintf(fm,"\nMemUP: %d\t\t\t %u",mem,m);
    if(memMax<mem)
    {
        memMax = mem;
        if(print)
        fprintf(fm,"\t\tMemMax");
    }
}

void diminui_memoria(Long m)
{
    if(m<=mem)
        mem -= m;
    if(print)
        fprintf(fm,"\n\tMemDOWN: %d\t\t%u",mem,-m);
}



/** Construtores  **/

QDD* cria_QDD(Short nqbit)
{
    QDD *Q;
    Q = malloc(sizeof(QDD));
    if(Q == NULL)
        ERRO("QDD");
    aumenta_memoria(sizeof(QDD));
    iQ++;

    Q->n = NULL;
    Q->nqbit = nqbit;
    Q->l = NULL;
    return Q;
}

no* cria_no_inicio()
{
    no *n;
    n = malloc(sizeof(no));
    if(n == NULL)
        ERRO("INICIO");
    aumenta_memoria(sizeof(no));
    ii++;

    n->tipo = Inicio;
    n->l = NULL;

    n->at.i.n = NULL;

    return n;
}

no* cria_no_meio(Short classe, Short nivel)
{
    no *n;
    n = malloc(sizeof(no));
    if(n == NULL)
        ERRO("MEIO");
    aumenta_memoria(sizeof(no));
    iM++;

    n->tipo = Meio;
    n->l = NULL;

    n->at.m.classe = classe;
    n->at.m.nivel = nivel;
    n->at.m.el = NULL;
    n->at.m.th = NULL;

    return n;
}

no* cria_no_fim(float re,float im)
{
    no *n;
    n = malloc(sizeof(no));
    if(n == NULL)
        ERRO("FIM");
    aumenta_memoria(sizeof(no));
    ifi++;

    n->tipo = Fim;
    n->l = NULL;

    n->at.f.re = re;
    n->at.f.im = im;

    return n;
}

lista* cria_lista()
{
    lista *l;
    l = malloc(sizeof(lista));
    if(l == NULL)
        ERRO("LISTA");
    aumenta_memoria(sizeof(lista));
    il++;

    l->l = NULL;
    l->n = NULL;
    return l;
}



/** Destrutores  **/

void libera_no_QDD(QDD *Q)
{
    diminui_memoria(sizeof(QDD));
    if(iQ == 0)
        ERRO("LIBERA QDD");
    iQ--;
    free(Q);
}

void libera_no(no *n)
{
    diminui_memoria(sizeof(no));
    switch(n->tipo)
    {
        case Inicio:
        if(ii == 0)
            ERRO("LIBERA INICIO");
        ii--;
        break;

        case Meio:
        if(iM == 0)
            ERRO("LIBERA MEIO");
        iM--;
        break;

        case Fim:
        if(ifi == 0)
            ERRO("LIBERA FIM");
        ifi--;
        break;
    }
    free(n);
}

void libera_lista_no(lista *l)
{
    diminui_memoria(sizeof(lista));
    if(il == 0)
        ERRO("LIBERA LISTA");
    il--;
    free(l);
}

void libera_lista_lista(lista *l)
{
    lista *lc;
    while(l != NULL)
    {
        lc = l->l;
        libera_lista_no(l);
        l = lc;
    }
}



/** Enlistadores  **/

lista* enlista_arvore(no *n)
{
    lista *la, *l, *laux;

    l = cria_lista();
    la = cria_lista();
    la->n = n;

    while(la != NULL)
    {
        n = la->n;
        for(laux = l; laux->l != NULL; laux = laux->l)
            if(laux->l->n == n)
                break;
        if(laux->l == NULL)
        {
            laux->l = cria_lista();
            laux = laux->l;
            laux->n = n;
            switch(n->tipo)
            {
                case Inicio:
                    la->n = n->at.i.n;
                    break;

                case Meio:
                    la->n = n->at.m.th;

                    laux = cria_lista();
                    laux->n = n->at.m.el;
                    laux->l = la;
                    la = laux;
                    break;
            }
        }
        else
        {
            laux = la->l;
            libera_lista_no(la);
            la = laux;
        }
    }

    laux = l->l;
    libera_lista_no(l);
    return laux;
}

lista* enlista_QDD(QDD *Q)
{
    lista *l;
    l = enlista_arvore(Q->n);
    return l;
}


/** Mostra  **/

void mostra_lista(lista *l)
{
    lista *lc;
    Long ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        printf("\tLigacao %u: %d\n",ligacao,lc->n);
        ligacao++;
    }
}

void mostra_no(no *n)
{
    printf("\nEndereco (no): %d\n",n);
    if(n == NULL)
        return;
    if(n->l != NULL)
    {
        printf("Ligacoes anteriores:\n");
        mostra_lista(n->l);
    }
    printf("Tipo");
    switch(n->tipo)
    {
        case Inicio:
        printf(": Inicio\n");
        printf("Ligacoes posteriores\n");
        printf("\tn: %d\n",n->at.i.n);
        break;

        case Meio:
        printf("/nivel: ");
        switch(n->at.m.classe)
        {
            case V:
            printf("V");
            break;

            case R:
            printf("R");
            break;

            case C:
            printf("C");
            break;
        }

        printf("%d\n",n->at.m.nivel);
        printf("Ligacoes posteriores\n");
        printf("\telse: %d\n",n->at.m.el);
        printf("\tThen: %d",n->at.m.th);
        break;

        case Fim:
        printf(": Numero\n");
        printf("%f %f",n->at.f.re,n->at.f.im);
        break;
    }
    printf("\n");
}

void mostra_lista_com_no(lista *l)
{
    lista *lc;
    Long ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        printf("\n\n\tLigacao %u:",ligacao);
        mostra_no(lc->n);
        ligacao++;
    }
    printf("\n");
}

void mostra_arvore(no *n)
{
    lista *l;
    l = enlista_arvore(n);
    mostra_lista_com_no(l);
    libera_lista_lista(l);
    printf("\n");
}

void mostra_QDD(QDD *Q)
{
    lista *l;
    l = enlista_QDD(Q);

    printf("\nEndereco (QDD): %d",Q);
    printf("\nNQBIT: %d\n",Q->nqbit);
    mostra_lista_com_no(l);
    printf("\n\nAMPLITUDES");
    mostra_lista_com_no(Q->l);
    libera_lista_lista(l);
    printf("\n");
}

void mostra_quantidades()
{
    Short vazio = 1;
    if(mem != 0)
    {
        vazio = 0;
        printf("\nMem: %d",mem);
    }
    if(iQ != 0)
    {
        vazio = 0;
        printf("\nQDD: %d",iQ);
    }
    if(ii != 0)
    {
        vazio = 0;
        printf("\ni:   %d",ii);
    }
    if(iM != 0)
    {
        vazio = 0;
        printf("\nm:   %d",iM);
    }
    if(ifi != 0)
    {
        vazio = 0;
        printf("\nf:   %d",ifi);
    }
    if(il != 0)
    {
        vazio = 0;
        printf("\nl:   %d",il);
    }
    if(ia != 0)
    {
        vazio = 0;
        printf("\na:   %d",ia);
    }
    if(ic != 0)
    {
        vazio = 0;
        printf("\nc:   %d",ic);
    }
    if(vazio)
        printf("\nTUDO ZERADO");
    printf("\n");
}

void mostra_tamanhos()
{
    printf("\nTAMANHOS");
    printf("\nQDD: %d",sizeof(QDD));
    printf("\nn:   %d",sizeof(no));
    printf("\nl:   %d",sizeof(lista));
    printf("\n");
}



/** Conexões  **/

void conecta_UM(no *n1, no *n2, Short lado)
{
    if(n1->tipo == Fim)
        ERRO("FIM NAO CONECTA");

    switch(lado)
    {
        case Inicio:
            n1->at.i.n = n2;
            break;

        case El:
            n1->at.m.el = n2;
            break;

        case Th:
            n1->at.m.th = n2;
            break;
    }

    lista *l;
    l = cria_lista();
    l->n = n1;

    l->l = n2->l;
    n2->l = l;
}

void conecta_DOIS(no *n, no *el, no *th)
{
    if(n->tipo == Inicio)
        ERRO("INICIO NAO CONECTA DOIS");

    conecta_UM(n,el,El);
    conecta_UM(n,th,Th);
}

Short desconecta_UM(no *n1, no *n2)
{
    if(n1->tipo == Fim)
        ERRO("FIM NAO DESCONECTA");

    lista *l;
    l = cria_lista();
    l->l = n2->l;

    lista *lc;
    for(lc = l; lc->l != NULL; lc = lc->l)
        if(lc->l->n == n1)
            break;
    if(lc->l == NULL)
        ERRO("NOS NAO CONECTADOS");

    lista *laux;
    laux = lc->l;
    lc->l = laux->l;
    libera_lista_no(laux);
    n2->l = l->l;
    libera_lista_no(l);

    switch(n1->tipo)
    {
        case Inicio:
            n1->at.i.n = NULL;
            return Inicio;
            break;

        case Meio:
            if(n1->at.m.el == n2)
            {
                n1->at.m.el = NULL;
                return El;
            }
            if(n1->at.m.th == n2)
            {
                n1->at.m.th = NULL;
                return Th;
            }
            ERRO("CONEXAO NAO ANOTADA");
            break;
    }
    return 0;
}

void desconecta_DOIS(no *n)
{
    switch(n->tipo)
    {
        case Inicio:
            desconecta_UM(n,n->at.i.n);
            break;

        case Meio:
            desconecta_UM(n,n->at.m.el);
            desconecta_UM(n,n->at.m.th);
            break;
    }
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



/** Descontrutores de estruturas complexas  **/

/*void libera_arvore(no *n)

void libera_QDD(QDD *Q)*/




/** Compara estruturas  **/

/*Short compara_no_meio(no *n1, no *n2)

Short compara_no_fim(no *n1, no *n2)

Short compara_zero(no *n)*/



/** Copia estrururas  **/

/*no* copia_no(no *n1)

lista* copia_lista_com_cabeca(lista *l1)

lista* copia_lista_sem_cabeca(lista *l1)

no* copia_arvore(no *n)

QDD* copia_QDD(QDD *Q1)*/




/** Operações algebricas com estruturas  **/

/*no* soma(no *n1, no *n2)

no* produto_no_no(no *n1, no *n2)

no* produto_no_no_conjugado(no *n1, no *n2)

void produto_no_real(no *n, float re)

void produto_arvore_real(no *n, float re)*/



/**  Le txt  **/

void completa_QDD_matriz(no *n, no ***nf, int r, int c, int exp)
{

}

QDD* le_matriz(char *nome)
{
    Long i, j, k;
    float re, im;

    FILE *fp;
    fp = fopen(nome,"r");

    Short N;
    fscanf(fp,"%hu\n",&N);
    printf("\n\nN: %d",N);
    Long exp;
    exp = (Long)pow(2,N);
    printf("\nexp: %d",exp);

    no ***nf;
    nf = malloc(exp*sizeof(no**));
    if(nf == NULL)
        ERRO("LE MATRIZ NF");
    aumenta_memoria(exp*sizeof(no**));
    for(i=0; i<exp; i++)
    {
        nf[i] = malloc(exp*sizeof(no*));
        if(nf[i] = NULL)
            ERRO("LE MATRIZ NF[]");
        aumenta_memoria(exp*sizeof(no*));

        for(j=0; j<exp; j++)
        {
            fscanf(fp,"%f",&re);
            fscanf(fp,"%f",&im);
            nf[i][j] = cria_no_fim(re,im);

            printf("%f %f ",re,im);
        }
        printf("\n");
    }
}

/**  Opeações estruturais  **/

/*lista* acha_lista_fim_arvore(no *n)

lista* acha_lista_fim_QDD(QDD *Q)

void reduz_lista(lista *l)

void reduz_QDD(QDD *Q)*/



/**  Operações QDD  **/

/*QDD* produto_tensorial(QDD *Q1, QDD *Q2)

QDD* potencia_tensorial(QDD *Q, Short i)

void produto_por_escalar(QDD *Q, float re, float im)

QDD* soma_QDD(QDD *Q1, QDD *Q2)

QDD* produto_matriz_matriz(QDD *Q1, QDD *Q2)*/



/** Blocos universais  **/

/*QDD* I_1()

QDD* I(Short i)

QDD* H_1()

QDD* H(Short i)*/





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

void configuracao(Short i)
{
    nqbit = i;
    eps = pow(2,-(i/2.0))/20;
}



int main()
{
    inicia_relatorio_memoria(1);
    configuracao(20);
    /***********************************/

    QDD *Q;
    Q = le_matriz("H2.txt");

    /***********************************/
    finaliza_relatorio_memoria();
    return 0;
}
