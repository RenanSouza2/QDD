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
unsigned long long iQ = 0, iI = 0, iM = 0, iF = 0, iL = 0, iA = 0, iC = 0, iS = 0, iP = 0;
unsigned short tQ, tN, tL, tA, tC, tS, tP;
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

struct pilha
{
    char m[30];
    struct pilha *p;
};



/** Typedefs e definitions  **/

typedef struct QDD   QDD;
typedef struct no    no;
typedef struct lista lista;

typedef struct apply   apply;
typedef struct conta   conta;
typedef struct suporte suporte;

typedef struct pilha pilha;

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
    Q = malloc(tQ);
    if(Q == NULL)
        ERRO("CRIA QDD");
    aumenta_memoria(tQ);
    iQ++;

    Q->n = NULL;
    Q->nqbit = nqbit;
    Q->l = NULL;
    return Q;
}

no* cria_no_inicio()
{
    no *n;
    n = malloc(tN);
    if(n == NULL)
        ERRO("CRIA INICIO");
    aumenta_memoria(tN);
    iI++;

    n->tipo = Inicio;
    n->l = NULL;

    n->at.i.n = NULL;

    return n;
}

no* cria_no_meio(Short classe, Short nivel)
{
    no *n;
    n = malloc(tN);
    if(n == NULL)
        ERRO("CRIA MEIO");
    aumenta_memoria(tN);
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
    n = malloc(tN);
    if(n == NULL)
        ERRO("CRIA FIM");
    aumenta_memoria(tN);
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
    l = malloc(tL);
    if(l == NULL)
        ERRO("CRIA LISTA");
    aumenta_memoria(tL);
    iL++;

    l->l = NULL;
    l->n = NULL;
    return l;
}

apply* cria_apply()
{
    apply *a;
    a = malloc(tA);
    if(a == NULL)
        ERRO("CRIA APPLY");
    aumenta_memoria(tA);
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
    c = malloc(tC);
    if(c == NULL)
        ERRO("CRIA CONTA");
    aumenta_memoria(tC);
    iC++;

    c->n = NULL;
    c->nivel = nivel;
    c->c = NULL;

    return c;
}

suporte* cria_suporte(Short nivel)
{
    suporte *s;
    s = malloc(tS);
    if(s == NULL)
        ERRO("CRIA SUPORTE");
    aumenta_memoria(tS);
    iS++;

    s->c[C] = NULL;
    s->c[R] = NULL;
    s->c[V] = NULL;

    s->nivel = nivel;
    s->s = NULL;

    return s;
}

pilha* cria_pilha(char m[30])
{
    pilha *p;
    p = malloc(tP);
    if(p == NULL)
            ERRO("CRIA PILHA");
    aumenta_memoria(tP);
    iP++;

    sprintf(p->m,m);
    p->p = NULL;

    return p;
}



/** Destrutores  **/

void libera_QDD_no(QDD *Q)
{
    diminui_memoria(tQ);
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

    diminui_memoria(tN);
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
    diminui_memoria(tL);
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
    diminui_memoria(tA);
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
    diminui_memoria(tC);
    if(iC == 0)
        ERRO("LIBERA CONTA");
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
    diminui_memoria(tS);
    if(iS == 0)
        ERRO("LIBERA SUPORTE");
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

void libera_pilha(pilha *p)
{
    diminui_memoria(tP);
    if(tP == 0)
        ERRO("LIBERA PILHA");
    iP--;
}



/** Enlistadores  **/

Long conta_items_lista(lista *l)
{
    Long i;
    i = 0;
    lista *lc;
    for(lc = l; lc != NULL; lc = lc->l)
        i++;
    return i;
}

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
                            ERRO("ENLISTA ARVORE| ARVORE DESCONEXA 1");
                    break;

                case Meio:
                    la->n = n->at.m.th;
                    if(la->n == NULL)
                            ERRO("ENLISTA ARVORE| ARVORE DESCONEXA 2");

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
    if(n == NULL)
        return;

    lista *l;
    l = enlista_arvore(n);
    mostra_lista_com_no(l);
    libera_lista_lista(l);
    printf("\n");
}

void mostra_QDD(QDD *Q)
{
    printf("\nEndereco (QDD): %d\n",Q);
    if(Q == NULL)
        return;

    lista *l;
    Short itens;
    l = enlista_QDD(Q);
    itens = conta_items_lista(l);

    printf("NQBIT: %d\n",Q->nqbit);
    printf("Itens: %hu\n",itens);
    mostra_lista_com_no(l);
    printf("\n\nAMPLITUDES");
    mostra_lista_com_no(Q->l);
    libera_lista_lista(l);
    printf("\n");
}

void mostra_apply_no(apply *a)
{
    printf("\nEndereco (apply): %d\n",a);
    if(a == NULL)
        return;

    printf("\nNo 1:");
    mostra_no(a->n1);
    printf("\nNo 2:");
    mostra_no(a->n2);
    printf("\nNo:");
    mostra_no(a->n);
    printf("\n");
    printf("\na1: %d",a->a1);
    printf("\na2: %d",a->a2);
    printf("\n\na proximo:  %d\n",a->a);
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
    printf("\nEndereco (conta): %d\n",c);
    if(c == NULL)
        return;

    printf("nivel: %d",c->nivel);
    printf("\n\nno: ");
    mostra_no(c->n);
    printf("\nc proximo: %d",c->c);
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
    printf("\nEndereco (suporte): %d\n",s);
    if(s == NULL)
        return;

    printf("nivel: %d",s->nivel);
    printf("\n\ncc: %d",s->c[C]);
    printf("\ncv: %d",s->c[V]);
    printf("\ncr: %d",s->c[R]);
    printf("\n\ns proximo: %d",s->s);
}

void mostra_suporte_no_com_conta(suporte *s)
{
    printf("\nEndereco (suporte): %d",s);
    if(s == NULL)
        return;

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
    if(iP != 0)
    {
        vazio = 0;
        printf("\np:   %llu",iP);
    }
    if(vazio)
        printf("\nTUDO ZERADO");
    printf("\n");
}

void mostra_tamanhos()
{
    printf("\nTAMANHOS");
    printf("\nQDD: %d",tQ);
    printf("\nn:   %d",tN);
    printf("\nl:   %d",tL);
    printf("\na:   %d",tA);
    printf("\nc:   %d",tC);
    printf("\ns:   %d",tS);
    printf("\np:   %d",tP);
    printf("\n");
}

void mostra_configuracao()
{
    printf("\nConfiguracao: ");
    printf("\nNqbit: %hu",Nqbit);
    printf("\nMax: %lld",MAX);
    printf("\neps: %.3e",eps);
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
    fprintf(fp,"\n");
}

void fmostra_arvore(FILE *fp, no *n)
{
    if(n == NULL)
        return;

    lista *l;
    l = enlista_arvore(n);
    fmostra_lista_com_no(fp,l);
    libera_lista_lista(l);
    fprintf(fp,"\n");
}

void fmostra_QDD(FILE *fp, QDD *Q)
{
    fprintf(fp,"\nEndereco (QDD): %d\n",Q);
    if(Q == NULL)
        return;

    lista *l;
    Short itens;
    l = enlista_QDD(Q);
    itens = conta_items_lista(l);

    fprintf(fp,"NQBIT: %d\n",Q->nqbit);
    fprintf(fp,"Itens: %hu\n",itens);
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
    if(fp == NULL)
        ERRO("FMOSTRA QDD SOZINHO| NAO ABRIU ARQUIVO");

    fmostra_QDD(fp,Q);
    fclose(fp);
}

void fmostra_apply_no(FILE *fp, apply *a)
{
    fprintf(fp,"\nEndereco (apply): %d\n",a);
    if(a == NULL)
        return;

    fprintf(fp,"\nNo 1:");
    fmostra_no(fp,a->n1);
    fprintf(fp,"\nNo 2:");
    fmostra_no(fp,a->n2);
    fprintf(fp,"\nNo:");
    fmostra_no(fp,a->n);
    fprintf(fp,"\n");
    fprintf(fp,"\na1: %d",a->a1);
    fprintf(fp,"\na2: %d",a->a2);
    fprintf(fp,"\n\na proximo:  %d\n",a->a);
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

void fmostra_apply_lista_sozinho(apply *a, char *arquivo)
{
    FILE *fp;
    fp = fopen(arquivo,"w");
    if(fp == NULL)
        ERRO("FMOSTRA APPLY LISTA SOZINHO| NAO ABRIU ARQUIVO");

    fmostra_apply_lista(fp,a);
    fclose(fp);
}

void fmostra_conta_no(FILE *fp, conta *c)
{
    fprintf(fp,"\nEndereco (conta): %d\n",c);
    if(c == NULL)
        return;

    fprintf(fp,"nivel: %d",c->nivel);
    fprintf(fp,"\n\nno: ");
    fmostra_no(fp,c->n);
    fprintf(fp,"\nc proximo: %d",c->c);
}

void fmostra_conta_lista(FILE *fp, conta *c)
{
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
    fprintf(fp,"\nEndereco (suporte): %d\n",s);
    if(s == NULL)
        return;

    fprintf(fp,"nivel: %d",s->nivel);
    fprintf(fp,"\n\ncc: %d",s->c[C]);
    fprintf(fp,"\ncv: %d",s->c[V]);
    fprintf(fp,"\ncr: %d",s->c[R]);
    fprintf(fp,"\n\ns proximo: %d",s->s);
}

void fmostra_suporte_no_com_conta(FILE *fp, suporte *s)
{
    fprintf(fp,"\nEndereco (suporte): %d",s);
    if(s == NULL)
        return;

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
        fprintf(fp,"\nMem: %llu",mem);
    }
    if(memF != 0)
    {
        vazio = 0;
        fprintf(fp,"\nMem: %llu",memF);
    }
    if(iQ != 0)
    {
        vazio = 0;
        fprintf(fp,"\nQDD: %llu",iQ);
    }
    if(iI != 0)
    {
        vazio = 0;
        fprintf(fp,"\ni:   %llu",iI);
    }
    if(iM != 0)
    {
        vazio = 0;
        fprintf(fp,"\nm:   %llu",iM);
    }
    if(iF != 0)
    {
        vazio = 0;
        fprintf(fp,"\nf:   %llu",iF);
    }
    if(iL != 0)
    {
        vazio = 0;
        fprintf(fp,"\nl:   %llu",iL);
    }
    if(iA != 0)
    {
        vazio = 0;
        fprintf(fp,"\na:   %llu",iA);
    }
    if(iC != 0)
    {
        vazio = 0;
        fprintf(fp,"\nc:   %llu",iC);
    }
    if(iS != 0)
    {
        vazio = 0;
        fprintf(fp,"\ns:   %llu",iS);
    }
    if(vazio)
        fprintf(fp,"\nTUDO ZERADO");
    fprintf(fp,"\n");
}

void fmostra_tamanhos(FILE *fp)
{
    fprintf(fp,"\nTAMANHOS");
    fprintf(fp,"\nQDD: %d",tQ);
    fprintf(fp,"\nn:   %d",tN);
    fprintf(fp,"\nl:   %d",tL);
    fprintf(fp,"\na:   %d",tA);
    fprintf(fp,"\nc:   %d",tC);
    fprintf(fp,"\ns:   %d",tS);
    fprintf(fp,"\n");
}

void fmostra_configuracao(FILE *fp)
{
    fprintf(fp,"\nConfiguracao: ");
    fprintf(fp,"\nNqbit: %hu",Nqbit);
    fprintf(fp,"\nMax: %lld",MAX);
    fprintf(fp,"\neps: %.3e",eps);
}




/** Conexões  **/

void conecta_UM(no *n1, no *n2, Short lado)
{
    if(n1 == NULL)
        ERRO("CONECTA UM| N1 E NULL");
    if(n2 == NULL)
        ERRO("CONECTA UM| N2 E NULL");
    if(n1->tipo == Fim)
        ERRO("CONECTA UM| FIM NAO TEM SUCESSORES");
    if(n2->tipo == Inicio)
        ERRO("CONECTA UM| INICIO NAO TEM ANTECESSORES");
    if(n1 == n2)
        ERRO("CONECTA UM| N1 E N2 SAO IGUAIS");

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
    if(n == NULL)
        ERRO("CONECTA DOIS| N E VAZIO");
    if(n->tipo == Inicio)
        ERRO("INICIO NAO CONECTA DOIS");

    conecta_UM(n,el,Else);
    conecta_UM(n,th,Then);
}

Short desconecta_UM(no *n1, no *n2)
{
    if(n1 == NULL)
        ERRO("DESCONECTA UM| N1 E NULL");
    if(n2 == NULL)
        ERRO("DESCONECTA UM| N2 E NULL");
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
    if(n == NULL)
        ERRO("DESCONECTA DOIS| N E VAZIO");
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
    if(n1 == NULL)
        ERRO("TRANSFERE CONEXAO| N1 E VAZIO");
    if(n2 == NULL)
        ERRO("TRANSFERE CONEXAO| N2 E VAZIO");
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



/**  auxiliar QDDs usuais  **/

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

QDD* matriz_delta_kronecker(Short r, Short c)
{
    no *ni;
    ni = cria_no_inicio();

    no *nmr, *nmc;
    nmr = cria_no_meio(R,0);
    nmc = cria_no_meio(C,0);

    no *nf0, *nf1;
    nf0 = cria_no_fim(0,0);
    nf1 = cria_no_fim(1,0);

    conecta_UM(ni,nmr,Inicio);
    if(r == 0)
        conecta_DOIS(nmr,nmc,nf0);
    else
        conecta_DOIS(nmr,nf0,nmc);

    if(c == 0)
        conecta_DOIS(nmc,nf1,nf0);
    else
        conecta_DOIS(nmc,nf0,nf1);

    lista *l;
    l = lista_fim_2(nf0,nf1);

    QDD *Q;
    Q = cria_QDD(1);
    Q->n = ni;
    Q->l = l;
    return Q;
}



/** QDDs usuais  **/

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



/**  estruturas globais  **/

QDD *QH, *QI, *QX, *QS, *Q00, *Q01, *Q10, *Q11;
QDD *Qred;
no  *nzero;
lista *lzero;

void inicia_structs_globais()
{
    tQ = sizeof(QDD);
    tN = sizeof(no);
    tL = sizeof(lista);
    tA = sizeof(apply);
    tC = sizeof(conta);
    tS = sizeof(suporte);
    tP = sizeof(pilha);

    Qred  = cria_QDD(1);
    Qred->n = cria_no_inicio();
    nzero = cria_no_fim(0,0);

    QH = H();
    QI = I();
    QX = X();
    QS = S();

    Q00 = matriz_delta_kronecker(0,0);
    Q01 = matriz_delta_kronecker(0,1);
    Q10 = matriz_delta_kronecker(1,0);
    Q11 = matriz_delta_kronecker(1,1);

    lzero = cria_lista();

    mem -= 9*tQ+45*tN+63*tL;
    iQ -= 9;
    iI -= 9;
    iM -= 19;
    iF -= 17;
    iL -= 63;
}

void finaliza_structs_globais()
{
    mem += 9*tQ+45*tN+63*tL;
    iQ += 9;
    iI += 9;
    iM += 19;
    iF += 17;
    iL += 63;

    libera_no(Qred->n);
    libera_QDD_no(Qred);
    libera_no(nzero);

    libera_QDD(QH);
    libera_QDD(QI);
    libera_QDD(QX);
    libera_QDD(QS);

    libera_QDD(Q00);
    libera_QDD(Q01);
    libera_QDD(Q10);
    libera_QDD(Q11);

    libera_lista_no(lzero);
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

Short compara_no_meio_completo(no *n1, no *n2, Short classe)
{
    if(n1 != n2)
    if(compara_no_meio_parcial(n1,n2))
    {
        if(n1->at.m.el == n2->at.m.el)
        if(n1->at.m.th == n2->at.m.th)
            return 1;
        if(n1->at.m.classe == classe)
        if(n1->at.m.el == n2->at.m.th)
        if(n1->at.m.th == n2->at.m.el)
            return 1;
    }
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
    if(n->tipo != Fim)
        ERRO("COMPARA NO FIM ZERO| TIPO DE NO ERRADO");

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



/**  Reduz  **/

void reduz_lista_fim(lista *l, Short ex)
{
    lista *lc1, *lc2, *laux;

    lc1 = l;
    do
    {
        if(lc1->n->tipo != Fim)
            ERRO("REDUZ LISTA FIM| NO DA LISTA NAO E FIM 1");

        lc2 = lc1;
        while(lc2->l != NULL)
        {
            if(lc2->l->n->tipo != Fim)
                ERRO("REDUZ LISTA FIM| NO DA LISTA NAO E FIM 1");

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

void reduz_QDD(QDD *Q, Short ex, Short classe)
{
    reduz_lista_fim(Q->l,ex);
    lista *l, *lf;
    l = copia_lista_sem_cabeca(Q->l);
    lf = acha_fim_lista(l);

    no *nc, *n1, *n2;
    lista *lnc1, *lnc2, *lnc3, *lnc4, *lr, *lrc;
    Short mudou;
    while(l != NULL)
    {
        nc = l->n;

        /* Regra 1 */
        do
        {
            mudou = 0;

            n1 = nc->l->n;

            lnc3 = NULL;
            lnc4 = NULL;
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
            lnc4 = lnc2;

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
                if(lnc1 == lnc3)
                    break;
            }
            lnc3 = lnc4;
        }
        while(mudou);

        /* Regra 2 */
        lnc1 = nc->l;
        while(lnc1->l != NULL)
        {
            n1 = lnc1->n;

            lnc2 = nc->l;
            while(lnc2 != NULL)
            {
                if(lnc2 == lnc1)
                {
                    lnc2 = lnc2->l;
                    continue;
                }

                n2 = lnc2->n;
                if(n1 == n2)
                    ERRO("REDUZ QDD| REDUDANCIA TIPO 1 DEVERIA TER SIDO ELIMINADA");

                if(compara_no_meio_completo(n1,n2,classe))
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
                        if(compara_no_meio_completo(n1,n2,classe))
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
            reduz_QDD(Qred,ex,4);

            Qred->n = ni;
            libera_lista_lista(Qred->l);
            break;

        case Meio:
            transfere_conexao(nzero,n0);

            conecta_UM(Qred->n,n0,Inicio);
            Qred->l = acha_lista_fim_arvore(n0);
            reduz_QDD(Qred,ex,4);

            n0 = Qred->n->at.i.n;
            desconecta_DOIS(Qred->n);
            transfere_conexao(n0,nzero);
            *n = n0;
            libera_lista_lista(Qred->l);
            break;
    }
}



/**  apply esqueleto  **/

void encaixa_apply(apply *a, apply *ac, Short lado)
{
    apply *ae;
    ae = NULL;
    switch(lado)
    {
        case Inicio:
            ae = ac->a1;
            ac->a = ae;
            return;
            break;

        case Else:
            ae = ac->a1;
            break;

        case Then:
            ae = ac->a2;
            break;

        default:
            ERRO("ENCAIXA APPLY| LADO INVALIDO");
            break;
    }
    if(ae == NULL)
        ERRO("ENCAIXA APPLY| LADO ESCOLHIDO VAZIO");

    apply *aaux;
    for(aaux = a; aaux != ac; aaux = aaux->a)
        if(compara_apply(aaux,ae))
            break;

    if(aaux == ac)
    {
        ae->a = ac->a;
        ac->a = ae;
    }
    else
    {
        switch(lado)
        {
            case Else:
                ac->a1 = aaux;
                break;

            case Then:
                ac->a2 = aaux;
                break;
        }
        libera_apply_no(ae);
    }
}

void monta_apply(apply *a, Short regra)
{
    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    Short i, j;
    i=0;
    j=0;
    if(n1->tipo == Meio)
        i = n1->at.m.nivel;
    if(n2->tipo == Meio)
        j = n2->at.m.nivel;

    no *n;
    Short avanco;
    avanco = 4;
    switch(regra)
    {
        case 0:
            n = cria_no_inicio();
            avanco = 0;
            break;

        case 1:
            n = cria_no_meio(R,i);
            avanco = 1;
            break;

        case 2:
            n = cria_no_meio(R,j);
            avanco = 2;
            break;

        case 3:
            n = cria_no_meio(R,i);
            avanco = 3;
            break;

        case 4:
            n = cria_no_meio(C,i);
            avanco = 1;
            break;

        case 5:
            n = cria_no_meio(C,j);
            avanco = 2;
            break;

        case 6:
            n = cria_no_meio(C,i);
            avanco = 3;
            break;

        case 7:
            n = cria_no_meio(V,i);
            avanco = 1;
            break;

        case 8:
            n = cria_no_meio(V,j);
            avanco = 2;
            break;

        case 9:
            n = cria_no_meio(V,i);
            avanco = 3;
            break;

        case 10:
            n = soma_no(n1,n2);
            break;

        case 11:
            n = produto_no_no(n1,n2);
            break;

        case 12:
            n = produto_no_conjugado_no(n1,n2);
            break;

        case 13:
            n = cria_no_fim(0,0);
            break;

        default:
            ERRO("MONTA APPLY| REGRA INDEFINIDA");
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

no* apply_base(no *n1, no *n2, Short(*regra_apply)(apply*))
{
    if(n1 == NULL)
        ERRO("APPLY BASE| N1 E NULL");
    if(n2 == NULL)
        ERRO("APPLY BASE| N2 E NULL");

    apply *a;
    a = cria_apply();
    a->n1 = n1;
    a->n2 = n2;

    no *n;
    apply *ac;
    Short regra;
    ac = a;
    while(ac != NULL)
    {
        regra = regra_apply(ac);
        monta_apply(ac,regra);

        n = ac->n;
        switch(n->tipo)
        {
            case Inicio:
                encaixa_apply(a,ac,Inicio);
                break;

            case Meio:
                encaixa_apply(a,ac,Then);
                encaixa_apply(a,ac,Else);
                break;
        }
        ac = ac->a;
    }

    apply *a1, *a2;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        a1 = ac->a1;
        a2 = ac->a2;

        n = ac->n;
        switch(n->tipo)
        {
            case Inicio:
                n1 = a1->n;

                conecta_UM(n,n1,Inicio);
                break;

            case Meio:
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
    if(a == NULL)
        ERRO("REGRA APPLY SOMA| A E NULL");

    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    if(n1 == NULL)
        ERRO("REGRA APPLY SOMA| N1 E NULL");
    if(n2 == NULL)
        ERRO("REGRA APPLY SOMA| N2 E NULL");

    switch(n1->tipo)
    {
        case Inicio:
            /* N1 e inicio */
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY SOMA| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            /* n1 e meio */
            switch(n2->tipo)
            {
                case Inicio:
                    /* n2 e inicio */
                    ERRO("REGRA APPLY SOMA| N1 E MEIO N2 E FIM");
                    break;

                case Meio:
                    /* n1 e meio n2 e meio */
                    if(n1->at.m.nivel < n2->at.m.nivel)
                    {
                        /* i < j */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                return 7;
                                break;

                            case R:
                                /* n1 e r */
                                return 1;
                                break;

                            case C:
                                /* n1 e c */
                                return 4;
                                break;
                        }
                    }
                    if(n1->at.m.nivel == n2->at.m.nivel)
                    {
                        /* i == j */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                return 9;
                                break;

                            case R:
                                /* n1 e r */
                                switch(n2->at.m.classe)
                                {
                                    case V:
                                        /* n1 e r n2 e v */
                                        ERRO("REGRA APPLY SOMA| QDD 1 E MATRIZ QDD 2 E VETOR 1");
                                        break;

                                    case R:
                                        /* n1 e r n2 e r */
                                        return 3;
                                        break;

                                    case C:
                                        /* n1 e r n2 e c */
                                        return 1;
                                        break;
                                }
                                break;

                            case C:
                                /* n1 e c */
                                switch(n2->at.m.classe)
                                {
                                    case V:
                                        /* n1 e c n2 e v */
                                        ERRO("REGRA APPLY SOMA| QDD 1 E MATRIZ QDD 2 E VETOR 2");
                                        break;

                                    case R:
                                        /* n1 e c n2 e r */
                                        return 2;
                                        break;

                                    case C:
                                        /* n1 e c n2 e c */
                                        return 6;
                                        break;
                                }
                                break;
                        }

                    }
                    if(n1->at.m.nivel > n2->at.m.nivel)
                    {
                        /* i > j */
                        switch(n2->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                return 8;
                                break;

                            case R:
                                /* n1 e r */
                                return 2;
                                break;

                            case C:
                                /* n1 e c */
                                return 5;
                                break;
                        }
                    }
                    break;

                case Fim:
                    /* n1 e meio n2 e fim */
                    switch(n1->at.m.classe)
                    {
                        case V:
                            /* n1 e v */
                            return 7;
                            break;

                        case R:
                            /* n1 e r */
                            return 1;
                            break;

                        case C:
                            /* n1 e c */
                            return 4;
                            break;
                    }
                    break;
            }
            break;

        case Fim:
            /* n1 e fim */
            switch(n2->tipo)
            {
                case Inicio:
                    /*n1 e fim n2 e inicio */
                    ERRO("REGRA APPLY SOMA| N1 E FIM N2 E INICIO");
                    break;

                case Meio:
                    /*n1 e fim n2 e meio */
                    switch(n2->at.m.classe)
                    {
                        case V:
                            /* n1 e v */
                            return 8;
                            break;

                        case R:
                            /* n1 e r */
                            return 2;
                            break;

                        case C:
                            /* n1 e c */
                            return 5;
                            break;
                    }
                    break;

                case Fim:
                    /*n1 e fim n2 e fim */
                    return 10;
                    break;
            }
            break;

    }
    ERRO("REGRA APPLY SOMA| NAO ATIVOU NENHUMA REGRA");
    return 0;
}

Short regra_apply_produto_matriz_matriz(apply *a)
{
    if(a == NULL)
        ERRO("REGRA APPLY SOMA| A E NULL");

    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    if(n1 == NULL)
        ERRO("REGRA APPLY SOMA| N1 E NULL");
    if(n2 == NULL)
        ERRO("REGRA APPLY SOMA| N2 E NULL");

    switch(n1->tipo)
    {
        case Inicio:
            /* N1 e inicio */
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            /* n1 e meio */
            switch(n2->tipo)
            {
                case Inicio:
                    /* n2 e inicio */
                    ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E MEIO N2 E FIM");
                    break;

                case Meio:
                    /* n1 e meio n2 e meio */
                    if(n1->at.m.nivel < n2->at.m.nivel)
                    {
                        /* i < j */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* i < j n1 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| Q1 E VETOR 1");
                                break;

                            case R:
                                /* i < j n1 e r */
                                return 1;
                                break;

                            case C:
                                /* i < j n1 e c */
                                return 7;
                                break;
                        }
                    }
                    if(n1->at.m.nivel == n2->at.m.nivel)
                    {
                        /* i == j */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* i == j n1 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| Q1 E VETOR 2");
                                break;

                            case R:
                                /* i == j n1 e r */
                                return 1;
                                break;

                            case C:
                                /* i == j n1 e c */
                                switch(n2->at.m.classe)
                                {
                                    case V:
                                        /* i == j n1 e c n2 e v */
                                        ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| Q2 E VETOR 1");
                                        break;

                                    case R:
                                        /* i == j n1 e c n2 e r */
                                        return 9;
                                        break;

                                    case C:
                                        /* i == j n1 e c n2 e c */
                                        return 7;
                                        break;
                                }
                                break;
                        }
                    }
                    if(n1->at.m.nivel > n2->at.m.nivel)
                    {
                        /* i > j */
                        switch(n2->at.m.classe)
                        {
                            case V:
                                /* i > j n1 e c n2 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| Q2 E VETOR 2");
                                break;

                            case R:
                                /* i > j n1 e c n2 e r */
                                return 8;
                                break;

                            case C:
                                /* i > j n1 e c n2 e c */
                                return 5;
                                break;
                        }
                    }
                    break;

                case Fim:
                    /* n1 e meio n2 e fim */
                    if(compara_no_fim_zero(n2,1))
                    {
                        /* n1 e meio n2 e zero */
                        return 13;
                    }
                    else
                    {
                        /* n1 e meio n2 e diferente de zero */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| Q1 E VETOR");
                                break;

                            case R:
                                /* n1 e r */
                                return 1;
                                break;

                            case C:
                                /* n1 e c */
                                return 7;
                                break;
                        }
                    }
                    break;
            }
            break;

        case Fim:
            /* n1 e fim */
            if(compara_no_fim_zero(n1,1))
            {
                /* n1 e xero */
                return 13;
            }
            else
            {
                /*n1 e diferente de zero */
                switch(n2->tipo)
                {
                    case Inicio:
                        /*n1 e fim n2 e inicio */
                        ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| N1 E FIM N2 E INICIO");
                        break;

                    case Meio:
                        /*n1 e fim n2 e meio */
                        switch(n2->at.m.classe)
                        {
                            case V:
                                /* n1 e fim n2 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ| Q2 E VETOR 2");
                                break;

                            case R:
                                /* n1 e c n2 e r */
                                return 8;
                                break;

                            case C:
                                /* n1 e c n2 e c */
                                return 5;
                                break;
                        }
                        break;

                    case Fim:
                        /*n1 e fim n2 e fim */
                        if(compara_no_fim_zero(n2,1))
                            return 13;
                        else
                            return 11;
                        break;
                }
            }
            break;

    }
    ERRO("REGRA APPLY PRODUTO MATRIZ MATRIZ|NAO ATIVOU NENHUMA REGRA");
    return 0;
}

Short regra_apply_produto_matriz_vetor(apply *a)
{
    if(a == NULL)
        ERRO("REGRA APPLY SOMA| A E NULL");

    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    if(n1 == NULL)
        ERRO("REGRA APPLY SOMA| N1 E NULL");
    if(n2 == NULL)
        ERRO("REGRA APPLY SOMA| N2 E NULL");

    switch(n1->tipo)
    {
        case Inicio:
            /* N1 e inicio */
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            /* n1 e meio */
            switch(n2->tipo)
            {
                case Inicio:
                    /* n2 e inicio */
                    ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 E MEIO N2 E FIM");
                    break;

                case Meio:
                    /* n1 e meio n2 e meio */
                    if(n2->at.m.classe != V)
                        ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N2 NAO E VETOR 1");

                    if(n1->at.m.nivel < n2->at.m.nivel)
                    {
                        /* i < j */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 NAO E MATRIZ 1");
                                break;

                            case R:
                                /* n1 e r */
                                return 7;
                                break;

                            case C:
                                /* n1 e c */
                                return 4;
                                break;
                        }
                    }
                    if(n1->at.m.nivel == n2->at.m.nivel)
                    {
                        /* i == j */
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 NAO E MATRIZ 2");
                                break;

                            case R:
                                /* n1 e r */
                                return 7;
                                break;

                            case C:
                                /* n1 e c */
                                return 6;
                                break;
                        }
                    }
                    if(n1->at.m.nivel > n2->at.m.nivel)
                    {
                        /* i > j */
                       if(n1->at.m.classe == V)
                            ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 NAO E MATRIZ 3");

                        return 5;
                    }
                    break;

                case Fim:
                    /* n1 e meio n2 e fim */
                    if(compara_no_fim_zero(n2,1))
                    {
                        return 13;
                    }
                    else
                    {
                        switch(n1->at.m.classe)
                        {
                            case V:
                                /* n1 e v */
                                ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 NAO E MATRIZ 4");
                                break;

                            case R:
                                /* n1 e r */
                                return 7;
                                break;

                            case C:
                                /* n1 e c */
                                return 4;
                                break;
                        }
                    }
                    break;
            }
            break;

        case Fim:
            /* n1 e fim */
            if(compara_no_fim_zero(n1,1))
            {
                return 13;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Inicio:
                        /*n1 e fim n2 e inicio */
                        ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N1 E FIM N2 E INICIO");
                        break;

                    case Meio:
                        /*n1 e fim n2 e meio */
                        if(n2->at.m.classe != V)
                            ERRO("REGRA APPLY PRODUTO MATRIZ VETOR| N2 NAO E VETOR 2");

                        return 5;
                        break;

                    case Fim:
                        /*n1 e fim n2 e fim */
                        if(compara_no_fim_zero(n2,1))
                            return 13;
                        else
                            return 11;
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
    if(a == NULL)
        ERRO("REGRA APPLY SOMA| A E NULL");

    no *n1, *n2;
    n1 = a->n1;
    n2 = a->n2;

    if(n1 == NULL)
        ERRO("REGRA APPLY SOMA| N1 E NULL");
    if(n2 == NULL)
        ERRO("REGRA APPLY SOMA| N2 E NULL");

    switch(n1->tipo)
    {
        case Inicio:
            /* N1 e inicio */
            if(n2->tipo != Inicio)
                ERRO("REGRA APPLY PRODUTO VETOR VETOR| N1 E INICIO N2 NAO");

            return 0;
            break;

        case Meio:
            /* n1 e meio */
            if(n1->at.m.classe != V)
                ERRO("REGRA APPLY PRODUTO VETOR VETOR| N1 NAO E VETOR");

            switch(n2->tipo)
            {
                case Inicio:
                    /* n2 e inicio */
                    ERRO("REGRA APPLY PRODUTO VETOR VETOR| N1 E MEIO N2 E FIM");
                    break;

                case Meio:
                if(n2->at.m.classe != V)
                    ERRO("REGRA APPLY PRODUTO VETOR VETOR| N2 NAO E VETOR 1");

                    /* n1 e meio n2 e meio */
                    if(n1->at.m.nivel < n2->at.m.nivel)
                        return 7;
                    if(n1->at.m.nivel == n2->at.m.nivel)
                        return 9;
                    if(n1->at.m.nivel > n2->at.m.nivel)
                        return 8;
                    break;

                case Fim:
                    /* n1 e meio n2 e fim */
                    if(compara_no_fim_zero(n2,1))
                        return 13;
                    else
                        return 7;
                    break;
            }
            break;

        case Fim:
            /* n1 e fim */
            if(compara_no_fim_zero(n1,1))
            {
                return 13;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Inicio:
                        /*n1 e fim n2 e inicio */
                        ERRO("REGRA APPLY PRODUTO VETOR VETOR| N1 E FIM N2 E INICIO");
                        break;

                    case Meio:
                        /*n1 e fim n2 e meio */
                        if(n2->at.m.classe != V)
                            ERRO("REGRA APPLY PRODUTO VETOR VETOR| N2 NAO E VETOR 2");

                        return 8;
                        break;

                    case Fim:
                        /*n1 e fim n2 e fim */
                        if(compara_no_fim_zero(n2,1))
                            return 13;
                        else
                            return 12;
                        break;
                }
            }
            break;

    }
    ERRO("REGRA APPLY PRODUTO VETOR VETOR| NAO ATIVOU NENHUMA REGRA");
    return 0;
}



/**  apply pronto  **/

no* apply_soma(no *n1, no *n2)
{
    no *n;
    printf("\nENTROU SOMA");
    n = apply_base(n1,n2,regra_apply_soma);
    printf("\nSAIU SOMA");
    return n;
}

no* apply_produto_matriz_matriz(no *n1, no *n2)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_matriz_matriz);
    return n;
}

no* apply_produto_matriz_vetor(no *n1, no *n2)
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



/** copia estruturas complexas **/

no* copia_arvore(no *n)
{
    apply *a;
    a = cria_apply();
    a->n1 = n;

    apply *ac, *a1, *a2;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n1;
        switch(n->tipo)
        {
            case Inicio:
                a1 = cria_apply();
                a1->n1 = n->at.i.n;
                ac->a1 = a1;
                encaixa_apply(a,ac,Inicio);
                break;

            case Meio:
                a2 = cria_apply();
                a2->n1 = n->at.m.th;
                ac->a2 = a2;
                encaixa_apply(a,ac,Then);

                a1 = cria_apply();
                a1->n1 = n->at.m.el;
                ac->a1 = a1;
                encaixa_apply(a,ac,Else);
                break;
        }
    }

    for(ac = a; ac != NULL; ac = ac->a)
        ac->n2 = copia_no(ac->n1);

    no *n1, *n2;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n2;

        a1 = ac->a1;
        a2 = ac->a2;

        switch(n->tipo)
        {
            case Inicio:
                n1 = a1->n2;
                conecta_UM(n,n1,Inicio);
                break;

            case Meio:
                n1 = a1->n2;
                n2 = a2->n2;
                conecta_DOIS(n,n1,n2);
                break;
        }
    }
    n = a->n2;
    libera_apply_lista(a);
    return n;
}

QDD* copia_QDD(QDD *Q1)
{
    QDD *Q2;
    Q2 = cria_QDD(Q1->nqbit);

    Q2->n = copia_arvore(Q1->n);
    Q2->l = acha_lista_fim_QDD(Q2);

    return Q2;
}



/**  produto QDD QDD base  **/

conta* espalha(suporte *s, Short classe)
{
    no *n0, *na, *nO, *naux;
    lista *lc;
    conta *c, *cc, *cp, *cpc, *caux;
    suporte *sc, *saux;
    Short delta, lado;
    nO = NULL;
    while(s->c[classe] != NULL)
    {
        c = s->c[classe];
        n0 = c->n;

        cp = cria_conta(0);
        cp->n = n0;

        for(lc = n0->l; lc != NULL; lc = lc->l)
        {
            na = lc->n;
            if(na->tipo == Inicio)
            {
                libera_suporte_no(s);
                c->n = na;
                return c;
            }

            for(sc = s; sc->s != NULL; sc = sc->s)
                if(sc->s->nivel < na->at.m.nivel)
                    break;

            if(sc->nivel == na->at.m.nivel)
            {
                /* tem suporte */
                for(cc = sc->c[na->at.m.classe]; cc != NULL; cc = cc->c)
                    if(cc->n == na)
                        break;

                if(cc == NULL)
                {
                    /* nao tem conta */
                    cc = cria_conta(c->nivel);
                    cc->n = na;

                    cc->c = sc->c[na->at.m.classe];
                    sc->c[na->at.m.classe] = cc;
                }
                else
                {
                    /* tem conta */
                    if(cc->nivel > c->nivel)
                    {
                        /* original maior */
                        if(n0 == na->at.m.el)
                            nO = na->at.m.th;
                        if(n0 == na->at.m.th)
                            nO = na->at.m.el;

                        delta  = cc->nivel - c->nivel;
                        delta = pow(2,delta);

                        naux = copia_arvore(nO);
                        produto_arvore_real(naux,delta);
                        lado = desconecta_UM(na,nO);
                        conecta_UM(na,naux,lado);
                        libera_arvore(nO);

                        cc->nivel = c->nivel;
                    }
                    if(cc->nivel < c->nivel)
                    {
                        /* atual maior */
                        delta  = c->nivel - cc->nivel;

                        for(cpc = cp; cpc->c != NULL; cpc = cpc->c)
                            if(cpc->c->nivel > delta)
                                break;

                        if(cpc->nivel == delta)
                        {
                            naux = cpc->n;
                        }
                        else
                        {
                            caux = cria_conta(delta);

                            naux = copia_arvore(n0);
                            delta = pow(2,delta);
                            produto_arvore_real(naux,delta);

                            caux->n = naux;

                            caux->c = cpc->c;
                            cpc->c = caux;
                        }
                        lzero->l = lc->l;
                        lado = desconecta_UM(na,n0);
                        conecta_UM(na,naux,lado);
                        lc = lzero;
                    }
                }
            }
            else
            {
                /* não tem suporte */
                cc = cria_conta(c->nivel);
                cc->n = na;

                saux = cria_suporte(na->at.m.nivel);
                saux->c[na->at.m.classe] = cc;

                saux->s = s->s;
                s->s = saux;
            }
        }
        libera_arvore(n0);
        libera_conta_lista(cp);

        s->c[classe] = c->c;
        libera_conta_no(c);
    }
    return NULL;
}

void contracao_conta(conta *c)
{
    no *na, *nd;
    conta *cc;
    for(cc = c; cc != NULL; cc = cc->c)
    {
        na = cc->n;
    printf("\n24");
        mostra_conta_no(cc);
        nd = apply_soma(na->at.m.el,na->at.m.th);
    printf("\n25");
        transfere_conexao(nd,na);
        libera_arvore(na);
        reduz_arvore(&nd,2);

        cc->n = nd;
        (cc->nivel)--;
    }
}

conta* tratamento(suporte *s, Short classe, Short classeRef)
{
    printf("\n21");
    if(classe == classeRef)
        contracao_conta(s->c[classe]);

    printf("\n22");
    conta *ci;
    ci = espalha(s,classe);
    printf("\n23");
    return ci;
}

void contracao_QDD(QDD *Q, Short classe)
{
    Short nqbit;
    nqbit = Q->nqbit;

    printf("\n9");
    lista *lc;
    conta *c, *cc;
    suporte *saux;
    c = cria_conta(nqbit);
    cc = c;
    for(lc = Q->l; lc != NULL; lc = lc->l)
    {
        cc->c = cria_conta(nqbit);
        cc = cc->c;
        cc->n = lc->n;
    }
    printf("\n10");
    cc = c->c;
    libera_conta_no(c);
    c = cc;

    printf("\n11");
    suporte *s;
    s = cria_suporte(nqbit);
    if(classe == R)
        s->c[C] = c;
    else
        s->c[R] = c;

    printf("\n12");
    conta *ci;
    ci = NULL;
    while(s != NULL)
    {
    printf("\n13");
        ci = tratamento(s,C,classe);
        if(ci != NULL)
            break;

    printf("\n14");
        ci = tratamento(s,V,classe);
        if(ci != NULL)
            break;

    printf("\n15");
        ci = tratamento(s,R,classe);
        if(ci != NULL)
            break;

    printf("\n16");
        saux = s->s;
        libera_suporte_no(s);
        s = saux;
    }
    printf("\n17");
    if(ci == NULL)
        ERRO("CONTRACAO QDD| NAO DETECTOU NO INICIO");

    printf("\n10");
    Short ex;
    ex = pow(2,ci->nivel);
    produto_arvore_real(ci->n,ex);

    printf("\n19");
    libera_conta_no(ci);
    printf("\n20");
}

QDD* produto_QDD_QDD(QDD *Q1, QDD *Q2, no* (*apply_operacao)(no *n1, no *n2), Short classe)
{
    if(Q1->nqbit != Q2->nqbit)
        ERRO("PRODUTO QDD QDD| Q1 E Q2 TEM QUANTIDADES DIFERENTES DE QBITS");

    printf("\n1");
    no *n;
    n = apply_operacao(Q1->n,Q2->n);

    printf("\n2");
    lista *l;
    l = acha_lista_fim_arvore(n);

    printf("\n3");
    QDD *Q;
    Q = cria_QDD(Q1->nqbit);
    Q->n = n;
    Q->l = l;
    reduz_QDD(Q,2,classe);

    printf("\n4");
    contracao_QDD(Q,classe);

    printf("\n5");
    libera_lista_lista(Q->l);
    printf("\n6");
    Q->l = acha_lista_fim_QDD(Q);
    printf("\n7");
    reduz_QDD(Q,1,4);
    printf("\n8");
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

void produto_QDD_no(QDD *Q, no *n1)
{
    no *n2, *naux;
    lista *l;
    for(l = Q->l; l != NULL; l = l->l)
    {
        n2 = l->n;

        naux = produto_no_no(n1,n2);
        transfere_conexao(naux,n2);
        libera_no(n2);

        l->n = naux;
    }
}

QDD* produto_tensorial(QDD *Q1, QDD *Q2)
{
    Short nqbit1, nqbit2;
    nqbit1 = Q1->nqbit;
    nqbit2 = Q2->nqbit;

    QDD *Q;
    Q = copia_QDD(Q1);
    Q->nqbit = nqbit1 + nqbit2;

    QDD *Q2a;
    lista *l1, *l2;
    Q2a = copia_QDD(Q2);
    l1 = enlista_QDD(Q2a);
    for(l2 = l1; l2 != NULL; l2 = l2->l)
        if(l2->n->tipo == Meio)
            (l2->n->at.m.nivel) += nqbit1;
    libera_lista_lista(l1);

    QDD *Q2b;
    no *n, *n1, *n2, *naux;
    l1 = Q->l;
    Q->l = NULL;
    while(l1 != NULL)
    {
        n1 = l1->n;
        if(compara_no_fim_zero(n1,1))
        {
            l2 = l1->l;
            l1->l = Q->l;
            Q->l = l1;
            l1 = l2;
        }
        else
        {
            Q2b = copia_QDD(Q2a);
            n = Q2b->n;
            if(n->tipo != Inicio)
                ERRO("PRODUTO TENSORIAL| PRIMEIRO NO DE QDD NAO E INICIO");

            for(l2 = Q2b->l; l2 != NULL; l2 = l2->l)
            {
                n2 = l2->n;

                naux = produto_no_no(n1,n2);
                transfere_conexao(naux,n2);
                l2->n = naux;
                libera_no(n2);

                if(l2->l == NULL)
                    break;
            }

            n2 = n->at.i.n;
            desconecta_UM(n,n2);
            transfere_conexao(n2,n1);
            libera_no(n1);

            l2->l = Q->l;
            Q->l = Q2b->l;

            libera_no(n);
            libera_QDD_no(Q2b);

            l2 = l1->l;
            libera_lista_no(l1);
            l1 = l2;
        }
    }
    libera_QDD(Q2a);

    reduz_QDD(Q,1,4);
    return Q;
}

QDD* potencia_tensorial(QDD *Q, Short n)
{
    Short ex;
    for(ex = 1; 2*ex < n; ex *= 2);
    n -= ex;

    QDD *Qp;
    Qp = copia_QDD(Q);

    QDD *Qt;
    for(ex /= 2; ex > 0; ex /= 2)
    {
        Qt = produto_tensorial(Qp,Qp);
        libera_QDD(Qp);
        Qp = Qt;

        if(ex <= n)
        {
            Qt = produto_tensorial(Qp,Q);
            libera_QDD(Qp);
            Qp = Qt;

            n -= ex;
        }
    }
    return Qp;
}

QDD* soma_QDD(QDD *Q1, QDD *Q2)
{
    if(Q1->nqbit != Q2->nqbit)
        ERRO("SOMA QDD| QDDs COM QUANTIDADES DIFERENTES DE QBITS");

    QDD *Q;
    Q = cria_QDD(Q1->nqbit);
    Q->n = apply_soma(Q1->n,Q2->n);

    Q->l = acha_lista_fim_QDD(Q);
    reduz_QDD(Q,1,4);

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

QDD* produto_vetor_matriz(QDD *Q1, QDD *Q2)
{
    QDD *Qaux;
    lista *l;
    Qaux = copia_QDD(Q2);
    for(l = Qaux->l; l != NULL; l = l->l)
        (l->n->at.f.im) *= -1;

    QDD *Q;
    Q = produto_matriz_vetor(Qaux,Q1);

    libera_QDD(Qaux);
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



/** QDDs usuais  **/

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

    conecta_UM(ni,nf,Inicio);

    lista *l;
    l = cria_lista();
    l->n = nf;

    QDD *Q;
    Q = cria_QDD(N);
    Q->n = ni;
    Q->l = l;

    return Q;
}

QDD* controle(QDD *Q, Short controle, Short ativa)
{
    if(Q == NULL)
        ERRO("CONTROLE| Q E NULL");
    if(Q->nqbit < 2)
        ERRO("CONTROLE| Q PRECISA TER MAIS QUE 1 QBIT");
    if(Q->nqbit <= controle)
        ERRO("CONTROLE| BIT DE CONTROLE ONTROLE PRECISA SER MENOR QUE NQBIT");

    QDD *QIn, *Q1, *Q2;
    Q1 = Q00;
    Q2 = Q11;

    QDD *Q1aux, *Q2aux;
    Q1aux = NULL;
    Q2aux = NULL;
    printf("\nA");
    if(controle > 0)
    {
    printf("\nB");
        QIn = potencia_tensorial(QI,controle);
        Q1 = produto_tensorial(QIn,Q1);
        Q2 = produto_tensorial(QIn,Q2);
        libera_QDD(QIn);
    }

    printf("\nC");
    if(Q->nqbit - controle > 1)
    {
    printf("\nD");
        QIn = potencia_tensorial(QI,Q->nqbit - controle-1);
        Q1aux = produto_tensorial(Q1,QIn);
        Q2aux = produto_tensorial(Q2,QIn);
        libera_QDD(QIn);
        if(Q1->nqbit > 1)
        {
    printf("\nE");
            libera_QDD(Q1);
            libera_QDD(Q2);
        }
    printf("\nF");
        Q1 = Q1aux;
        Q2 = Q2aux;
    }
    printf("\nG");

    if(ativa == 0)
    {
    printf("\nH");
        Q1aux = produto_matriz_matriz(Q1,Q);
        libera_QDD(Q1);
        Q1 = Q1aux;
    }
    else
    {
        printf("\nI");
        Q2aux = produto_matriz_matriz(Q2,Q);
    printf("\nM");
        libera_QDD(Q2);
    printf("\nN");
        Q2 = Q2aux;
    }
    printf("\nJ");

    QDD *Qc;
    Qc = soma_QDD(Q1,Q2);

    printf("\nK");
    libera_QDD(Q1);
    printf("\nL");
    libera_QDD(Q2);

    return Qc;
}



/**  Testes  **/

double teste_velocidade_unico(char *nome, QDD* (*le)(char*), FILE *fp, FILE *fr, Short primeiro, Long *Quantidade)
{
    time_t antesT, depoisT, deltaT;
    double tempoT;
    antesT = clock();

    QDD *Q1;
    Q1 = le(nome);

    if(primeiro)
    {
        mostra_quantidades();
        fmostra_quantidades(fr);
        fprintf(fp,"%llu|%llu|%llu|%llu|",mem,iM,iF,iL);
    }

    time_t antes, depois, delta;
    double tempo, precisao, clk;
    clk = (double)CLOCKS_PER_SEC;

    antes = clock();
    reduz_QDD(Q1,1,4);
    depois = clock();

    delta = depois - antes;
    tempo = (double)delta/clk;
    precisao = 1/clk;

    if(primeiro)
    {
        mostra_quantidades();
        fmostra_quantidades(fr);
        fprintf(fp,"%llu|%llu|%llu|%llu|",mem,iM,iF,iL);
        printf("\nTempo   1:");
        fprintf(fr,"\nTempo   1:");
    }
    libera_QDD(Q1);

    double iir = 0.001;

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

        *Quantidade = 1;

        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        printf("\t%02d:%02d  %02d/%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_mday,timeinfo->tm_mon+1);
        fprintf(fr,"\t%02d:%02d  %02d/%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_mday,timeinfo->tm_mon+1);

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
            reduz_QDD(Q[i],1,4);
        depois = clock();

        delta = depois-antes;
        tempo = (double)delta/CLOCKS_PER_SEC;
        tempo /= quantidade;

        for(i=0; i<quantidade; i++)
            libera_QDD(Q[i]);
        diminui_memoria_fora(quantidade*sizeof(QDD*));
        free(Q);

        if(precisao < 5e-9)
            break;
    }

    printf(" %.3E",tempo);
    fprintf(fr," %.3E",tempo);
    fprintf(fp,"=%E|",tempo);

    depoisT = clock();
    deltaT = depoisT-antesT;
    tempoT = deltaT/clk;
    printf("\t\tTotal: %.3e",tempoT);
    fprintf(fr,"\t\tTotal: %.3e",tempoT);

    *Quantidade = quantidade;

    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("\t%02d:%02d  %2d/%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_mday,timeinfo->tm_mon+1);
    fprintf(fr,"\t%02d:%02d  %02d/%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_mday,timeinfo->tm_mon+1);

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
    fprintf(fp,"|implicita|quantidade\n");



    Short j;
    Long quantidade;
    float precisao;
    char nome[10];
    time_t antesi, depoisi, deltai;
    double tempoi;
    for(i=limiteinf; i<=limitesup; i++)
    {
        antesi = clock();

        nome[0] = '\0';
        sprintf(nome,"%s%d.txt",nomeI,i);
        printf("\n\n\nTestando: %s\n",nome);
        fprintf(fr,"\n\n\nTestando: %s\n",nome);
        configuracao(i);
        fprintf(fp,"%hu|",i);

        precisao = teste_velocidade_unico(nome,le,fp,fr,1,&quantidade);
        for(j=2; j<=amostras; j++)
        {
            printf("\nTempo %3d:",j);
            fprintf(fr,"\nTempo %3d:",j);
            teste_velocidade_unico(nome,le,fp,fr,0,&quantidade);
        }

        fprintf(fp,"%e|%llu\n",precisao,quantidade);
        printf("\n\nPrecisao: %.3e",precisao);
        fprintf(fr,"\n\nPrecisao: %.3e",precisao);
        printf("\nQuantidade: %llu\n\n",quantidade);
        fprintf(fr,"\nQuantidade: %llu\n\n",quantidade);
        depoisi = clock();

        deltai = depoisi-antesi;
        tempoi = (double)deltai/CLOCKS_PER_SEC;
        printf("Tempo %s%d: %.3e\n\n",nomeI,i,tempoi);
        fprintf(fr,"Tempo %s%d: %.3e\n\n",nomeI,i,tempoi);
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

void teste_curto(Short amostras, Short arquivo, FILE *fr)
{
    double tempo;
    time_t antes, depois, delta;
    Short ri = 0;
    char nome[5];
    nome[0] = '\0';
    if(fr == NULL)
    {
        sprintf(nome,"RelatorioTesteCurto%d.txt",arquivo);
        fr = fopen(nome,"w");
        ri = 1;
    }

    antes = clock();
    teste_velocidade_matriz("H",1,9,amostras,arquivo,fr);
    teste_velocidade_matriz("I",1,9,amostras,arquivo,fr);
    teste_velocidade_matriz("QFT",1,11,amostras,arquivo,fr);
    teste_velocidade_vetor("V",1,22,amostras,arquivo,fr);
    depois = clock();

    delta = depois-antes;
    tempo = (double)delta/CLOCKS_PER_SEC;
    printf("\n\nTempo teste curto: %.3e",tempo);
    fprintf(fr,"\n\nTempo teste curto: %.3e",tempo);
    if(ri)
        fclose(fr);
}

void teste_medio(Short amostras, Short arquivo, FILE *fr)
{
    double tempo;
    time_t antes, depois, delta;
    Short ri = 0;
    char nome[50];
    nome[0] = '\0';
    if(fr == NULL)
    {
        sprintf(nome,"RelatorioTesteMedio%d.txt",arquivo);
        fr = fopen(nome,"w");
        ri = 1;
    }

    antes = clock();
    teste_velocidade_matriz("H",10,10,amostras,arquivo,fr);
    teste_velocidade_matriz("I",10,10,amostras,arquivo,fr);
    teste_velocidade_vetor("V",23,23,amostras,arquivo,fr);
    depois = clock();

    delta = depois-antes;
    tempo = (double)delta/CLOCKS_PER_SEC;
    printf("\n\nTempo teste medio: %.3e",tempo);
    fprintf(fr,"\n\nTempo teste medio: %.3e",tempo);
    if(ri)
        fclose(fr);
}

void teste_longo(Short amostras, Short arquivo, FILE *fr)
{
    double tempo;
    time_t antes, depois, delta;
    Short ri = 0;
    char nome[50];
    nome[0] = '\0';
    if(fr == NULL)
    {
        sprintf(nome,"RelatorioTesteLongo%d.txt",arquivo);
        fr = fopen(nome,"w");
        ri = 1;
    }

    antes = clock();
    teste_velocidade_matriz("H",11,11,amostras,arquivo,fr);
    teste_velocidade_matriz("I",11,11,amostras,arquivo,fr);
    teste_velocidade_matriz("QFT",12,12,amostras,arquivo,fr);
    teste_velocidade_vetor("V",24,24,amostras,arquivo,fr);
    depois = clock();

    delta = depois-antes;
    tempo = (double)delta/CLOCKS_PER_SEC;
    printf("\n\nTempo teste longo: %.3e",tempo);
    fprintf(fr,"\n\nTempo teste longo: %.3e",tempo);
    if(ri)
        fclose(fr);
}

void teste_completo(Short amostras, Short arquivo)
{
    double tempo;
    time_t antes, depois, delta;
    FILE *fr;
    fr = fopen("RelatorioTesteCompleto.txt","w");

    antes = clock();
    teste_curto(amostras,arquivo,fr);
    teste_medio(amostras,arquivo+1,fr);
    teste_longo(amostras,arquivo+2,fr);
    depois = clock();

    delta = depois-antes;
    tempo = (double)delta/CLOCKS_PER_SEC;
    printf("\n\nTempo teste Completo: %.3e",tempo);
    fprintf(fr,"\n\nTempo teste Completo: %.3e",tempo);
    fclose(fr);
}

Short teste_memoria()
{
    Long memt;
    memt = memF + iQ*tQ + (iI+iM+iF)*tN + iL*tL + iA*tA + iC*tC + iS*tS;
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

    configuracao(60);

    QDD *QIn, *Qaux;
    QIn = potencia_tensorial(QI,39);
    Qaux = produto_tensorial(QIn,QH);
    libera_QDD(QIn);

    QDD *Q0;
    QIn = potencia_tensorial(QI,20);
    Q0 = produto_tensorial(Qaux,QIn);
    libera_QDD(Qaux);
    libera_QDD(QIn);

    QDD *Qc;
    Qc = controle(Q0,20,1);
    mostra_tamanhos();

    /***********************************/
    finaliza_structs_globais();
    finaliza_relatorio_memoria();
    return 0;
}
