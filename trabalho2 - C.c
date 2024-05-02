#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

typedef struct Lista1 {
    int idLivro;
    char titulo[50], assunto[100], autor[50];
    struct Lista1 *prox;
} lDispo;

typedef struct Lista2 {
    int idCliente, idLivro;
    char titulo[50], assunto[100], autor[50];
    struct Lista2 *prox;
} lEmp;

typedef struct Lista3 {
    int idCliente;
    char nome[50];
    struct Lista1 *emprestados;
    struct Lista3 *prox;
} lClient;

lDispo* iniciadispo() {
    return NULL;
}

lEmp* iniciaemp() {
    return NULL;
}

lClient* iniciaclient() {
    return NULL;
}

lDispo* insereLivro(lDispo* disp) {
    lDispo *novo;
    novo = (lDispo*)malloc(sizeof(lDispo));
    printf("Escolha um código de registro para o livro: ");
    scanf("%d", &novo->idLivro);
    printf("Digite o título do livro: ");
    fflush(stdin);
    gets(novo->titulo);
    printf("Digite o assunto do livro: ");
    fflush(stdin);
    gets(novo->assunto);
    printf("Digite o autor do livro: ");
    fflush(stdin);
    gets(novo->autor);
    novo->prox = disp;
    return novo;
}

lClient* insereCliente(lClient* client) {
    lClient *novo;
    novo = (lClient*)malloc(sizeof(lClient));
    printf("Escolha um código para o cliente: ");
    scanf("%d", &novo->idCliente);
    printf("Digite o nome do cliente: ");
    fflush(stdin);
    gets(novo->nome);
    novo->emprestados=NULL;
    novo->prox = client;
    return novo;
}

lDispo* removerLivro(lDispo* inicio,lDispo** noRetornado, int livroBuscado){
    lDispo *ant, *aux=inicio;
    ant= iniciadispo();
    while((aux!=NULL) && (aux->idLivro!=livroBuscado)){ //Procurando o livro na lista dispo//
        ant=aux;
        aux=aux->prox;
    }

    if(ant==NULL){
        inicio=inicio->prox;
    }
    else{
        ant->prox=aux->prox;
    }
    aux->prox=NULL;
    (*noRetornado)=aux;

    return inicio;
}

lEmp* insereEmp(lEmp *emprestados, int idLivro, int idCliente, char titulo[], char assunto[], char autor[]){
    lEmp *novo;
    novo = (lEmp*)malloc(sizeof(lEmp));
    novo->idCliente=idCliente;
    strcpy(novo->titulo,titulo);
    strcpy(novo->assunto,assunto);
    strcpy(novo->autor,autor);
    novo->idLivro=idLivro;
    novo->prox=emprestados;
    return novo;
}

void imprimeDispo(lDispo *aux){
    if(aux==NULL){printf("Nenhum livro disponível\n");}
    else{
        printf("Livros Disponiveis:\n\n");
        while(aux!=NULL){
            printf("-----------------------------------------------------\n");
            printf("Código: %d\n",aux->idLivro);
            printf("Título do livro: %s\n",aux->titulo);
            printf("Assunto: %s\n",aux->assunto);
            printf("Autor: %s\n",aux->autor);
            printf("-----------------------------------------------------\n\n");
            aux=aux->prox;
        }
    }
}

void emprestarLivro(lDispo** disp,lClient** client,lEmp** emprestados){
    int livroBuscado,clienteBuscado;
    lDispo *noRetornado=NULL, *auxD=*disp;
    lClient *auxC=*client;
    char titulo[50],assunto[100],autor[50];

    printf("Digite o código do livro a ser emprestado: ");
    scanf("%d", &livroBuscado);

    while((auxD!=NULL)&&(auxD->idLivro!=livroBuscado)){ //Procurando livro na lista de disponiveis//
        auxD=auxD->prox;
    }

    if(auxD==NULL){printf("Livro não encontrado");}
    else{
        printf("Digite o código do cliente: ");
        scanf("%d", &clienteBuscado);

        while((auxC!=NULL) && (auxC->idCliente!=clienteBuscado)){ //Procurando cliente na lClient//
            auxC=auxC->prox;
        }
        if(auxC==NULL){
            printf("Cliente não encontrado");
        }
        else{
            (*disp)=removerLivro((*disp),&noRetornado,livroBuscado); //Remover livro da lDispo//

            strcpy(titulo,noRetornado->titulo);     //Copiar informções do livro//
            strcpy(assunto,noRetornado->assunto);
            strcpy(autor,noRetornado->autor);

            (*emprestados)=insereEmp((*emprestados),livroBuscado,clienteBuscado,titulo,assunto,autor); //Inserir na lEmp//

            if(auxC->emprestados!=NULL){ //Inserir livro na lista do cliente//
            noRetornado->prox=auxC->emprestados;
            }
            auxC->emprestados=noRetornado;
        }
    }

}

lClient* removerLclient(lClient *client,lDispo **noRetornado,int clienteBuscado,int livroBuscado){
    lDispo *ant=NULL,*aux;
    lClient *auxC=client;
    while(auxC!=NULL){
        if(auxC->idCliente==clienteBuscado){
            aux=auxC->emprestados;
            while((aux!=NULL) && (aux->idLivro!=livroBuscado)){
                ant=aux;
                aux=aux->prox;
            }
            if(ant==NULL){
                auxC->emprestados=auxC->emprestados->prox;
            }
            else{
                ant->prox=aux->prox;
            }
            aux->prox=NULL;
            (*noRetornado)=aux;
        }
        auxC=auxC->prox;
    }
    return client;
}

lEmp* removerEmp(lEmp *inicio,int livroDevolvido){
    lEmp *ant=NULL, *aux=inicio;
    while((aux!=NULL) && (aux->idLivro!=livroDevolvido)){
        ant=aux;
        aux=aux->prox;
    }
    if(aux!=NULL){
        if(ant==NULL){
            inicio=inicio->prox;
        }
        else{
            ant->prox=aux->prox;
        }
        free(aux);
    }
    return inicio;
}

void devolverLivro(lDispo **disp,lClient **client,lEmp **emprestados){
    int livroDevolvido,clienteBuscado;
    lClient *auxC=*client;
    lDispo *noBuscado=NULL;
    lEmp *auxE=*emprestados;
    printf("Digite o código do livro que deseja devolver: ");
    scanf("%d", &livroDevolvido);

    while((auxE!=NULL)&&(auxE->idLivro!=livroDevolvido)){
        auxE=auxE->prox;
    }
    if(auxE==NULL){printf("Livro não encontrado");}
    else{
        printf("Digite o código do cliente que o possui: ");
        scanf("%d", &clienteBuscado);

        while((auxC!=NULL) && (auxC->idCliente!=clienteBuscado)){ //Procurando cliente na lClient//
            auxC=auxC->prox;
        }
        if(auxC==NULL){
            printf("Cliente não encontrado");
        }
        else{
            *client=removerLclient((*client),&noBuscado,clienteBuscado,livroDevolvido); //Remover da lista de livros do cliente//

            (*emprestados)=removerEmp((*emprestados),livroDevolvido); //Remover da lista de emprestados//

            if(disp!=NULL){
                noBuscado->prox=*disp;
            }
            *disp=noBuscado;
        }
    }
}

void imprimeEmprestadoCliente(lClient* aux,int idBuscado){
    lDispo *empClient;
    while(aux!=NULL){
        if(aux->idCliente==idBuscado){
            printf("\nLivros Possuídos: \n\n");
            empClient=aux->emprestados;
            while(empClient!=NULL){
                printf("-----------------------------------------------------\n");
                printf("\tCódigo do livro: %d\n",empClient->idLivro);
                printf("Titulo: %s\n",empClient->titulo);
                printf("Assunto: %s\n",empClient->assunto);
                printf("Autor: %s\n",empClient->autor);
                printf("-----------------------------------------------------\n\n");
                empClient=empClient->prox;
            }
        }
        aux=aux->prox;
    }
}

void imprimeTodosCliente(lClient *aux){
    while(aux!=NULL){
        printf("************************************************************\n");
        printf("Código do cliente: %d\n",aux->idCliente);
        printf("Nome: %s\n",aux->nome);
        imprimeEmprestadoCliente(aux,aux->idCliente);
        printf("************************************************************\n\n");
        aux=aux->prox;
    }
}

void imprimeTodosEmprestados(lEmp *inicio){
    lEmp *aux=inicio;
    if(inicio==NULL){printf("Nenhum livro emprestado\n");}
    else{
        printf("Livros Emprestados:\n\n");
        while(aux!=NULL){
            printf("-----------------------------------------------------\n");
            printf("Cliente responsável: %d\n",aux->idCliente);
            printf("Código: %d\n",aux->idLivro);
            printf("Título do livro: %s\n",aux->titulo);
            printf("Assunto: %s\n",aux->assunto);
            printf("Autor: %s\n\n",aux->autor);
            printf("-----------------------------------------------------\n\n");
            aux=aux->prox;
        }
    }
}

int main() {
    setlocale(LC_ALL, "portuguese");
    lDispo *dispo;
    lClient *clientes;
    lEmp *emprestados;
    dispo = iniciadispo();
    clientes = iniciaclient();
    emprestados = iniciaemp();

    //Menu:
    int menu,idBuscado;
    printf("\t\tBem vindo ao sistema da biblioteca\n");
    printf("\nComandos do sistema:\n\n");
    printf("0: Mostar comandos do sistema\n");
    printf("1: Inserir um livro novo na biblioteca\n");
    printf("2: Inserir um novo cliente\n");
    printf("3: Emprestar um livro para um cliente\n");
    printf("4: Devolver um livro de um determinado cliente\n");
    printf("5: Imprimir os livros que estão emprestados por um dado cliente\n");
    printf("6: Imprimir a Lista de todos os clientes com os respectivos livros emprestados\n");
    printf("7: Imprimir a Lista de Todos os Livros Emprestado\n");
    printf("8: Imprimir a lista de Livros Disponíveis da Biblioteca\n");
    printf("9: Sair do Sistema\n");
    do{
        printf("\nEscolha uma opção: ");
        scanf("%d", &menu);
        printf("\n");
        switch(menu){
            case 0:
                printf("\t\tBem vindo ao sistema da biblioteca\n");
                printf("\nComandos do sistema:\n\n");
                printf("0: Mostar comandos do sistema\n");
                printf("1: Inserir um livro novo na biblioteca\n");
                printf("2: Inserir um novo cliente\n");
                printf("3: Emprestar um livro para um cliente\n");
                printf("4: Devolver um livro de um determinado cliente\n");
                printf("5: Imprimir os livros que estão emprestados por um dado cliente\n");
                printf("6: Imprimir a Lista de todos os clientes com os respectivos livros emprestados\n");
                printf("7: Imprimir a Lista de Todos os Livros Emprestado\n");
                printf("8: Imprimir a lista de Livros Disponíveis da Biblioteca\n");
                printf("9: Sair do Sistema\n");
                break;
            case 1:
                dispo = insereLivro(dispo);
                break;
            case 2:
                clientes = insereCliente(clientes);
                break;
            case 3:
                emprestarLivro(&dispo,&clientes,&emprestados);
                break;
            case 4:
                devolverLivro(&dispo,&clientes,&emprestados);
                break;
            case 5:
                printf("Digite o cliente que deseja consultar: ");
                scanf("%d", &idBuscado);
                imprimeEmprestadoCliente(clientes,idBuscado);
                break;
            case 6:
                imprimeTodosCliente(clientes);
                break;
            case 7:
                imprimeTodosEmprestados(emprestados);
                break;
            case 8:
                imprimeDispo(dispo);
                break;
            case 9:
                printf("\nDesligando o sistema...");
                break;
            default:
                printf("Comando Inválido");
        }
    }while(menu!=9);
    return 0;
}
