#include<stdio.h>
#include<stdlib.h>
#include<math.h>



FILE *fm;
unsigned long mem = 0, memMax = 0, iQ = 0, ii = 0, im = 0, ifi = 0, il = 0, ia = 0;
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



typedef struct QDD QDD;

typedef struct no no;

typedef struct lista lista;

typedef struct inicio inicio;

typedef struct meio meio;

typedef struct fim fim;

typedef struct apply apply;

typedef unsigned short Short;

typedef unsigned long Long;



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
    if(m>mem)
    {
        printf("\n\nERRO MEM");
        exit(EXIT_FAILURE);
    }
    mem -= m;
    if(print)
    fprintf(fm,"\n\tMemDOWN: %d\t\t%u",mem,-m);
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
    {
        printf("\n\nERRO INICIO");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(sizeof(no));
    ii++;

    n->tipo = 0;
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
    {
        printf("\n\nERRO MEIO");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(sizeof(no));
    im++;

    n->tipo = 1;
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
    {
        printf("\n\nERRO FIM");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(sizeof(no));
    ifi++;

    n->tipo = 2;
    n->l = NULL;

    fim f;
    f.re = re;
    f.im = im;
    n->at.f = f;

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
    {
        printf("\n\nERRO APPLY");
        exit(EXIT_FAILURE);
    }
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

void libera_no_apply(apply *a)
{
    diminui_memoria(sizeof(apply));
    free(a);
}

void libera_lista_apply(apply *a)
{
    apply *ac;
    while(a != NULL)
    {
        ac = a->a;
        libera_no_apply(a);
        a = ac;
    }
}



lista* enlista_QDD(QDD *Q)
{
    /**  l é a lista final                      ***
    ***  la é a pilha de nós para adicionar a l  **/
    lista *l, *la, *lc, *lf;
    no *n;

    l = cria_no_lista();
    la = cria_no_lista();
    la->n = Q->n;
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
            lf->l = cria_no_lista();
            lf = lf->l;
            lf->n = n;

            if(n->tipo == 1)
            {
                /**  caso tenha filhos  **/
                la->n = n->at.m.th;
                lc = cria_no_lista();
                lc->n = n->at.m.el;
                lc->l = la;
                la = lc;
            }
        }
        else
        {
            /**  caso já esteja  */
            lc = la->l;
            libera_no_lista(la);
            la = lc;
        }
    }
    l->n = Q->n->l->n;
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
    if(n->l != NULL)
    {
        printf("Ligacoes anteriores:\n");
        mostra_lista(n->l);
    }
    printf("Tipo");
    switch(n->tipo)
    {
        case 0:
        printf(": Inicio\n");
        printf("Ligacoes posteriores\n");
        printf("\tn: %d\n",n->at.i.n);
        break;

        case 1:
        printf("/nivel: ");
        switch(n->at.m.classe)
        {
            case 0:
            printf("V");
            break;

            case 1:
            printf("R");
            break;

            case 2:
            printf("C");
            break;
        }

        printf("%d\n",n->at.m.nivel);
        printf("Ligacoes posteriores\n");
        printf("\telse: %d\n",n->at.m.el);
        printf("\tThen: %d\n",n->at.m.th);
        break;

        case 2:
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
        if(lc->n != NULL)
            mostra_no(lc->n);
        ligacao++;
    }
}

void mostra_arvore_ineficiente(no *n)
{
    if(n == NULL)
        return;
    mostra_no(n);
    if(n->tipo == 1)
    {
        mostra_arvore_ineficiente(n->at.m.el);
        mostra_arvore_ineficiente(n->at.m.th);
    }
}

void mostra_QDD(QDD *Q)
{
    lista *l;
    l = enlista_QDD(Q);
    printf("\nNQBIT: %d\n",Q->nqbit);
    mostra_lista_com_no(l);
    printf("\n");
    mostra_lista(Q->l);
    libera_lista(l);
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
}

void mostra_lista_apply(apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
        printf("\nLigacao %d: %d",i++,ac);
}

void mostra_lista_apply_com_no(apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        printf("\n\n\n\n\nLigacao %d: %d\n",i++,ac);
        mostra_apply(ac);
    }
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
    if(n->l != NULL)
    {
        fprintf(fp,"Ligacoes anteriores:\n");
        fmostra_lista(fp,n->l);
    }
    fprintf(fp,"Tipo");
    switch(n->tipo)
    {
        case 0:
        fprintf(fp,": Inicio\n");
        fprintf(fp,"Ligacoes posteriores\n");
        fprintf(fp,"\tn: %d\n",n->at.i.n);
        break;

        case 1:
        fprintf(fp,"/nivel: ");
        switch(n->at.m.classe)
        {
            case 0:
            fprintf(fp,"V");
            break;

            case 1:
            fprintf(fp,"R");
            break;

            case 2:
            fprintf(fp,"C");
            break;
        }

        fprintf(fp,"%d\n",n->at.m.nivel);
        fprintf(fp,"Ligacoes posteriores\n");
        fprintf(fp,"\telse: %d\n",n->at.m.el);
        fprintf(fp,"\tThen: %d\n",n->at.m.th);
        break;

        case 2:
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
        if(lc->n != NULL)
            fmostra_no(fp,lc->n);
        ligacao++;
    }
}

void fmostra_arvore_ineficiente(FILE *fp, no *n)
{
    if(n == NULL)
        return;
    fmostra_no(fp,n);
    if(n->tipo == 1)
    {
        fmostra_arvore_ineficiente(fp,n->at.m.el);
        fmostra_arvore_ineficiente(fp,n->at.m.th);
    }
}

void fmostra_QDD(FILE *fp, QDD *Q)
{
    lista *l;
    l = enlista_QDD(Q);
    fprintf(fp,"\nNQBIT: %d\n",Q->nqbit);
    fmostra_lista_com_no(fp,l);
    fprintf(fp,"\n");
    fmostra_lista(fp,Q->l);
    libera_lista(l);
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
}

void fmostra_lista_apply(FILE *fp, apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
        fprintf(fp,"\nLigacao %d: %d",i++,ac);
}

void fmostra_lista_apply_com_no(FILE *fp, apply *a)
{
    apply *ac;
    Long i=0;
    for(ac = a; ac != NULL; ac = ac->a)
    {
        fprintf(fp,"\n\n\n\n\nLigacao %d: %d\n",i++,ac);
        fmostra_apply(fp,ac);
    }
}



void conecta_UM(no *n1, no *n2, Short lado)
{
    lista *l;

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

    l = cria_no_lista();
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
    if(n1->tipo == 0)
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
    switch(n->tipo)
    {
        case 0:
        desconecta_UM(n,n->at.i.n);
        break;

        case 1:
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
    n2 = NULL;
    switch(n1->tipo)
    {
        case 0:
        n2 = cria_no_inicio();
        break;

        case 1:
        n2 = cria_no_meio(n1->at.m.classe,n1->at.m.nivel);
        break;

        case 2:
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
            case 1:
            /**  n1 é intermediario  **/
            switch(n2->tipo)
            {
                case 1:
                /**  n2 é intermediario  **/
                if(n1->at.m.nivel < n2->at.m.nivel)
                    regra = 1;
                if(n1->at.m.nivel > n2->at.m.nivel)
                    regra = 2;
                if(n1->at.m.nivel == N2->at.m.nivel)
                {
                    if(n1->at.m.classe < n2->at.m.classe)
                        regra = 1;
                    if(n1->at.m.classe > n2->at.m.classe)
                        regra = 2;
                    if(n1->at.m.classe == n2->at.m.classe)
                        regra = 3;
                }
                break;

                case 2:
                /**  n2 é número  **/
                regra = 1;
                break;
            }
            break;

            case 2:
            /**  n1 é numero  **/
            switch(n2->tipo)
            {
                case 1:
                /**  n2 é intermediario  **/
                regra = 2;
                break;

                case 2:
                /**  n2 é número  **/
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
                libera_no_apply(a1);
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
                libera_no_apply(a2);
            }
        }
    }

    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n;
        if(n->tipo == 1)
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
    libera_lista_apply(a);
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
            case 1:
            /**  n1 é intermediario  **/
            switch(n2->tipo)
            {
                case 1:
                /**  n2 é intermediario  **/
                if(n1->at.m.nivel < n2->at.m.nivel)
                    regra = 1;
                if(n1->at.m.nivel > n2->at.m.nivel)
                    regra = 2;
                if(n1->at.m.nivel == N2->at.m.nivel)
                {
                    if(n1->at.m.classe < n2->at.m.classe)
                        regra = 1;
                    if(n1->at.m.classe > n2->at.m.classe)
                        regra = 2;
                    if(n1->at.m.classe == n2->at.m.classe)
                        regra = 3;
                }
                break;

                case 2:
                /**  n2 é número  **/
                if(compara_zero(n2))
                    regra = 5;
                else
                    regra = 1;
                break;
            }
            break;

            case 2:
            /**  n1 é numero  **/
            if(compara_zero(n1))
            {
                regra = 5;
            }
            else
            {
                switch(n2->tipo)
                {
                    case 1:
                    /**  n2 é intermediario  **/
                    regra = 2;
                    break;

                    case 2:
                    /**  n2 é número  **/
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
            n = produto_complexo_conjugado(n1,n2);
            ac->n = n;
            break;

            case 5:
            n = cria_no_fim(0,0);
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
                libera_no_apply(a1);
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
                libera_no_apply(a2);
            }
        }
    }

    for(ac = a; ac != NULL; ac = ac->a)
    {
        n = ac->n;
        if(n->tipo == 1)
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
    libera_lista_apply(a);
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
                libera_no_lista(laux);
                libera_no(n2);
            }
            else
            {
                lc2 = lc2->l;
            }
        }
    }
}

lista* acha_lista_fim(QDD *Q)
{
    lista *l, *lc, *laux;
    l = enlista_QDD(Q);
    lc = l;

    while(lc->l != NULL)
    {
        if(lc->l->n->tipo == 2)
        {
            lc = lc->l;
        }
        else
        {
            laux = lc->l;
            lc->l = laux->l;
            libera_no_lista(laux);
        }
    }

    lc = l->l;
    libera_no_lista(l);
    l = lc;

    return l;
}

lista* acha_lista_classe(QDD *Q, Short classe)
{
    lista *l;
    l = enlista_QDD(Q);
    l->n = NULL;

    no *n;
    lista *lc, *laux;
    Short caso = 0;
    lc = l;
    while(lc->l != NULL)
    {
        n = lc->l->n;
        switch(n->tipo)
        {
            case 1:
            if(n->at.m.classe == classe)
                caso = 0;
            else
                caso = 1;
            break;

            case 2:
            caso = 1;
            break;
        }

        switch(caso)
        {
            case 0:
            lc = lc->l;
            break;

            case 1:
            laux = lc->l;
            lc->l = laux->l;
            libera_no_lista(laux);
            break;
        }
    }

    lc = l->l;
    libera_no_lista(l);
    l = lc;

    return l;
}

lista* mergesort_nivel_recursivo(lista *l, Long N)
{
    lista *l1, *l2, *laux, *lc;
    Short N1, N2, i;
    if(N>2)
    {
        N1 = N/2;
        N2 = N-N1;

        lc = l;
        for(i=1; i<N1; i++)
            lc = lc->l;
        l1 = l;
        l2 = lc->l;
        lc->l = NULL;

        printf("\nN: %d\nLISTA 1",N);
        mostra_lista_com_no(l1);
        printf("\nLISTA2");
        mostra_lista_com_no(l2);

        l1 = mergesort_nivel_recursivo(l1,N1);
        l2 = mergesort_nivel_recursivo(l2,N2);

        printf("\nFORA");
        printf("\nN: %d\nLISTA 1 ORDENADA",N);
        mostra_lista_com_no(l1);
        printf("\nLISTA2 ORDENADA");
        mostra_lista_com_no(l2);

        l = cria_no_lista();
        lc = l;
        while((l1 != NULL)&&(l2 != NULL))
        {
            N1 = l1->n->at.m.nivel;
            N2 = l2->n->at.m.nivel;
            if(N1>N2)
            {
                laux = l1;
                l1 = laux->l;
            }
            else
            {
                laux = l2;
                l2 = laux->l;
            }
            lc->l = laux;
            laux->l = NULL;
            lc = lc->l;


            printf("\nDENTRO");
            printf("\nN: %d\nLISTA 1",N);
            mostra_lista_com_no(l1);
            printf("\nLISTA2");
            mostra_lista_com_no(l2);
            printf("\nLISTA PROVISORIA");
            mostra_lista_com_no(l);
        }
        printf("\nSAIU");
        printf("\nN: %d\nLISTA 1",N);
        mostra_lista_com_no(l1);
        printf("\nLISTA2");
        mostra_lista_com_no(l2);
        printf("\nLISTA PROVISORIA");
        mostra_lista_com_no(l);
        printf("\nL1: %d\tL2: %d",l1,l2);

        if(l1 != NULL)
            lc->l = l1;
        if(l2 != NULL)
            lc->l = l2;

        lc = l->l;
        libera_no_lista(l);
        l = lc;

        printf("\nN: %d\nLISTA ORDENADA",N);
        mostra_lista_com_no(l);
    }
    if(N == 2)
    {
        N1 = l->n->at.m.nivel;
        lc = l->l;
        N2 = lc->n->at.m.nivel;
        if(N2>N1)
        {
            lc->l = l;
            l->l = NULL;
            l = lc;
            printf("\nALTEROU A ORDEM");
            mostra_lista_com_no(l);
        }
    }
    printf("\nL SAINDO\nN: %d",N);
    mostra_lista_com_no(l);
    return l;
}

void mergesort_nivel(lista *l)
{
    lista *lc;
    Short N = 0;
    for(lc = l; lc != NULL; lc = lc->l)
        N++;
    l = mergesort_nivel_recursivo(l,N);
}



void completa_QDD_matriz(no *n, Long r, Long c, Long ex, Long **M, lista **L)
{
    no *el, *th;
    Long ind1, ind2;
    if((n->at.m.classe == 2)&&(ex == 1))
    {
        ind1 = M[r][c];
        ind2 = M[r][c+1];

        el = L[ind1]->n;
        th = L[ind2]->n;

        conecta_DOIS(n,el,th);
    }
    else
    {
        if(n->at.m.classe == 1)
        {
            completa_QDD_matriz(n->at.m.el,r,c,ex,M,L);
            completa_QDD_matriz(n->at.m.th,r+ex,c,ex,M,L);
        }
        if(n->at.m.classe == 2)
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

    Long N1, N2, N3;
    fscanf(fp,"%lu %lu\n%lu\n",&N1, &N2, &N3);

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
        L[i]->n = cria_no_fim(re,im);
    }
    fscanf(fp,"\n");
    for(i=0; i<N3-1; i++)
        L[i]->l = L[i+1];

    Long **M;
    M = malloc(N2*sizeof(Long*));
    if(M == NULL)
    {
        printf("\n\nERRO M");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(N2*sizeof(Long*));
    for(i=0; i<N2; i++)
    {
        M[i] = malloc(N2*sizeof(Long));
        if(M[i] == NULL)
        {
            printf("\n\nERRO M[%d]",i);
            exit(EXIT_FAILURE);
        }
        aumenta_memoria(N2*sizeof(Long));
        for(j=0; j<N2; j++)
            fscanf(fp,"%lu",&M[i][j]);
    }

    no **N;
    N = malloc((N2*N2-1)*sizeof(no*));
    if(N == NULL)
    {
        printf("\n\nERRO N");
        exit(EXIT_FAILURE);
    }
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
    free(fp);

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

    Long N1, N2, N3;
    fscanf(fp,"%lu %lu\n%lu\n",&N1,&N2,&N3);
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
        L[i]->n = cria_no_fim(re,im);
    }

    fscanf(fp,"\n");
    for(i=0; i<N3-1; i++)
        L[i]->l = L[i+1];

    Long *M;
    M = malloc(N2*sizeof(Long));
    if(M == NULL)
    {
        printf("\n\nERRO M  ");
        exit(EXIT_FAILURE);
    }
    aumenta_memoria(N2*sizeof(Long));
    for(i=0; i<N2; i++)
        fscanf(fp,"%lu",&M[i]);

    no **N;
    N = malloc((N2-1)*sizeof(no*));
    if(N == NULL)
    {
        printf("\n\nERRO N");
        exit(EXIT_FAILURE);
    }
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
    free(fp);

    return Q;
}



void reduz_QDD(QDD *Q)
{
    no *n1, *n2, *nc;
    lista *l, *laux, *lnc1, *lnc2, *lr, *lrc, *lf;
    Short mudou, saida, regra;
    reduz_lista(Q->l);
    l = copia_lista(Q->l);
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
                                    lr = cria_no_lista();
                                    lr->n = n1;
                                    lrc = cria_no_lista();
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

                                    if(n1->at.m.el != n1->at.m.th)
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

        laux = l->l;
        libera_no_lista(l);
        l = laux;
    }
    libera_no_lista(l);
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
            case 0:
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

            case 1:
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
        if(lc2b->n->tipo == 2)
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
    lista *l;

    Q = copia_QDD(Q1);
    reduz_QDD(Q);
    Q->nqbit = Q1->nqbit + Q2->nqbit;
    l = Q->l;
    Q->l = NULL;

    QDD *Q2a;
    lista *l2, *lc;
    Q2a = copia_QDD(Q2);
    reduz_QDD(Q2a);
    l2 = enlista_QDD(Q2a);
    for(lc = l2; lc != NULL; lc = lc->l)
        if(lc->n->tipo == 1)
            (lc->n->at.m.nivel) += (Q1->nqbit);
    libera_lista(l2);

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
            libera_no_lista(l);
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
    Q->l = acha_lista_fim(Q);

    reduz_QDD(Q);

    return Q;
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
    eps = pow(2,-(i/2))/20;
}



int main()
{
    inicia_relatorio_memoria(0);
    configuracao(20);
    /***********************************/

    QDD *Q;
    Q = le_matriz("H4.txt");
    reduz_QDD(Q);

    lista *l;
    l = acha_lista_classe(Q,2);
    mergesort_nivel(l);
    printf("\n\n\nLISTA");
    mostra_lista_com_no(l);

    /*lista **L1, **L2;
    int i;
    L1 = malloc(4*sizeof(lista*));
    L2 = malloc(4*sizeof(lista*));
    for(i=0; i<4; i++)
    {
        L1[i] = cria_no_lista();
        L2[i] = cria_no_lista();

        L1[i]->n = cria_no_meio(2,6-(2*i));
        L2[i]->n = cria_no_meio(2,7-(2*i));
    }
    for(i=0; i<3; i++)
    {
        L1[i]->l = L1[i+1];
        L2[i]->l = L2[i+1];
    }

    lista *l1, *l2;
    l1 = cria_no_lista();
    l1->l = L1[0];
    l2 = cria_no_lista();
    l2->l = L2[0];

    printf("\nLISTA 1");
    mostra_lista_com_no(l1);
    printf("\nLISTA 2");
    mostra_lista_com_no(l2);

    Short N1, N2;

    lista *l, *lc, *laux;
    l = cria_no_lista();
    lc = l;
    while((l1->l != NULL) && (l2->l != NULL))
    {
        N1 = l1->l->n->at.m.nivel;
        N2 = l2->l->n->at.m.nivel;
        if(N1>N2)
        {
            laux = l1->l;
            l1->l = laux->l;
        }
        else
        {
            laux = l2->l;
            l2->l = laux->l;
        }
        lc->l = laux;
        laux->l = NULL;

        lc = lc->l;
    }
    if(l1->l != NULL)
        lc->l = l1->l;
    if(l2->l != NULL)
        lc->l = l2->l;

    printf("\nLISTA");
    mostra_lista_com_no(l);*/

    /***********************************/
    finaliza_relatorio_memoria();
    return 0;
}
