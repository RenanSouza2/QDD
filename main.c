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



FILE *fm;
unsigned long mem = 0, memMax = 0, iQ = 0, ii = 0, im = 0, ifi = 0, il = 0, ia = 0, ic = 0, is = 0;
unsigned short print, nqbit;
float eps;



struct QDD
{
    struct no *n;
    unsigned short nqbit;
    struct lista *l;
};

struct inicio
{
    struct no *n;
};

struct meio
{
    unsigned short classe, nivel;
    struct no *el, *th;
};

struct fim
{
    float re, im;
};

struct no
{
    unsigned short tipo;
    struct lista *l;
    union atributos
    {
        struct inicio i;
        struct meio m;
        struct fim f;
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
    unsigned short nivel;
    struct conta *cV, *cC, *cR;
    struct suporte *s;
};

struct quant
{
    unsigned long mem, iQ, ii, im, ifi, il, ia, ic;
};



typedef struct QDD QDD;

typedef struct no no;

typedef struct lista lista;

typedef struct inicio inicio;

typedef struct meio meio;

typedef struct fim fim;

typedef struct apply apply;

typedef struct conta conta;

typedef struct suporte suporte;

typedef struct quant quant;

typedef unsigned short Short;

typedef unsigned long Long;



void ERRO(char *s)
{
    printf("\n\nERRO %s",s);
    if(print)
        fprintf(fm,"\n\nERRO %s",s);
    exit(EXIT_FAILURE);
}



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



QDD* cria_QDD()
{
    QDD *Q;
    Q = malloc(sizeof(QDD));
    if(Q == NULL)
        ERRO("QDD");
    aumenta_memoria(sizeof(QDD));
    iQ++;

    Q->n = NULL;
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

    inicio i;
    i.n = NULL;
    n->at.i = i;

    return n;
}

no* cria_no_meio(Short classe, Short nivel)
{
    no *n;
    n = malloc(sizeof(no));
    if(n == NULL)
        ERRO("MEIO");
    aumenta_memoria(sizeof(no));
    im++;

    n->tipo = Meio;
    n->l = NULL;

    meio m;
    m.classe = classe;
    m.nivel = nivel;
    m.el = NULL;
    m.th = NULL;
    n->at.m = m;

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

    fim f;
    f.re = re;
    f.im = im;
    n->at.f = f;

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
    if(a == NULL)
        ERRO("APPLY");
    aumenta_memoria(sizeof(apply));
    ia++;

    a->n = NULL;
    a->n1 = NULL;
    a->n2 = NULL;

    a->a = NULL;
    a->a1 = NULL;
    a->a2 = NULL;

    return a;
}

conta* cria_conta(Short nivel)
{
    conta *c;
    c = malloc(sizeof(conta));
    if(c == NULL)
        ERRO("CONTA");
    ic++;

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
        ERRO("SUPORTE");
    aumenta_memoria(sizeof(suporte));
    is++;

    s->nivel = nivel;
    s->cV = NULL;
    s->cC = NULL;
    s->cR = NULL;
    s->s = NULL;
    return s;
}

quant cria_quantidades()
{
    quant q;
    q.ia = ia;
    q.ic = ic;
    q.ifi = ifi;
    q.ii = ii;
    q.il = il;
    q.im = im;
    q.iQ = iQ;
    q.mem = mem;
    return q;
}



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
        if(im == 0)
            ERRO("LIBERA MEIO");
        im--;
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

void libera_apply_no(apply *a)
{
    diminui_memoria(sizeof(apply));
    if(ia == 0)
        ERRO("LIBERA APPLY");
    ia--;
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

void libera_conta(conta *c)
{
    diminui_memoria(sizeof(conta));
    if(ic == 0)
        ERRO("LIBERA CONTA");
    ic--;
    free(c);
}

void libera_suporte(suporte *s)
{
    if(is == 0)
        ERRO("LIBERA SUPORTE");
    diminui_memoria(sizeof(suporte));
    is--;
    free(s);
}


lista* enlista_arvore(no *N)
{
    /**  l é a lista final                      ***
    ***  la é a pilha de nós para adicionar a l  **/
    lista *l, *la, *lc, *lf;
    no *n;

    l = cria_lista();
    la = cria_lista();
    la->n = N;
    lf = l;

    while(la != NULL)
    {
        n = la->n;
        /**  Verifica se o no n ká está na lista  **/
        for(lc = l; lc != NULL; lc = lc->l)
            if(lc->n == n)
                break;
        if(lc == NULL)
        {
            /**  caso não esteja  **/
            lf->l = cria_lista();
            lf = lf->l;
            lf->n = n;

            switch(n->tipo)
            {
                case Inicio:
                la->n = n->at.i.n;
                break;

                case Meio:
                la->n = n->at.m.th;
                lc = cria_lista();
                lc->n = n->at.m.el;
                lc->l = la;
                la = lc;
                break;
            }
        }
        else
        {
            /**  caso já esteja  */
            lc = la->l;
            libera_lista_no(la);
            la = lc;
        }
    }

    la = l->l;
    libera_lista_no(l);
    l = la;

    return l;
}

lista* enlista_QDD(QDD *Q)
{
    lista *l;
    l = cria_lista();
    l->n = Q->n->l->n;
    l->l = enlista_arvore(Q->n);
    return l;
}



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
    printf("\nEndereco: %d\n",n);
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
        printf("\tThen: %d\n",n->at.m.th);
        break;

        case Fim:
        printf(": Numero\n");
        printf("%f %f\n",n->at.f.re,n->at.f.im);
        break;
    }
    printf("\n");
}

void mostra_lista_com_no(lista *l)
{
    lista *lc;
    Long ligacao = 0;
    lc = l;
    printf("\n");
    for(lc = l; lc != NULL; lc = lc->l)
    {
        printf("\tLigacao %u: %d\n",ligacao,lc->n);
        mostra_no(lc->n);
        ligacao++;
    }
}

void mostra_arvore(no *n)
{
    lista *l;
    l = enlista_arvore(n);
    mostra_lista_com_no(l);
    libera_lista_lista(l);
}

void mostra_apply(apply *a)
{
    printf("\nEndereco (apply): %d\n",a);
    printf("\nN1");
    mostra_no(a->n1);
    printf("N2");
    mostra_no(a->n2);
    if(a->n != NULL)
    {
        printf("N");
        mostra_no(a->n);
    }
    else
        printf("\nSem N");
    if(a->a1 != NULL)
    {
        printf("\nLigacoes posteriores (apply):");
        printf("\n\t%d",a->a1);
        printf("\n\t%d",a->a2);
    }
    printf("\nProximo: %d",a->a);
}

void mostra_apply_lista(apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
        printf("\nLigacao %d: %d",i++,ac);
}

void mostra_apply_lista_com_no(apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        printf("\n\n\n\n\nLigacao %d: %d\n",i++,ac);
        mostra_apply(ac);
    }
}

void mostra_conta_no(conta *c)
{
    printf("\nEndereco (conta): %d",c);
    printf("\nNivel: %d",c->nivel);
    printf("\nNo: ");
    mostra_no(c->n);
    printf("\nProximo: %d",c->c);
}

void mostra_conta_lista(conta *c)
{
    conta *cc;
    cc = c;
    while(cc != NULL)
    {
        mostra_conta_no(cc);
        cc = cc->c;
    }
}

void mostra_suporte_no(suporte *s)
{
    printf("\nEndereco (suporte): %d",s);
    printf("\nNivel: %d",s->nivel);
    printf("\nLista conta V");
    mostra_conta_lista(s->cV);
    printf("\nLista conta R");
    mostra_conta_lista(s->cR);
    printf("\nLista conta C");
    mostra_conta_lista(s->cC);
    printf("\nProximo (suporte): %d",s->s);
}

void mostra_suporte_lista(suporte *s)
{
    suporte *sc;
    for(sc = s; sc != NULL; sc = sc->s)
        mostra_suporte_no(sc);
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
    if(im != 0)
    {
        vazio = 0;
        printf("\nm:   %d",im);
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
    printf("\na:   %d",sizeof(apply));
    printf("\nc:   %d",sizeof(conta));
    printf("\n");
}



void fmostra_lista(FILE *fp, lista *l)
{
    lista *lc;
    Long ligacao = 0;
    lc = l;
    for(lc = l; lc != NULL; lc = lc->l)
    {
        fprintf(fp,"\tLigacao %u: %d\n",ligacao,lc->n);
        ligacao++;
    }
}

void fmostra_no(FILE *fp, no *n)
{
    fprintf(fp,"\nEndereco: %d\n",n);
    if(n == NULL)
        return;
    if(n->l != NULL)
    {
        fprintf(fp,"Ligacoes anteriores:\n");
        fmostra_lista(fp,n->l);
    }
    fprintf(fp,"Tipo");
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
        fprintf(fp,"\tThen: %d\n",n->at.m.th);
        break;

        case Fim:
        fprintf(fp,": Numero\n");
        fprintf(fp,"%f %f\n",n->at.f.re,n->at.f.im);
        break;
    }
    fprintf(fp,"\n");
}

void fmostra_lista_com_no(FILE *fp, lista *l)
{
    lista *lc;
    Long ligacao = 0;
    lc = l;
    fprintf(fp,"\n");
    for(lc = l; lc != NULL; lc = lc->l)
    {
        fprintf(fp,"\tLigacao %u: %d\n",ligacao,lc->n);
        fmostra_no(fp,lc->n);
        ligacao++;
    }
}

void fmostra_arvore(FILE *fp, no *n)
{
    lista *l;
    l = enlista_arvore(n);
    fmostra_lista_com_no(fp,l);
    libera_lista_lista(l);
}

void fmostra_apply(FILE *fp, apply *a)
{
    fprintf(fp,"\nEndereco (apply): %d\n",a);
    fprintf(fp,"\nN1");
    fmostra_no(fp,a->n1);
    fprintf(fp,"N2");
    fmostra_no(fp,a->n2);
    if(a->n != NULL)
    {
        fprintf(fp,"N");
        fmostra_no(fp,a->n);
    }
    else
        fprintf(fp,"\nSem N");
    if(a->a1 != NULL)
    {
        fprintf(fp,"\nLigacoes posteriores (apply):");
        fprintf(fp,"\n\t%d",a->a1);
        fprintf(fp,"\n\t%d",a->a2);
    }
    fprintf(fp,"\nProximo: %d",a->a);
}

void fmostra_apply_lista(FILE *fp, apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
        fprintf(fp,"\nLigacao %d: %d",i++,ac);
}

void fmostra_apply_lista_com_no(FILE *fp, apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        fprintf(fp,"\n\n\n\n\nLigacao %d: %d\n",i++,ac);
        fmostra_apply(fp,ac);
    }
}

void fmostra_conta_no(FILE *fp, conta *c)
{
    fprintf(fp,"\nEndereco (conta): %d",c);
    fprintf(fp,"\nNivel: %d",c->nivel);
    fprintf(fp,"\nNo: ");
    fmostra_no(fp,c->n);
    fprintf(fp,"\nProximo: %d",c->c);
}

void fmostra_conta_lista(FILE *fp, conta *c)
{
    conta *cc;
    cc = c;
    while(cc != NULL)
    {
        fmostra_conta_no(fp,cc);
        cc = cc->c;
    }
}

void fmostra_suporte_no(FILE *fp, suporte *s)
{
    fprintf(fp,"\nEndereco (suporte): %d",s);
    fprintf(fp,"\nNivel: %d",s->nivel);
    fprintf(fp,"\nLista conta V");
    fmostra_conta_lista(fp,s->cV);
    fprintf(fp,"\nLista conta R");
    fmostra_conta_lista(fp,s->cR);
    fprintf(fp,"\nLista conta C");
    fmostra_conta_lista(fp,s->cC);
    fprintf(fp,"\nProximo (suporte): %d",s->s);
}

void fmostra_suporte_lista(FILE *fp, suporte *s)
{
    suporte *sc;
    for(sc = s; sc != NULL; sc = sc->s)
        fmostra_suporte_no(fp,sc);
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
    if(ii != 0)
    {
        vazio = 0;
        fprintf(fp,"\ni:   %d",ii);
    }
    if(im != 0)
    {
        vazio = 0;
        fprintf(fp,"\nm:   %d",im);
    }
    if(ifi != 0)
    {
        vazio = 0;
        fprintf(fp,"\nf:   %d",ifi);
    }
    if(il != 0)
    {
        vazio = 0;
        fprintf(fp,"\nl:   %d",il);
    }
    if(ia != 0)
    {
        vazio = 0;
        fprintf(fp,"\na:   %d",ia);
    }
    if(ic != 0)
    {
        vazio = 0;
        fprintf(fp,"\nc:   %d",ic);
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
    fprintf(fp,"\n");
}



void conecta_UM(no *n1, no *n2, Short lado)
{
    lista *l;

    if(n1->tipo == Fim)
        ERRO("FINAL NAO CONECTA");

    switch(lado)
    {
        case 0:
        n1->at.i.n = n2;
        break;

        case 1:
        n1->at.m.el = n2;
        break;

        case 2:
        n1->at.m.th = n2;
        break;
    }

    l = cria_lista();
    l->n = n1;
    l->l = n2->l;
    n2->l = l;
}

void conecta_DOIS(no *n, no *el, no *th)
{
    conecta_UM(n,el,1);
    conecta_UM(n,th,2);
}

Short desconecta_UM(no *n1, no *n2)
{
    lista *l, *lc, *laux;
    Short lado;
    if(n1->tipo == Fim)
        ERRO("FINAO NAO DESCONECTA");
    if(n1->tipo == Inicio)
    {
        n1->at.i.n = NULL;
        lado = 0;
    }
    else
    {
        if(n1->at.m.el == n2)
        {
            n1->at.m.el = NULL;
            lado = 1;
        }
        else
        {
            n1->at.m.th = NULL;
            lado = 2;
        }
    }

    l = cria_lista();
    l->l = n2->l;
    for(lc = l; lc->l != NULL; lc = lc->l)
    {
        if(lc->l->n == n1)
        {
            laux = lc->l;
            lc->l = laux->l;
            libera_lista_no(laux);
            break;
        }
    }
    n2->l = l->l;
    libera_lista_no(l);
    return lado;
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



void libera_arvore(no *n)
{
    no *el, *th;
    lista *l, *laux;
    if(n->l != NULL)
        ERRO("ARVORE CONECTADA");
    l = cria_lista();
    l->n = n;
    while(l != NULL)
    {
        n = l->n;

        switch(n->tipo)
        {
            case Inicio:
            el = n->at.i.n;
            desconecta_DOIS(n);
            libera_no(n);

            l->n = el;
            break;

            case Meio:
            el = n->at.m.el;
            th = n->at.m.th;
            desconecta_DOIS(n);
            libera_no(n);

            if(th->l == NULL)
            {
                l->n = th;
                if(el->l == NULL)
                {
                    laux = cria_lista();
                    laux->n = el;
                    laux->l = l;
                    l = laux;
                }
            }
            else
            {
                if(el->l == NULL)
                {
                    l->n = el;
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
    libera_arvore(Q->n->l->n);
    libera_lista_lista(Q->l);
    libera_no_QDD(Q);
}



no* copia_no(no *n1)
{
    no *n2;
    n2 = NULL;
    switch(n1->tipo)
    {
        case Inicio:
        n2 = cria_no_inicio();
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

Short compara_no_meio(no *n1, no *n2)
{
    if(n1             != n2       )
    if(n1->tipo       == n2->tipo )
    if(n1->at.m.nivel == n2->at.m.nivel)
    if(n1->at.m.el    == n2->at.m.el   )
    if(n1->at.m.th    == n2->at.m.th   )
        return 1;
    return 0;
}

Short compara_no_fim(no *n1, no *n2)
{
    float re, im;
    re = (n1->at.f.re)-(n2->at.f.re);
    im = (n1->at.f.im)-(n2->at.f.im);

    if(re>-eps)
    if(re< eps)
    if(im>-eps)
    if(im< eps)
        return 1;
    return 0;
}

Short compara_zero(no *n)
{
    float re, im;
    re = n->at.f.re;
    im = n->at.f.im;

    if(re>-eps)
    if(re< eps)
    if(im>-eps)
    if(im< eps)
        return 1;
    return 0;
}

Short compara_apply(apply *a1, apply *a2)
{
    if(a1->n1 == a2->n1)
    if(a1->n2 == a2->n2)
        return 1;
    return 0;
}



lista* copia_lista_com_cabeca(lista *l1)
{
    lista *l2, *lc, *lc2;
    l2 = cria_lista();
    lc2 = l2;
    for(lc = l1; lc != NULL; lc = lc->l)
    {
        lc2->l = cria_lista();
        lc2 = lc2->l;
        lc2->n = lc->n;
    }
    return l2;
}

lista* copia_lista_sem_cabeca(lista *l1)
{
    lista *l2, *laux;
    l2 = copia_lista_com_cabeca(l1);
    laux = l2;
    l2 = laux->l;
    libera_lista_no(laux);
    return l2;
}



no* soma(no *n1, no *n2)
{
    no *n;
    float re, im;

    re = n1->at.f.re + n2->at.f.re;
    im = n1->at.f.im + n2->at.f.im;
    n = cria_no_fim(re,im);

    return n;
}

no* produto_complexo(no *n1, no *n2)
{
    no *n;
    float re, im;
    re = (n1->at.f.re)*(n2->at.f.re)-(n1->at.f.im)*(n2->at.f.im);
    im = (n1->at.f.re)*(n2->at.f.im)+(n1->at.f.im)*(n2->at.f.re);
    n = cria_no_fim(re,im);
    return n;
}

no* produto_complexo_conjugado(no *n1, no *n2)
{
    no *n;
    float re, im;
    re = (n1->at.f.re)*(n2->at.f.re)+(n1->at.f.im)*(n2->at.f.im);
    im = (n1->at.f.re)*(n2->at.f.im)-(n1->at.f.im)*(n2->at.f.re);
    n = cria_no_fim(re,im);
    return n;
}

void produto_por_real(no *n, float re)
{
    (n->at.f.re) *= re;
    (n->at.f.im) *= im;
}

no* copia_arvore(no *n)
{

    /**  l1 guarda Q1 elistado   ***
    ***  l2 lista dos nos de l2  **/
    lista *l1, *l2, *lc1a, *lc2a, *lc1b, *lc2b;
    no *n1, *n2, *nf, *nt1, *nt2;

    l1 = enlista_arvore(n);
    l2 = copia_lista_sem_cabeca(l1);
    for(lc2a = l2; lc2a != NULL; lc2a = lc2a->l)
        lc2a->n = copia_no(lc2a->n);

    lc1a = l1;
    lc2a = l2;
    do
    {
        /**  lc1a perocrre a lista 1 pela primeira vez  ***
        ***  lc2a percorre a lista 2 pela primeira vez  **/
        n1 = lc1a->n;
        n2 = lc2a->n;

        switch(n1->tipo)
        {
            case Inicio:
            nf = n1->at.i.n;

            lc1b = l1;
            lc2b = l2;
            do
            {
                /**  lc1a perocrre a lista 1 pela segunda vez para buscar filhos  ***
                ***  lc2a percorre a lista 2 pela segunda vez para buscar filhos  **/
                nt1 = lc1b->n;
                nt2 = lc2b->n;

                lc1b = lc1b->l;
                lc2b = lc2b->l;
            }
            while(nt1 != nf);

            conecta_UM(n2,nt2,0);
            break;

            case Meio:
            nf = n1->at.m.el;

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

            nf = n1->at.m.th;

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

            conecta_UM(n2,nt2,2);
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

no* apply_soma(no *N1, no *N2)
{
    apply *a;
    a = cria_apply();
    a->n1 = N1;
    a->n2 = N2;

    apply *ac, *a1 = NULL, *a2 = NULL, *aaux;
    no *n, *n1, *n2;
    Short regra = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        n1 = ac->n1;
        n2 = ac->n2;
        switch(n1->tipo)
        {
            case Meio:
            switch(n2->tipo)
            {
                case Meio:
                if(n1->at.m.nivel < n2->at.m.nivel)
                    regra = 1;
                if(n1->at.m.nivel > n2->at.m.nivel)
                    regra = 2;
                if(n1->at.m.nivel == n2->at.m.nivel)
                {
                    if(n1->at.m.classe < n2->at.m.classe)
                        regra = 1;
                    if(n1->at.m.classe > n2->at.m.classe)
                        regra = 2;
                    if(n1->at.m.classe == n2->at.m.classe)
                        regra = 3;
                }
                break;

                case Fim:
                regra = 1;
                break;
            }
            break;

            case Fim:
            switch(n2->tipo)
            {
                case Meio:
                regra = 2;
                break;

                case Fim:
                regra = 4;
                break;
            }
            break;
        }

        switch(regra)
        {
            case 1:
            n = copia_no(n1);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2;
            break;

            case 2:
            n = copia_no(n2);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1;
            a2->n2 = n2->at.m.th;
            break;

            case 3:
            n = copia_no(n1);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2->at.m.th;
            break;

            case 4:
            n = soma(n1,n2);
            ac->n = n;
            break;
        }

        if(regra != 4)
        {
            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(compara_apply(aaux,a1))
                    break;
            if(aaux == NULL)
            {
                ac->a1 = a1;
                a1->a = ac->a;
                ac->a = a1;
            }
            else
            {
                ac->a1 = aaux;
                libera_apply_no(a1);
            }


            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(compara_apply(aaux,a2))
                    break;
            if(aaux == NULL)
            {
                ac->a2 = a2;
                a2->a = ac->a;
                ac->a = a2;
            }
            else
            {
                ac->a2 = aaux;
                libera_apply_no(a2);
            }
        }
    }

    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n;
        if(n->tipo == Meio)
        {
            a1 = ac->a1;
            a2 = ac->a2;

            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(aaux == a1)
                    break;
            n1 = aaux->n;

            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(aaux == a2)
                    break;
            n2 = aaux->n;

            conecta_DOIS(n,n1,n2);
        }
    }
    n = a->n;
    libera_apply_lista(a);
    return n;
}

no* apply_produto_interno(no *N1, no *N2)
{
    apply *a;
    a = cria_apply();
    a->n1 = N1;
    a->n2 = N2;

    apply *ac, *a1 = NULL, *a2 = NULL, *aaux;
    no *n, *n1, *n2;
    Short regra = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        n1 = ac->n1;
        n2 = ac->n2;

        switch(n1->tipo)
        {
            case Meio:
            switch(n2->tipo)
            {
                case Meio:
                if(n1->at.m.nivel < n2->at.m.nivel)
                    regra = 1;
                if(n1->at.m.nivel > n2->at.m.nivel)
                    regra = 2;
                if(n1->at.m.nivel == n2->at.m.nivel)
                {
                    if(n1->at.m.classe < n2->at.m.classe)
                        regra = 1;
                    if(n1->at.m.classe > n2->at.m.classe)
                        regra = 2;
                    if(n1->at.m.classe == n2->at.m.classe)
                        regra = 3;
                }
                break;

                case Fim:
                if(compara_zero(n2))
                    regra = 5;
                else
                    regra = 1;
                break;
            }
            break;

            case Fim:
            if(compara_zero(n1))
            {
                regra = 5;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Meio:
                    regra = 2;
                    break;

                    case Fim:
                    if(compara_zero(n2))
                        regra = 5;
                    else
                        regra = 4;
                    break;
                }
            }
            break;
        }

        switch(regra)
        {
            case 1:
            /** n1 antes de n2 **/
            n = copia_no(n1);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2;
            break;

            case 2:
            /** n1 depois de n2 **/
            n = copia_no(n2);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1;
            a2->n2 = n2->at.m.th;
            break;

            case 3:
            /** n1 no mesmo nivel de n2 intermediario **/
            n = copia_no(n1);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2->at.m.th;
            break;

            case 4:
            /** n1 no mesmo nivel de n2 fim **/
            n = produto_complexo_conjugado(n1,n2);
            ac->n = n;
            break;

            case 5:
            /** n1 ou n2 0 **/
            n = cria_no_fim(0,0);
            ac->n = n;
            break;
        }

        if((regra != 4)&&(regra != 5))
        {
            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(compara_apply(aaux,a1))
                    break;
            if(aaux == NULL)
            {
                ac->a1 = a1;
                a1->a = ac->a;
                ac->a = a1;
            }
            else
            {
                ac->a1 = aaux;
                libera_apply_no(a1);
            }


            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(compara_apply(aaux,a2))
                    break;
            if(aaux == NULL)
            {
                ac->a2 = a2;
                a2->a = ac->a;
                ac->a = a2;
            }
            else
            {
                ac->a2 = aaux;
                libera_apply_no(a2);
            }
        }
    }

    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n;
        if(n->tipo == Meio)
        {
            a1 = ac->a1;
            a2 = ac->a2;

            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(aaux == a1)
                    break;
            n1 = aaux->n;

            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(aaux == a2)
                    break;
            n2 = aaux->n;

            conecta_DOIS(n,n1,n2);
        }
    }
    n = a->n;
    libera_apply_lista(a);
    return n;
}

no* apply_produto_matriz_matriz(no *N1, no *N2)
{
    apply *a;
    a = cria_apply();
    a->n1 = N1;
    a->n2 = N2;

    apply *ac, *a1 = NULL, *a2 = NULL, *aaux;
    no *n, *n1, *n2;
    Short regra = 0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        n1 = ac->n1;
        n2 = ac->n2;

        switch(n1->tipo)
        {
            case Meio:
            switch(n2->tipo)
            {
                case Meio:
                if(n1->at.m.nivel < n2->at.m.nivel)
                {
                    switch(n1->at.m.classe)
                    {
                        case R:
                        regra = 1;
                        break;

                        case C:
                        regra = 3;
                        break;
                    }
                }

                if(n1->at.m.nivel > n2->at.m.nivel)
                {
                    switch(n2->at.m.classe)
                    {
                        case R:
                        regra = 4;
                        break;

                        case C:
                        regra = 2;
                        break;
                    }
                }

                if(n1->at.m.nivel == n2->at.m.nivel)
                {
                    switch(n1->at.m.classe)
                    {
                        case R:
                        regra = 1;
                        break;

                        case C:
                        switch(n2->at.m.classe)
                        {
                            case R:
                            regra = 5;
                            break;

                            case C:
                            regra = 3;
                            break;
                        }
                        break;
                    }
                }
                break;

                case Fim:
                if(compara_zero(n2))
                {
                    regra = 7;
                }
                else
                {
                    switch(n1->at.m.classe)
                    {
                        case R:
                        regra = 1;
                        break;

                        case C:
                        regra = 3;
                        break;
                    }
                }
                break;
            }
            break;

            case Fim:
            if(compara_zero(n1))
            {
                regra = 7;
            }
            else
            {
                switch(n2->tipo)
                {
                    case Meio:
                    switch(n2->at.m.classe)
                    {
                        case R:
                        regra = 4;
                        break;

                        case C:
                        regra = 2;
                        break;
                    }
                    break;

                    case Fim:
                    if(compara_zero(n2))
                        regra = 7;
                    else
                        regra = 6;
                    break;
                }
            }
            break;
        }

        switch(regra)
        {
            case 0:
            n = cria_no_inicio();
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.i.n;
            a1->n2 = n2->at.i.n;

            ac->a1 = a1;
            ac->a = a1;
            break;

            case 1:
            n = copia_no(n1);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2;
            break;

            case 2:
            n = copia_no(n2);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1;
            a2->n2 = n2->at.m.th;
            break;

            case 3:
            n = cria_no_meio(0,n1->at.m.nivel);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2;
            break;

            case 4:
            n = cria_no_meio(0,n2->at.m.nivel);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1;
            a2->n2 = n2->at.m.th;
            break;

            case 5:
            n = cria_no_meio(0,n1->at.m.nivel);
            ac->n = n;

            a1 = cria_apply();
            a1->n1 = n1->at.m.el;
            a1->n2 = n2->at.m.el;

            a2 = cria_apply();
            a2->n1 = n1->at.m.th;
            a2->n2 = n2->at.m.th;
            break;

            case 6:
            n = produto_complexo(n1,n2);
            ac->n = n;
            break;

            case 7:
            n = cria_no_fim(0,0);
            ac->n = n;
            break;
        }

        if((regra != 0)&&(regra != 6)&&(regra != 7))
        {
            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(compara_apply(aaux,a1))
                    break;
            if(aaux == NULL)
            {
                ac->a1 = a1;
                a1->a = ac->a;
                ac->a = a1;
            }
            else
            {
                ac->a1 = aaux;
                libera_apply_no(a1);
            }


            for(aaux = a; aaux != NULL; aaux = aaux->a)
                if(compara_apply(aaux,a2))
                    break;
            if(aaux == NULL)
            {
                ac->a2 = a2;
                a2->a = ac->a;
                ac->a = a2;
            }
            else
            {
                ac->a2 = aaux;
                libera_apply_no(a2);
            }
        }
    }

    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n;
        if(n->tipo == Meio)
        {
            a1 = ac->a1;
            a2 = ac->a2;

            n1 = a1->n;
            n2 = a2->n;

            conecta_DOIS(n,n1,n2);
        }
    }
    n = a->n;
    libera_apply_lista(a);
    return n;
}



void reduz_lista(lista *l)
{
    no *n1, *n2;
    lista *lc1, *lc2, *laux;
    for(lc1 = l; lc1 != NULL; lc1 = lc1->l)
    {
        n1 = lc1->n;
        lc2 = lc1;
        while(lc2->l != NULL)
        {
            laux = lc2->l;
            n2 = laux->n;
            if(compara_no_fim(n1,n2))
            {
                transfere_conexao(n1,n2);

                lc2->l = laux->l;
                libera_lista_no(laux);
                libera_no(n2);
            }
            else
            {
                lc2 = lc2->l;
            }
        }
    }
}

lista* acha_lista_fim_arvore(no *n)
{
    lista *l, *lc, *laux;
    l = enlista_arvore(n);
    lc = l;

    while(lc->l != NULL)
    {
        if(lc->l->n->tipo == Fim)
        {
            lc = lc->l;
        }
        else
        {
            laux = lc->l;
            lc->l = laux->l;
            libera_lista_no(laux);
        }
    }

    lc = l->l;
    libera_lista_no(l);
    l = lc;

    return l;
}

lista* acha_lista_fim_QDD(QDD *Q)
{
    lista *l;
    l = acha_lista_fim_arvore(Q->n);
    return l;
}



void completa_QDD_matriz(no *n, Long r, Long c, Long ex, Long **M, lista **L)
{
    no *el, *th;
    Long ind1, ind2;
    if((n->at.m.classe == C)&&(ex == 1))
    {
        ind1 = M[r][c];
        ind2 = M[r][c+1];

        el = L[ind1]->n;
        th = L[ind2]->n;

        conecta_DOIS(n,el,th);
    }
    else
    {
        if(n->at.m.classe == R)
        {
            completa_QDD_matriz(n->at.m.el,r,c,ex,M,L);
            completa_QDD_matriz(n->at.m.th,r+ex,c,ex,M,L);
        }
        if(n->at.m.classe == C)
        {
            completa_QDD_matriz(n->at.m.el,r,c,ex/2,M,L);
            completa_QDD_matriz(n->at.m.th,r,c+ex,ex/2,M,L);
        }
    }
}

QDD* le_matriz(char *nome)
{
    Long i, j, k;

    FILE *fp;
    fp = fopen(nome,"r");
    if(fp == NULL)
        ERRO("LE MATRIZ");

    Long N1, N2, N3;
    fscanf(fp,"%lu %lu\n%lu\n",&N1, &N2, &N3);

    lista **L;
    float re, im;
    L = malloc(N3*sizeof(lista*));
    if(L == NULL)
        ERRO("L");
    aumenta_memoria(N3*sizeof(lista*));
    for(i=0; i<N3; i++)
    {
        L[i] = cria_lista();
        fscanf(fp,"%f %f",&re, &im);
        L[i]->n = cria_no_fim(re,im);
    }
    fscanf(fp,"\n");
    for(i=0; i<N3-1; i++)
        L[i]->l = L[i+1];

    Long **M;
    M = malloc(N2*sizeof(Long*));
    if(M == NULL)
        ERRO("M");
    aumenta_memoria(N2*sizeof(Long*));
    for(i=0; i<N2; i++)
    {
        M[i] = malloc(N2*sizeof(Long));
        if(M[i] == NULL)
            ERRO("M[]");
        aumenta_memoria(N2*sizeof(Long));
        for(j=0; j<N2; j++)
            fscanf(fp,"%lu",&M[i][j]);
    }

    no **N;
    N = malloc((N2*N2-1)*sizeof(no*));
    if(N == NULL)
        ERRO("N");
    aumenta_memoria((N2*N2-1)*sizeof(no*));

    Long ex, ind;
    ex = 1;
    ind = 0;
    for(i=0; i<N1; i++)
    {
        for(j=1; j<=2; j++)
        {
            for(k=0; k<ex; k++)
            {
                N[ind] = cria_no_meio(j,i);
                ind++;
            }
            ex *= 2;
        }
    }

    for(i=0; i<(N2*N2-1)/2; i++)
        conecta_DOIS(N[i],N[2*i+1],N[2*i+2]);

    completa_QDD_matriz(N[0],0,0,N2/2,M,L);

    QDD *Q;
    Q = cria_QDD();
    Q->n = cria_no_inicio();
    conecta_UM(Q->n,N[0],0);
    Q->n = N[0];
    Q->nqbit = N1;
    Q->l = L[0];

    free(N);
    diminui_memoria((N2*N2-1)*sizeof(no*));
    free(L);
    diminui_memoria(N3*sizeof(lista*));
    for(i=0; i<N2; i++)
    {
        free(M[i]);
        diminui_memoria(N2*sizeof(Long));
    }
    free(M);
    diminui_memoria(N2*sizeof(Long*));
    fclose(fp);

    return Q;
}

void completa_QDD_vetor(no *n, Long v, Long ex, Long *M, lista **L)
{
    no *el, *th;
    Long ind1, ind2;
    if(ex == 1)
    {

        ind1 = M[v];
        ind2 = M[v+1];

        el = L[ind1]->n;
        th = L[ind2]->n;

        conecta_DOIS(n,el,th);
    }
    else
    {
        completa_QDD_vetor(n->at.m.el,v,ex/2,M,L);
        completa_QDD_vetor(n->at.m.th,v+ex,ex/2,M,L);
    }
}

QDD* le_vetor(char *nome)
{
    Long i, j;

    FILE *fp;
    fp = fopen(nome,"r");
    if(fp == NULL)
        ERRO("LE VETOR");

    Long N1, N2, N3;
    fscanf(fp,"%lu %lu\n%lu\n",&N1,&N2,&N3);
    lista **L;
    float re, im;
    L = malloc(N3*sizeof(lista*));
    if(L == NULL)
        ERRO("L");
    aumenta_memoria(N3*sizeof(lista*));
    for(i=0; i<N3; i++)
    {
        L[i] = cria_lista();
        fscanf(fp,"%f %f",&re, &im);
        L[i]->n = cria_no_fim(re,im);
    }

    fscanf(fp,"\n");
    for(i=0; i<N3-1; i++)
        L[i]->l = L[i+1];

    Long *M;
    M = malloc(N2*sizeof(Long));
    if(M == NULL)
        ERRO("M");
    aumenta_memoria(N2*sizeof(Long));
    for(i=0; i<N2; i++)
        fscanf(fp,"%lu",&M[i]);

    no **N;
    N = malloc((N2-1)*sizeof(no*));
    if(N == NULL)
        ERRO("N");
    aumenta_memoria((N2-1)*sizeof(no*));

    Long ex, ind;
    no *n;
    ex = 1;
    ind = 0;
    for(i=0; i<N1; i++)
    {
        for(j=0; j<ex; j++)
        {
            n = cria_no_meio(0,i);
            N[ind] = n;
            ind++;
        }
        ex *= 2;
    }

    for(i=0; i<(N2/2)-1; i++)
        conecta_DOIS(N[i],N[2*i+1],N[2*i+2]);

    completa_QDD_vetor(N[0],0,N2/2,M,L);

    n = cria_no_inicio();
    conecta_UM(n,N[0],0);

    QDD *Q;
    Q = cria_QDD();
    Q->n = N[0];
    Q->nqbit = N1;
    Q->l = L[0];

    free(N);
    diminui_memoria((N2-1)*sizeof(no*));
    free(L);
    diminui_memoria(N3*sizeof(lista*));
    free(M);
    diminui_memoria(N2*sizeof(Long));
    fclose(fp);

    return Q;
}



void reduz_QDD(QDD *Q)
{
    no *n1, *n2, *nc;
    lista *l, *laux, *lnc1, *lnc2, *lr, *lrc, *lf;
    Short mudou, saida, regra;
    reduz_lista(Q->l);
    l = copia_lista_com_cabeca(Q->l);
    while(l->l != NULL)
    {
        nc = l->l->n;
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
                                    saida = 1;
                                }
                                break;

                                case 2:
                                if(compara_no_meio(n1,n2))
                                {
                                    lr = cria_lista();
                                    lr->n = n1;
                                    lrc = cria_lista();
                                    lrc->n = n2;
                                    lr->l = lrc;

                                    while(lnc2->l != NULL)
                                    {
                                        lnc2 = lnc2->l;
                                        n2 = lnc2->n;

                                        if(compara_no_meio(n1,n2))
                                        {
                                            if(n1->at.m.el == n1->at.m.th)
                                            {

                                                for(lrc = lr; lrc->l != NULL; lrc = lrc->l)
                                                    if(lrc->l->n == n2)
                                                        break;
                                                if(lrc->l != NULL)
                                                    continue;
                                            }
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
                                        libera_no(n2);

                                        lr->l = lrc->l;
                                        libera_lista_no(lrc);
                                    }
                                    libera_lista_no(lrc);

                                    if(n1->at.m.el != n1->at.m.th)
                                    {
                                        for(lf = l; lf->l != NULL; lf = lf->l);
                                        lf->l = cria_lista();
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

        laux = l->l;
        libera_lista_no(l);
        l = laux;
    }
    libera_lista_no(l);
}

QDD* copia_QDD(QDD *Q1)
{
    /**  l1 guarda Q1 elistado   ***
    ***  l2 lista dos nos de l2  **/
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
        /**  lc1a perocrre a lista 1 pela primeira vez  ***
        ***  lc2a percorre a lista 2 pela primeira vez  **/
        n1 = lc1a->n;
        n2 = lc2a->n;

        switch(n1->tipo)
        {
            case Inicio:
            nf = n1->at.i.n;

            lc1b = l1;
            lc2b = l2;
            do
            {
                /**  lc1a perocrre a lista 1 pela segunda vez para buscar filhos  ***
                ***  lc2a percorre a lista 2 pela segunda vez para buscar filhos  **/
                nt1 = lc1b->n;
                nt2 = lc2b->n;

                lc1b = lc1b->l;
                lc2b = lc2b->l;
            }
            while(nt1 != nf);

            conecta_UM(n2,nt2,0);
            break;

            case Meio:
            nf = n1->at.m.el;

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

            nf = n1->at.m.th;

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

            conecta_UM(n2,nt2,2);
            break;
        }
        lc1a = lc1a->l;
        lc2a = lc2a->l;
    }
    while(lc1a != NULL);

    QDD *Q2;
    Q2 = cria_QDD();
    Q2->nqbit = Q1->nqbit;
    Q2->n = l2->l->n;

    lc2a = l2;
    while(lc2a->l != NULL)
    {
        lc2b = lc2a->l;
        if(lc2b->n->tipo == Fim)
            lc2a = lc2b;
        else
        {
            lc2a->l = lc2b->l;
            libera_lista_no(lc2b);
        }
    }

    Q2->l = l2->l;
    libera_lista_no(l2);
    libera_lista_lista(l1);

    return Q2;
}

QDD* produto_tensorial(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    lista *l;

    Q = copia_QDD(Q1);
    Q->nqbit = Q1->nqbit + Q2->nqbit;
    l = Q->l;
    Q->l = NULL;

    QDD *Q2a;
    lista *l2, *lc;
    Q2a = copia_QDD(Q2);
    l2 = enlista_QDD(Q2a);
    for(lc = l2; lc != NULL; lc = lc->l)
        if(lc->n->tipo == Meio)
            (lc->n->at.m.nivel) += (Q1->nqbit);
    libera_lista_lista(l2);

    QDD *Q2b;
    no *n1, *n2, *naux;


    while(l != NULL)
    {
        n1 = l->n;
        if(compara_zero(n1))
        {
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
            libera_lista_no(l);
            l = lc;
        }
    }

    libera_QDD(Q2a);

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
        libera_QDD(Qs);
        Qs = Qaux;
    }
    return Qs;
}

void produto_por_escalar(QDD *Q, float re, float im)
{
    no *n;
    n = cria_no_fim(re,im);

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

QDD* soma_QDD(QDD *Q1, QDD *Q2)
{
    QDD *Q;
    Q = cria_QDD();
    Q->n = apply_soma(Q1->n,Q2->n);

    no *n;
    n = cria_no_inicio();
    conecta_UM(n,Q->n,0);

    Q->nqbit = Q1->nqbit;
    Q->l = acha_lista_fim_QDD(Q);

    reduz_QDD(Q);

    return Q;
}

no* produto_interno(QDD *Q1, QDD *Q2)
{
    no *n0, *n1;
    n0 = cria_no_inicio();
    n1 = apply_produto_interno(Q1->n,Q2->n);
    conecta_UM(n0,n1,0);

    QDD *Q;
    Q = cria_QDD();
    Q->n = n1;
    Q->nqbit = Q1->nqbit;
    Q->l = acha_lista_fim_QDD(Q);

    no *n;
    n = copia_no(Q->n);
    libera_QDD(Q);
    return n;
}


QDD* I_1()
{
    QDD *Q;
    Q = le_matriz("I1.txt");
    reduz_QDD(Q);
    return Q;
}

QDD* I(Short i)
{
    QDD *Q1, *Q2;

    Q1 = I_1();
    Q2 = potencia_tensorial(Q1,i);
    libera_QDD(Q1);

    return Q2;
}

QDD* H_1()
{
    QDD *Q;
    Q = le_matriz("H1.txt");
    reduz_QDD(Q);
    return Q;
}

QDD* H(Short i)
{
    QDD *Q1, *Q2;

    Q1 = H_1();
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

void configuracao(Short i)
{
    nqbit = i;
    eps = pow(2,-(i/2.0))/20;
}



int main()
{
    inicia_relatorio_memoria(0);
    configuracao(20);
    /***********************************/

    /*clock_t begin, end, delta;
    int tempo;

    char c[30][10]={{"V1.txt"},{"V2.txt"},{"V3.txt"},{"V4.txt"},{"V5.txt"},{"V6.txt"},{"V7.txt"},{"V8.txt"},{"V9.txt"},{"V10.txt"},{"V11.txt"},{"V12.txt"},{"V13.txt"},{"V14.txt"},{"V15.txt"},{"V16.txt"},{"V17.txt"},{"V18.txt"},{"V19.txt"},{"V20.txt"},{"V21.txt"},{"V22.txt"},{"V23.txt"}};
    FILE *ft;
    ft = fopen("Relatorio.csv","w");
    fprintf(ft,"sep=-\n");
    QDD *Q;
    for(int i=0; i<23; i++)
    {
    int i=22;
        printf("\n\n%s",c[i]);
        Q = le_vetor(c[i]);
        configuracao(Q->nqbit);
        mostra_quantidades();
        fprintf(ft,"%d-%d-%d-%d-",mem,im,ifi,il);
        reduz_QDD(Q);
        mostra_quantidades();
        fprintf(ft,"%d-%d-%d-%d",mem,im,ifi,il);
        libera_QDD(Q);
        for(int j=0; j<10; j++)
        {
            Q = le_vetor(c[i]);
            configuracao(Q->nqbit);
            begin = clock();
            reduz_QDD(Q);
            end = clock();
            delta = (end-begin)%(CLOCKS_PER_SEC);
            tempo = (end-begin)/(CLOCKS_PER_SEC);
            printf("\nAmostra %d: %d,",j+1,tempo);
            if(delta < 100)
                printf("0");
            if(delta < 10)
                printf("0");
            printf("%d",delta);
            fprintf(ft,"-%d,",tempo);
            if(delta < 100)
                fprintf(ft,"0");
            if(delta < 10)
                fprintf(ft,"0");
            fprintf(ft,"%d",delta);
            libera_QDD(Q);
        }
        fprintf(ft,"\n");
        mostra_quantidades();
    }*/

    QDD *Q1;
    Q1 = H(1);

    QDD *Q2;
    Q2 = H(1);

    no *n;
    n = apply_produto_matriz_matriz(Q1->n,Q2->n);
    mostra_arvore(n);

    libera_QDD(Q1);
    libera_QDD(Q2);
    libera_arvore(n);


    /***********************************/
    finaliza_relatorio_memoria();
    return 0;
}
