#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    int dia, mes, ano, hora, min;
}dt;

typedef struct No{
    struct No *prox;
    dt data; 
    char desc[81];
}no;

no *cria_no(){
    no *new = (no*)malloc(sizeof(no));
    new->prox = NULL;
    printf("Digite o dia: ");
    scanf(" %d", &new->data.dia);
    printf("Digite o mes: ");
    scanf(" %d", &new->data.mes);
    printf("Digite o ano: ");
    scanf(" %d", &new->data.ano);
    printf("Digite o horario: ");
    scanf(" %d", &new->data.hora);
    printf("Digite os minutos: ");
    scanf(" %d", &new->data.min);
    printf("Descricao: ");
    scanf(" %80[^\n]", new->desc);
    return new;
}

int maior(no *no1, no* no2){
    if(no1->data.ano > no2->data.ano)
        return 1;
    else if(no1->data.ano == no2->data.ano){
        if(no1->data.mes > no2->data.mes)
            return 1;
        else if(no1->data.mes == no2->data.mes){
            if(no1->data.dia > no2->data.dia)
                return 1;
            else if(no1->data.dia == no2->data.dia){
                if(no1->data.hora > no2->data.hora)
                    return 1;
                else if(no1->data.hora == no2->data.hora){
                    if(no1->data.min > no2->data.min)
                        return 1;
                }
            }
        }
    }
    return 0;
}

void insere_inicio(no **head, no **new){
    (*new)->prox = *head;
    *head = *new;
}

void insere_fim(no **head, no **new){
    no *aux = *head;
    while(aux->prox)
        aux = aux->prox;
    aux->prox = *new;
}

void insere_ordenado(no **head, no *new){
    no *aux = *head;
    if(!(*head) || !maior(new, *head)){
        insere_inicio(head, &new);
    }
    else{
        while(aux->prox && maior(new, aux->prox))
            aux = aux->prox;
        if(aux->prox){
            new->prox = aux->prox;
            aux->prox = new;
        }
        else
            insere_fim(head, &new);
    }
}

int mesmo_dia(no *aux, int dia, int mes, int ano){
    if(aux->data.dia == dia && aux->data.mes == mes && aux->data.ano == ano)
        return 1;
    return 0;
}

void remove_compromisso_data(no **head){
    int ano, mes, dia;
    no *remover;
    printf("Digite o dia: ");
    scanf("%d", &dia);
    printf("Digite o mes: ");
    scanf("%d", &mes);
    printf("Digite o ano: ");
    scanf("%d", &ano);
    while(*head && mesmo_dia(*head, dia, mes, ano)){
        remover = *head;
        *head = remover->prox;
        free(remover);
    }
    no *aux = *head;
    if(aux){
        while(aux->prox){
            if(mesmo_dia(aux->prox, dia, mes, ano)){
                remover = aux->prox;
                aux->prox = remover->prox;
                free(remover);
            }
            else
                aux = aux->prox;
        }
    }
}

int possui_a_palavra(no *aux, char palavra[]){
    char *ptr = NULL;
    ptr = strstr(aux->desc, palavra);
    if(ptr != NULL)
        return 1;
    return 0;
}

int sim_nao(char *escolha){
    do{
        scanf(" %c", escolha);
        *escolha = toupper(*escolha);
    }while(*escolha != 'S' && *escolha != 'N');
    if(*escolha == 'S')
        return 1;
    return 0;
}

void remove_compromisso_palavra(no **head){
    no *aux = *head, *remover;
    char palavra[81], escolha;
    printf("Digite a palavra que deseja procurar: ");
    scanf(" %80[^\n]", palavra);
    while((*head) && possui_a_palavra((*head), palavra)){
        printf("Data: %02d/%02d/%04d, %02d:%02d\n Descricao: %s\n\n", aux->data.dia, aux->data.mes, aux->data.ano, aux->data.hora, aux->data.min, aux->desc);
        printf("Deseja excluir (S/N)? ");
        if(sim_nao(&escolha)){
            remover = *head;
            *head = remover->prox;
            free(remover);
        }
        else
            break;
    }
    aux = *head;
    if(aux){
        while(aux->prox){
            if(possui_a_palavra(aux->prox, palavra)){
                printf("Data: %02d/%02d/%04d, %02d:%02d\n Descricao: %s\n\n", aux->prox->data.dia, aux->prox->data.mes, aux->prox->data.ano, aux->prox->data.hora, aux->prox->data.min, aux->prox->desc);
                printf("Deseja excluir (S/N)? ");
                if(sim_nao(&escolha)){
                    remover = aux->prox;
                    aux->prox = remover->prox;
                    free(remover);
                }
                else
                    aux = aux->prox;
            }
            else
                aux = aux->prox;
        }
    }
}

void consulta(no *head){
    int ano, mes, dia;
    printf("Digite o dia: ");
    scanf("%d", &dia);
    printf("Digite o mes: ");
    scanf("%d", &mes);
    printf("Digite o ano: ");
    scanf("%d", &ano);
    printf("\n---------------AGENDA----------------\n\n");
    while(head){
        if(head->data.dia == dia && head->data.mes == mes && head->data.ano == ano)
            printf("Horario: %02d:%02d\nDescricao: %s\n\n", head->data.hora, head->data.min, head->desc);
        head = head->prox;
    }
    printf("-------------------------------------\n");
}

void consulta_palavra(no *head){
    char palavra[81], *ptr;
    printf("Digite a palavra a ser procurada: ");
    scanf(" %80[^\n]", palavra);
    printf("\n---------------AGENDA----------------\n\n");
    while(head){
        ptr = strstr(head->desc, palavra);
        if(ptr != NULL)
            printf("Data: %02d/%02d/%04d, %02d:%02d\n Descricao: %s\n\n", head->data.dia, head->data.mes, head->data.ano, head->data.hora, head->data.min, head->desc);
        head = head->prox;
    }
    printf("-------------------------------------\n");
}

void salvar_arquivo(no *head){
    FILE *arq = fopen("agenda.txt", "w");
    while(head){
        fprintf(arq, "%02d/%02d/%04d %02d:%02d\n", head->data.dia, head->data.mes, head->data.ano, head->data.hora, head->data.min);
        fprintf(arq, "%s\n\n", head->desc);
        head = head->prox;
    }
    printf("Agenda salva!\n");
    fclose(arq);
}

no *aloca_mem(){
    no *new = (no*)malloc(sizeof(no));
    new->prox = NULL;
    return new;
}

void le_arquivo(no **head){
    FILE *arq = fopen("agenda.txt", "r");
    no *new = aloca_mem();
    fscanf(arq, "%d/%d/%d %d:%d\n%80[^\n]\n\n", &(new->data.dia), &(new->data.mes), &(new->data.ano), &(new->data.hora), &(new->data.min), new->desc);
    *head = new;
    new = aloca_mem();
    while(fscanf(arq, "%d/%d/%d %d:%d\n%80[^\n]\n\n", &(new->data.dia), &(new->data.mes), &(new->data.ano), &(new->data.hora), &(new->data.min), new->desc) == 6){
        insere_fim(head, &new);
        new = aloca_mem();
    }
    printf("Arquivo lido!\n");
    fclose(arq);
}

void altera_data(no **aux){
    printf("Digite o dia: ");
    scanf("%d", &(*aux)->data.dia);
    printf("Digite o mes: ");
    scanf("%d", &(*aux)->data.mes);
    printf("Digite o ano: ");
    scanf("%d", &(*aux)->data.ano);
    printf("Digite a hora: ");
    scanf("%d", &(*aux)->data.hora);
    printf("Digite os minutos: ");
    scanf("%d", &(*aux)->data.min);
}

void remove_no(no **head, no **ant, no **atual){
    no *remove;
    if(*head == *ant){
        remove = *head;
        *head = *ant = *atual = remove->prox;
    }
    else{
        remove = (*ant)->prox;
        printf("remover: %d/%d/%d desc: %s\n", (*ant)->prox->data.dia, (*ant)->prox->data.mes, (*ant)->prox->data.ano, (*ant)->prox->desc);
        (*ant)->prox = *atual = remove->prox;
    }
    free(remove);
}

void altera_arquivo_palavra(no **head){
    char palavra[81], escolha;
    no *ant, *atual;
    ant = atual = *head;
    printf("Digite a palavra que deseja pesquisar: ");
    scanf(" %80[^\n]", palavra);
    while(atual){
        if(possui_a_palavra(atual, palavra)){
           printf("Data: %02d/%02d/%04d, %02d:%02d\n Descricao: %s\n\n", atual->data.dia, atual->data.mes, atual->data.ano, atual->data.hora, atual->data.min, atual->desc);
           printf("Deseja mudar a data (S/N)? ");
           if(sim_nao(&escolha)){
               no *aux = aloca_mem();
               strcpy(aux->desc, atual->desc);
               altera_data(&aux);
               remove_no(head, &ant, &atual);
               insere_ordenado(head, aux);
           }
        }
        if(atual && ant != atual)
            ant = ant->prox;
        if(atual)
            atual = atual->prox;
    }
}

void altera_compromisso_data(no *head){
    int dia, mes, ano;
    printf("Digite o dia: "); 
    scanf("%d", &dia);
    printf("Digite o mes: ");
    scanf("%d", &mes);
    printf("Digite o ano: ");
    scanf("%d", &ano);
    while(head){
    if(mesmo_dia(head, dia, mes, ano)){
            printf("Data: %02d/%02d/%04d, %02d:%02d\n Descricao: %s\n\n", head->data.dia, head->data.mes, head->data.ano, head->data.hora, head->data.min, head->desc);
            printf("Digite a frase: ");
            scanf(" %80[^\n]", head->desc);
        }
        head = head->prox;
    }
}

void mostra(no *head){
    while(head){
        printf("Data: %d/%d/%d %d:%d\nDesc: %s\n", head->data.dia, head->data.mes, head->data.ano, head->data.hora, head->data.min, head->desc);
        head = head->prox;
    }
}

int main(){
    no *head = NULL;
    int escolha;
    do{
        printf("\n\t=============================MENU=============================\n");
        printf("\n\t 0) Sair\n\t 1) Consulta compromissos\n\t 2) Consulta compromisso por palavra\n\t 3) Salvar em arquivo\n\t 4) Ler arquivo\n\t 5) Inserir compromisso\n\t 6) Remover compromissos do dia\n\t 7) Remover compromissos por palavra\n\t 8) Altera o compromisso por palavra\n\t 9) Altera o compromisso por dia\n\t 10) Exibir todos os compromissos\n");
        printf("\n\t==============================================================\n");
        scanf("%d", &escolha);
        switch(escolha){
            case 1:
                consulta(head); break;
            case 2:
                consulta_palavra(head); break;
            case 3:
                salvar_arquivo(head); break;
            case 4:
                le_arquivo(&head); break;
            case 5:
                no *new = cria_no(); 
                insere_ordenado(&head, new); break;
            case 6:
                remove_compromisso_data(&head); break;
            case 7:
                remove_compromisso_palavra(&head); break;
            case 8:
                altera_arquivo_palavra(&head); break;
            case 9:
                altera_compromisso_data(head); break;
            case 10:
                mostra(head); break;
        }
    }while(escolha);
}
