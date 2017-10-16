#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<limits.h>
#include <locale.h>

#define Inicio 0
#define Meio 1
#define Fim 2

#define V 0
#define R 1
#define C 2

#define Else 1
#define Then 2

#define pi 3.1415926535897932384626433832795



FILE *fm;
unsigned long long mem = 0, memMax = 0, memF = 0;
unsigned long long iQ = 0, iI = 0, iM = 0, iF = 0, iL = 0, iA = 0, iC = 0, iS = 0;
unsigned long long MAX;
unsigned short print, Nqbit;
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

struct apply
{
    struct no    *n, *n1, *n2;
    struct apply *a, *a1, *a2;
};

struct conta
{
    struct no *n;
    unsigned short nivel;
    struct conta *c;
};

struct suporte
{

    struct conta *c[3];
    unsigned short nivel;
    struct suporte *s;
};

struct complexo
{
    float re, im;
};


/** Typedefs e definitions  **/

typedef struct QDD   QDD;
typedef struct no    no;
typedef struct lista lista;

typedef struct apply   apply;
typedef struct conta   conta;
typedef struct suporte suporte;

typedef struct complexo complexo;

typedef unsigned short Short;
typedef unsigned long long Long;



/**  Relatorios e configuração  **/

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
        fprintf(fm,"\nMemFinal: %d",mem);
        fclose(fm);
    }
}

void configuracao(Short N)
{
    Nqbit = N;
    MAX = pow(2,N);
    eps = pow(2,-0.5*N)/20;
}



/** ERRO**/

void ERRO(char *s)
{
    printf("\n\nERRO %s",s);
    if(print)
        fprintf(fm,"\n\nERRO %s",s);
    finaliza_relatorio_memoria();
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
    if(m > mem)
        ERRO("DIMINUI MEMORIA| MEMORIA NEGATIVA");

    mem -= m;
    if(print)
        fprintf(fm,"\n\tMemDOWN: %d\t\t-%u",mem,m);
}

void aumenta_memoria_fora(Long m)
{
    memF += m;
    aumenta_memoria(m);
}

void diminui_memoria_fora(Long m)
{
    if(m > memF)
        ERRO("DIMINUI MEMORIA FORA| MEMORIA NEGATIVA");

    memF -= m;
    diminui_memoria(m);
}



/** Construtores  **/

QDD* cria_QDD(Short nqbit)
{
    QDD *Q;
    Q = malloc(sizeof(QDD));
    if(Q == NULL)
        ERRO("CRIA QDD");
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
        ERRO("CRIA INICIO");
    aumenta_memoria(sizeof(no));
    iI++;

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
        ERRO("CRIA MEIO");
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
        ERRO("CRIA FIM");
    aumenta_memoria(sizeof(no));
    iF++;

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
        ERRO("CRIA LISTA");
    aumenta_memoria(sizeof(lista));
    iL++;

    l->l = NULL;
    l->n = NULL;
    return l;
}

apply* cria_apply()
{
    apply *a;
    a = malloc(sizeof(apply));
    if(a == NULL)
        ERRO("CRIA APPLY");
    aumenta_memoria(sizeof(apply));
    iA++;

    a->n  = NULL;
    a->n1 = NULL;
    a->n2 = NULL;

    a->a  = NULL;
    a->a1 = NULL;
    a->a2 = NULL;

    return a;
}

conta* cria_conta(Short nivel)
{
    conta *c;
    c = malloc(sizeof(conta));
    if(c == NULL)
        ERRO("CRIA CONTA");
    aumenta_memoria(sizeof(conta));
    iC++;

    c->n = NULL;
    c->nivel = nivel;
    c->c = NULL;

    return c;
}

suporte* cria_suporte(Short nivel)
{
    suporte *s;
    s = malloc(sizeof(suporte));
    if(s == NULL)
        ERRO("CRIA SUPORTE");
    aumenta_memoria(sizeof(suporte));
    iS++;

    s->c[C] = NULL;
    s->c[R] = NULL;
    s->c[V] = NULL;

    s->nivel = nivel;
    s->s = NULL;

    return s;
}


complexo** cria_matriz_complexo(Short N)
{
    Long ex;
    ex = pow(2,N);

    complexo **m;
    m = malloc(ex*sizeof(complexo*));
    if(m == NULL)
        ERRO("LE MATRIZ NORMAL| ALOCA M");
    aumenta_memoria_fora(ex*sizeof(complexo*));

    Long i;
    for(i=0; i<ex; i++)
    {
        m[i] = malloc(ex*sizeof(complexo));
        if(m[i] == NULL)
            ERRO("LE MATRIZ NORMAL| ALOCA M[]");
        aumenta_memoria_fora(ex*sizeof(complexo));
    }
    return m;
}



/** Destrutores  **/

void libera_QDD_no(QDD *Q)
{
    diminui_memoria(sizeof(QDD));
    if(iQ == 0)
        ERRO("LIBERA QDD");
    iQ--;
    free(Q);
}

void libera_no(no *n)
{
    if(n->l != NULL)
        ERRO("LIBERA NO| NO DEVERIA ESTAR DESCONECTADO");
    switch(n->tipo)
    {
        case Inicio:
            if(n->at.i.n != NULL)
                ERRO("LIBERA NO| NO INICIO AINDA CONECTADO");
            break;

        case Meio:
            if(n->at.m.el != NULL)
                ERRO("LIBERA NO| NO MEIO AINDA CONECTADO EL");
            if(n->at.m.th != NULL)
                ERRO("LIBERA NO| NO MEIO AINDA CONECTADO TH");
            break;
    }

    diminui_memoria(sizeof(no));
    switch(n->tipo)
    {
        case Inicio:
        if(iI == 0)
            ERRO("LIBERA INICIO");
        iI--;
        break;

        case Meio:
        if(iM == 0)
            ERRO("LIBERA MEIO");
        iM--;
        break;

        case Fim:
        if(iF == 0)
            ERRO("LIBERA FIM");
        iF--;
        break;
    }
    free(n);
}

void libera_lista_no(lista *l)
{
    diminui_memoria(sizeof(lista));
    if(iL == 0)
        ERRO("LIBERA LISTA");
    iL--;
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

void libera_apply_no(apply *a)
{
    diminui_memoria(sizeof(apply));
    if(iA == 0)
        ERRO("LIBERA APPLY");
    iA--;
    free(a);
}

void libera_apply_lista(apply *a)
{
    apply *ac;
    while(a != NULL)
    {
        ac = a->a;
        libera_apply_no(a);
        a = ac;
    }
}

void libera_conta_no(conta *c)
{
    diminui_memoria(sizeof(conta));
    if(iC == 0)
        ERRO("LIBERA APPLY");
    iC--;
    free(c);
}

void libera_conta_lista(conta *c)
{
    conta *cc;
    while(c != NULL)
    {
        cc = c->c;
        libera_conta_no(c);
        c = cc;
    }
}

void libera_suporte_no(suporte *s)
{
    diminui_memoria(sizeof(suporte));
    if(iS == 0)
        ERRO("LIBERA APPLY");
    iS--;
    free(s);
}

void libera_suporte_lista(suporte *s)
{
    suporte *sc;
    while(s != NULL)
    {
        sc = s->s;
        libera_suporte_no(s);
        s = sc;
    }
}


void libera_matriz_complexo(complexo **m, Short N)
{
    Long ex;
    ex = pow(2,N);

    Long i;
    for(i=0; i<ex; i++)
    {
        free(m[i]);
        diminui_memoria_fora(ex*sizeof(complexo));
    }
    free(m);
    diminui_memoria_fora(ex*sizeof(complexo*));
}



/**  structs globais  **/

QDD *Qred;
no  *nzero;

void inicia_structs_globais()
{
    Qred  = cria_QDD(1);
    Qred->n = cria_no_inicio();
    nzero = cria_no_fim(0,0);
}

void finaliza_structs_globais()
{
    libera_no(Qred->n);
    libera_QDD_no(Qred);
    libera_no(nzero);
}



/** Enlistadores  **/

lista* enlista_arvore(no *n)
{
    if(n == NULL)
        ERRO("ENLISTA ARVORE| ARVORE VAZIA");

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
                    if(la->n == NULL)
                            ERRO("ENISTA ARVORE| ARVORE DESCONEXA 1");
                    break;

                case Meio:
                    la->n = n->at.m.th;
                    if(la->n == NULL)
                            ERRO("ENISTA ARVORE| ARVORE DESCONEXA 2");

                    laux = cria_lista();
                    laux->n = n->at.m.el;
                    laux->l = la;
                    la = laux;
                    if(la->n == NULL)
                            ERRO("ENISTA ARVORE| ARVORE DESCONEXA 3");
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



/**  Mostra  **/

void mostra_lista(lista *l)
{
    no *n;
    lista *lc;
    Long ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        n = lc->n;
        printf("\n\tLigacao %d:",ligacao);
        printf(" %d",n);
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
        printf("Ligacoes anteriores:");
        mostra_lista(n->l);
    }
    printf("\nTipo");
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
            printf("%.3e %.3e",n->at.f.re,n->at.f.im);
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

void mostra_apply_no(apply *a)
{
    printf("\nEndereco (apply): %d",a);
    printf("\n\nNo 1:");
    mostra_no(a->n1);
    printf("\nNo 2:");
    mostra_no(a->n2);
    printf("\nNo:");
    mostra_no(a->n);
    printf("\n");
    printf("\na1: %d",a->a1);
    printf("\na2: %d",a->a2);
    printf("\n\na:  %d\n",a->a);
}

void mostra_apply_lista(apply *a)
{
    apply *ac;
    Long ligacao = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        printf("\n\n\n\n\nLigacao apply %d",ligacao);
        mostra_apply_no(ac);
        ligacao++;
    }
}

void mostra_conta_no(conta *c)
{
    printf("\nEndereco (conta): %d",c);
    printf("\nnivel: %d",c->nivel);
    printf("\n\nno: ");
    mostra_no(c->n);
    printf("\nc: %d",c->c);
}

void mostra_conta_lista(conta *c)
{
    conta *cc;
    Short ligacao = 0;
    for(cc = c; cc != NULL; cc = cc->c)
    {
        printf("\n\n\n\nLigacao conta %d: ",ligacao);
        mostra_conta_no(cc);
        ligacao++;
    }
}

void mostra_suporte_no(suporte *s)
{
    printf("\nEndereco (suporte): %d",s);
    printf("\nnivel: %d",s->nivel);
    printf("\n\ncc: %d",s->c[C]);
    printf("\ncv: %d",s->c[V]);
    printf("\ncr: %d",s->c[R]);
    printf("\n\ns: %d",s->s);
}

void mostra_suporte_no_com_conta(suporte *s)
{
    printf("\nEndereco (suporte): %d",s);
    printf("\nnivel: %d",s->nivel);
    printf("\n\ncc: %d",s->c[C]);
    mostra_conta_lista(s->c[C]);
    printf("\ncv: %d",s->c[V]);
    mostra_conta_lista(s->c[V]);
    printf("\ncr: %d",s->c[R]);
    mostra_conta_lista(s->c[R]);
    printf("\n\ns: %d",s->s);
}

void mostra_suporte_lista(suporte *s)
{
    suporte *sc;
    Short ligacao = 0;
    for(sc = s; sc != NULL; sc = sc->s)
    {
        printf("\n\n\n\nLigacao suporte %d: ",ligacao);
        mostra_suporte_no(sc);
        ligacao++;
    }
}

void mostra_suporte_lista_com_conta(suporte *s)
{
    suporte *sc;
    Short ligacao = 0;
    for(sc = s; sc != NULL; sc = sc->s)
    {
        printf("\n\n\n\nLigacao suporte %d: ",ligacao);
        mostra_suporte_no_com_conta(sc);
        ligacao++;
    }
}

void mostra_quantidades()
{
    Short vazio = 1;
    if(mem != 0)
    {
        vazio = 0;
        printf("\nMem: %llu",mem);
    }
    if(memF != 0)
    {
        vazio = 0;
        printf("\nMem: %llu",memF);
    }
    if(iQ != 0)
    {
        vazio = 0;
        printf("\nQDD: %llu",iQ);
    }
    if(iI != 0)
    {
        vazio = 0;
        printf("\ni:   %llu",iI);
    }
    if(iM != 0)
    {
        vazio = 0;
        printf("\nm:   %llu",iM);
    }
    if(iF != 0)
    {
        vazio = 0;
        printf("\nf:   %llu",iF);
    }
    if(iL != 0)
    {
        vazio = 0;
        printf("\nl:   %llu",iL);
    }
    if(iA != 0)
    {
        vazio = 0;
        printf("\na:   %llu",iA);
    }
    if(iC != 0)
    {
        vazio = 0;
        printf("\nc:   %llu",iC);
    }
    if(iS != 0)
    {
        vazio = 0;
        printf("\ns:   %llu",iS);
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
    printf("\na:   %d",sizeof(apply));
    printf("\nc:   %d",sizeof(conta));
    printf("\ns:   %d",sizeof(suporte));
    printf("\n");
}

void mostra_configuracao()
{
    printf("\nConfiguracao: ");
    printf("\nNqbit: %hu",Nqbit);
    printf("\nMax: %lld",MAX);
    printf("\neps: %.3e",eps);
}


void mostra_matriz(double **m, Long r, Long c)
{
    Long i, j;
    for(i = 0; i < r; i++)
    {
        for(j = 0; j < c; j++)
            printf("%f ",m[i][j]);
        printf("\n");
    }
}

void mostra_matriz_complexo(complexo **m, Short N)
{
    Long ex;
    ex = pow(2,N);

    Long i, j;
    for(i=0; i<ex; i++)
    {
        printf("\n");
        for(j=0; j<ex; j++)
            printf("%f %f ",m[i][j].re,m[i][j].im);
    }
}



/**  Fmostra  **/

void fmostra_lista(FILE *fp, lista *l)
{
    no *n;
    lista *lc;
    Long ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        n = lc->n;
        fprintf(fp,"\n\tLigacao %d:",ligacao);
        fprintf(fp," %d",n);
        ligacao++;
    }
}

void fmostra_no(FILE *fp, no *n)
{
    fprintf(fp,"\nEndereco (no): %d\n",n);
    if(n == NULL)
        return;
    if(n->l != NULL)
    {
        fprintf(fp,"Ligacoes anteriores:");
        fmostra_lista(fp,n->l);
    }
    fprintf(fp,"\nTipo");
    switch(n->tipo)
    {
        case Inicio:
            fprintf(fp,": Inicio\n");
            fprintf(fp,"Ligacoes posteriores\n");
            fprintf(fp,"\tn: %d\n",n->at.i.n);
            break;

        case Meio:
            fprintf(fp,"/nivel: ");
            switch(n->at.m.classe)
            {
                case V:
                    fprintf(fp,"V");
                    break;

                case R:
                    fprintf(fp,"R");
                    break;

                case C:
                    fprintf(fp,"C");
                    break;
            }

            fprintf(fp,"%d\n",n->at.m.nivel);
            fprintf(fp,"Ligacoes posteriores\n");
            fprintf(fp,"\telse: %d\n",n->at.m.el);
            fprintf(fp,"\tThen: %d",n->at.m.th);
            break;

        case Fim:
            fprintf(fp,": Numero\n");
            fprintf(fp,"%.3e %.3e",n->at.f.re,n->at.f.im);
            break;
    }
    fprintf(fp,"\n");
}

void fmostra_lista_com_no(FILE *fp, lista *l)
{
    lista *lc;
    Long ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        fprintf(fp,"\n\n\tLigacao %u:",ligacao);
        fmostra_no(fp,lc->n);
        ligacao++;
    }
    printf("\n");
}

void fmostra_arvore(FILE *fp, no *n)
{
    lista *l;
    l = enlista_arvore(n);
    fmostra_lista_com_no(fp,l);
    libera_lista_lista(l);
    fprintf(fp,"\n");
}

void fmostra_QDD(FILE *fp, QDD *Q)
{
    lista *l;
    l = enlista_QDD(Q);

    fprintf(fp,"\nEndereco (QDD): %d",Q);
    fprintf(fp,"\nNQBIT: %d\n",Q->nqbit);
    fmostra_lista_com_no(fp,l);
    fprintf(fp,"\n\nAMPLITUDES");
    fmostra_lista_com_no(fp,Q->l);
    libera_lista_lista(l);
    fprintf(fp,"\n");
}

void fmostra_QDD_sozinho(QDD *Q, char *arquivo)
{
    FILE *fp;
    fp = fopen(arquivo,"w");
    fmostra_QDD(fp,Q);
    fclose(fp);
}

void fmostra_apply_no(FILE *fp, apply *a)
{
    fprintf(fp,"\nEndereco (apply): %d",a);
    fprintf(fp,"\n\nNo 1:");
    fmostra_no(fp,a->n1);
    fprintf(fp,"\nNo 2:");
    fmostra_no(fp,a->n2);
    fprintf(fp,"\nNo:");
    fmostra_no(fp,a->n);
    fprintf(fp,"\n");
    fprintf(fp,"\na1: %d",a->a1);
    fprintf(fp,"\na2: %d",a->a2);
    fprintf(fp,"\n\na:  %d\n",a->a);
}

void fmostra_apply_lista(FILE *fp, apply *a)
{
    apply *ac;
    Long ligacao = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        fprintf(fp,"\n\n\n\n\nLigacao apply %d",ligacao);
        fmostra_apply_no(fp,ac);
        ligacao++;
    }
}

void fmostra_conta_no(FILE *fp, conta *c)
{
    fprintf(fp,"\nEndereco (conta): %d",c);
    fprintf(fp,"\nnivel: %d",c->nivel);
    fprintf(fp,"\n\nno: ");
    fmostra_no(fp,c->n);
    fprintf(fp,"\nc: %d",c->c);
}

void fmostra_conta_lista(FILE *fp, conta *c)
{
    if(c == NULL)
        return;

    conta *cc;
    Short ligacao = 0;
    for(cc = c; cc != NULL; cc = cc->c)
    {
        fprintf(fp,"\n\n\n\nLigacao conta %d: ",ligacao);
        fmostra_conta_no(fp,cc);
        ligacao++;
    }
}

void fmostra_suporte_no(FILE *fp, suporte *s)
{
    fprintf(fp,"\nEndereco (suporte): %d",s);
    fprintf(fp,"\nnivel: %d",s->nivel);
    fprintf(fp,"\n\ncc: %d",s->c[C]);
    fprintf(fp,"\ncv: %d",s->c[V]);
    fprintf(fp,"\ncr: %d",s->c[R]);
    fprintf(fp,"\n\ns: %d",s->s);
}

void fmostra_suporte_no_com_conta(FILE *fp, suporte *s)
{
    fprintf(fp,"\nEndereco (suporte): %d",s);
    fprintf(fp,"\nnivel: %d",s->nivel);
    fprintf(fp,"\n\ncc: %d",s->c[C]);
    fmostra_conta_lista(fp,s->c[C]);
    fprintf(fp,"\ncv: %d",s->c[V]);
    fmostra_conta_lista(fp,s->c[V]);
    fprintf(fp,"\ncr: %d",s->c[R]);
    fmostra_conta_lista(fp,s->c[R]);
    fprintf(fp,"\n\ns: %d",s->s);
}

void fmostra_suporte_lista(FILE *fp, suporte *s)
{
    suporte *sc;
    Short ligacao = 0;
    for(sc = s; sc != NULL; sc = sc->s)
    {
        fprintf(fp,"\n\n\n\nLigacao suporte %d: ",ligacao);
        fmostra_suporte_no(fp,sc);
        ligacao++;
    }
}

void fmostra_suporte_lista_com_conta(FILE *fp, suporte *s)
{
    suporte *sc;
    Short ligacao = 0;
    for(sc = s; sc != NULL; sc = sc->s)
    {
        fprintf(fp,"\n\n\n\nLigacao suporte %d: ",ligacao);
        fmostra_suporte_no_com_conta(fp,sc);
        ligacao++;
    }
}

void fmostra_quantidades(FILE *fp)
{
    Short vazio = 1;
    if(mem != 0)
    {
        vazio = 0;
        fprintf(fp,"\nMem: %d",mem);
    }
    if(iQ != 0)
    {
        vazio = 0;
        fprintf(fp,"\nQDD: %d",iQ);
    }
    if(iI != 0)
    {
        vazio = 0;
        fprintf(fp,"\ni:   %d",iI);
    }
    if(iM != 0)
    {
        vazio = 0;
        fprintf(fp,"\nm:   %d",iM);
    }
    if(iF != 0)
    {
        vazio = 0;
        fprintf(fp,"\nf:   %d",iF);
    }
    if(iL != 0)
    {
        vazio = 0;
        fprintf(fp,"\nl:   %d",iL);
    }
    if(iA != 0)
    {
        vazio = 0;
        fprintf(fp,"\na:   %d",iA);
    }
    if(iC != 0)
    {
        vazio = 0;
        fprintf(fp,"\nc:   %d",iC);
    }
    if(iS != 0)
    {
        vazio = 0;
        fprintf(fp,"\ns:   %d",iS);
    }
    if(vazio)
        fprintf(fp,"\nTUDO ZERADO");
    fprintf(fp,"\n");
}

void fmostra_tamanhos(FILE *fp)
{
    fprintf(fp,"\nTAMANHOS");
    fprintf(fp,"\nQDD: %d",sizeof(QDD));
    fprintf(fp,"\nn:   %d",sizeof(no));
    fprintf(fp,"\nl:   %d",sizeof(lista));
    fprintf(fp,"\na:   %d",sizeof(apply));
    fprintf(fp,"\nc:   %d",sizeof(conta));
    fprintf(fp,"\ns:   %d",sizeof(suporte));
    fprintf(fp,"\n");
}

void fmostra_configuracao(FILE *fp)
{
    fprintf(fp,"\nConfiguracao: ");
    fprintf(fp,"\nNqbit: %hu",Nqbit);
    fprintf(fp,"\nMax: %lld",MAX);
    fprintf(fp,"\neps: %.3e",eps);
}

void fmostra_matriz(FILE *fp, double **m, Long r, Long c)
{
    Long i, j;
    for(i = 0; i < r; i++)
    {
        for(j = 0; j < c; j++)
            fprintf(fp,"%f ",m[i][j]);
        fprintf(fp,"\n");
    }
}



/** Conexões  **/

void conecta_UM(no *n1, no *n2, Short lado)
{
    if(n1->tipo == Fim)
        ERRO("CONECTA UM| FIM NAO TEM SUCESSORES");
    if(n2->tipo == Inicio)
        ERRO("CONECTA UM| INICIO NAO TEM ANTECESSORES");

    switch(lado)
    {
        case Inicio:
            if(n1->tipo != Inicio)
                ERRO("CONECTA UM| FUNCAO CHAMADA PARA INICIO MAS N1 NAO E");
            if(n1->at.i.n != NULL)
                ERRO("CONCETA UM| NO JA CONECTADO 1");
            if(n2->l != NULL)
                ERRO("CONCETA UM| N1 E INICIO E N2 JA TEM CONEXAO ANTERIOR");

            n1->at.i.n = n2;
            break;

        case Else:
            if(n1->tipo != Meio)
                ERRO("CONECTA UM| FUNCAO CHAMADA PARA MEIO MAS N1 NAO E 1");
            if(n1->at.m.el != NULL)
                ERRO("CONCETA UM| NO JA CONECTADO 2");

            n1->at.m.el = n2;
            break;

        case Then:
            if(n1->tipo != Meio)
                ERRO("CONECTA UM| FUNCAO CHAMADA PARA MEIO MAS N1 NAO E 2");
            if(n1->at.m.th != NULL)
                ERRO("CONCETA UM| NO JA CONECTADO 3");

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

    conecta_UM(n,el,Else);
    conecta_UM(n,th,Then);
}

Short desconecta_UM(no *n1, no *n2)
{
    if(n1->tipo == Fim)
        ERRO("DESCONECTA UM| FIM NAO TEM SUCESSORES");

    lista *l;
    l = cria_lista();
    l->l = n2->l;

    lista *lc;
    for(lc = l; lc->l != NULL; lc = lc->l)
        if(lc->l->n == n1)
            break;
    if(lc->l == NULL)
        ERRO("DESCONECTA UM| REGISTRO INCOMPATIVEL 1");

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
                return Else;
            }
            if(n1->at.m.th == n2)
            {
                n1->at.m.th = NULL;
                return Then;
            }
            ERRO("DESCONECTA UM| REGISTRO INCOMPATIVEL 2");
            break;
    }
    return 0;
}

void desconecta_DOIS(no *n)
{
    if(n->tipo == Fim)
        ERRO("DESCONECTA DOIS| FIM NAO TEM SUCESSORES");

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
    if(n1 == n2)
        ERRO("TRANSFERE CONEXAO| NOS IGUAIS");

    no *n;
    Short lado;
    while(n2->l != NULL)
    {
        n = n2->l->n;
        lado = desconecta_UM(n,n2);
        conecta_UM(n,n1,lado);
    }
}



/** Destrutores de estruturas complexas  **/

void libera_arvore(no *n)
{
    if(n->l != NULL)
        return;

    lista *l, *laux;
    l = cria_lista();
    l->n = n;

    no *naux1, *naux2;

    while(l != NULL)
    {
        n = l->n;
        switch(n->tipo)
        {
            case Inicio:
                naux1 = n->at.i.n;
                desconecta_DOIS(n);
                libera_no(n);

                if(naux1->l == NULL)
                    l->n = naux1;
                break;

            case Meio:
                naux1 = n->at.m.el;
                naux2 = n->at.m.th;
                desconecta_DOIS(n);
                libera_no(n);

                if(naux1->l == NULL)
                {
                    if(naux2->l == NULL)
                    {
                        l->n = naux2;
                        laux = cria_lista();
                        laux->n = naux1;
                        laux->l = l;
                        l = laux;
                    }
                    else
                    {
                        l->n = naux1;
                    }
                }
                else
                {
                    if(naux2->l == NULL)
                    {
                        l->n = naux2;
                    }
                    else
                    {
                        laux = l->l;
                        libera_lista_no(l);
                        l = laux;
                    }
                }
                break;

            case Fim:
                libera_no(n);

                laux = l->l;
                libera_lista_no(l);
                l = laux;
                break;
        }
    }
}

void libera_QDD(QDD *Q)
{
    libera_arvore(Q->n);
    libera_lista_lista(Q->l);
    libera_QDD_no(Q);
}



/** Compara estruturas  **/

Short compara_no_meio_parcial(no *n1, no *n2)
{
    if(n1->tipo == Meio)
    if(n2->tipo == Meio)
    if(n1->at.m.classe == n2->at.m.classe)
    if(n1->at.m.nivel  == n2->at.m.nivel )
        return 1;
    return 0;
}

Short compara_no_meio_completo(no *n1, no *n2)
{
    if(n1 != n2)
    if(compara_no_meio_parcial(n1,n2))
    if(n1->at.m.el == n2->at.m.el)
    if(n1->at.m.th == n2->at.m.th)
        return 1;
    return 0;
}

Short compara_no_fim(no *n1, no *n2, Short ex)
{
    if(n1->tipo != Fim||n2->tipo != Fim)
        ERRO("COMPARA NO FIM| TIPO DE NO ERRADO");

    double re, im, ep;
    ep = pow(eps,ex);
    re = (n1->at.f.re) - (n2->at.f.re);
    im = (n1->at.f.im) - (n2->at.f.im);

    if(re< ep)
    if(re>-ep)
    if(im< ep)
    if(im>-ep)
        return 1;
    return 0;
}

Short compara_no_fim_zero(no *n, Short ex)
{
    Short res;
    res = compara_no_fim(n,nzero,ex);
    return res;
}

Short compara_apply(apply *a1, apply *a2)
{
    if(a1->n1 == a2->n1)
    if(a1->n2 == a2->n2)
        return 1;
    return 0;
}



/**  Operações estruturais simples  **/

Long conta_items_lista(lista *l)
{
    Long i;
    i = 0;
    lista *lc;
    for(lc = l; lc != NULL; lc = lc->l)
        i++;
    return i;
}

lista* acha_lista_fim_arvore(no *n)
{
    lista *l, *lc, *laux;;
    l =  cria_lista();
    l->l = enlista_arvore(n);

    lc = l;
    do
    {
        if(lc->l->n->tipo != Fim)
        {
            laux = lc->l;
            lc->l = laux->l;
            libera_lista_no(laux);
            if(lc == NULL)
                break;
        }
        else
        {
            lc = lc->l;
        }
    }
    while(lc->l != NULL);

    laux = l->l;
    libera_lista_no(l);

    return laux;
}

lista* acha_lista_fim_QDD(QDD *Q)
{
    lista *l;
    l = acha_lista_fim_arvore(Q->n);
    return l;
}

lista* acha_fim_lista(lista *l)
{
    if(l == NULL)
        ERRO("ACHA FIM LISTA| LISTA VAZIA");

    lista *lc;
    for(lc = l; lc->l != NULL; lc = lc->l);
    return lc;
}

void reduz_lista_fim(lista *l, Short ex)
{
    lista *lc1, *lc2, *laux;

    lc1 = l;
    do
    {
        lc2 = lc1;
        while(lc2->l != NULL)
        {
            if(compara_no_fim(lc1->n,lc2->l->n,ex))
            {
                laux = lc2->l;
                lc2->l = laux->l;

                transfere_conexao(lc1->n,laux->n);
                libera_no(laux->n);
                libera_lista_no(laux);
            }
            else
            {
                lc2 = lc2->l;
            }
        }
        lc1 = lc1->l;
    }
    while(lc1 != NULL);
}



/** Copia estrururas  **/

no* copia_no(no *n1)
{
    no *n2 = NULL;
    switch(n1->tipo)
    {
        case Inicio:
            n2 = cria_no_inicio(n1->at.i.n);
            break;

        case Meio:
            n2 = cria_no_meio(n1->at.m.classe,n1->at.m.nivel);
            break;

        case Fim:
            n2 = cria_no_fim(n1->at.f.re,n1->at.f.im);
            break;
    }
    return n2;
}

lista* copia_lista_com_cabeca(lista *l1)
{
    lista *l2, *lc1, *lc2;
    l2 = cria_lista();
    lc2 = l2;

    for(lc1 = l1; lc1 != NULL; lc1 = lc1->l)
    {
        lc2->l = cria_lista();
        lc2 = lc2->l;
        lc2->n = lc1->n;
    }
    return l2;
}

lista* copia_lista_sem_cabeca(lista *l1)
{
    lista *l2, *laux;
    l2 = copia_lista_com_cabeca(l1);
    laux = l2->l;
    libera_lista_no(l2);
    return laux;
}

no* copia_arvore(no *n)
{
    lista *l1, *l2;
    l1 = enlista_arvore(n);
    l2 = copia_lista_sem_cabeca(l1);

    lista *lc1a, *lc1b, *lc2a, *lc2b;
    for(lc2a = l2; lc2a != NULL; lc2a = lc2a->l)
        lc2a->n = copia_no(lc2a->n);

    no *n1, *n2, *nf1, *nf2;
    lc1a = l1;
    lc2a = l2;
    do
    {
        n1 = lc1a->n;
        n2 = lc2a->n;

        switch(n1->tipo)
        {
            case Inicio:
                nf1 = n1->at.i.n;

                lc1b = l1;
                lc2b = l2;
                do
                {
                    lc1b = lc1b->l;
                    lc2b = lc2b->l;
                }
                while(lc1b->n != nf1);

                nf2 = lc2b->n;
                conecta_UM(n2,nf2,Inicio);
                break;

            case Meio:
                nf1 = n1->at.m.el;

                lc1b = l1;
                lc2b = l2;
                do
                {
                    lc1b = lc1b->l;
                    lc2b = lc2b->l;
                }
                while(lc1b->n != nf1);

                nf2 = lc2b->n;
                conecta_UM(n2,nf2,Else);


                nf1 = n1->at.m.th;

                lc1b = l1;
                lc2b = l2;
                do
                {
                    lc1b = lc1b->l;
                    lc2b = lc2b->l;
                }
                while(lc1b->n != nf1);

                nf2 = lc2b->n;
                conecta_UM(n2,nf2,Then);
                break;
        }

        lc1a = lc1a->l;
        lc2a = lc2a->l;
    }
    while(lc1a != NULL);

    n2 = l2->n;
    libera_lista_lista(l1);
    libera_lista_lista(l2);

    return n2;
}

QDD* copia_QDD(QDD *Q1)
{
    QDD *Q2;
    Q2 = cria_QDD(Q1->nqbit);

    Q2->n = copia_arvore(Q1->n);
    Q2->l = acha_lista_fim_QDD(Q2);

    return Q2;
}



/** Operações algebricas com estruturas simples  **/

no* soma_no(no *n1, no *n2)
{
    no *n;
    double re, im;
    re = (n1->at.f.re) + (n2->at.f.re);
    im = (n1->at.f.im) + (n2->at.f.im);
    n = cria_no_fim(re,im);
    return n;
}

no* produto_no_no(no *n1, no *n2)
{
    no *n;
    double re, im;
    re = (n1->at.f.re)*(n2->at.f.re) - (n1->at.f.im)*(n2->at.f.im);
    im = (n1->at.f.re)*(n2->at.f.im) + (n1->at.f.im)*(n2->at.f.re);
    n = cria_no_fim(re,im);
    return n;
}

no* produto_no_conjugado_no(no *n1, no *n2)
{
    no *n;
    double re, im;
    re = (n1->at.f.re)*(n2->at.f.re) + (n1->at.f.im)*(n2->at.f.im);
    im = (n1->at.f.re)*(n2->at.f.im) - (n1->at.f.im)*(n2->at.f.re);
    n = cria_no_fim(re,im);
    return n;
}

void produto_no_real(no *n, double re)
{
    (n->at.f.re) *= re;
    (n->at.f.im) *= re;
}

void produto_arvore_real(no *n, double re)
{
    lista *l, *lc;
    l = enlista_arvore(n);
    for(lc = l; lc != NULL; lc=  lc->l)
        if(lc->n->tipo == Fim)
            produto_no_real(lc->n,re);
    libera_lista_lista(l);
}



/** Operações algebricas com complexos  **/

complexo zero()
{
    complexo c;
    c.re = 0;
    c.im = 0;
    return c;
}

complexo soma_complexo(complexo c1, complexo c2)
{
    complexo c;
    c.re = c1.re + c2.re;
    c.im = c1.im + c2.im;
    return c;
}

complexo produto_complexo_real(complexo c, float re)
{
    c.re *= re;
    c.im *= re;
    return c;
}

complexo produto_complexo_complexo(complexo c1, complexo c2)
{
    complexo c;
    c.re = (c1.re)*(c2.re) - (c1.im)*(c2.im);
    c.im = (c1.re)*(c2.im) + (c1.im)*(c2.re);
    return c;
}

complexo produto_complexo_conjugado_complexo(complexo c1, complexo c2)
{
    complexo c;
    c.re = (c1.re)*(c2.re) + (c1.im)*(c2.im);
    c.im = (c1.re)*(c2.im) - (c1.im)*(c2.re);
    return c;
}

complexo** produto_matriz_matriz_complexo(complexo **m1, complexo **m2, Short N)
{
    Long ex;
    ex = pow(2,N);

    complexo **m;
    m = cria_matriz_complexo(N);

    complexo c, cp;
    Long i, j, k;
    for(i=0; i<ex; i++)
    {
        for(j=0; j<ex; j++)
        {
            c = zero();
            for(k=0; k<ex; k++)
            {
                cp = produto_complexo_complexo(m1[i][k],m2[k][j]);
                c = soma_complexo(c,cp);
            }
            m[i][j] = c;
        }
    }
    return m;
}


/**  Le txt  **/

void completa_QDD_matriz(no *n, no ***nf, int r, int c, int exp)
{
    switch(n->at.m.classe)
    {
        case R:
            completa_QDD_matriz(n->at.m.el,nf,r,c,exp);
            completa_QDD_matriz(n->at.m.th,nf,r+exp,c,exp);
            break;

        case C:
            if(exp == 1)
            {
                conecta_DOIS(n,nf[r][c],nf[r][c+1]);
            }
            else
            {
                completa_QDD_matriz(n->at.m.el,nf,r,c,exp/2);
                completa_QDD_matriz(n->at.m.th,nf,r,c+exp,exp/2);
            }
            break;
    }
}

QDD* le_matriz(char *nome)
{
    Long i, j;

    FILE *fp;
    fp = fopen(nome,"r");
    if(fp == NULL)
        ERRO("LE MATRIZ| NAO CONSEGUIU ABRIR ARQUIVO");

    Short N;
    fscanf(fp,"%hu\n",&N);
    Long exp;
    exp = (Long)pow(2,N);

    no ***nf;
    float re, im;
    nf = malloc(exp*sizeof(no**));
    if(nf == NULL)
        ERRO("LE MATRIZ NF");
    aumenta_memoria_fora(exp*sizeof(no**));

    for(i=0; i<exp; i++)
    {
        nf[i] = malloc(exp*sizeof(no*));
        if(nf[i] == NULL)
            ERRO("LE MATRIZ NF[]");
        aumenta_memoria_fora(exp*sizeof(no*));

        for(j=0; j<exp; j++)
        {
            fscanf(fp,"%f",&re);
            fscanf(fp,"%f",&im);
            nf[i][j] = cria_no_fim(re,im);
        }
    }
    fclose(fp);

    no ***nm;
    nm = malloc(2*N*sizeof(no**));
    if(nm == NULL)
        ERRO("LE MATRIZ NM");
    aumenta_memoria_fora(2*N*sizeof(no**));

    Long exp2 = 1;
    for(i=0; i<2*N; i++)
    {
        nm[i] = malloc(exp2*sizeof(no*));
        if(nm[i] == NULL)
            ERRO("LE MATRIZ NM[]");
        aumenta_memoria_fora(exp2*sizeof(no*));

        for(j=0; j<exp2; j++)
        {
            if(i%2 == 0)
                nm[i][j] = cria_no_meio(R,i/2);
            else
                nm[i][j] = cria_no_meio(C,i/2);

            if(i>0)
            {
                if(j%2 == 0)
                    conecta_UM(nm[i-1][j/2],nm[i][j],Else);
                else
                    conecta_UM(nm[i-1][j/2],nm[i][j],Then);
            }
        }
        exp2 *= 2;
    }

    completa_QDD_matriz(nm[0][0],nf,0,0,exp/2);

    QDD *Q;
    Q = cria_QDD(N);
    Q->n = cria_no_inicio();
    conecta_UM(Q->n,nm[0][0],Inicio);

    lista *l, *lc;
    l = cria_lista();
    lc = l;
    for(i=0; i<exp; i++)
    {
        for(j=0; j<exp; j++)
        {
            lc->l = cria_lista();
            lc = lc->l;
            lc->n = nf[i][j];
        }
    }
    Q->l = l->l;

    libera_lista_no(l);

    for(i=0; i<exp; i++)
    {
        diminui_memoria_fora(exp*sizeof(no*));
        free(nf[i]);
    }
    diminui_memoria_fora(exp*sizeof(no**));
    free(nf);

    exp2 = 1;
    for(i=0; i<2*N; i++)
    {
        diminui_memoria_fora(exp2*sizeof(no*));
        free(nm[i]);
        exp2 *= 2;
    }
    diminui_memoria_fora(2*N*sizeof(no**));
    free(nm);

    return Q;
}

void completa_QDD_vetor(no *n, no **nf, int i, int exp)
{
    if(exp == 1)
    {
        conecta_DOIS(n,nf[i],nf[i+1]);
    }
    else
    {
        completa_QDD_vetor(n->at.m.el,nf,i,exp/2);
        completa_QDD_vetor(n->at.m.th,nf,i+exp,exp/2);
    }
}

QDD* le_vetor(char *nome)
{
    Long i, j;

    FILE *fp;
    fp = fopen(nome,"r");
    if(fp == NULL)
        ERRO("LE VETOR| NAO CONSEGUIU ABRIR ARQUIVO");

    Short N;
    fscanf(fp,"%hu\n",&N);
    Long exp;
    exp = (Long)pow(2,N);

    no **nf;
    float re, im;
    nf = malloc(exp*sizeof(no*));
    if(nf == NULL)
        ERRO("LE VETOR NF");
    aumenta_memoria_fora(exp*sizeof(no*));
    for(i=0; i<exp; i++)
    {
        fscanf(fp,"%f",&re);
        fscanf(fp,"%f",&im);
        nf[i] = cria_no_fim(re,im);
    }
    fclose(fp);

    no ***nm;
    nm = malloc(N*sizeof(no**));
    if(nm == NULL)
        ERRO("LE MATRIZ NM");
    aumenta_memoria_fora(N*sizeof(no**));

    Long exp2 = 1;
    for(i=0; i<N; i++)
    {
        nm[i] = malloc(exp2*sizeof(no*));
        if(nm[i] == NULL)
            ERRO("LE VETOR NM[]");
        aumenta_memoria_fora(exp2*sizeof(no*));

        for(j=0; j<exp2; j++)
        {
            nm[i][j] = cria_no_meio(V,i);

            if(i>0)
            {
                if(j%2 == 0)
                    conecta_UM(nm[i-1][j/2],nm[i][j],Else);
                else
                    conecta_UM(nm[i-1][j/2],nm[i][j],Then);
            }
        }
        exp2 *= 2;
    }

    completa_QDD_vetor(nm[0][0],nf,0,exp/2);

    QDD *Q;
    Q = cria_QDD(N);
    Q->n = cria_no_inicio();
    conecta_UM(Q->n,nm[0][0],Inicio);

    lista *l, *lc;
    l = cria_lista();
    lc = l;
    for(i=0; i<exp; i++)
    {
        lc->l = cria_lista();
        lc = lc->l;
        lc->n = nf[i];
    }
    Q->l = l->l;

    libera_lista_no(l);

    diminui_memoria_fora(exp*sizeof(no*));
    free(nf);

    exp2 = 1;
    for(i=0; i<N; i++)
    {
        diminui_memoria_fora(exp2*sizeof(no*));
        free(nm[i]);
        exp2 *= 2;
    }
    diminui_memoria_fora(N*sizeof(no**));
    free(nm);

    return Q;
}


complexo** le_matriz_normal(char *nome)
{
    FILE *fp;
    fp = fopen(nome,"r");

    Short N;
    Long ex;
    fscanf(fp,"%hu",&N);
    ex = pow(2,N);

    complexo **m;
    Long i, j;
    m = cria_matriz_complexo(N);
    for(i=0; i<ex; i++)
    {

        for(j=0; j<ex; j++)
        {
            fscanf(fp,"%f",&m[i][j].re);
            fscanf(fp,"%f",&m[i][j].im);
        }
    }
    return m;
}



/**  Operações QDD estruturais   **/

void completa_conversao_QDD_matriz(no *n, no *nesp, Long i, Long j, Long exp, double **m)
{
    no *naux;
    if(exp == 0)
    {
        m[i][2*j]   = n->at.f.re;
        m[i][2*j+1] = n->at.f.im;
    }
    else
    {
        if(compara_no_meio_parcial(n,nesp))
        {
            switch(n->at.m.classe)
            {
                case R:
                    naux = cria_no_meio(C,n->at.m.nivel);
                    completa_conversao_QDD_matriz(n->at.m.el,naux,i    ,j,exp,m);
                    completa_conversao_QDD_matriz(n->at.m.th,naux,i+exp,j,exp,m);
                    libera_no(naux);
                    break;

                case C:
                    naux = cria_no_meio(R,1+n->at.m.nivel);
                    completa_conversao_QDD_matriz(n->at.m.el,naux,i,j    ,exp/2,m);
                    completa_conversao_QDD_matriz(n->at.m.th,naux,i,j+exp,exp/2,m);
                    libera_no(naux);
                    break;
            }
        }
        else
        {
            switch(nesp->at.m.classe)
            {
                case R:
                    naux = cria_no_meio(C,n->at.m.nivel);
                    completa_conversao_QDD_matriz(n,naux,i    ,j,exp,m);
                    completa_conversao_QDD_matriz(n,naux,i+exp,j,exp,m);
                    libera_no(naux);
                    break;

                case C:
                    naux = cria_no_meio(R,1+n->at.m.nivel);
                    completa_conversao_QDD_matriz(n,naux,i,j    ,exp/2,m);
                    completa_conversao_QDD_matriz(n,naux,i,j+exp,exp/2,m);
                    libera_no(naux);
                    break;
            }
        }
    }
}

double** converte_QDD_matriz(QDD *Q)
{
    Long i;

    Long exp;
    exp = (Long)pow(2,Q->nqbit);

    double **m;
    m = malloc(exp*sizeof(double*));
    if(m == NULL)
        ERRO("CONVERTE QDD MATRIZ M");
    aumenta_memoria(exp*sizeof(double*));
    for(i = 0; i < exp; i++)
    {
        m[i] = malloc(2*exp*sizeof(double));
        if(m[i] == NULL)
            ERRO("CONVERTE QDD MATRIZ M[]");
        aumenta_memoria(2*exp*sizeof(double));
    }

    no *naux;
    naux = cria_no_meio(R,0);
    completa_conversao_QDD_matriz(Q->n->at.i.n,naux,0,0,exp/2,m);
    libera_no(naux);

    return m;
}

void reduz_QDD(QDD *Q, Short ex)
{
    reduz_lista_fim(Q->l,ex);
    lista *l, *lf;
    l = copia_lista_sem_cabeca(Q->l);
    lf = acha_fim_lista(l);

    no *nc, *n1, *n2;
    lista *lnc1, *lnc2, *lr, *lrc;
    Short mudou;
    while(l != NULL)
    {
        nc = l->n;

        /* Regra 1 */
        do
        {
            mudou = 0;

            n1 = nc->l->n;

            while(n1->at.m.el == n1->at.m.th)
            {
                if(n1->tipo == Inicio)
                    break;

                desconecta_DOIS(n1);
                transfere_conexao(nc,n1);
                libera_no(n1);
                n1 = nc->l->n;
            }
            if(n1->tipo == Inicio)
                break;

            lnc2 = nc->l;
            lnc1 = lnc2->l;

            while(lnc1 != NULL)
            {
                n1 = lnc1->n;
                if(n1->at.m.el == n1->at.m.th)
                {
                    mudou = 1;
                    desconecta_DOIS(n1);
                    transfere_conexao(nc,n1);
                    libera_no(n1);
                    lnc1 = lnc2->l;
                }
                else
                {
                    lnc2 = lnc1;
                    lnc1 = lnc1->l;
                }
            }
        }
        while(mudou);

        /* Regra 2 */
        lnc1 = nc->l;
        while(lnc1->l != NULL)
        {
            n1 = lnc1->n;

            lnc2 = lnc1->l;
            while(lnc2 != NULL)
            {
                n2 = lnc2->n;

                if(compara_no_meio_completo(n1,n2))
                {
                    lr = cria_lista();
                    lrc = cria_lista();

                    lr->n = n1;
                    lr->l = lrc;
                    lrc->n = n2;

                    while(lnc2->l != NULL)
                    {
                        lnc2 = lnc2->l;

                        n2 = lnc2->n;
                        if(compara_no_meio_completo(n1,n2))
                        {
                            lrc->l = cria_lista();
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

                        lr->l = lrc->l;

                        libera_no(n2);
                        libera_lista_no(lrc);
                    }
                    libera_lista_no(lr);

                    lf->l = cria_lista();
                    lf = lf->l;
                    lf->n = n1;

                    break;
                }
                else
                {
                    lnc2 = lnc2->l;
                }
            }
            if(lnc1->l == NULL)
                break;
            lnc1 = lnc1->l;
        }

        lnc1 = l->l;
        libera_lista_no(l);
        l = lnc1;
    }
}

void reduz_arvore(no **n, Short ex)
{
    no *ni, *n0;
    n0 = *n;
    switch(n0->tipo)
    {
        case Inicio:
            ni = Qred->n;

            Qred->n = n0;
            Qred->l = acha_lista_fim_arvore(n0);
            reduz_QDD(Qred,ex);

            Qred->n = ni;
            break;

        case Meio:
            transfere_conexao(nzero,n0);

            conecta_UM(Qred->n,n0,Inicio);
            Qred->l = acha_lista_fim_arvore(n0);
            reduz_QDD(Qred,ex);

            n0 = Qred->n->at.i.n;
            desconecta_DOIS(Qred->n);
            transfere_conexao(n0,nzero);
            break;
    }
    *n = n0;
    libera_lista_lista(Qred->l);
}



/**  apply esqueleto  **/

void encaixa_apply(apply *a, apply *ac, Short lado)
{
    apply *aa;
    aa = NULL;
    switch(lado)
    {
        case Else:
            aa = ac->a1;
            break;

        case Then:
            aa = ac->a2;
            break;

        default:
            ERRO("ENCAIXA APPLY| LADO INVALIDO");
            break;
    }

    apply *ae;
    for(ae = a; ae != NULL; ae = ae->a)
        if(compara_apply(aa,ae))
            break;

    if(ae == NULL)
    {
        aa->a = ac->a;
        ac->a = aa;
    }
    else
    {
        switch(lado)
        {
            case Else:
                ac->a1 = ae;
                break;

            case Then:
                ac->a2 = ae;
                break;
        }
        libera_apply_no(aa);
    }
}

void monta_apply(apply *a, Short regra)
{
    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    no *n;
    Short avanco;
    n = NULL;
    avanco = 4;
    switch(regra)
    {
        case 0:
            n = cria_no_inicio();
            avanco = 0;
            break;

        case 1:
            n = copia_no(n1);
            avanco = 1;
            break;

        case 2:
            n = copia_no(n2);
            avanco = 2;
            break;

        case 3:
            n = cria_no_meio(V,n1->at.m.nivel);
            avanco = 1;
            break;

        case 4:
            n = cria_no_meio(V,n2->at.m.nivel);
            avanco = 2;
            break;

        case 5:
            n = cria_no_meio(V,n1->at.m.nivel);
            avanco =  3;
            break;

        case 6:
            n = produto_no_no(n1,n2);
            break;

        case 7:
            n = produto_no_conjugado_no(n1,n2);
            break;

        case 8:
            n = cria_no_fim(0,0);
            break;

        case 9:
            n = soma_no(n1,n2);
            break;

        case 10:
            n = copia_no(n1);
            avanco = 3;
            break;

        case 11:
            n = cria_no_meio(C,n2->at.m.nivel);
            avanco = 2;
            break;

        default:
            ERRO("MONTA PPLY| REGRA NAO DEFINIDA");
            break;
    }
    a->n = n;

    apply *a1, *a2;
    a1 = NULL;
    a2 = NULL;
    switch(avanco)
    {
        case 0:
            a1 = cria_apply();
            a1->n1 = n1->at.i.n;
            a1->n2 = n2->at.i.n;
            break;

        case 1:
            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2;
            break;

        case 2:
            a1 = cria_apply();
            a1->n1 = n1;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1;
            a2->n2 = n2->at.m.th;
            break;

        case 3:
            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2->at.m.th;
            break;
    }
    a->a1 = a1;
    a->a2 = a2;
}

no* apply_base(no *n1, no *n2, Short (*regra_apply)(apply *a))
{
    apply *a;
    a = cria_apply();
    a->n1 = n1;
    a->n2 = n2;

    no *n;
    apply *ac;
    Short regra;
    ac = a;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        regra = regra_apply(ac);
        monta_apply(ac,regra);

        n = ac->n;
        switch(n->tipo)
        {
            case Inicio:
                ac->a = ac->a1;
                break;

            case Meio:
                encaixa_apply(a,ac,Then);
                encaixa_apply(a,ac,Else);
                break;
        }
    }

    apply *a1, *a2;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n;
        switch(n->tipo)
        {
            case Inicio:
                a1 = ac->a1;
                n1 = a1->n;

                conecta_UM(n,n1,Inicio);
                break;

            case Meio:
                a1 = ac->a1;
                a2 = ac->a2;

                n1 = a1->n;
                n2 = a2->n;

                conecta_DOIS(n,n1,n2);
                break;
        }
    }

    n = a->n;
    libera_apply_lista(a);

    return n;
}



/**  regra apply  **/

Short regra_apply_soma(apply *a)
{
    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    switch(n1->tipo)
    {
        case Inicio:
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY SOMA| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            switch(n2->tipo)
            {
                case Inicio:
                    ERRO("REGRA APPLY SOMA| N1 E MEIO N2 E INICIO");
                    break;

                case Meio:
                    if(n1->at.m.nivel < n2->at.m.nivel)
                        return 1;
                    if(n1->at.m.nivel > n2->at.m.nivel)
                        return 2;
                    if(n1->at.m.nivel == n2->at.m.nivel)
                    {
                        switch(n1->at.m.classe)
                        {
                            case C:
                                if(n2->at.m.classe == C)
                                    return 10;
                                return 2;
                                break;

                            case V:
                                switch(n2->at.m.classe)
                                {
                                    case C:
                                        return 1;
                                        break;

                                    case V:
                                        return 10;
                                        break;

                                    case R:
                                        return 2;
                                        break;
                                }
                                break;

                            case R:
                                if(n2->at.m.classe == R)
                                    return 10;
                                return 1;
                                break;
                        }
                    }
                    break;

                case Fim:
                    return 1;
                    break;
            }
            break;

        case Fim:
            switch(n2->tipo)
            {
                case Inicio:
                    ERRO("REGRA APPLY SOMA| N1 E FIM N2 E INICIO");
                    break;

                case Meio:
                    return 2;
                    break;

                case Fim:
                    return 9;
                    break;
            }
            break;
    }
    ERRO("REGRA APPLY SOMA| NAO ATIVOU NENHUMA REGRA");
    return 0;
}

Short regra_apply_produto_matriz_matriz(apply *a)
{
    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    switch(n1->tipo)
    {
        case Inicio:
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            switch(n2->tipo)
            {
                case Inicio:
                    ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E MEIO N2 INICIO");
                    break;

                case Meio:
                    if(n1->at.m.nivel < n2->at.m.nivel)
                    {
                        switch(n1->at.m.classe)
                        {
                            case V:
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E VETOR 1");
                                break;

                            case R:
                                return 1;
                                break;

                            case C:
                                return 3;
                                break;
                        }
                    }
                    if(n1->at.m.nivel == n2->at.m.nivel)
                    {
                        switch(n1->at.m.classe)
                        {
                            case V:
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E VETOR 2");
                                break;

                            case R:
                                return 1;
                                break;

                            case C:
                                switch(n2->at.m.classe)
                                {
                                    case V:
                                        ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N2 E VETOR 1");
                                        break;

                                    case R:
                                        return 5;
                                        break;

                                    case C:
                                        return 3;
                                        break;
                                }
                                break;
                        }
                    }
                    if(n1->at.m.nivel > n2->at.m.nivel)
                    {
                        switch(n2->at.m.classe)
                        {
                            case V:
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N2 E VETOR 2");
                                break;

                            case R:
                                return 4;
                                break;

                            case C:
                                return 2;
                                break;
                        }
                    }
                    break;

                case Fim:
                    if(compara_no_fim_zero(n2,1))
                    {
                        return 8;
                    }
                    else
                    {
                        switch(n1->at.m.classe)
                        {
                            case V:
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E VETOR 3");
                                break;

                            case R:
                                return 1;
                                break;

                            case C:
                                return 3;
                                break;
                        }
                    }
                    break;

            }
            break;

        case Fim:
            if(compara_no_fim_zero(n1,1))
            {
                return 8;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Inicio:
                        ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E FIM N2 INICIO");
                        break;

                    case Meio:
                        switch(n2->at.m.classe)
                        {
                            case V:
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N2 E VETOR 3");
                                break;

                            case R:
                                return 4;
                                break;

                            case C:
                                return 2;
                                break;
                        }
                        break;

                    case Fim:
                        if(compara_no_fim_zero(n2,1))
                            return 8;
                        else
                            return 6;
                        break;
                }
            }
            break;
    }
    ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| NAO ACIOONOU NENHUMA REGRA");
    return 0;
}

Short regra_apply_produto_matriz_vetor(apply *a)
{
    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    switch(n1->tipo)
    {
        case Inicio:
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            switch(n2->tipo)
            {
                case Inicio:
                    ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 E MEIO N2 E INICIO");
                    break;

                case Meio:
                    if(n2->at.m.classe != V)
                        ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N2 TEM R OU C 1");

                    if(n1->at.m.nivel < n2->at.m.nivel)
                    {
                        switch(n1->at.m.classe)
                        {
                                case V:
                                    ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 TEM V 1");
                                    break;

                                case C:
                                    return 1;
                                    break;

                                case R:
                                    return 3;
                                    break;
                        }
                    }
                    if(n1->at.m.nivel == n2->at.m.nivel)
                    {
                        switch(n1->at.m.classe)
                        {
                                case V:
                                    ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 TEM V 2");
                                    break;

                                case C:
                                    return 10;
                                    break;

                                case R:
                                    return 3;
                                    break;
                        }
                    }
                    if(n1->at.m.nivel > n2->at.m.nivel)
                    {
                        return 11;
                    }
                    break;

                case Fim:
                    if(compara_no_fim_zero(n2,1))
                    {
                        return 8;
                    }
                    else
                    {
                        switch(n1->at.m.classe)
                        {
                            case V:
                                ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 TEM V 3");
                                break;

                            case C:
                                return 1;
                                break;

                            case R:
                                return 3;
                                break;
                        }
                    }
                    break;
            }
            break;

        case Fim:
            if(compara_no_fim_zero(n1,1))
            {
                return 8;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Inicio:
                        ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 E FIM N2 E INICIO");
                        break;

                    case Meio:
                        if(n2->at.m.classe != V)
                            ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N2 TEM R OU C 2");

                        return 11;
                        break;

                    case Fim:
                        if(compara_no_fim_zero(n2,1))
                            return 8;
                        else
                            return 6;
                        break;
                }
            }
            break;
    }
    ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| NAO ATIVOU NENHUMA REGRA");
    return 0;
}

Short regra_apply_produto_vetor_vetor(apply *a)
{
    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    switch(n1->tipo)
    {
        case Inicio:
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY PRODUTO VETOR VETOR| N1 E INICIO E N2 NAO");

            return 0;
            break;

        case Meio:
            if(n1->at.m.classe != V)
                ERRO("REGRA APPLY PRODUTO VETOR VETOR| Q1 NAO E VETOR");

            switch(n2->tipo)
            {
                case Inicio:
                    ERRO("REGRA APPLY PRODUTO VETOR VETOR| N2 E MEIO N2 INICIO");
                    break;

                case Meio:
                    if(n1->at.m.classe != V)
                        ERRO("REGRA APPLY PRODUTO VETOR VETOR| Q2 NAO E VETOR");

                    if(n1->at.m.nivel < n2->at.m.nivel)
                        return 1;
                    if(n1->at.m.nivel == n2->at.m.nivel)
                        return 5;
                    if(n1->at.m.nivel > n2->at.m.nivel)
                        return 2;
                    break;

                case Fim:
                    if(compara_no_fim_zero(n2,1))
                        return 8;
                    else
                        return 1;
                    break;

            }
            break;

        case Fim:
            if(compara_no_fim_zero(n1,1))
            {
                return 8;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Inicio:
                        ERRO("REGRA APPLY PRODUTO VETOR VETOR| N1 E FIM N2 INICIO");
                        break;

                    case Meio:
                        return 2;
                        break;

                    case Fim:
                        if(compara_no_fim_zero(n2,1))
                            return 8;
                        else
                            return 7;
                        break;
                }
            }
            break;
    }
    ERRO("REGRA APPLY PRODUTO VETOR VETOR| NAO ACIONOU NENHUMA REGRA");
    return 0;
}



/**  apply pronto  **/

no* apply_soma(no *n1, no *n2)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_soma);
    return n;
}

no* apply_produto_matriz_matriz(no *n1, no *n2)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_matriz_matriz);
    return n;
}

no *apply_produto_matriz_vetor(no *n1, no *n2)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_matriz_vetor);
    return n;
}

no* apply_produto_vetor_vetor(no *n1, no *n2)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_vetor_vetor);
    return n;
}



/**  produto QDD QDD  **/

Short espalha(suporte *s, Short classe)
{
    conta *c;
    c = s->c[classe];
    if(c == NULL)
        return 0;

    no *n;
    n = c->n;

    lista *l;
    l = copia_lista_sem_cabeca(n->l);

    conta *cp;
    cp = cria_conta(0);
    cp->n = n;

    no *ne, *nt1, *nt2;
    lista *lc;
    conta *cc, *cpc, *caux;
    suporte *sc, *saux;
    Short delta, ex, lado;
    while(l != NULL)
    {
        ne = l->n;
        if(ne->tipo == Inicio)
        {
            libera_lista_lista(l);
            libera_conta_lista(cp);
            return 1;
        }

        for(sc = s; sc->s != NULL; sc = sc->s)
            if(sc->s->nivel < ne->at.m.nivel)
                break;

        if(sc->nivel == ne->at.m.nivel)
        {
            /* Tem suporte */
            for(cc = sc->c[ne->at.m.classe]; cc != NULL; cc = cc->c)
                if(cc->n == ne)
                    break;

            if(cc == NULL)
            {
                /* Nao tem o no */
                cc = cria_conta(c->nivel);
                cc->n = ne;
                cc->c = sc->c[ne->at.m.classe];
                sc->c[ne->at.m.classe] = cc;
            }
            else
            {
                /* Já tem o no */
                if(cc->nivel < c->nivel)
                {
                    /* Ajustar novo */
                    delta = c->nivel - cc->nivel;
                    for(cpc = cp; cpc->c != NULL; cpc = cpc->c)
                        if(cp->c->nivel > delta)
                            break;

                    if(cp->nivel == delta)
                    {
                        /* já tem em cp */
                        nt1 = cp->n;
                    }
                    else
                    {
                        /* Não tem em cp */
                        nt1 = copia_arvore(n);
                        ex = pow(2,delta);
                        produto_arvore_real(nt1,ex);

                        caux = cria_conta(delta);
                        caux->c = cpc->c;
                        cpc->c = caux;

                        caux->n = nt1;
                    }
                    lado = desconecta_UM(ne,n);
                    conecta_UM(ne,nt1,lado);

                }
                if(cc->nivel > c->nivel)
                {
                    /* Ajustar velho */
                    if(ne->at.m.el == n)
                        nt1 = ne->at.m.th;
                    else
                        nt1 = ne->at.m.el;

                    nt2 = copia_arvore(nt1);
                    delta = cc->nivel - c->nivel;
                    ex = pow(2,delta);
                    produto_arvore_real(nt2,ex);

                    lado = desconecta_UM(ne,nt1);
                    conecta_UM(ne,nt2,lado);

                    cc->nivel = c->nivel;
                }
            }
        }
        else
        {
            /* Não tem suporte */
            saux = cria_suporte(ne->at.m.nivel);
            saux->s = sc->s;
            sc->s = saux;

            cc = cria_conta(c->nivel);
            cc->n = ne;
            saux->c[ne->at.m.classe] = cc;
        }

        lc = l->l;
        libera_lista_no(l);
        l = lc;
    }

    s->c[classe] = c->c;
    libera_conta_no(c);
    libera_conta_lista(cp);

    return 0;
}

void contrai_conta(conta *c)
{
    no *n, *nc;
    conta *cc;
    for(cc = c; cc != NULL; cc = cc->c)
    {
        n = cc->n;
        nc = apply_soma(n->at.m.el,n->at.m.th);
        reduz_arvore(&nc,2);

        transfere_conexao(nc,n);
        cc->n = nc;
        (cc->nivel)--;

        libera_arvore(n);
    }
}

void contrai(QDD *Q, Short classe)
{
    lista *lc;
    conta *c, *cc;
    c = cria_conta(0);
    cc = c;
    for(lc = Q->l; lc != NULL; lc = lc->l)
    {
        cc->c = cria_conta(Q->nqbit);
        cc = cc->c;
        cc->n = lc->n;
    }

    suporte *s, *saux;
    s = cria_suporte(Q->nqbit);
    if(classe == R)
        s->c[C] = c->c;
    else
        s->c[R] = c->c;
    libera_conta_no(c);

    conta *ci = NULL;
    Short inicio = 0;
    while(s != NULL)
    {
        if(classe == C)
            contrai_conta(s->c[C]);
        while(s->c[C] != NULL)
        {
            inicio = espalha(s,C);
            if(inicio)
                break;
        }
        if(inicio)
        {
            ci = s->c[C];
            break;
        }

        if(classe == V)
            contrai_conta(s->c[V]);
        while(s->c[V] != NULL)
        {
            inicio = espalha(s,V);
            if(inicio)
                break;
        }
        if(inicio)
        {
            ci = s->c[V];
            break;
        }

        if(classe == R)
            contrai_conta(s->c[R]);
        while(s->c[R] != NULL)
        {
            inicio = espalha(s,R);
            if(inicio)
                break;
        }
        if(inicio)
        {
            ci = s->c[R];
            break;
        }

        saux = s->s;
        libera_suporte_no(s);
        s = saux;
    }

    Long ex;
    no *n;
    ex = pow(2,ci->nivel);
    n = ci->n;
    produto_arvore_real(n,ex);
    libera_conta_no(ci);
}

QDD* produto_QDD_QDD(QDD *Q1, QDD *Q2, no* (*apply)(no *n1, no *n2), Short classe)
{
    if(Q1->nqbit != Q2->nqbit)
        ERRO("PRODUTO QDD QDD| QDDs COM QUANTIDADES DIFERENTES DE QBITS");

    no *n;
    n = apply(Q1->n,Q2->n);

    QDD *Q;
    Q = cria_QDD(Q1->nqbit);
    Q->n = n;
    Q->l = acha_lista_fim_QDD(Q);
    reduz_QDD(Q,2);

    contrai(Q,classe);
    libera_lista_lista(Q->l);
    Q->l = acha_lista_fim_QDD(Q);
    reduz_QDD(Q,1);

    return Q;
}



/**  Operações QDD algebricas  **/

void produto_QDD_escalar(QDD *Q, no *n1)
{
    no *n2, *n3;
    lista *l;
    for(l = Q->l; l != NULL; l = l->l)
    {
        n2 = l->n;
        n3 = produto_no_no(n1,n2);
        transfere_conexao(n3,n2);
        libera_no(n2);
        l->n = n3;
    }
}

QDD* produto_tensorial(QDD *Q1, QDD *Q2)
{
    Short nqbit1, nqbit2;
    nqbit1 = Q1->nqbit;
    nqbit2 = Q2->nqbit;

    QDD *Q;
    lista *l;
    Q = copia_QDD(Q1);
    Q->nqbit = nqbit1 + nqbit2;

    QDD *Q2a;
    lista *lc;
    Q2a = copia_QDD(Q2);
    l = enlista_QDD(Q2a);
    for(lc = l; lc != NULL; lc = lc->l)
        if(lc->n->tipo == Meio)
            (lc->n->at.m.nivel) += nqbit1;
    libera_lista_lista(l);

    QDD *Q2b;
    no *n1, *n2, *n3;
    lista *lf;
    l = Q->l;
    Q->l = NULL;
    while(l != NULL)
    {
        n1 = l->n;
        if(compara_no_fim_zero(n1,1))
        {
            lc = l->l;

            l->l = Q->l;
            Q->l = l;
            l = lc;
        }
        else
        {
            Q2b = copia_QDD(Q2a);
            produto_QDD_escalar(Q2b,n1);

            lf = acha_fim_lista(Q2b->l);
            lf->l = Q->l;
            Q->l = Q2b->l;

            n3 = Q2b->n;
            if(n3->tipo != Inicio)
                ERRO("PRODUTO TENSORIAL| NO DEVERIA SER INICIO");
            n2 = n3->at.i.n;
            desconecta_DOIS(n3);
            libera_no(n3);

            transfere_conexao(n2,n1);
            libera_no(n1);
            libera_QDD_no(Q2b);

            lc = l->l;
            libera_lista_no(l);
            l = lc;
        }
    }
    libera_QDD(Q2a);

    reduz_QDD(Q,1);
    return Q;
}

QDD* potencia_tensorial(QDD *Q, Short n)
{
    QDD *Qf, *Qaux;
    Qf = copia_QDD(Q);

    Short j;
    for(j=1; j<n; j++)
    {
        Qaux = produto_tensorial(Qf,Q);
        libera_QDD(Qf);
        Qf = Qaux;
    }
    return Qf;
}

QDD* soma_QDD(QDD *Q1, QDD *Q2)
{
    if(Q1->nqbit != Q2->nqbit)
        ERRO("SOMA QDD| QDDs COM QUANTIDADES DIFERENTES DE QBITS");

    QDD *Q;
    Q = cria_QDD(Q1->nqbit);
    Q->n = apply_soma(Q1->n,Q2->n);

    Q->l = acha_lista_fim_QDD(Q);
    reduz_QDD(Q,1);

    return Q;
}

QDD* produto_matriz_matriz(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    Q = produto_QDD_QDD(Q1,Q2,apply_produto_matriz_matriz,V);
    return Q;
}

QDD* produto_matriz_vetor(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    Q = produto_QDD_QDD(Q1,Q2,apply_produto_matriz_vetor,C);
    return Q;
}

no* produto_vetor_vetor(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    Q = produto_QDD_QDD(Q1,Q2,apply_produto_vetor_vetor,V);

    no *n;
    n = copia_no(Q->l->n);
    libera_QDD(Q);

    return n;
}



/**  QDDs usuais  **/

lista* lista_fim_2(no *nf1, no *nf2)
{
    lista *l1, *l2;
    l1 = cria_lista();
    l2 = cria_lista();

    l1->l = l2;
    l1->n = nf1;
    l2->n = nf2;

    return l1;
}

QDD* matriz_cruzada(no *nf1, no *nf2)
{
    no *ni, *n1, *n2, *n3;
    ni = cria_no_inicio();
    n1 = cria_no_meio(R,0);
    n2 = cria_no_meio(C,0);
    n3 = cria_no_meio(C,0);

    conecta_UM(ni,n1,Inicio);
    conecta_DOIS(n1,n2,n3);
    conecta_DOIS(n2,nf1,nf2);
    conecta_DOIS(n3,nf2,nf1);

    QDD *Q;
    Q = cria_QDD(1);
    Q->n = ni;
    Q->l = lista_fim_2(nf1,nf2);
    return Q;
}

QDD* I()
{
    no *nf1, *nf2;
    nf1 = cria_no_fim(1,0);
    nf2 = cria_no_fim(0,0);

    QDD *Q;
    Q = matriz_cruzada(nf1,nf2);
    return Q;
}

QDD* X()
{
    no *nf1, *nf2;
    nf1 = cria_no_fim(0,0);
    nf2 = cria_no_fim(1,0);

    QDD *Q;
    Q = matriz_cruzada(nf1,nf2);
    return Q;
}

QDD* S()
{
    no *nf1, *nf2;
    nf1 = cria_no_fim(0.5,0.5);
    nf2 = cria_no_fim(0.5,-0.5);

    QDD *Q;
    Q = matriz_cruzada(nf1,nf2);
    return Q;
}

QDD* H()
{
    no *ni, *n1, *n2;
    ni = cria_no_inicio();
    n1 = cria_no_meio(R,0);
    n2 = cria_no_meio(C,0);

    no *nf1, *nf2;
    double re;
    re = pow(2,-0.5);
    nf1 = cria_no_fim( re,0);
    nf2 = cria_no_fim(-re,0);

    conecta_UM(ni,n1,Inicio);
    conecta_DOIS(n1,nf1,n2);
    conecta_DOIS(n2,nf1,nf2);

    QDD *Q;
    Q = cria_QDD(1);
    Q->n = ni;
    Q->l = lista_fim_2(nf1,nf2);

    return Q;
}

QDD* Ro(double theta)
{
    if(theta <  eps)
    if(theta > -eps)
        return I();

    no *ni, *n1, *n2, *n3, *n4, *n5;

    ni = cria_no_inicio();
    n1 = cria_no_meio(R,0);
    conecta_UM(ni,n1,Inicio);

    n2 = cria_no_meio(C,0);
    n3 = cria_no_meio(C,0);
    conecta_DOIS(n1,n2,n3);

    double re, im;
    n1 = n2;
    n2 = n3;
    n3 = cria_no_fim(1,0);
    n4 = cria_no_fim(0,0);
    re = cos(theta);
    im = sin(theta);
    n5 = cria_no_fim(re,im);
    conecta_DOIS(n1,n3,n4);
    conecta_DOIS(n2,n4,n5);

    lista *l1, *l2, *l3;
    l1 = cria_lista();
    l2 = cria_lista();
    l3 = cria_lista();

    l1->n = n3;
    l1->l = l2;
    l2->n = n4;
    l2->l = l3;
    l3->n = n5;

    QDD *Q;
    Q = cria_QDD(1);
    Q->n = ni;
    Q->l = l1;

    return Q;
}

QDD* BASE(Short N, Long n)
{
    no *ni, *n1, *n2, *nf0, *nf1;
    ni = cria_no_inicio();
    nf0 = cria_no_fim(0,0);
    nf1 = cria_no_fim(1,0);

    n1 = cria_no_meio(V,0);
    conecta_UM(ni,n1,Inicio);

    Short i;
    Long ex;
    ex = pow(2,N-1);
    for(i = 1; i < N; i++)
    {
        n2 = cria_no_meio(V,i);
        if(n < ex)
            conecta_DOIS(n1,n2,nf0);
        else
            conecta_DOIS(n1,nf0,n2);

        n1 = n2;
        ex /= 2;
    }
    if(n == 0)
        conecta_DOIS(n1,nf1,nf0);
    else
        conecta_DOIS(n1,nf0,nf1);

    QDD *Q;
    Q = cria_QDD(N);
    Q->n = ni;
    Q->l = lista_fim_2(nf0,nf1);
    return Q;
}

QDD* W(Short N)
{
    no *ni;
    ni = cria_no_inicio();

    no *nf;
    float re;
    re = pow(2,-0.5*N);
    nf = cria_no_fim(re,0);

    lista *l;
    l = cria_lista();
    l->n = nf;

    QDD *Q;
    Q = cria_QDD(N);
    Q->n = ni;
    Q->l = l;

    return Q;
}



/**  Testes  **/

double teste_velocidade_unico(char *nome, QDD* (*le)(char*), FILE *fp, FILE *fr, Short primeiro)
{
    time_t antesT, depoisT, deltaT;
    double tempoT;
    antesT = clock();

    QDD *Q1;
    Q1 = le(nome);

    Long memantes, memdepois;
    if(primeiro)
    {
        mostra_quantidades();
        fmostra_quantidades(fr);
        fprintf(fp,"%llu|%llu|%llu|%llu|",mem,iM,iF,iL);
    }
    memantes = mem;

    time_t antes, depois, delta;
    double tempo, precisao, clk;
    clk = (double)CLOCKS_PER_SEC;

    antes = clock();
    reduz_QDD(Q1,1);
    depois = clock();

    delta = depois - antes;
    tempo = (double)delta/clk;
    precisao = 1/clk;

    if(primeiro)
    {
        mostra_quantidades();
        fmostra_quantidades(fr);
        fprintf(fp,"%llu|%llu|%llu|%llu|",mem,iM,iF,iL);
        printf("\ntempo   1:");
        fprintf(fr,"\ntempo   1:");
    }
    memdepois = mem;
    libera_QDD(Q1);

    double iir = 0.001;

    if(memantes == memdepois)
    {
        tempo = 0;
        printf(" %.3E",tempo);
        fprintf(fr," %.3E",tempo);
        fprintf(fp,"=%E|",tempo);

        depoisT = clock();
        deltaT = depoisT-antesT;
        tempoT = deltaT/clk;
        printf("\t\tTotal: %.3e",tempoT);
        fprintf(fr,"\t\tTotal: %.3e",tempoT);

        precisao = 0;
        return precisao;
    }
    if(precisao/tempo < iir)
    {
        printf(" %.3E",tempo);
        fprintf(fr," %.3E",tempo);
        fprintf(fp,"=%E|",tempo);

        depoisT = clock();
        deltaT = depoisT-antesT;
        tempoT = deltaT/clk;
        printf("\t\tTotal: %.3e",tempoT);
        fprintf(fr,"\t\tTotal: %.3e",tempoT);

        return precisao;
    }

    QDD **Q;
    Long quantidade, i;
    quantidade = 1;

    while(precisao/tempo > iir)
    {
        quantidade *= 10;
        precisao /= 10;

        Q = malloc(quantidade*sizeof(QDD*));
        if(Q == NULL)
            ERRO("TESTE VELOCIDADE UNICO| ALLOCAR Q");
        aumenta_memoria_fora(quantidade*sizeof(QDD*));

        for(i=0; i<quantidade; i++)
            Q[i] = le(nome);

        antes = clock();
        for(i=0; i<quantidade; i++)
            reduz_QDD(Q[i],1);
        depois = clock();

        delta = depois-antes;
        tempo = (double)delta/CLOCKS_PER_SEC;
        tempo /= quantidade;

        for(i=0; i<quantidade; i++)
            libera_QDD(Q[i]);
        diminui_memoria_fora(quantidade*sizeof(QDD*));
        free(Q);
    }

    printf(" %.3E",tempo);
    fprintf(fr," %.3E",tempo);
    fprintf(fp,"=%E|",tempo);

    depoisT = clock();
    deltaT = depoisT-antesT;
    tempoT = deltaT/clk;
    printf("\t\tTotal: %.3e",tempoT);
    fprintf(fr,"\t\tTotal: %.3e",tempoT);

    return precisao;
}

void teste_velocidade_base(char *nomeI, Short limiteinf, Short limitesup, Short amostras, Short arquivo, FILE *fr, QDD* (*le)(char*))
{
    time_t antesT, depoisT, deltaT;
    double tempoT;
    antesT = clock();

    char nomeR[50];
    Short rin = 0;
    if(fr == NULL)
    {
        nomeR[0] = '\0';
        sprintf(nomeR,"RelatorioTesteVelocidade%s%d.txt",nomeI,arquivo);
        fr = fopen(nomeR,"w");
        rin = 1;
    }

    Short i;
    char s[40];
    FILE *fp;
    s[0] = '\0';
    sprintf(s,"RelatorioVelocidade%s%d.csv",nomeI,arquivo);
    fp = fopen(s,"w");
    fprintf(fp,"sep=|\n");
    fprintf(fp,"|mem|iM|iF|iL|mem|iM|iF|iL");
    for(i=1; i<=amostras; i++)
        fprintf(fp,"|%hu",i);
    fprintf(fp,"|implicita\n");



    Short j;
    float precisao;
    char nome[10];
    time_t antesi, depoisi, deltai;
    double tempoi;
    for(i=limiteinf; i<=limitesup; i++)
    {
        antesi = clock();

        nome[0] = '\0';
        strcpy(nome,nomeI);
        sprintf(nome,"%s%d.txt",nomeI,i);
        printf("\n\n\nTestando: %s\n",nome);
        fprintf(fr,"\n\n\nTestando: %s\n",nome);
        configuracao(i);
        fprintf(fp,"%hu|",i);

        precisao = teste_velocidade_unico(nome,le,fp,fr,1);
        for(j=2; j<=amostras; j++)
        {
            printf("\nTempo %3d:",j);
            fprintf(fr,"\nTempo %3d:",j);
            teste_velocidade_unico(nome,le,fp,fr,0);
        }

        fprintf(fp,"=%E\n",precisao);
        printf("\n\nPrecisao: %.3e\n\n",precisao);
        fprintf(fr,"\n\nPrecisao: %.3e\n\n",precisao);
        depoisi = clock();

        deltai = depoisi-antesi;
        tempoi = (double)deltai/CLOCKS_PER_SEC;
        printf("Tempo %s%d: %e\n\n",nomeI,i,tempoi);
        fprintf(fr,"Tempo %s%d: %e\n\n",nomeI,i,tempoi);
    }
    fclose(fp);

    depoisT = clock();
    deltaT = depoisT-antesT;
    tempoT = (double)deltaT/CLOCKS_PER_SEC;
    printf("\n\nTempo total %s: %.3e",nomeI,tempoT);
    fprintf(fr,"\n\nTempo total %s: %.3e",nomeI,tempoT);
    if(rin)
        fclose(fr);
}

void teste_velocidade_matriz(char *nomeI, Short limiteinf, Short limitesup, Short amostras, Short arquivo, FILE *fr)
{
    teste_velocidade_base(nomeI,limiteinf,limitesup,amostras,arquivo,fr,le_matriz);
}

void teste_velocidade_vetor(char *nomeI, Short limiteinf, Short limitesup, Short amostras, Short arquivo, FILE *fr)
{
    teste_velocidade_base(nomeI,limiteinf,limitesup,amostras,arquivo,fr,le_vetor);
}

void teste_curto(Short amostras)
{
    double tempo;
    time_t antes, depois, delta;
    FILE *fr;
    fr = fopen("RelatorioTesteCurto.txt","w");

    antes = clock();
    teste_velocidade_matriz("H",1,9,amostras,1,fr);
    teste_velocidade_matriz("I",1,9,amostras,1,fr);
    teste_velocidade_matriz("QFT",1,10,amostras,1,fr);
    teste_velocidade_vetor("V",1,22,amostras,1,fr);
    depois = clock();

    delta = depois-antes;
    tempo = (double)delta/CLOCKS_PER_SEC;
    printf("\n\nTempo teste curto: %.3e",tempo);
    fprintf(fr,"\n\nTempo teste curto: %.3e",tempo);
    fclose(fr);
}

void teste_longo(Short amostras)
{
    double tempo;
    time_t antes, depois, delta;
    FILE *fr;
    fr = fopen("RelatorioTesteLongo.txt","w");

    antes = clock();
    teste_velocidade_matriz("H",10,11,amostras,2,fr);
    teste_velocidade_matriz("I",10,11,amostras,2,fr);
    teste_velocidade_matriz("QFT",11,12,amostras,2,fr);
    teste_velocidade_vetor("V",23,24,amostras,2,fr);
    depois = clock();

    delta = depois-antes;
    tempo = (double)delta/CLOCKS_PER_SEC;
    printf("\n\nTempo teste longo: %.3e",tempo);
    fprintf(fr,"\n\nTempo teste longo: %.3e",tempo);
    fclose(fr);
}

Short teste_memoria()
{
    Long memt;
    memt = memF + iQ*sizeof(QDD) + (iI+iM+iF)*sizeof(no) + iL*sizeof(lista) + iA*sizeof(apply) + iC*sizeof(conta) + iS*sizeof(suporte);
    if(memt == mem)
    {
        printf("\nRegistro de memoria correto");
        return 1;
    }
    printf("\nRegistro de memoria errado");
    return 0;

}




int main()
{
    inicia_relatorio_memoria(0);
    configuracao(20);
    inicia_structs_globais();
    setlocale(LC_ALL, "Portuguese");
    /***********************************/

    teste_curto(1);

    /***********************************/
    finaliza_structs_globais();
    finaliza_relatorio_memoria();
    return 0;
}
