#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<limits.h>
#include<locale.h>

#define Inicio 0
#define Meio 1
#define Fim 2

#define R 0
#define V 1
#define C 2

#define Else 1
#define Then 2

#define pi 3.1415926535897932384626433832795



FILE *fm, *fmr = NULL;;
unsigned long long mem = 0, memMax = 0, memF = 0;
unsigned long long iQ = 0, iI = 0, iM = 0, iF = 0, iL = 0, iA = 0, iC = 0, iS = 0, iB = 0, iR = 0, iD = 0; // contagem total
unsigned long long cQ = 0, cI = 0, cM = 0, cF = 0, cL = 0, cA = 0, cC = 0, cS = 0, cB = 0, cR = 0, cD = 0; // contagem criados
unsigned long long lQ = 0, lI = 0, lM = 0, lF = 0, lL = 0, lA = 0, lC = 0, lS = 0, lB = 0, lR = 0, lD = 0; // contagem liberado
unsigned long long mem0, iQ0, iI0, iM0, iF0, iL0, iR0; // Contagem inicial
unsigned short tQ, tN, tL, tA, tC, tS, tB, tR, tD; // tamanho structs
unsigned short print, Nqbit, ale = 0;
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
    unsigned char tipo;
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

struct busca
{
    struct no    *n;
    float         p;
    struct rota  *r;
    struct busca *b;
};

struct rota
{
    char num[65];
    struct rota *r;
};

struct destrutivo
{
    float              p[2];
    struct QDD       **Q;
    struct rota       *r[2];
    struct destrutivo *d[2];
};



/** Typedefs e definitions  **/

typedef struct QDD   QDD;
typedef struct no    no;
typedef struct lista lista;

typedef struct apply      apply;
typedef struct conta      conta;
typedef struct suporte    suporte;
typedef struct busca      busca;
typedef struct rota       rota;
typedef struct destrutivo destrutivo;

typedef unsigned short     Short;
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
        fprintf(fm,"\nMemFora:  %d",memF);
        fclose(fm);
    }
}

void ERRO(char *s)
{
    printf("\n\nERRO %s",s);
    FILE *fe;
    fe = fopen("ERRO.txt","w");
    fprintf(fe,"ERRO %s",s);
    fclose(fe);
    finaliza_relatorio_memoria();
    exit(EXIT_FAILURE);
}

void MENSAGEM(char *s)
{
    FILE *fp;
    fp = fopen("MENSAGEM.txt","w");
    fprintf(fp,s);
    fclose(fp);
}

void MENSAGEM_ACUMULATIVA(char *s)
{
    if(fmr == NULL)
        fmr = fopen("MensagemAcumulativa.txt","w");

    fprintf(fmr,"\n%s",s);
}

void configuracao(Short N)
{
    if(N > 64)
        ERRO("CONFIGURACAO| N>64PODE GERAR ERROS");

    Nqbit = N;
    eps = pow(2,-0.5*N)/20;
}



/** Memoria  **/

void aumenta_memoria(Long m)
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
    if(print)
        fprintf(fm,"\tMemForaUp");
}

void diminui_memoria_fora(Long m)
{
    if(m > memF)
        ERRO("DIMINUI MEMORIA FORA| MEMORIA NEGATIVA");

    memF -= m;
    diminui_memoria(m);
    if(print)
        fprintf(fm,"\tMemForaDOWN");
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
    cQ++;

    Q->n = NULL;
    Q->nqbit = nqbit;
    Q->l = NULL;
    return Q;
}

QDD** cria_QDD_array(Long N)
{
    QDD **Q;
    Q = malloc(N*sizeof(QDD*));
    if(Q == NULL)
        ERRO("CRIA ARRAY QDD");
    aumenta_memoria_fora(N*sizeof(QDD*));

    Long i;
    for(i=0; i<N; i++)
        Q[i] = NULL;

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
    cI++;

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
    cM++;

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
    cF++;

    n->tipo = Fim;
    n->l = NULL;

    n->at.f.re = re;
    n->at.f.im = im;

    return n;
}

no** cria_no_array(Long N)
{
    no **n;
    n = malloc(N*sizeof(no*));
    if(n == NULL)
        ERRO("CRIA NO ARRAY");
    aumenta_memoria_fora(N*sizeof(no*));
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
    cL++;

    l->l = NULL;
    l->n = NULL;
    return l;
}

lista* cria_lista_2(no *nf1, no *nf2)
{
    lista *l1, *l2;
    l1 = cria_lista();
    l2 = cria_lista();

    l1->l = l2;
    l1->n = nf1;
    l2->n = nf2;

    return l1;
}

apply* cria_apply()
{
    apply *a;
    a = malloc(tA);
    if(a == NULL)
        ERRO("CRIA APPLY");
    aumenta_memoria(tA);
    iA++;
    cA++;

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
    cC++;

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
    cS++;

    s->c[C] = NULL;
    s->c[R] = NULL;
    s->c[V] = NULL;

    s->nivel = nivel;
    s->s = NULL;

    return s;
}

busca* cria_busca()
{
    busca *b;
    b = malloc(tB);
    if(b == NULL)
        ERRO("CRIA BASE");
    aumenta_memoria(tB);
    iB++;
    cB++;

    b->n = NULL;
    b->p = 0;
    b->r = NULL;
    b->b = NULL;

    return b;
}

rota* cria_rota(char *num)
{
    rota *r;
    r = malloc(tR);
    if(r == NULL)
        ERRO("CRIA ROTA");
    aumenta_memoria(tR);
    iR++;
    cR++;

    sprintf(r->num,"%s",num);
    r->r = NULL;

    return r;
}

rota* cria_rota_vazia()
{
    rota *r;
    r = malloc(tR);
    if(r == NULL)
        ERRO("CRIA ROTA VAZIA");
    aumenta_memoria(tR);
    iR++;
    cR++;

    r->num[0] = '\0';
    r->r = NULL;

    return r;
}

rota* cria_rota_bifurcacao()
{
    rota *r;
    r = cria_rota("0");
    r->r = cria_rota("1");
    return r;
}

destrutivo* cria_destrutivo()
{
    destrutivo *d;
    d = malloc(tD);
    if(d == NULL)
        ERRO("CRIA DESTRUTIVO");
    aumenta_memoria(tD);
    iD++;
    cD++;

    d->Q = NULL;

    for(int i=0; i<2; i++)
    {
        d->p[i] = 0;
        d->r[i] = 0;
        d->d[i] = 0;
    }
    return d;
}



/** Destrutores  **/

void libera_QDD_no(QDD *Q)
{
    diminui_memoria(tQ);
    if(iQ == 0)
        ERRO("LIBERA QDD");
    iQ--;
    lQ++;
    free(Q);
}

void libera_QDD_array(QDD **Q, Long N)
{
    diminui_memoria_fora(N*sizeof(QDD*));
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
        lI++;
        break;

        case Meio:
        if(iM == 0)
            ERRO("LIBERA MEIO");
        iM--;
        lM++;
        break;

        case Fim:
        if(iF == 0)
            ERRO("LIBERA FIM");
        iF--;
        lF++;
        break;
    }
    free(n);
}

void libera_no_array(no **n, Long N)
{
    diminui_memoria_fora(N*sizeof(no*));
    free(n);
}

void libera_lista_no(lista *l)
{
    if(l == NULL)
        return;

    diminui_memoria(tL);
    if(iL == 0)
        ERRO("LIBERA LISTA");
    iL--;
    lL++;
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
    lA++;
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
    lC++;
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
    lS++;
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

void libera_busca_no(busca *b)
{
    diminui_memoria(tB);
    if(iB == 0)
        ERRO("LIBERA BUSCA");
    iB--;
    lB++;
    free(b);
}

void libera_rota_no(rota *r)
{
    diminui_memoria(tR);
    if(iR == 0)
        ERRO("LIBERA ROTA");
    iR--;
    lR++;
    free(r);
}

void libera_rota_lista(rota *r)
{
    rota *raux;
    while(r != NULL)
    {
        raux = r->r;
        libera_rota_no(r);
        r = raux;
    }
}

void libera_busca_lista(busca *b)
{
    busca *bc;
    while(b != NULL)
    {
        if(b->n != NULL)
            libera_no(b->n);
        if(b->r != NULL)
            libera_rota_lista(b->r);

        bc = b->b;
        libera_busca_no(b);
        b = bc;
    }
}

void libera_destrutivo_no(destrutivo *d)
{
    diminui_memoria(tD);
    if(iD == 0)
        ERRO("LIBERA NOTA");
    iD--;
    lD++;
    free(d);
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



/**  Conta itens  **/

Long conta_itens_lista(lista *l)
{
    Long i;
    i = 0;
    lista *lc;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        i++;
        if(i == ULLONG_MAX)
            break;
    }
    return i;
}

Long conta_itens_QDD(QDD *Q)
{
    lista *l;
    Long itens;
    l = enlista_QDD(Q);
    itens = conta_itens_lista(l);
    libera_lista_lista(l);
    return itens;
}



/**  Mostra  **/

void mostra_lista(lista *l)
{
    if(l == NULL)
        return;

    lista *lc;
    Long itens;
    itens = 0;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        printf("\n\tLigacao %llu: %d",itens,lc->n);
        itens++;
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

void mostra_no_numero(no *n)
{
    if(n == NULL)
        return;
    if(n->tipo != Fim)
        ERRO("MOSTRA NUMERO NO| NO TEM QUE SER TIPO FIM");

    printf("\n%e",n->at.f.re);
    if(n->at.f.im >  eps)
    if(n->at.f.im < -eps)
        printf("\t%e",n->at.f.im > eps);
}

void mostra_lista_numero(lista *l)
{
    if(l == NULL)
        return;

    lista *lc;
    for(lc = l; lc != NULL; lc = lc->l)
        mostra_no_numero(lc->n);
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
    itens = conta_itens_lista(l);

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

void mostra_apply_compacto_no(apply *a)
{
    printf("\nEndereco (apply): %d\n",a);
    if(a == NULL)
        return;

    printf("\nNo 1: %d",a->n1);
    printf("\nNo 2: %d",a->n2);
    printf("\nNo  : %d",a->n);
    printf("\n");
    printf("\na1: %d",a->a1);
    printf("\na2: %d",a->a2);
    printf("\n\na proximo:  %d\n",a->a);
}

void mostra_apply_compacto_lista(apply *a)
{
    apply *ac;
    Long ligacao = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        printf("\n\nLigacao apply %d",ligacao);
        mostra_apply_compacto_no(ac);
        ligacao++;
    }
}

void mostra_apply_compacto_matriz(apply ***A, Short linhas, Short colunas)
{
    Short i, j;
    for(i=0; i<linhas; i++)
    {
        for(j=0; j<colunas; j++)
        {
            printf("\n\n\t\t\tA[%hu][%hu]",i,j);
            mostra_apply_compacto_lista(A[i][j]);
        }
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

void mostra_rotas(rota *r)
{
    Long i;
    for(i=0; r != NULL; i++)
    {
        printf("\n\tRota %3llu: %s",i,r->num);
        r = r->r;
    }
}

void mostra_busca_no(busca *b)
{
    printf("\nEndereco (busca): %d",b);
    if(b == NULL)
        return;

    printf("\nNo: ");
    mostra_no(b->n);

    printf("\nRotas: ");
    mostra_rotas(b->r);
}

void mostra_busca_lista(busca *b)
{
    busca *bc;
    Long itens;
    for(bc = b, itens = 0; bc != NULL; bc = bc->b, itens++)
    {
        printf("\n\nBusca: %d",itens);
        mostra_busca_no(bc);
    }
}

void mostra_busca_p_no(busca *b)
{
    printf("\nEndereco (busca): %d",b);
    if(b == NULL)
        return;

    printf("\nP: %f",b->p);
    printf("\nRotas: ");
    mostra_rotas(b->r);
}

void mostra_busca_p_lista(busca *b)
{
    busca *bc;
    Long itens;
    for(bc = b, itens = 0; bc != NULL; bc = bc->b, itens++)
    {
        printf("\n\nBusca: %d",itens);
        mostra_busca_p_no(bc);
    }
}

void mostra_quantidades()
{
    Short vazio = 1;
    if(mem != 0)
    {
        vazio = 0;
        printf("\nMem:  %llu",mem);
    }
    if(memF != 0)
    {
        vazio = 0;
        printf("\nMemF: %llu",memF);
    }
    if(iQ != 0)
    {
        vazio = 0;
        printf("\nQDD:  %llu",iQ);
    }
    if(iI != 0)
    {
        vazio = 0;
        printf("\ni:    %llu",iI);
    }
    if(iM != 0)
    {
        vazio = 0;
        printf("\nm:    %llu",iM);
    }
    if(iF != 0)
    {
        vazio = 0;
        printf("\nf:    %llu",iF);
    }
    if(iL != 0)
    {
        vazio = 0;
        printf("\nl:    %llu",iL);
    }
    if(iA != 0)
    {
        vazio = 0;
        printf("\na:    %llu",iA);
    }
    if(iC != 0)
    {
        vazio = 0;
        printf("\nc:    %llu",iC);
    }
    if(iS != 0)
    {
        vazio = 0;
        printf("\ns:    %llu",iS);
    }
    if(iB != 0)
    {
        vazio = 0;
        printf("\nb:    %llu",iB);
    }
    if(iR != 0)
    {
        vazio = 0;
        printf("\nr:    %llu",iR);
    }
    if(vazio)
        printf("\nTUDO ZERADO");
    printf("\n");
}

void mostra_quantidades_zero()
{
    printf("\nMem0: %llu",mem0);
    printf("\nQ0:   %llu",iQ0);
    printf("\nI0:   %llu",iI0);
    printf("\nM0:   %llu",iM0);
    printf("\nF0:   %llu",iF0);
    printf("\nL0:   %llu",iL0);
    printf("\nR0:   %llu",iR0);
}

void mostra_contagem()
{
    Short vazio = 1;
    if(cQ != 0)
    {
        vazio = 0;
        printf("\ncQ: %llu\tlQ: %llu",cQ,lQ);
    }
    if(cI!= 0)
    {
        vazio = 0;
        printf("\ncI: %llu\tlI: %llu",cI,lI);
    }
    if(cM != 0)
    {
        vazio = 0;
        printf("\ncM: %llu\tlM: %llu",cM,lM);
    }
    if(cF != 0)
    {
        vazio = 0;
        printf("\ncF: %llu\tlF: %llu",cF,lF);
    }
    if(cL != 0)
    {
        vazio = 0;
        printf("\ncL: %llu\tlL: %llu",cL,lL);
    }
    if(cA != 0)
    {
        vazio = 0;
        printf("\ncA: %llu\tlA: %llu",cA,lA);
    }
    if(cC != 0)
    {
        vazio = 0;
        printf("\ncC: %llu\tlC: %llu",cC,lC);
    }
    if(cS != 0)
    {
        vazio = 0;
        printf("\ncS: %llu\tlS: %llu",cS,lS);
    }
    if(cB != 0)
    {
        vazio = 0;
        printf("\ncB: %llu\tlS: %llu",cB,lB);
    }
    if(cR != 0)
    {
        vazio = 0;
        printf("\ncLS: %llu\tlS: %llu",cR,lR);
    }
    if(vazio)
        printf("\nNAO CRIOU NADA");
}

void mostra_tamanhos()
{
    printf("\n\nTAMANHOS\n");
    printf("\nQDD: %d",tQ);
    printf("\nn:   %d",tN);
    printf("\nl:   %d",tL);
    printf("\na:   %d",tA);
    printf("\nc:   %d",tC);
    printf("\ns:   %d",tS);
    printf("\nb:   %d",tB);
    printf("\nr:   %d",tR);
    printf("\n");
}

void mostra_configuracao()
{
    printf("\nConfiguracao: ");
    printf("\nNqbit: %hu",Nqbit);
    printf("\neps: %.3e",eps);
}

void mostra_destrutivo_no(destrutivo *d)
{
    printf("\nEndereco (destrutivo): %d",d);
    if(d == NULL)
        return;

    printf("\n");
    printf("\nP[0]: %e\tP[1]: %e",d->p[0],d->p[1]);

    for(int i=0; i<2; i++)
    {
        if(d->Q    != NULL)
        if(d->Q[i] != NULL)
            printf("\nQ[%d]: %d",i,d->Q[i]);

        if(d->r[i] != NULL)
            printf("\nr[%d]: %s",i,d->r[i]);

        if(d->d[i] != NULL)
            printf("\nd[%d]: %s",i,d->d[i]);
    }
}

void mostra_destrutivo_arvore(destrutivo *d)
{
    mostra_destrutivo_no(d);
    printf("\n");

    for(int i=0; i<2; i++)
    {
        if(d->d[i] != NULL)
        {
            mostra_destrutivo_arvore(d->d[i]);
            printf("\n");
        }
    }
}



/**  Fmostra  **/

void fmostra_lista(FILE *fp, lista *l)
{
    if(l == NULL)
        return;

    lista *lc;
    Long itens;
    itens = 0;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        fprintf(fp,"\n\tLigacao %llu: %d",itens,lc->n);
        itens++;
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

void fmostra_no_numero(FILE *fp, no *n)
{
    if(n == NULL)
        return;
    if(n->tipo != Fim)
        ERRO("MOSTRA NUMERO NO| NO TEM QUE SER TIPO FIM");

    fprintf(fp,"\n%e",n->at.f.re);
    if(n->at.f.im >  eps)
    if(n->at.f.im < -eps)
        fprintf(fp,"\t%e",n->at.f.im > eps);
}

void fmostra_lista_numero(FILE *fp, lista *l)
{
    if(l == NULL)
        return;

    lista *lc;
    for(lc = l; lc != NULL; lc = lc->l)
        fmostra_no_numero(fp,lc->n);
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
    itens = conta_itens_lista(l);

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

void fmostra_apply_compacto_no(FILE *fp, apply *a)
{
    fprintf(fp,"\nEndereco (apply): %d\n",a);
    if(a == NULL)
        return;

    fprintf(fp,"\nNo 1: %d",a->n1);
    fprintf(fp,"\nNo 2: %d",a->n2);
    fprintf(fp,"\nNo  : %d",a->n);
    fprintf(fp,"\n");
    fprintf(fp,"\na1: %d",a->a1);
    fprintf(fp,"\na2: %d",a->a2);
    fprintf(fp,"\n\na proximo:  %d\n",a->a);
}

void fmostra_apply_compacto_lista(FILE *fp, apply *a)
{
    apply *ac;
    Long ligacao = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        fprintf(fp,"\n\nLigacao apply %d",ligacao);
        fmostra_apply_compacto_no(fp,ac);
        ligacao++;
    }
}

void fmostra_apply_compacto_matriz(FILE *fp, apply ***A, Short linhas, Short colunas)
{
    Short i, j;
    for(i=0; i<linhas; i++)
    {
        for(j=0; j<colunas; j++)
        {
            fprintf(fp,"\n\n\t\t\tA[%hu][%hu]",i,j);
            fmostra_apply_compacto_lista(fp,A[i][j]);
        }
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

void fmostra_rotas(FILE *fp, rota *r)
{
    Long i;
    for(i=0; r != NULL; i++)
    {
        fprintf(fp,"\n\tRota %3llu: %s",i,r->num);
        r = r->r;
    }
}

void fmostra_busca_no(FILE *fp, busca *b)
{
    fprintf(fp,"\nEndereco (busca): %d",b);
    if(b == NULL)
        return;

    fprintf(fp,"\nNo: ");
    fmostra_no(fp,b->n);

    fprintf(fp,"\nRotas: ");
    fmostra_rotas(fp,b->r);
}

void fmostra_busca_lista(FILE *fp, busca *b)
{
    busca *bc;
    Long itens;
    for(bc = b, itens = 0; bc != NULL; bc = bc->b, itens++)
    {
        fprintf(fp,"\n\nBusca: %d",itens);
        fmostra_busca_no(fp,bc);
    }
}

void fmostra_busca_p_no(FILE *fp, busca *b)
{
    fprintf(fp,"\nEndereco (busca): %d",b);
    if(b == NULL)
        return;

    fprintf(fp,"\nP: %f",b->p);
    fprintf(fp,"\nRotas: ");
    fmostra_rotas(fp,b->r);
}

void fmostra_busca_p_lista(FILE *fp, busca *b)
{
    busca *bc;
    Long itens;
    for(bc = b, itens = 0; bc != NULL; bc = bc->b, itens++)
    {
        fprintf(fp,"\n\nBusca: %d",itens);
        fmostra_busca_p_no(fp,bc);
    }
}

void fmostra_quantidades(FILE *fp)
{
    Short vazio = 1;
    if(mem != 0)
    {
        vazio = 0;
        fprintf(fp,"\nMem:  %llu",mem);
    }
    if(memF != 0)
    {
        vazio = 0;
        fprintf(fp,"\nMemF: %llu",memF);
    }
    if(iQ != 0)
    {
        vazio = 0;
        fprintf(fp,"\nQDD:  %llu",iQ);
    }
    if(iI != 0)
    {
        vazio = 0;
        fprintf(fp,"\ni:    %llu",iI);
    }
    if(iM != 0)
    {
        vazio = 0;
        fprintf(fp,"\nm:    %llu",iM);
    }
    if(iF != 0)
    {
        vazio = 0;
        fprintf(fp,"\nf:    %llu",iF);
    }
    if(iL != 0)
    {
        vazio = 0;
        fprintf(fp,"\nl:    %llu",iL);
    }
    if(iA != 0)
    {
        vazio = 0;
        fprintf(fp,"\na:    %llu",iA);
    }
    if(iC != 0)
    {
        vazio = 0;
        fprintf(fp,"\nc:    %llu",iC);
    }
    if(iS != 0)
    {
        vazio = 0;
        fprintf(fp,"\ns:    %llu",iS);
    }
    if(iB != 0)
    {
        vazio = 0;
        fprintf(fp,"\nb:    %llu",iB);
    }
    if(iR != 0)
    {
        vazio = 0;
        fprintf(fp,"\nr:    %llu",iR);
    }
    if(vazio)
        fprintf(fp,"\nTUDO ZERADO");
    fprintf(fp,"\n");
}

void fmostra_quantidades_zero(FILE *fp)
{
    fprintf(fp,"\nMem0: %llu",mem0);
    fprintf(fp,"\nQ0:   %llu",iQ0);
    fprintf(fp,"\nI0:   %llu",iI0);
    fprintf(fp,"\nM0:   %llu",iM0);
    fprintf(fp,"\nF0:   %llu",iF0);
    fprintf(fp,"\nL0:   %llu",iL0);
    fprintf(fp,"\nR0:   %llu",iR0);
}

void fmostra_contagem(FILE *fp)
{
    Short vazio = 1;
    if(cQ != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncQ: %llu\tlQ: %llu",cQ,lQ);
    }
    if(cI!= 0)
    {
        vazio = 0;
        fprintf(fp,"\ncI: %llu\tlI: %llu",cI,lI);
    }
    if(cM != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncM: %llu\tlM: %llu",cM,lM);
    }
    if(cF != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncF: %llu\tlF: %llu",cF,lF);
    }
    if(cL != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncL: %llu\tlL: %llu",cL,lL);
    }
    if(cA != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncA: %llu\tlA: %llu",cA,lA);
    }
    if(cC != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncC: %llu\tlC: %llu",cC,lC);
    }
    if(cS != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncS: %llu\tlS: %llu",cS,lS);
    }
    if(cB != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncB: %llu\tlS: %llu",cB,lB);
    }
    if(cR != 0)
    {
        vazio = 0;
        fprintf(fp,"\ncLS: %llu\tlS: %llu",cR,lR);
    }
    if(vazio)
        fprintf(fp,"\nNAO CRIOU NADA");
}

void fmostra_tamanhos(FILE *fp)
{
    fprintf(fp,"\n\nTAMANHOS\n");
    fprintf(fp,"\nQDD: %d",tQ);
    fprintf(fp,"\nn:   %d",tN);
    fprintf(fp,"\nl:   %d",tL);
    fprintf(fp,"\na:   %d",tA);
    fprintf(fp,"\nc:   %d",tC);
    fprintf(fp,"\ns:   %d",tS);
    fprintf(fp,"\nb:   %d",tB);
    fprintf(fp,"\nr:   %d",tR);
    fprintf(fp,"\n");
}

void fmostra_configuracao(FILE *fp)
{
    fprintf(fp,"\nConfiguracao: ");
    fprintf(fp,"\nNqbit: %hu",Nqbit);
    fprintf(fp,"\neps: %.3e",eps);
}

void fmostra_destrutivo_no(FILE *fp, destrutivo *d)
{
    fprintf(fp,"\nEndereco (destrutivo): %d",d);
    if(d == NULL)
        return;

    fprintf(fp,"\n");
    fprintf(fp,"\nP[0]: %e\tP[1]: %e",d->p[0],d->p[1]);

    for(int i=0; i<2; i++)
    {
        if(d->Q    != NULL)
        if(d->Q[i] != NULL)
            fprintf(fp,"\nQ[%d]: %d",i,d->Q[i]);

        if(d->r[i] != NULL)
            fprintf(fp,"\nr[%d]: %s",i,d->r[i]);

        if(d->d[i] != NULL)
            fprintf(fp,"\nd[%d]: %s",i,d->d[i]);
    }
}

void fmostra_destrutivo_arvore(FILE *fp, destrutivo *d)
{
    fmostra_destrutivo_no(fp,d);
    fprintf(fp,"\n");

    for(int i=0; i<2; i++)
    {
        if(d->d[i] != NULL)
        {
            fmostra_destrutivo_arvore(fp,d->d[i]);
            fprintf(fp,"\n");
        }
    }
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
    if(n2->tipo == Inicio)
        ERRO("DESCONECTA UM| INICIO NAO TEM ANTECESSORES");

    lista *l, *laux;
    if(n2->l->n == n1)
    {
        l = n2->l->l;
        libera_lista_no(n2->l);
        n2->l = l;
    }
    else
    {
        for(l = n2->l; l->l != NULL; l = l->l)
            if(l->l->n == n1)
                break;
        if(l->l == NULL)
            ERRO("DESCONECTA UM| REGISTRO INCOMPATIVEL 1");

        laux = l->l;
        l->l = laux->l;
        libera_lista_no(laux);
    }

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



/** Construtores de estruturas complexas  **/

QDD* cria_QDD_vetor(no **nf, Short N)
{
    no ***nm;
    nm = malloc((N+1)*sizeof(no**));
    if(nm == NULL)
        ERRO("LE VETOR| ALLOCA NM");
    aumenta_memoria_fora((N+1)*sizeof(no**));

    Long i, j, exp_2;
    exp_2 = 1;
    for(i=0; i<N; i++)
    {
        nm[i] = malloc(exp_2*sizeof(no*));
        if(nm[i] == NULL)
            ERRO("LE VETOR| ALLOCA NM[]");
        aumenta_memoria_fora(exp_2*sizeof(no*));

        for(j=0; j<exp_2; j++)
            nm[i][j] = cria_no_meio(V,i);

        exp_2 *= 2;
    }
    nm[N] = nf;

    no *n, *n1, *n2;
    exp_2 = 1;
    for(i=0; i<N; i++)
    {
        for(j=0; j<exp_2; j++)
        {
            n  = nm[i][j];
            n1 = nm[i+1][2*j];
            n2 = nm[i+1][2*j+1];

            conecta_DOIS(n,n1,n2);
        }
        exp_2 *= 2;
    }


    QDD *Q;
    Q = cria_QDD(N);
    Q->n = cria_no_inicio();
    conecta_UM(Q->n,nm[0][0],Inicio);

    lista *l, *lc;
    l = cria_lista();
    lc = l;
    for(i=0; i<exp_2; i++)
    {
        lc->l = cria_lista();
        lc = lc->l;
        lc->n = nf[i];
    }
    Q->l = l->l;
    libera_lista_no(l);

    exp_2 = 1;
    for(i=0; i<N; i++)
    {
        diminui_memoria_fora(exp_2*sizeof(no*));
        free(nm[i]);
        exp_2 *= 2;
    }
    diminui_memoria_fora((N+1)*sizeof(no**));
    free(nm);

    return Q;
}



/** Destrutores de estruturas complexas  **/

void libera_arvore(no *n)
{
    if(n->l != NULL)
        return;

    lista *l, *laux;
    l = cria_lista();
    l->n = n;

    no *n1, *n2;

    while(l != NULL)
    {
        n = l->n;
        switch(n->tipo)
        {
            case Inicio:
                n1 = n->at.i.n;
                desconecta_DOIS(n);
                libera_no(n);

                l->n = n1;
                break;

            case Meio:
                n1 = n->at.m.el;
                n2 = n->at.m.th;
                desconecta_DOIS(n);
                libera_no(n);

                if(n1->l == NULL)
                {
                    if(n1 != n2 && n2->l == NULL)
                    {
                        l->n = n2;

                        laux = cria_lista();
                        laux->n = n1;

                        laux->l = l;
                        l = laux;
                    }
                    else
                    {
                        l->n = n1;
                    }
                }
                else
                {
                    if(n2->l == NULL)
                    {
                        l->n = n2;
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

void libera_destrutivo_arvore(destrutivo *d)
{
    if(d == NULL)
        return;

    libera_destrutivo_arvore(d->d[0]);
    libera_destrutivo_arvore(d->d[1]);

    if(d->Q != NULL)
    {
        if(d->Q[0] != NULL)
            libera_QDD(d->Q[0]);
        if(d->Q[1] != NULL)
            libera_QDD(d->Q[1]);

        libera_QDD_array(d->Q,2);
    }

    if(d->r[0] != NULL)
        libera_rota_no(d->r[0]);
    if(d->r[1] != NULL)
        libera_rota_no(d->r[1]);

    libera_destrutivo_no(d);
}



/**  auxiliar QDDs usuais  **/

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
    Q->l = cria_lista_2(nf1,nf2);
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
    l = cria_lista_2(nf0,nf1);

    QDD *Q;
    Q = cria_QDD(1);
    Q->n = ni;
    Q->l = l;
    return Q;
}

QDD* matriz_faixa_diagonal(no *nf1, no *nf2, no *nf3, Short direcao)
{
    no *ni, *n1, *n2, *n3;
    ni = cria_no_inicio();
    n1 = cria_no_meio(R,0);
    n2 = cria_no_meio(C,0);
    n3 = cria_no_meio(C,0);

    conecta_UM(ni,n1,Inicio);
    conecta_DOIS(n1,n2,n3);

    if(direcao == 0)
    {
        conecta_DOIS(n2,nf1,nf2);
        conecta_DOIS(n3,nf2,nf3);
    }
    else
    {
        conecta_DOIS(n2,nf2,nf1);
        conecta_DOIS(n3,nf3,nf2);
    }

    lista *l;
    l = cria_lista();
    l->n = nf1;
    l->l = cria_lista_2(nf2,nf3);

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
    Q->l = cria_lista_2(nf1,nf2);
    return Q;
}

QDD* Y()
{
    no *nf0, *nfi1, *nfi2;
    nf0  = cria_no_fim(0, 0);
    nfi1 = cria_no_fim(0, 1);
    nfi2 = cria_no_fim(0,-1);

    QDD *Q;
    Q = matriz_faixa_diagonal(nfi2,nf0,nfi1,1);
    return Q;
}

QDD* Z()
{
    no *nf11, *nf0, *nf12;
    nf11 = cria_no_fim(-1,0);
    nf0  = cria_no_fim( 0,0);
    nf12 = cria_no_fim( 1,1);

    QDD *Q;
    Q = matriz_faixa_diagonal(nf11,nf0,nf12,0);
    return Q;
}



/**  estruturas globais  **/

QDD *QH, *QI, *QX, *QS, *QY, *Q00, *Q01, *Q10, *Q11;
QDD *Qred;
no *nzero;
apply *A[66][3];
rota *rb;

void inicia_structs_globais()
{
    tQ = sizeof(QDD);
    tN = sizeof(no);
    tL = sizeof(lista);
    tA = sizeof(apply);
    tC = sizeof(conta);
    tS = sizeof(suporte);
    tB = sizeof(busca);
    tR = sizeof(rota);
    tD = sizeof(destrutivo);

    Qred  = cria_QDD(1);
    Qred->n = cria_no_inicio();
    nzero = cria_no_fim(0,0);

    QH = H();
    QI = I();
    QX = X();
    QS = S();
    QY = Y();

    Q00 = matriz_delta_kronecker(0,0);
    Q01 = matriz_delta_kronecker(0,1);
    Q10 = matriz_delta_kronecker(1,0);
    Q11 = matriz_delta_kronecker(1,1);

    rb = cria_rota_bifurcacao();

    mem0 = mem;
    iQ0  = iQ;
    iI0  = iI;
    iM0  = iM;
    iF0  = iF;
    iL0  = iL;
    iR0  = iR;

    mem = 0;
    iQ  = 0;
    iI  = 0;
    iM  = 0;
    iF  = 0;
    iL  = 0;
    iR  = 0;

    cQ = 0;
    cI = 0;
    cM = 0;
    cF = 0;
    cL = 0;

    Short i, j;
    for(i=0; i<66; i++)
        for(j=0; j<3; j++)
            A[i][j] = NULL;
}

void finaliza_structs_globais()
{
    mem += mem0;
    iQ  += iQ0;
    iI  += iI0;
    iM  += iM0;
    iF  += iF0;
    iL  += iL0;
    iR  += iR0;

    libera_no(Qred->n);
    libera_QDD_no(Qred);
    libera_no(nzero);

    libera_QDD(QH);
    libera_QDD(QI);
    libera_QDD(QX);
    libera_QDD(QS);
    libera_QDD(QY);

    libera_QDD(Q00);
    libera_QDD(Q01);
    libera_QDD(Q10);
    libera_QDD(Q11);

    libera_rota_lista(rb);

    if(fmr != NULL)
        fclose(fmr);
}



/** Compara estruturas simples  **/

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

Short compara_no(no *n1, no *n2)
{
    if(n1->tipo != n2->tipo)
        return 0;

    switch(n1->tipo)
    {
        case Inicio:
            return 1;
            break;

        case Meio:
            return compara_no_meio_parcial(n1,n2);
            break;

        case Fim:
            return compara_no_fim(n1,n2,1);
            break;
    }
    ERRO("COMPARA NO| SWITCH NAO ATIVOU");
    return 0;
}

Short compara_apply(apply *a1, apply *a2)
{
    if(a1 != NULL)
    if(a2 != NULL)
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

void ordena_lista_recursivo(lista *l, Long n)
{
    if(n == 1)
        return;

    Long n1, n2;
    n1 = n/2;
    n2 = n - n1;

    lista *l1, *l2;
    l1 = cria_lista();
    l2 = cria_lista();

    lista *lc;
    Long i;
    lc = l;
    for(i=0; i<n1; i++)
        lc = lc->l;
    l2->l = lc->l;
    lc->l = NULL;
    l1->l = l->l;

    ordena_lista_recursivo(l1,n1);
    ordena_lista_recursivo(l2,n2);

    lista *laux;
    lc = l;
    while(l1->l != NULL && l2->l != NULL)
    {
        if(l1->l->n->at.f.re < l2->l->n->at.f.re)
        {
            laux = l1->l;
            l1->l = laux->l;
        }
        else
        {
            laux = l2->l;
            l2->l = laux->l;
        }

        laux->l = NULL;
        lc->l = laux;
        lc = laux;
    }

    if(l1->l != NULL)
        lc->l = l1->l;
    if(l2->l != NULL);
        lc->l = l2->l;

    libera_lista_no(l1);
    libera_lista_no(l2);
}

void ordena_lista(lista **L, Short ordem)
{
    lista *l;
    l = cria_lista();
    l->l = *L;

    lista *lc;
    Long n;
    n = 0;
    for(lc = l->l; lc != NULL; lc = lc->l)
        n++;

    ordena_lista_recursivo(l,n);

    lista *laux;
    lc = NULL;
    if(ordem)
    {
        while(l->l != NULL)
        {
            laux = l->l;
            l->l = laux->l;

            laux->l = lc;
            lc = laux;
        }
        l->l = lc;
    }

    *L = l->l;
    libera_lista_no(l);
}



/**  Conta itens  */

Long conta_itens_fim_arvore(no *n)
{
    lista *l;
    Long itens;
    l = acha_lista_fim_arvore(n);
    itens = conta_itens_lista(l);
    libera_lista_lista(l);
    return itens;
}

Long conta_itens_classe_QDD(QDD *Q, Short classe)
{
    lista *l, *lc;
    Long itens;
    l = enlista_QDD(Q);
    itens = 0;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        if(lc->n->tipo == Meio)
        if(lc->n->at.m.classe == classe)
            itens++;
    }
    return itens;
}

Long conta_itens_apply_lista(apply *a)
{
    apply *ac;
    Long itens;
    itens = 0;
    for(ac = a; ac != NULL; ac = ac->a)
        itens++;
    return itens;
}

Long conta_itens_conta_lista(conta *c)
{
    conta *cc;
    Long itens;
    itens = 0;
    for(cc = c; cc != NULL; cc = cc->c)
        itens++;
    return itens;
}

Long conta_item_arvore(no *n)
{
    lista *l, *lc;
    Long itens;
    l = enlista_arvore(n);
    itens = 0;
    for(lc=  l; lc != NULL; lc = lc->l)
        itens++;
    libera_lista_lista(l);
    return itens;
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

rota* copia_rota(rota *r1)
{
    rota *r;
    r = cria_rota(r1->num);
    return r;
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

no* modulo_2_no(no *n)
{
    n = produto_no_conjugado_no(n,n);
    return n;
}

float modulo_no(no *n)
{
    float m;
    m = (n->at.f.re)*(n->at.f.re) + (n->at.f.im)*(n->at.f.im);

    if(m < 0)
        ERRO("PUTA MERDA");

    m = sqrt(m);
    return m;
}

void produto_no_real(no *n, double re)
{
    (n->at.f.re) *= re;
    (n->at.f.im) *= re;
}

no* divisao_no_no(no *n1, no *n2)
{
    no *n;
    float m;
    m = modulo_no(n2);
    m = 1/(m*m);
    n = produto_no_conjugado_no(n2,n1);
    produto_no_real(n,m);
    return n;

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

QDD* le_vetor(char *nome)
{

    FILE *fp;
    fp = fopen(nome,"r");
    if(fp == NULL)
        ERRO("LE VETOR| NAO CONSEGUIU ABRIR ARQUIVO");

    Short N;
    fscanf(fp,"%hu\n",&N);
    Long exp1;
    exp1 = (Long)pow(2,N);

    no **nf;
    Long i;
    float re, im;
    nf = malloc(exp1*sizeof(no*));
    if(nf == NULL)
        ERRO("LE VETOR| ALLOCA NF");
    aumenta_memoria_fora(exp1*sizeof(no*));
    for(i=0; i<exp1; i++)
    {
        fscanf(fp,"%f",&re);
        fscanf(fp,"%f",&im);
        nf[i] = cria_no_fim(re,im);
    }
    fclose(fp);

    QDD *Q;
    Q = cria_QDD_vetor(nf,N);

    diminui_memoria_fora(exp1*sizeof(no*));
    free(nf);

    return Q;
}

void salva_QDD(FILE *fp, QDD *Q)
{
    lista *l;
    Long itens;
    l = enlista_QDD(Q);
    itens = conta_itens_lista(l);
    if(itens == ULLONG_MAX)
    {
        printf("\nQDD Muito grande para ser salva");
        libera_lista_lista(l);
        return;
    }

    no **N;
    N = malloc(itens*sizeof(no*));
    if(N == NULL)
        ERRO("SALVA QDD| ALLOCA N");
    aumenta_memoria_fora(itens*sizeof(no*));

    lista *lc;
    Long i;
    lc = l;
    for(i=0; i<itens; i++)
    {
        N[i] = lc->n;
        lc = lc->l;
    }
    libera_lista_lista(l);

    fprintf(fp,"\n%llu %hu",itens,Q->nqbit);

    no *n, *n1, *n2;
    Long j, el, th;
    for(i=0; i<itens; i++)
    {
        n = N[i];
        switch(n->tipo)
        {
            case Inicio:
                fprintf(fp,"\n1 0 I");
                break;

            case Meio:
                n1 = n->at.m.el;
                n2 = n->at.m.th;
                el = 0;
                th = 0;
                for(j=0; j<itens; j++)
                {
                    if(N[j] == n1)
                    {
                        el = j;
                        if(th != 0)
                        {
                            break;
                        }
                    }
                    if(N[j] == n2)
                    {
                        th = j;
                        if(el != 0)
                        {
                            break;
                        }
                    }
                }
                fprintf(fp,"\n%llu %llu M %hu %hu",el,th,n->at.m.classe,n->at.m.nivel);
                break;

            case Fim:
                fprintf(fp,"\n0 0 F %e %e",n->at.f.re,n->at.f.im);
                break;
        }
    }

    diminui_memoria_fora(itens*sizeof(no*));
    free(N);
}

void salva_QDD_sozinho(QDD *Q, char *nome)
{
    FILE *fp;
    char nomeT[30];
    sprintf(nomeT,"%s.QDD",nome);
    fp = fopen(nomeT,"w");

    salva_QDD(fp,Q);

    fclose(fp);
}

QDD* le_QDD(FILE *fp)
{
    Long itens;
    Short nqbit;
    fscanf(fp,"\n%llu",&itens);
    fscanf(fp," %hu",&nqbit);

    apply **Al;
    Long i;
    Al = malloc(itens*sizeof(apply*));
    if(Al == NULL)
        ERRO("LE QDD| ALLOCA A");
    aumenta_memoria_fora(itens*sizeof(apply*));
    for(i=0; i<itens; i++)
        Al[i] = cria_apply();

    no *n;
    apply *a;
    Short classe, nivel;
    Long el, th;
    float re, im;
    char tipo;
    for(i=0; i<itens; i++)
    {
        a = Al[i];
        if(i < itens-1)
            a->a = Al[i+1];

        fscanf(fp,"\n%llu",&el);
        fscanf(fp," %llu",&th);

        fscanf(fp," %c",&tipo);

        n = NULL;
        switch(tipo)
        {
            case 'I':
                a->a1 = Al[el];

                n = cria_no_inicio();
                break;

            case 'M':
                fscanf(fp," %hu",&classe);
                fscanf(fp," %hu",&nivel);

                a->a1 = Al[el];
                a->a2 = Al[th];

                n = cria_no_meio(classe,nivel);
                break;

            case 'F':
                fscanf(fp," %e",&re);
                fscanf(fp," %e",&im);

                n = cria_no_fim(re,im);
                break;
        }
        if(n == NULL)
            ERRO("LE QDD| NO NAO E I M OU F");

        a->n = n;
    }

    a = Al[0];
    diminui_memoria_fora(itens*sizeof(apply*));
    free(Al);

    apply *ac, *a1, *a2;
    no *n1, *n2;
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

    lista *l;
    l = acha_lista_fim_arvore(n);

    QDD *Q;
    Q = cria_QDD(nqbit);
    Q->n = n;
    Q->l = l;
    return Q;
}

QDD* le_QDD_sozinho(char *nome)
{
    FILE *fp;
    char nomeT[30];
    sprintf(nomeT,"%s.QDD",nome);
    fp = fopen(nomeT,"r");
    if(fp == NULL)
        ERRO("LE QDD| NAO CONSEGUIU ABIR ARQUIVO");

    QDD *Q;
    Q = le_QDD(fp);
    fclose(fp);

    return Q;
}



/**  Reduz  **/

void reduz_lista_fim(lista *l, Short ex)
{
    no *n1, *n2;
    lista *lc1, *lc2, *laux;
    lc1 = l;
    while(lc1->l != NULL)
    {
        n1 = lc1->n;
        if(n1->tipo != Fim)
            ERRO("REDUZ LISTA FIM| NO DA LISTA NAO E FIM 1");

        lc2 = lc1;
        while(lc2->l != NULL)
        {
            laux = lc2->l;
            n2 = laux->n;

            if(n2->tipo != Fim)
                ERRO("REDUZ LISTA FIM| NO DA LISTA NAO E FIM 2");

            if(compara_no_fim(n1,n2,ex))
            {
                lc2->l = laux->l;

                transfere_conexao(n1,n2);
                libera_no(n2);
                libera_lista_no(laux);
            }
            else
            {
                lc2 = lc2->l;
            }
        }
        lc1 = lc1->l;
        if(lc1 == NULL)
            break;
    }
}

void reduz_QDD(QDD *Q, Short ex, Short classe)
{
    reduz_lista_fim(Q->l,ex);
    lista *l;
    l = copia_lista_sem_cabeca(Q->l);

    lista *ll, *llc;
    ll = NULL;

    no *nc, *n1, *n2;
    lista *lnc1, *lnc2, *lnc3, *lnc4, *lp, *laux, *le;
    Short mudou, inicio;
    inicio = 0;
    while(l != NULL)
    {
        nc = l->n;
        if(nc->l != NULL)
        {
            /* Regra 1 */
            do
            {
                mudou = 0;

                n1 = nc->l->n;
                if(n1->tipo == Inicio)
                {
                    inicio = 1;
                    break;
                }

                lnc3 = NULL;
                lnc4 = NULL;
                while(n1->at.m.el == n1->at.m.th)
                {
                    for(le = l; le != NULL; le = le->l)
                        if(le->n == n1)
                            ERRO("REDUZ QDD| ELIMINANDO NO QUE ESTA NA LISTA DE CHECAGEM");

                    desconecta_DOIS(n1);
                    transfere_conexao(nc,n1);

                    llc = cria_lista();
                    llc->n = n1;
                    llc->l = ll;
                    ll = llc;

                    n1 = nc->l->n;
                    if(n1->tipo == Inicio)
                    {
                        inicio = 1;
                        break;
                    }
                }

                lnc2 = nc->l;
                lnc1 = lnc2->l;
                lnc4 = lnc2;

                while(lnc1 != NULL)
                {
                    n1 = lnc1->n;
                    if(n1->at.m.el == n1->at.m.th)
                    {
                        for(le = l; le != NULL; le = le->l)
                            if(le->n == n1)
                                ERRO("REDUZ QDD| ELIMINANDO NO QUE ESTA NA LISTA DE CHECAGEM");

                        mudou = 1;
                        desconecta_DOIS(n1);
                        transfere_conexao(nc,n1);

                        llc = cria_lista();
                        llc->n = n1;
                        llc->l = ll;
                        ll = llc;

                        lnc1 = lnc2->l;
                    }
                    else
                    {
                        lnc2 = lnc1;
                        lnc1 = lnc2->l;
                    }
                    if(lnc1 == lnc3)
                        break;
                }
                lnc3 = lnc4;
            }
            while(mudou);
            if(inicio)
                break;

            /* Regra 2 */
            for(lnc1 = nc->l; lnc1 != NULL; lnc1 = lnc1->l)
            {
                n1 = lnc1->n;
                if(n1->tipo == Inicio)
                    ERRO("REDUZ QDD| NO INICIO NAO DEVERIA CHEGAR AQUI");
                if(n1->at.m.el == NULL)
                    ERRO("REDUZ QDD| NO SEM CONEXAO EM EL");
                if(n1->at.m.th == NULL)
                    ERRO("REDUZ QDD| NO SEM CONEXAO EM TH");
                if(n1->at.m.el == n1->at.m.th)
                    ERRO("REDUZ QDD| REDUNDANCIA TIPO 1 JA DEVERIA TER SIDO ELIMINADA");

                mudou = 0;
                laux = NULL;

                for(lnc2 = nc->l; lnc2 != NULL; lnc2 = lp)
                {
                    lp = lnc2->l;

                    if(lnc1 == lnc2)
                        continue;

                    n2 = lnc2->n;
                    if(compara_no_meio_completo(n1,n2,classe))
                    {
                        mudou = 1;

                        if(laux == NULL)
                            for(laux = l; laux != NULL; laux = laux->l)
                                if(laux->n == n2)
                                {
                                    laux->n = NULL;
                                    break;
                                }

                        desconecta_DOIS(n2);
                        transfere_conexao(n1,n2);

                        llc = cria_lista();
                        llc->n = n2;
                        llc->l = ll;
                        ll = llc;
                    }
                }

                if(mudou)
                {
                    if(laux == NULL)
                    {
                        for(laux = l; laux->l != NULL; laux = laux->l)
                        {
                            n2 = laux->l->n;

                            if(n2->tipo == Fim)
                                continue;
                            if(n2->at.m.nivel > n1->at.m.nivel)
                                continue;
                            if(n2->at.m.nivel == n1->at.m.nivel)
                            {
                                if(n2->at.m.classe == C)
                                    continue;
                                if(n1->at.m.classe == R)
                                    continue;
                            }

                            break;
                        }
                        lp = cria_lista();
                        lp->n = n1;

                        lp->l = laux->l;
                        laux->l = lp;
                    }
                    else
                    {
                        laux->n = n1;
                    }
                }
            }
        }

        lnc1 = l->l;
        libera_lista_no(l);
        l = lnc1;
    }
    libera_lista_no(l);

    while(ll != NULL)
    {
        llc = ll->l;
        libera_no(ll->n);
        libera_lista_no(ll);
        ll = llc;
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

void reduz_arvore_regra_1(no **n)
{
    lista *l;
    l = cria_lista();
    l->l = enlista_arvore(*n);

    no *n1, *n2;
    lista *lc, *laux;
    Short mudou;
    do
    {
        mudou = 0;

        lc = l;
        while(lc->l != NULL)
        {
            laux = lc->l;
            n1 = laux->n;
            if(n1->tipo == Meio)
            if(n1->at.m.el == n1->at.m.th)
            {
                n2 = n1->at.m.el;

                desconecta_DOIS(n1);
                transfere_conexao(n2,n1);
                libera_no(n1);

                lc->l = laux->l;
                libera_lista_no(laux);

                mudou = 1;
                continue;
            }
            lc = lc->l;
        }
    }
    while(mudou);

    lc = l->l;
    n1 = lc->n;
    n = &n1;
    libera_lista_lista(l);
}



/**  apply esqueleto  **/

void acha_indice(apply *a, Short *indice1, Short *indice2, Short N)
{
    no *n;
    n = a->n1;

    if(n == NULL)
        ERRO("ACHA INDICE| N E NULL");

    switch(n->tipo)
    {
        case Inicio:
            *indice1 = 0;
            *indice2 = 0;
            break;

        case Meio:
            *indice1 = n->at.m.nivel+1;
            *indice2 = n->at.m.classe;
            break;

        case Fim:
            *indice1 = N+1;
            *indice2 = 0;
            break;
    }
    if(*indice1 > N+1)
        ERRO("ACHA INDICE| INDICE MAIOR QUE LIMITE");
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

apply* encaixa_apply(apply *a, Short N)
{
    Short indice1, indice2;
    acha_indice(a,&indice1,&indice2,N);

    if(A[indice1][indice2] == NULL)
    {
        A[indice1][indice2] = a;
        return a;
    }

    apply *ac;
    for(ac = A[indice1][indice2]; ac->a != NULL; ac = ac->a)
        if(compara_apply(ac,a))
            break;

    if(compara_apply(ac,a))
    {
        libera_apply_no(a);
        return ac;
    }

    if(ac->a != NULL)
        ERRO("ENCAIXA APPLY| CONDICAO IMPOSSIVEL");

    ac->a = a;
    return a;
}

no* monta_arvore(Short N)
{
    no *n0, *n, *nc;
    apply *a, *ac;
    Short i, j, ini;
    n0 = NULL;
    ini = 1;
    for(i=0; i<N+2; i++)
    {
        for(j=0; j<3; j++)
        {
            if(ini)
            if(A[i][j] != NULL)
            {
                ini = 0;
                n0 = A[i][j]->n;
            }

            for(a = A[i][j]; a != NULL; a = a->a)
            {
                n = a->n;

                if(n == NULL)
                    ERRO("MONTA ARVORE| APPLY VAZIO");

                switch(n->tipo)
                {
                    case Inicio:
                        ac = a->a1;
                        nc = ac->n;
                        conecta_UM(n,nc,Inicio);
                        break;

                    case Meio:
                        ac = a->a1;
                        nc = ac->n;
                        conecta_UM(n,nc,Else);

                        ac = a->a2;
                        nc = ac->n;
                        conecta_UM(n,nc,Then);
                        break;
                }
            }
        }
    }
    return n0;
}

void limpa_apply_matriz(Short N)
{
    Short i, j;
    for(i=0; i<N+2; i++)
    {
        for(j=0; j<3; j++)
        {
            libera_apply_lista(A[i][j]);
            A[i][j] = NULL;
        }
    }
}

no* apply_base(no *n1, no *n2, Short(*regra_apply)(apply*), Short N)
{
    apply *a;
    a = cria_apply();
    a->n1 = n1;
    a->n2 = n2;

    encaixa_apply(a,N);

    apply *ac;
    Short i, j, regra;
    for(i=0; i<N+2; i++)
    {
        for(j=0; j<3; j++)
        {
            for(ac = A[i][j]; ac != NULL; ac = ac->a)
            {
                regra = regra_apply(ac);
                monta_apply(ac,regra);

                switch(ac->n->tipo)
                {
                    case Meio:
                        ac->a2 = encaixa_apply(ac->a2,N);

                    case Inicio:
                        ac->a1 = encaixa_apply(ac->a1,N);
                        break;
                }
            }
        }
    }

    no *n;
    n = monta_arvore(N);

    limpa_apply_matriz(N);

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
    static Short i=0;
    i++;
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

no* apply_soma(no *n1, no *n2, Short N)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_soma,N);
    return n;
}

no* apply_produto_matriz_matriz(no *n1, no *n2, Short N)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_matriz_matriz,N);
    return n;
}

no* apply_produto_matriz_vetor(no *n1, no *n2, Short N)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_matriz_vetor,N);
    return n;
}

no* apply_produto_vetor_vetor(no *n1, no *n2, Short N)
{
    no *n;
    n = apply_base(n1,n2,regra_apply_produto_vetor_vetor,N);
    return n;
}



/** copia estruturas complexas **/

void monta_apply_matriz(no *n, Short N)
{
    apply *a;
    a = cria_apply();
    a->n1 = n;
    encaixa_apply(a,N);

    apply *ac;
    Short i, j;
    for(i=0; i<N+2; i++)
    {
        for(j=0; j<3; j++)
        {
            for(a = A[i][j]; a != NULL; a = a->a)
            {
                n = a->n1;
                switch(n->tipo)
                {
                    case Inicio:
                        ac = cria_apply();
                        ac->n1 = n->at.i.n;
                        a->a1 = encaixa_apply(ac,N);
                        break;

                    case Meio:
                        ac = cria_apply();
                        ac->n1 = n->at.m.th;
                        a->a2 = encaixa_apply(ac,N);

                        ac = cria_apply();
                        ac->n1 = n->at.m.el;
                        a->a1 = encaixa_apply(ac,N);
                        break;
                }
            }
        }
    }
}

no* monta_copia(Short N)
{
    apply *a;
    Short i, j;
    for(i=0; i<N+2; i++)
        for(j=0; j<3; j++)
            for(a = A[i][j]; a != NULL; a = a->a)
                a->n = copia_no(a->n1);

    no *n;
    n = monta_arvore(N);
    return n;
}

no* copia_arvore(no *n, Short N)
{
    monta_apply_matriz(n,N);
    n = monta_copia(N);
    limpa_apply_matriz(N);
    return n;
}

QDD* copia_QDD(QDD *Q1)
{
    no *n;
    n = copia_arvore(Q1->n,Q1->nqbit);

    lista *l;
    l = acha_lista_fim_arvore(n);

    QDD *Q;
    Q = cria_QDD(Q1->nqbit);
    Q->n = n;
    Q->l = l;
    return Q;
}

no** copia_arvore_varios(no *n0, Short N, Long quantidade)
{
    monta_apply_matriz(n0,N);

    no **n;
    Long i;
    n = cria_no_array(quantidade);
    for(i=0; i<quantidade; i++)
        n[i] = monta_copia(N);

    limpa_apply_matriz(N);

    return n;
}

QDD** copia_QDD_varios(QDD *Q0, Long quantidade)
{
    no **n;
    n = copia_arvore_varios(Q0->n,Q0->nqbit,quantidade);

    QDD **Q;
    lista *l;
    Long i;
    Q = cria_QDD_array(quantidade);
    for(i=0; i<quantidade; i++)
    {
        l = acha_lista_fim_arvore(n[i]);

        Q[i] = cria_QDD(Q0->nqbit);
        Q[i]->n = n[i];
        Q[i]->l = l;
    }
    libera_no_array(n,quantidade);

    return Q;
}



/**  produto QDD QDD base  **/

conta* espalha(suporte *s, Short classe, Short N)
{
    no *n, *na, *naux, *nlixo;
    lista *lc, *lp, *lr, *lrc;
    conta *c, *cc, *cp, *cpc, *caux;
    suporte *sc, *saux;
    Short lado, delta, i;
    while(s->c[classe] != NULL)
    {
        c = s->c[classe];
        n = c->n;

        cp = cria_conta(0);
        cp->n = n;

        lc = n->l;
        while(lc != NULL)
        {
            lp = lc->l;

            na = lc->n;
            if(na->tipo == Inicio)
            {
                libera_conta_no(cp);
                libera_suporte_no(s);
                return c;
            }

            for(sc = s; sc->s != NULL; sc = sc->s)
                if(sc->s->nivel < na->at.m.nivel)
                    break;

            if(sc->nivel == na->at.m.nivel)
            {
                /* Tem suporte */
                for(cc = sc->c[na->at.m.classe]; cc != NULL; cc = cc->c)
                    if(cc->n == na)
                        break;

                if(cc == NULL)
                {
                    /* Nao tem conta */
                    caux = cria_conta(c->nivel);
                    caux->n = na;

                    caux->c = sc->c[na->at.m.classe];
                    sc->c[na->at.m.classe] = caux;
                }
                else
                {
                    /* Tem conta */
                    if(cc->nivel > c->nivel)
                    {
                        /* Alterar original */
                        if(n == na->at.m.el)
                            nlixo = na->at.m.th;
                        else
                            nlixo = na->at.m.el;

                        lado = desconecta_UM(na,nlixo);
                        naux = copia_arvore(nlixo,N);

                        delta = cc->nivel - c->nivel;
                        lr = acha_lista_fim_arvore(naux);
                        for(i = 0; i < delta; i++)
                        {
                            for(lrc = lr; lrc != NULL; lrc = lrc->l)
                                produto_no_real(lrc->n,2);
                        }
                        libera_lista_lista(lr);

                        conecta_UM(na,naux,lado);
                        libera_arvore(nlixo);

                        cc->nivel = c->nivel;
                    }
                    if(cc->nivel < c->nivel)
                    {
                        /* alterar atual */
                        delta = c->nivel - cc->nivel;

                        for(cpc = cp; cpc->c != NULL; cpc = cpc->c)
                            if(cpc->c->nivel > delta)
                                break;

                        if(cpc->nivel == delta)
                        {
                            naux = cpc->n;
                        }
                        else
                        {
                            naux = copia_arvore(cpc->n,N);
                            lr = acha_lista_fim_arvore(naux);
                            for(i = cpc->nivel; i < delta; i++)
                            {
                                for(lrc = lr; lrc != NULL; lrc = lrc->l)
                                    produto_no_real(lrc->n,2);
                            }
                            libera_lista_lista(lr);

                            caux = cria_conta(delta);
                            caux->n = naux;

                            caux->c = cpc->c;
                            cpc->c = caux;
                        }
                        lado = desconecta_UM(na,n);
                        conecta_UM(na,naux,lado);
                    }
                }
            }
            else
            {
                /* Não tem suporte */
                caux = cria_conta(c->nivel);
                caux->n = na;

                saux = cria_suporte(na->at.m.nivel);
                saux->c[na->at.m.classe] = caux;

                saux->s = sc->s;
                sc->s = saux;
            }

            lc = lp;
        }
        libera_conta_lista(cp);

        libera_arvore(n);

        cc = c->c;
        libera_conta_no(c);
        c = cc;

        s->c[classe] = c;
    }
    return NULL;
}

void contrai_conta(conta *c,Short N)
{
    no *na, *nd;
    conta *cc;

    for(cc = c; cc != NULL; cc = cc->c)
    {
        na = cc->n;

        nd = apply_soma(na->at.m.el,na->at.m.th,N);
        reduz_arvore(&nd,2);

        transfere_conexao(nd,na);
        libera_arvore(na);

        cc->n = nd;
        (cc->nivel)--;
    }
}

conta* tratamento(suporte *s, Short classe, Short classeRef, Short N)
{
    if(s->c[classe] == NULL)
        return NULL;

    if(classe == classeRef)
        contrai_conta(s->c[classe],N);

    conta *ci;
    ci = espalha(s,classe,N);

    return ci;
}

void contrai_QDD(QDD *Q, Short classe)
{
    Short nqbit;
    nqbit = Q->nqbit;

    lista *lc;
    conta *c, *cc;
    c = cria_conta(nqbit);
    cc = c;
    for(lc = Q->l; lc != NULL; lc = lc->l)
    {
        cc->c = cria_conta(nqbit);
        cc = cc->c;
        cc->n = lc->n;
    }
    cc = c->c;
    libera_conta_no(c);
    c = cc;
    libera_lista_lista(Q->l);
    Q->l = NULL;

    suporte *s;
    s = cria_suporte(nqbit);
    if(classe == R)
        s->c[C] = c;
    else
        s->c[R] = c;

    conta *ci;
    suporte *saux;
    while(s != NULL)
    {
        ci = tratamento(s,C,classe,Q->nqbit);
        if(ci != NULL)
            break;

        ci = tratamento(s,V,classe,Q->nqbit);
        if(ci != NULL)
            break;

        ci = tratamento(s,R,classe,Q->nqbit);
        if(ci != NULL)
            break;

        saux = s->s;
        libera_suporte_no(s);
        s = saux;
    }
    if(ci == NULL)
        ERRO("CONTRACAO QDD| NAO DETECTOU INICIO");

    Long ex;
    ex = pow(2,ci->nivel);
    produto_arvore_real(Q->n,ex);

    libera_conta_no(ci);

    Q->l = acha_lista_fim_QDD(Q);
}

QDD* produto_QDD_QDD(QDD *Q1, QDD *Q2, no* (*apply_operacao)(no *n1, no *n2, Short), Short classe)
{
    if(Q1 == NULL)
        ERRO("PRODUTO QDD QDD| Q1 E NULL");
    if(Q2 == NULL)
        ERRO("PRODUTO QDD QDD| Q2 E NULL");
    if(Q1->nqbit != Q2->nqbit)
        ERRO("PRODUTO QDD QDD| QDDS TEM QUANTIDADES DIFERENTES DE NQBITS");

    no *n;
    n = apply_operacao(Q1->n,Q2->n,Q1->nqbit);

    lista *l;
    l = acha_lista_fim_arvore(n);

    QDD *Q;
    Q = cria_QDD(Q1->nqbit);
    Q->n = n;
    Q->l = l;
    reduz_QDD(Q,2,classe);

    contrai_QDD(Q,classe);
    libera_lista_lista(Q->l);
    Q->l = acha_lista_fim_QDD(Q);
    reduz_QDD(Q,1,4);

    return Q;
}



/**  Operações QDD algebricas  **/

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

void produto_QDD_real(QDD *Q, float re)
{
    if(Q == NULL)
        ERRO("PRODUTO QDD REAL| Q E NULL");

    lista *l;
    for(l = Q->l; l != NULL; l = l->l)
        produto_no_real(l->n,re);
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
    Q2a->nqbit = Q->nqbit;
    l1 = enlista_QDD(Q2a);
    for(l2 = l1; l2 != NULL; l2 = l2->l)
        if(l2->n->tipo == Meio)
            (l2->n->at.m.nivel) += nqbit1;
    libera_lista_lista(l1);

    QDD **Q2B;
    Long itens;
    itens = conta_itens_lista(Q1->l);
    Q2B = copia_QDD_varios(Q2a,itens);
    libera_QDD(Q2a);

    QDD *Q2b;
    no *n, *n1, *n2, *naux;
    Short i;
    l1 = Q->l;
    Q->l = NULL;
    i = 0;
    while(l1 != NULL)
    {
        n1 = l1->n;
        if(compara_no_fim_zero(n1,1))
        {
            l2 = l1->l;
            l1->l = Q->l;
            Q->l = l1;
            l1 = l2;

            libera_QDD(Q2B[i]);
        }
        else
        {
            Q2b = Q2B[i];
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
        i++;
    }
    libera_QDD_array(Q2B,itens);

    reduz_QDD(Q,1,4);
    return Q;
}

QDD* potencia_tensorial(QDD *Q, Short n)
{
    Short ex;
    for(ex = 1; 2*ex <= n; ex *= 2);
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
    Q->n = apply_soma(Q1->n,Q2->n,Q->nqbit);
    Q->l = acha_lista_fim_QDD(Q);
    reduz_QDD(Q,1,4);

    return Q;
}

QDD* subtracao_QDD(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    produto_QDD_real(Q2,-1);
    Q = soma_QDD(Q1,Q2);
    produto_QDD_real(Q2,-1);
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

float modulo_vetor(QDD *Q)
{
    QDD *Q1;
    no *n0, *n1;
    lista *l;
    Q1 = copia_QDD(Q);
    for(l = Q1->l; l != NULL; l = l->l)
    {
        n0 = l->n;

        n1 = modulo_2_no(n0);
        transfere_conexao(n1,n0);
        libera_no(n0);

        l->n = n1;
    }
    reduz_QDD(Q1,2,V);

    float m;
    contrai_QDD(Q1,V);
    n1 = Q1->l->n;
    m = sqrt(n1->at.f.re);
    libera_QDD(Q1);

    return m;
}



/**  Compara QDD  **/

Short compara_QDD_zero(QDD *Q, Short ex)
{
    if(Q == NULL)
        ERRO("COMPARA QDD|Q E NULL");

    lista *l;
    l = Q->l;
    if(l->l != NULL)
        return 0;

    Short s;
    s = compara_no_fim_zero(l->n,ex);
    return s;
}

Short compara_QDD(QDD *Q1, QDD *Q2, Short ex)
{
    Short N;
    if(Q1->nqbit != Q2->nqbit)
        return 0;
    N = Q1->nqbit;

    apply *a;
    a = cria_apply();
    a->n1 = Q1->n;
    a->n2 = Q2->n;
    encaixa_apply(a,N);

    no *n1, *n2;
    short i, j, res;
    res = 1;
    for(i=0; i<N; i++)
    {
        for(j=0; j<3; j++)
        {
            for(a = A[i][j]; A != NULL; a = a->a)
            {
                n1 = a->n1;
                n2 = a->n2;

                if(compara_no(n1,n2) == 0)
                {
                    res = 0;
                    break;
                }

            }
            if(res == 0)
                break;
        }
        if(res == 0)
            break;
    }
    limpa_apply_matriz(N);
    return res;
}



/**  Auxiliar QDD  **/

QDD* aplica(QDD *Q, Short N, Short n)
{
    if(n+Q->nqbit > N)
        ERRO("APLICA| QDD NAO SE ENCAIXA EM N");

    QDD *Q1, *Q2, *Q3;
    Q1 = copia_QDD(Q);
    if(n > 0)
    {
        Q2 = potencia_tensorial(QI,n);
        Q3 = produto_tensorial(Q2,Q1);

        libera_QDD(Q2);
        libera_QDD(Q1);

        Q1 = Q3;
    }
    if(N > n+(Q->nqbit))
    {
        Q2 = potencia_tensorial(QI,N-(n+(Q->nqbit)));
        Q3 = produto_tensorial(Q1,Q2);

        libera_QDD(Q2);
        libera_QDD(Q1);

        Q1 = Q3;
    }
    return Q1;
}

QDD** M01(Short N, Short n)
{
    QDD **Q;
    Q = cria_QDD_array(2);

    QDD *QIn, *Q1, *Q2;
    Q1 = matriz_delta_kronecker(0,0);
    Q2 = matriz_delta_kronecker(1,1);

    QDD *Q1aux, *Q2aux;
    Q1aux = NULL;
    Q2aux = NULL;
    if(n > 0)
    {
        QIn = potencia_tensorial(QI,n);
        Q1aux = produto_tensorial(QIn,Q1);
        Q2aux = produto_tensorial(QIn,Q2);

        libera_QDD(QIn);
        libera_QDD(Q1);
        libera_QDD(Q2);

        Q1 = Q1aux;
        Q2 = Q2aux;
    }

    if(N - n > 1)
    {
        QIn = potencia_tensorial(QI,N - n-1);
        Q1aux = produto_tensorial(Q1,QIn);
        Q2aux = produto_tensorial(Q2,QIn);

        libera_QDD(QIn);
        libera_QDD(Q1);
        libera_QDD(Q2);

        Q1 = Q1aux;
        Q2 = Q2aux;
    }

    Q[0] = Q1;
    Q[1] = Q2;
    return Q;
}



/** QDDs usuais  **/

QDD* Rx(double theta)
{
    no *ni, *n1, *n2, *n3;
    ni = cria_no_inicio();
    n1 = cria_no_meio(R,0);
    n2 = cria_no_meio(C,0);
    n3 = cria_no_meio(C,0);
    conecta_UM(ni,n1,Inicio);
    conecta_DOIS(n1,n2,n3);

    double si, co;
    theta /= 2;
    si = sin(theta);
    co = cos(theta);

    no *nf1,*nf2, *nf3, *nf4;
    nf1 = cria_no_fim( co,0);
    nf2 = cria_no_fim(-si,0);
    nf3 = cria_no_fim(0,si);
    nf4 = cria_no_fim(0,co);
    conecta_DOIS(n2,nf1,nf2);
    conecta_DOIS(n3,nf3,nf4);

    lista *l1, *l2, *l3, *l4;
    l1 = cria_lista();
    l2 = cria_lista();
    l3 = cria_lista();
    l4 = cria_lista();

    l1->n = nf1;
    l2->n = nf2;
    l3->n = nf3;
    l4->n = nf4;

    l1->l = l2;
    l2->l = l3;
    l3->l = l4;

    QDD *Q;
    Q = cria_QDD(1);
    Q->n = ni;
    Q->l = l1;
    return Q;
}

QDD* Ry(double theta)
{
    double si, co;
    theta /= 2;
    si = sin(theta);
    co = cos(theta);

    no *nf1, *nf2, *nf3;
    nf1 = cria_no_fim(-si,0);
    nf2 = cria_no_fim( co,0);
    nf3 = cria_no_fim( si,0);

    QDD *Q;
    Q = matriz_faixa_diagonal(nf1,nf2,nf3,1);
    return Q;
}

QDD* Rz(double theta)
{
    if(theta <  eps)
    if(theta > -eps)
        return I();

    no *nf0, *nf1, *nft;
    float si, co;
    si = sin(theta);
    co = cos(theta);
    nf0 = cria_no_fim(0,0);
    nf1 = cria_no_fim(1,0);
    nft = cria_no_fim(co,si);

    QDD *Q;
    Q = matriz_faixa_diagonal(nf1,nf0,nft,0);
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
        if(n >= ex)
        {
            n -= ex;
            conecta_DOIS(n1,nf0,n2);
        }
        else
        {
            conecta_DOIS(n1,n2,nf0);
        }

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
    Q->l = cria_lista_2(nf0,nf1);
    return Q;
}

QDD* ALL(Short N)
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

QDD* W(Short N)
{
    QDD *Q, *Qi, *Qaux;
    Q = BASE(N,1);

    Short i;
    Long ex;
    ex = 1;
    for(i=1; i<N; i++)
    {
        ex *= 2;

        Qi = BASE(N,ex);
        Qaux = soma_QDD(Q,Qi);

        libera_QDD(Q);
        libera_QDD(Qi);
        Q = Qaux;
    }

    float re;
    re = pow(N,-0.5);
    produto_QDD_real(Q,re);

    return Q;
}

QDD* controla(QDD *Q, Short controle, Short ativa) // controle: indice qbit controle, ativa 0 ou 1
{
    if(Q == NULL)
        ERRO("CONTROLE| Q E NULL");
    if(Q->nqbit < 2)
        ERRO("CONTROLE| Q PRECISA TER MAIS QUE 1 QBIT");
    if(Q->nqbit <= controle)
        ERRO("CONTROLE| BIT DE CONTROLE ONTROLE PRECISA SER MENOR QUE NQBIT");

    QDD **QM, *Q1, *Q2;
    QM = M01(Q->nqbit,controle);
    Q1 = QM[0];
    Q2 = QM[1];
    libera_QDD_array(QM,2);

    QDD *Q1aux, *Q2aux;
    if(ativa == 0)
    {
        Q1aux = produto_matriz_matriz(Q1,Q);
        libera_QDD(Q1);
        Q1 = Q1aux;
    }
    else
    {
        Q2aux = produto_matriz_matriz(Q2,Q);
        libera_QDD(Q2);
        Q2 = Q2aux;
    }

    QDD *Qc;
    Qc = soma_QDD(Q1,Q2);

    libera_QDD(Q1);
    libera_QDD(Q2);

    return Qc;
}

QDD* Switch(Short nqbit)
{
    QDD *Q1, *Q2, *Q3, *Q4;
    Q1 = Q00;
    Q2 = Q01;
    Q3 = Q10;
    Q4 = Q11;

    QDD *QIn;
    if(nqbit > 2)
    {
        QIn = potencia_tensorial(QI,nqbit-2);

        Q1 = produto_tensorial(Q1,QIn);
        Q2 = produto_tensorial(Q2,QIn);
        Q3 = produto_tensorial(Q3,QIn);
        Q4 = produto_tensorial(Q4,QIn);

        libera_QDD(QIn);
    }

    QDD *Q1aux, *Q2aux, *Q3aux, *Q4aux;
    Q1aux = produto_tensorial(Q1,Q00);
    Q2aux = produto_tensorial(Q2,Q10);
    Q3aux = produto_tensorial(Q3,Q01);
    Q4aux = produto_tensorial(Q4,Q11);

    if(nqbit > 2)
    {
        libera_QDD(Q1);
        libera_QDD(Q2);
        libera_QDD(Q3);
        libera_QDD(Q4);
    }

    Q3 = soma_QDD(Q3aux,Q4aux);
    Q2 = soma_QDD(Q2aux,Q3);
    Q1 = soma_QDD(Q1aux,Q2);

    libera_QDD(Q1aux);
    libera_QDD(Q2aux);
    libera_QDD(Q3aux);
    libera_QDD(Q4aux);

    libera_QDD(Q2);
    libera_QDD(Q3);

    return Q1;
}

QDD* QFT(QDD *Q)
{
    QDD *Qa;
    Qa = copia_QDD(Q);

    Short N;
    N = Q->nqbit;

    QDD **Qr;
    Qr = cria_QDD_array(N);
    Qr[0] = QH;

    QDD *Q1, *Q2;
    Short i;
    float theta;
    theta = pi;
    for(i=2; i<=N; i++)
    {
        theta /= 2;
        Q1 = Rz(theta);
        Q2 = aplica(Q1,i,0);
        libera_QDD(Q1);
        Q1 = controla(Q2,i-1,1);
        libera_QDD(Q2);
        Qr[i-1] = Q1;
    }

    Short j;
    for(j=N; j>0; j--)
    {
        printf("\n\nj: %d",j);
        for(i=0; i<j; i++)
        {
            printf("\n\ti: %d",i);
            Q1 = aplica(Qr[i],N,N-j);
            Q2 = produto_matriz_vetor(Q1,Qa);
            libera_QDD(Q1);
            libera_QDD(Qa);
            Qa = Q2;
        }
    }

    for(i=1; i<N; i++)
        libera_QDD(Qr[i]);
    libera_QDD_array(Qr,N);

    printf("\n\n");
    for(i=N; i>1; i-=2)
    {
        printf("\ni: %d",i);

        Q1 = Switch(i);
        Q2 = aplica(Q1,N,(N-i)/2);
        libera_QDD(Q1);

        Q1 = produto_matriz_vetor(Q2,Qa);
        libera_QDD(Q2);
        libera_QDD(Qa);
        Qa = Q1;
    }

    return Qa;
}

QDD* QFT_inv(QDD *Q)
{
    QDD *Qa;
    Qa = copia_QDD(Q);

    Short N;
    N = Q->nqbit;

    QDD *Q1, *Q2;
    short i;
    for(i=N; i>1; i-=2)
    {
        Q1 = Switch(i);
        Q2 = aplica(Q1,N,(N-i)/2);
        libera_QDD(Q1);

        Q1 = produto_matriz_vetor(Q2,Qa);
        libera_QDD(Q2);
        libera_QDD(Qa);
        Qa = Q1;
    }

    QDD **Qr;
    Qr = cria_QDD_array(N);
    Qr[0] = QH;

    float theta;
    theta = -pi;
    for(i=2; i<=N; i++)
    {
        theta /= 2;
        Q1 = Rz(theta);
        Q2 = aplica(Q1,i,0);
        libera_QDD(Q1);
        Q1 = controla(Q2,i-1,1);
        libera_QDD(Q2);
        Qr[i-1] = Q1;
    }

    Short j;
    for(j=1; j<=N; j++)
    {
        printf("\n\nj: %d",j);
        for(i=j; i>=0; i--)
        {
            printf("\n\ti: %d",i);
            Q1 = aplica(Qr[i],N,N-j);
            Q2 = produto_matriz_vetor(Q1,Qa);
            libera_QDD(Q1);
            libera_QDD(Qa);
            Qa = Q2;
        }
    }

    for(i=1; i<N; i++)
        libera_QDD(Qr[i]);
    libera_QDD_array(Qr,N);

    return Qa;
}



/** Auxiliar busca  **/

rota* concatena_rotas(rota *r1, rota *r2)
{
    rota *r, *rc, *rc1, *rc2;
    r = cria_rota_vazia();
    rc = r;
    for(rc1 = r1; rc1 != NULL; rc1 = rc1->r)
    {
        for(rc2 = r2; rc2 != NULL; rc2 = rc2->r)
        {
            rc->r = cria_rota_vazia();
            rc = rc->r;

            sprintf(rc->num,"%s%s",rc1->num,rc2->num);
        }
    }

    rc = r->r;
    libera_rota_no(r);
    r = rc;

    return r;
}

float probabilidade_total(busca *b)
{
    busca *bc;
    rota *r;
    float p, p_i;
    int i;
    p = 0;
    for(bc = b; bc != NULL; bc = bc->b)
    {
        p_i = bc->n->at.f.re;
        i=0;
        for(r = bc->r; r != NULL; r = r->r)
            i++;
        p += p_i*i;
    }
    return p;
}



/**   Busca   **/

rota* busca_rotas(no *n, Short N)
{
    busca *b;
    b = cria_busca();
    b->n = n;
    b->r = cria_rota_vazia();

    no *nc;
    lista *lc;
    busca *bc, *bn, *baux;
    rota *r, *rc;
    Short Na, Nc, i;
    while(b != NULL)
    {
        n = b->n;
        if(n == NULL)
            ERRO("BUSCA ROTAS| BUSCA COM NO VAZIO");

        if(n->tipo == Inicio)
            break;

        if(n->l == NULL)
            ERRO("BUSCA NOTAS| NO SEM ANTERIORES");

        Na = 0;
        switch(n->tipo)
        {
            case Meio:
                Na = n->at.m.nivel;
                break;

            case Fim:
                Na = N;
                break;
        }

        for(lc = n->l; lc != NULL; lc = lc->l)
        {
            nc = lc->n;

            Nc = 0;
            r  = NULL;
            switch(nc->tipo)
            {
                case Inicio:
                    Nc = Na;
                    r  = cria_rota_vazia();
                    break;

                case Meio:
                    Nc = Na - (nc->at.m.nivel) - 1;
                    if(nc->at.m.el == n)
                        r = cria_rota("0");
                    if(nc->at.m.th == n)
                        r = cria_rota("1");
                    break;
            }

            for(i=0; i<Nc; i++)
            {
                rc = concatena_rotas(r,rb);
                libera_rota_lista(r);
                r = rc;
            }
            rc = concatena_rotas(r,b->r);
            libera_rota_lista(r);
            r = rc;

            for(bc = b; bc != NULL; bc = bc->b)
                if(bc->n == nc)
                    break;

            if(bc == NULL)
            {
                bn = cria_busca();
                bn->n = nc;
                bn->r = r;

                switch(nc->tipo)
                {
                    case Inicio:
                        for(bc = b; bc->b != NULL; bc = bc->b);
                        bc->b = bn;
                        break;

                    case Meio:
                        for(bc = b; bc->b != NULL; bc = bc->b)
                        {
                            baux = bc->b;
                            if(baux->n->tipo == Inicio)
                                break;
                            if(baux->n->at.m.nivel < nc->at.m.nivel)
                                break;
                        }
                        bn->b = bc->b;
                        bc->b = bn;
                        break;
                }
            }
            else
            {
                for(rc = bc->r; rc->r != NULL; rc = rc->r);
                rc->r = r;
            }
        }

        bc = b->b;
        if(bc == NULL)
            break;
        libera_rota_lista(b->r);
        libera_busca_no(b);
        b = bc;
    }
    if(b == NULL)
        ERRO("BUSCA ROTAS| NAO RETORNOU BUSCA");

    r = b->r;
    libera_busca_no(b);

    return r;
}

busca* busca_mais_provavel(QDD *Q)
{
    busca *b;
    b    = cria_busca();
    b->n = Q->l->n;

    no *n, *naux;
    lista *l;
    double p;
    p = 0;
    for(l = Q->l; l!= NULL; l = l->l)
    {
        n    = l->n;
        naux = modulo_2_no(n);

        if(naux->at.f.re > p)
        {
            b->n = n;
            b->p = naux->at.f.re;

            p = naux->at.f.re;
        }

        libera_no(naux);
    }

    b->r = busca_rotas(b->n,Q->nqbit);

    return b;
}

busca* busca_probabilidade_maior(QDD *Q, double p) // Retorna nulo caso não ache nada
{
    no *n0, *n1;
    lista *l;
    Q = copia_QDD(Q);
    for(l = Q->l; l != NULL; l = l->l)
    {
        n0 = l->n;

        n1 = modulo_2_no(n0);
        transfere_conexao(n1,n0);
        libera_no(n0);

        l->n = n1;
    }
    reduz_QDD(Q,1,4);

    busca *b, *bc;
    b = cria_busca();
    bc = b;
    for(l = Q->l; l != NULL; l  = l->l)
    {
        n0 = l->n;
        if(n0->at.f.re > p)
        {
            bc->b = cria_busca();
            bc = bc->b;

            bc->p = n0->at.f.re;
            bc->r = busca_rotas(n0,Q->nqbit);

        }
    }

    bc = b->b;
    libera_busca_no(b);
    b = bc;

    libera_QDD(Q);

    return b;
}

no* acessa_amplitude(QDD *Q, Long num)
{
    Short N;
    N = Q->nqbit;

    no *n;
    Short i;
    Long exp_2;
    exp_2 = pow(2,N-1);
    n = Q->n->at.i.n;
    for(i=0; i<N; i++)
    {
        if(i == n->at.m.nivel)
        {
            if(num < exp_2)
                n = n->at.m.el;
            else
                n = n->at.m.th;
        }

        if(num > exp_2)
            num -= exp_2;
        exp_2 /= 2;
    }
    return n;
}



/**  Auxiliar Medidas  **/

float gera_aleatorio()
{
    int aleatorio;
    if(ale == 0)
    {
        ale = 1;
        srand(1e5*time(NULL));
    }
    aleatorio = rand();

    float P;
    P = aleatorio;
    P /= RAND_MAX;
    return P;
}

void salva_destrutivo_recursivo(FILE *fp, destrutivo *d)
{
    fprintf(fp,"\n%e %e",d->p[0],d->p[1]);
    for(int i=0; i<2; i++)
    {
        if(d->Q[i] != NULL)
        {
            fprintf(fp,"\nQ");
            salva_QDD(fp,d->Q[i]);
        }

        if(d->r[i] != NULL)
        {
            fprintf(fp,"\nR");
            fprintf(fp,"\n%s",d->r[i]->num);
        }

        if(d->d[i] != NULL)
        {
            fprintf(fp,"\nD");
            salva_destrutivo_recursivo(fp,d->d[i]);
        }
    }
}

void salva_destrutivo(destrutivo *d, char *nome)
{
    FILE *fp;
    char Nome[30];
    sprintf(Nome,"%s.des",nome);
    fp = fopen(Nome,"w");

    fprintf(fp,"D");
    salva_destrutivo_recursivo(fp,d);
    fclose(fp);
}

destrutivo* le_destrutivo_recursivo(FILE *fp)
{
    destrutivo *d;
    d = cria_destrutivo();

    float p[2];
    fscanf(fp,"\n%e",&p[0]);
    fscanf(fp," %e",&p[1]);
    d->p[0] = p[0];
    d->p[1] = p[1];

    rota *r;
    Short i;
    char c, num[65];
    for(int o=0; o<100; o++)
    {
        fscanf(fp,"\n%c",&c);
        printf("\nA%cA",c);
    }

    for(i=0; i<2; i++)
    {
        switch(c)
        {
            case 'Q':
                le_QDD(fp);
                break;

            case 'R':
                fscanf(fp,"\n%s",num);
                r = cria_rota(num);
                d->r[i] = r;
                break;

            case 'D':
                d->d[i] = le_destrutivo_recursivo(fp);
                break;

            default:
                ERRO("LE DESTRUTIVO RECURSIVO| NAO LEU NADA");
                break;
        }
    }
    return d;
}

destrutivo* le_destrutivo(char *nome)
{
    FILE *fp;
    char Nome[30];
    sprintf(Nome,"%s.des",nome);
    fp = fopen(Nome,"w");
    fscanf(fp,"D");

    destrutivo *d;
    d = le_destrutivo_recursivo(fp);
    fclose(fp);

    return d;
}



/**   Medidas   **/

QDD** mede_conservativo(QDD *Q, Short nqbit, float p[2])
{
    QDD **QM, **QF;
    QM = M01(Q->nqbit,nqbit);
    QF = cria_QDD_array(2);

    QF[0] = produto_matriz_vetor(QM[0],Q);
    QF[1] = subtracao_QDD(Q,QF[0]);

    float paux[2]; //paux armazena as amplitudes e p as probabilidades

    paux[0] = modulo_vetor(QF[0]);
    p[0] = paux[0]*paux[0];
    p[1] = 1-p[0];
    paux[1] = sqrt(p[1]);

    float P;
    P = p[0] + p[1];
    P -= 1;
    if(P < 0)
        P *= -1;

    if(P > 0.05)
        ERRO("MEDE CONSERVATIVO| PROBABILIDADE NAO SOMA 1");

    if(p[0] > eps)
        produto_QDD_real(QF[0],1/paux[0]);
    if(p[1] > eps)
        produto_QDD_real(QF[1],1/paux[1]);

    libera_QDD(QM[0]);
    libera_QDD(QM[1]);
    libera_QDD_array(QM,2);

    return QF;
}

QDD* mede_destrutivo(QDD *Q, Short nqbit, Short *resultado)
{
    QDD **QM;
    float p[2];
    QM = mede_conservativo(Q,nqbit,p);

    float P;
    P = gera_aleatorio();
    if(P < p[0])
    {
        Q = QM[0];
        libera_QDD(QM[1]);
        *resultado = 0;
    }
    else
    {
        Q = QM[1];
        libera_QDD(QM[0]);
        *resultado = 1;
    }
    libera_QDD_array(QM,2);
    return Q;
}

void mede_individual(QDD *Q)
{
    time_t antes, depois;
    float delta, tempo;

    QDD **QM;
    Short i;
    float p[2];
    printf("\n");
    for(i=0;i<Q->nqbit;i++)
    {
        antes = clock();
        QM = mede_conservativo(Q,i,p);
        libera_QDD(QM[0]);
        libera_QDD(QM[1]);
        libera_QDD_array(QM,2);
        depois = clock();

        delta = depois - antes;
        tempo = delta/CLOCKS_PER_SEC;

        printf("\ni: %2hu\tp0: %f\tp1: %f\tt: %.3f",i,p[0],p[1],tempo);
    }
}

QDD* mede_tudo_unico(QDD *Q)
{
    QDD *Qr;
    Qr = copia_QDD(Q);

    QDD *Qaux;
    Short i, N, p;
    N = Q->nqbit;
    for(i=0; i<N; i++)
    {
        Qaux = mede_destrutivo(Qr,i,&p);
        libera_QDD(Qr);
        Qr = Qaux;
    }
    return Qr;
}

rota* mede_tudo_varios(QDD *Q, Long n)
{
    Short N;
    N = Q->nqbit;

    destrutivo *d;
    d = cria_destrutivo();
    d->Q = mede_conservativo(Q,0,d->p);

    rota *r, *rc;
    r  = cria_rota_vazia();
    rc = r;

    Long mostra, amostragem, tam;
    mostra     = 100;
    amostragem = 100;
    tam = n/amostragem;
    if(tam == 0)
        tam = 1;
    printf("\nMostra: %d\ttam: %d\tn: %d", mostra,tam,n);

    destrutivo *dc, *daux;
    busca *b;
    Short P;
    Long i, j;
    float p;
    for(i=0; i<n; i++)
    {
        if(i < mostra)
            printf("\ni: %4d\tj:",i);
        if(i == mostra)
            printf("\n");
        if(i >= mostra)
        if(i%tam == 0)
            printf("\ni: %d/%d",i/tam,amostragem);

        dc = d;
        for(j=1; j<=N; j++)
        {

            if(i < mostra)
                printf(" %d",j);

            p = gera_aleatorio();
            if(p < dc->p[0])
                P = 0;
            else
                P = 1;

            if(j < N)
            {
                // Não acabou

                if(dc->d[P] == NULL)
                {
                    // Nao saiu esse resultado

                    Q = dc->Q[P];
                    dc->Q[P] = NULL;

                    daux = cria_destrutivo();
                    daux->Q = mede_conservativo(Q,j,daux->p);
                    libera_QDD(Q);

                    dc->d[P] = daux;
                    dc = daux;
                }
                else
                {
                    // Ja saiu esse resultado

                    dc = dc->d[P];
                }
            }
            else
            {
                // Acabou

                if(dc->r[0] == NULL)
                {
                    for(int k=0; k<2; k++)
                    {
                        Q = dc->Q[k];
                        dc->Q[k] = NULL;

                        b = busca_mais_provavel(Q);
                        dc->r[k] = b->r;

                        libera_QDD(Q);
                        libera_busca_no(b);
                    }
                }

                rc->r = copia_rota(dc->r[P]);
                rc = rc->r;
            }
        }
    }

    rc = r->r;
    libera_rota_no(r);
    r = rc;

    salva_destrutivo(d,"EITA");
    libera_destrutivo_arvore(d);

    return r;
}


/**  Testes  **/

void mostra_hora(FILE *fp)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    printf("\t%02d:%02d  %02d/%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_mday,timeinfo->tm_mon+1);
    if(fp != NULL)
        fprintf(fp,"\t%02d:%02d  %02d/%02d",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_mday,timeinfo->tm_mon+1);
}

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

        mostra_hora(fr);

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

void teste_aleatorio_fourrier(Short N)
{
    Long max;
    max = pow(2,N);

    float *p;
    Long i;
    p = malloc(max*sizeof(float));
    for(i=0; i<max; i++)
        p[i] = gera_aleatorio();

    no **n;
    n = cria_no_array(max);
    for(i=0; i<max; i++)
        n[i] = cria_no_fim(p[i],0);
    free(p);

    QDD *Q;
    Q = cria_QDD_vetor(n,N);
    libera_no_array(n,max);

    float m;
    m = modulo_vetor(Q);
    produto_QDD_real(Q,1/m);

    QDD *Qft;
    Qft = QFT(Q);
    libera_QDD(Q);

    salva_QDD_sozinho(Qft,"Ale10");
    libera_QDD(Qft);

    mostra_quantidades();
}



int main()
{
    inicia_relatorio_memoria(0);
    configuracao(20);
    inicia_structs_globais();
    setlocale(LC_ALL, "Portuguese");
    /***********************************/

    /*QDD *Q;
    Q = le_QDD_sozinho("QftW3");
    printf("Leu\n");

    rota *r;
    r = mede_tudo_varios(Q,1000);

    libera_QDD(Q);
    libera_rota_lista(r);

    mostra_quantidades();*/

    /***/

    destrutivo *d;
    d = le_destrutivo("EITA");
    mostra_destrutivo_arvore(d);

    /***********************************/
    finaliza_structs_globais();
    finaliza_relatorio_memoria();
    return 0;
}

