#include <ncurses.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

typedef struct{
    int dia, mes, ano;
}data;

typedef struct{
    char nome[21], classif[51];
    int ID, existe;
    data vencimento, producao;
}produto;

WINDOW *create_win(int height, int width, int starty, int startx);
void destroy_win(WINDOW *localw);
int menu(WINDOW *localw, int height, int width, int *escolha);
int menu2(int xmax, int ymax);
int help();
int cadastraEstoque(WINDOW *localw, int height, int width);
int consultarEstoque(WINDOW *localw, int height, int width);
int alteraEstoque(WINDOW *localw, int height);
int excluirEstoque(WINDOW *localw);

int main(){
    WINDOW *w, *sombra;
    int xmax, ymax, startx, starty, width, height;
    int ch=0;
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();
    keypad(stdscr, true);
    w = create_win(height, width, starty, startx);
    keypad(w, true);
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
    refresh();

    getmaxyx(stdscr, ymax, xmax);
    width = 3*xmax/4;
    height = 3*ymax/4;
    starty = (LINES - height)/2;
    startx = (COLS - width)/2;
    int flag=1;
    while(ch !=27){
        sombra = create_win(height, width, starty+1, startx+3);
        wbkgd(sombra, COLOR_PAIR(4));
        wrefresh(sombra);
        w = create_win(height, width, starty, startx);
        mvprintw(ymax-1, 0, "1/2");
        mvchgat(ymax-1, 0, 3, A_STANDOUT, 0, NULL);
        refresh();
        wrefresh(w);
        menu(w, height, width, &ch);
        flag=1;
        ch = getch();
        switch(ch){
            case KEY_RIGHT:
                    destroy_win(w);
                    destroy_win(sombra);
                    //w = create_win(height/2, width/2, (LINES-height/2)/2, (COLS-width/2)/2); 
                    mvprintw(ymax-1, 0, "2/2");
                    mvchgat(ymax-1, 0, 3, A_STANDOUT, 0, NULL);
                    flag = 0;
                    menu2(xmax, ymax);
                    wclear(stdscr);
                    refresh();
                break;
            /*case KEY_LEFT:
                if(!flag){
                    wclear(stdscr);
                    //destroy_win(w);
                    mvprintw(ymax-1, 0, "1/2");
                    mvchgat(ymax-1, 0, 3, A_STANDOUT, 0, NULL);
                    sombra = create_win(height, width, starty+1, startx+3);
                    wbkgd(sombra, COLOR_PAIR(4));
                    wrefresh(sombra);
                    w = create_win(height, width, starty, startx);
                    menu(w, height, width);
                    flag = 1;
                }
                break;
            */
        }
    }
    endwin();
}

WINDOW *create_win(int height, int width, int starty, int startx){
    WINDOW *localw = newwin(height, width, starty, startx);
    wbkgd(localw, COLOR_PAIR(2));
    box(localw, 0, 0);
    keypad(localw, true);
    mvwaddch(localw, 0, width/2 - 9, ACS_VLINE| COLOR_PAIR(2));
    mvwprintw(localw, 0, width/2 - 8, " ALGORITMOS II ");
    mvwaddch(localw, 0, width/2 + 7, ACS_VLINE | COLOR_PAIR(2));
    mvwchgat(localw, 0, width/2 - 8, 15, A_REVERSE, 3, NULL);
    wrefresh(localw);
    refresh(); 
    return localw;
}

void destroy_win(WINDOW *localw){
    wborder(localw, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wclear(localw);
    wbkgd(localw, COLOR_PAIR(1));
    wrefresh(localw);
    delwin(localw);
}

int help(){
    int xmax, ymax;
    getmaxyx(stdscr, ymax, xmax);
    WINDOW *ajuda = create_win(ymax, xmax, 0, 0);
    mvwprintw(ajuda, 1, 1, "Este programa eh constituido de duas telas principais.\n");
    mvwprintw(ajuda, 2, 1,  "-> Para ir para a tela direita, pressione 2 vezes [KEY_RIGHT].\n");
    mvwprintw(ajuda, 3, 1, "-> Para ir para a tela esquerda, pressione 2 vezes [KEY_LEFT].\n");
    mvwprintw(ajuda, 5, 1, "TELA 1/2\n");
    mvwprintw(ajuda, 6, 1, "A tela 1 se refere ao estoque de uma loja.");
    mvwprintw(ajuda, 7, 1, "Para selecionar o que deseja fazer, mova com [KEY_UP] e [KEY_DOWN] e pressione [ENTER]");
    mvwchgat(ajuda, 5, 1, 8, A_REVERSE, 3, NULL);
    mvwprintw(ajuda, 9, 1, "TELA 2/2\n");
    mvwprintw(ajuda, 10, 1, "A tela 2 se refere aos arquivos da atual pasta deste arquivo.");
    mvwprintw(ajuda, 11, 1, "Para selecionar um arquivo, pressione [ENTER]");
    mvwprintw(ajuda, 12, 1, "Ao selecionar um arquivo, uma nova aba de escolhas aparecera.");
    mvwprintw(ajuda, 13, 1, "Para selecionar uma opcao, mova com [KEY_UP] e [KEY_DOWN] e aperte [ENTER]");
    mvwchgat(ajuda, 9, 1, 8, A_REVERSE, 3, NULL);
    wrefresh(ajuda);
    refresh();
    getch();
    destroy_win(ajuda);
}

int menu(WINDOW* localw, int height, int width, int *escolha){
    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);
    char opcoes[4][20] = {"Cadastrar", "Alterar", "Excluir", "Consultar"};
    int destaque=0;
    for(int i=0; i<4; i++){
        if(!i) wattron(localw, A_REVERSE);
        else wattroff(localw, A_REVERSE);
        mvwprintw(localw, i+2, 1, "%s", opcoes[i]);
    }
    wrefresh(localw);
    refresh();
    while(*escolha!=KEY_RIGHT && *escolha!=27 && *escolha!=10){
        *escolha = wgetch(localw);
        switch(*escolha){
            case KEY_UP:
                destaque--;
                if(destaque==-1)
                    destaque=3;
                for(int i=0; i<4; i++){
                    if(i==destaque)wattron(localw, A_REVERSE);
                    mvwprintw(localw, i+2, 1, "%s", opcoes[i]);
                    wattroff(localw, A_REVERSE);
                }
                wrefresh(localw);
                break;
            case KEY_DOWN:
                destaque++;
                if(destaque==4)
                    destaque=0;
                for(int i=0; i<4; i++){
                    if(i==destaque)wattron(localw, A_REVERSE);
                    mvwprintw(localw, i+2, 1, "%s", opcoes[i]);
                    wattroff(localw, A_REVERSE);
                }
                wrefresh(localw);
                break;
            case KEY_F(1):
                help();
                WINDOW *sombra = create_win(height, width, (LINES - height)/2 + 1, (COLS - width)/2 + 3);
                wbkgd(sombra, COLOR_PAIR(4));
                wrefresh(sombra);
                localw = create_win(height, width, (LINES - height)/2, (COLS - width)/2);
                destaque=0;
                for(int i=0; i<4; i++){
                    if(i==destaque)wattron(localw, A_REVERSE);
                    mvwprintw(localw, i+2, 1, "%s", opcoes[i]);
                    wattroff(localw, A_REVERSE);
                }
                keypad(localw, true);
                mvprintw(ymax-1, 0, "1/2");
                mvchgat(ymax-1, 0, 3, A_STANDOUT, 0, NULL);
                refresh();
                wrefresh(localw);
            break;
        }
        wrefresh(localw);
        refresh();
    }
    if(destaque==0 && *escolha==10){
        wclear(localw);
        localw = create_win(height, width, (LINES - height)/2, (COLS-width)/2);
        cadastraEstoque(localw, height, width);
    }
    if(destaque==1 && *escolha==10){
        wclear(localw);
        localw = create_win(height, width, (LINES - height)/2, (COLS-width)/2);
        alteraEstoque(localw, height);
    }
    if(destaque==2 && *escolha==10){
        wclear(localw);
        localw = create_win(height, width, (LINES - height)/2, (COLS-width)/2);
        excluirEstoque(localw); 
    }
    if(destaque==3 && *escolha==10){
        wclear(localw);
        localw = create_win(height, width, (LINES - height)/2, (COLS - width)/2);
        consultarEstoque(localw, height, width);
    }
} 

int menu2(int xmax, int ymax){
    char cwd[151];
    DIR *dir;
    if(getcwd(cwd, sizeof(cwd)) != NULL)
        dir = opendir(cwd);
    else{
        perror("getcwd() error");
        return -1;
    }
    struct dirent *files = NULL;
    int escolha, destaque=0, i=0;
    move(0,0);
    char nomes[2001][51];
    while((files = readdir(dir)) != NULL){
        char aux[2001];
        if(!i){
            attron(A_REVERSE);
            printw("..\n");
        }
        if(i==1)
            printw(".\n");
        strcpy(nomes[i], files->d_name);
        if(!strcmp(nomes[i], "..")){
            strcpy(aux, nomes[0]);
            strcpy(nomes[0], nomes[i]);
            strcpy(nomes[i], aux);
        }
        if(!strcmp(nomes[i], ".")){
            strcpy(aux, nomes[1]);
            strcpy(nomes[1], nomes[i]);
            strcpy(nomes[i], aux);
        }
        if(i>1)printw("%s\n", nomes[i]);
        i++;
        attroff(A_REVERSE);
    }
    refresh();
    while((escolha = getch())!= KEY_LEFT && escolha!=10 && escolha!=27){
        int j=0;
        switch(escolha){
            case KEY_UP:
                j=0;
                destaque--;
                if(destaque==-1)
                    destaque=i-1;
                move(0,0);
                for(int j=0; j<i; j++){
                    if(j==destaque) attron(A_REVERSE);
                    printw("%s\n", nomes[j]);
                    attroff(A_REVERSE);
                }
                break;
            case KEY_DOWN:
                j=0;
                destaque++;
                if(destaque==i)
                    destaque=0;
                move(0,0);
                for(int j=0; j<i; j++){
                    if(j==destaque) attron(A_REVERSE);
                    printw("%s\n", nomes[j]);
                    attroff(A_REVERSE);
                }
                break;
        }
    }
    if(escolha==10 && (destaque==0 || destaque==1))
        chdir("..");
    else if(escolha==10){
        WINDOW *opcao = create_win(10, xmax, ymax-10, 0);
        int escolha2, destaque2=0;
        char opcoes[6][51] = {"Alterar nome do arquivo", "Remover arquivo", "Criar pasta", "Mudar arquivo de pasta", "Alterar data e hora do sistema", "Ir para diretorio"};
        for(int i=0; i<6; i++){
            if(!i)wattron(opcao, A_REVERSE);
            mvwprintw(opcao, 1+i, 1, "%s", opcoes[i]);
            wattroff(opcao, A_REVERSE);
        }
        mvwprintw(opcao, 9, 0, "2/2");
        wrefresh(opcao);
        while((escolha2 = getch()) != 10 && escolha2!=27){
            switch(escolha2){
                case KEY_UP:
                    destaque2--;
                    if(destaque2 == -1)
                        destaque2 = 5;
                    for(int i=0; i<6; i++){
                        if(i==destaque2) wattron(opcao, A_REVERSE);
                        mvwprintw(opcao, 1+i, 1, "%s", opcoes[i]);
                        wattroff(opcao, A_REVERSE);
                    }
                    wrefresh(opcao);
                    break;
                case KEY_DOWN:
                    destaque2++;
                    if(destaque2 == 6)
                        destaque2 = 0;
                    for(int i=0; i<6; i++){
                        if(i==destaque2) wattron(opcao, A_REVERSE);
                        mvwprintw(opcao, 1+i, 1, "%s", opcoes[i]);
                        wattroff(opcao, A_REVERSE);
                    }
                    wrefresh(opcao);
                    break;
            }
        }
       if(destaque2==0 && escolha2==10){
           wclear(opcao);
           wclear(stdscr);
           //muda o nome do arquivo;
           char novoN[31];
           echo();
           FILE *arqMuda = fopen(nomes[destaque], "r+");
           if((arqMuda = fopen(nomes[destaque], "r+"))==NULL){
               mvprintw(0,0, "Nao foi possivel realizar a acao!");
           }
           else{
               mvprintw(0,0, "Digite o novo nome do arquivo: ");
               mvscanw(0, 31, " %s", novoN);
               rename(nomes[destaque], novoN);
               noecho();
               printw("Arquivo renomeado!\n\n\nAperte qualquer tecla para sair.");
               fclose(arqMuda);
           }
           refresh();
           getch();
       } 
       else if(destaque2==1 && escolha2==10){
           wclear(opcao);
           wclear(stdscr);
           //remove arquivo
           FILE *arqDel = fopen(nomes[destaque], "r+");
           if((arqDel = fopen(nomes[destaque], "r+"))==NULL)
               mvprintw(0,0, "Nao foi possivel realizar a acao!");
           else{
               remove(nomes[destaque]);
               mvprintw(0,0,"Arquivo deletado com sucesso!\n\n\n\nAperte qualquer tecla para sair.");
               fclose(arqDel);
           }
           getch();
           refresh();
       }
       else if(destaque2==2 && escolha2==10){
           struct stat st;
           char nomeDir[31];
           //cria pasta
           wclear(opcao);
           wclear(stdscr);
           echo();
           mvprintw(0,0,"Digite o nome da pasta: ");
           scanw(" %s", nomeDir);
           noecho();
           strcat(cwd, "/");
           strcat(cwd, nomeDir);
           if(stat(cwd, &st)==-1){
               mkdir(cwd, 0777);
               printw("\n\n\nPasta feita com sucesso");
           }
           else
               printw("\n\n\nNao foi possivel realizar a acao!");
           refresh();
           getch();
       }
       else if(destaque2==3 && escolha2==10){
           wclear(stdscr);
           wclear(opcao);
           //muda arquivo de pasta
           FILE *arqMov;
           if((arqMov = fopen(nomes[destaque], "r+"))==NULL){
               mvprintw(0,0, "Nao foi possivel realizar a acao!");
           }
           else{
               char diretorio[301];
               mvprintw(0,0, "Voce esta no diretorio: %s\n", cwd);
               printw("Digite o diretorio onde deseja mover o arquivo: ");
               echo();
               scanw(" %[^\n]", diretorio);
               noecho();
                   strcat(diretorio, "/");
                   strcat(diretorio, nomes[destaque]);
                   rename(nomes[destaque], diretorio);
                   fclose(arqMov);
            }
           refresh();
           getch();
       }
       else if(destaque2==5 && escolha2==10){
           strcat(cwd, "/");
           strcat(cwd, nomes[destaque]);
           struct stat st;
           if(chdir(cwd)!=0){
               wclear(stdscr);
               wclear(opcao);
               mvprintw(0,0, "%s\n", cwd);
               printw("Nao foi possivel realizar a acao!");
               refresh();
               getch();
           }
       }
    }
    closedir(dir);
}

int cadastraEstoque(WINDOW *localw, int height, int width){
    FILE *arq;
    produto p;
    if((arq = fopen("estoque.txt", "a+b"))==NULL){
        mvwprintw(localw, 1, 1, "Nao foi possivel criar o arquivo");
        getch();
        return -1;
    }
    echo();
    mvwprintw(localw, 1, 1, "Digite o nome do produto: ");
    mvwscanw(localw, 1, 27, " %20[^\n]", p.nome);
    mvwprintw(localw, 2, 1, "Digite a classificacao (limpeza, comestivel, ...): ");
    mvwscanw(localw, 2, 52, " %50[^\n]", p.classif);
    mvwprintw(localw, 3, 1, "Digite o ID: ");
    mvwscanw(localw, 3, 14, "%d", &p.ID);
    mvwprintw(localw, 4, 1, "Digite a data de producao (dd/mm/aaaa): ");
    mvwscanw(localw, 4, 41, "%d%d%d", &p.producao.dia, &p.producao.mes, &p.producao.ano);
    mvwprintw(localw, 5, 1, "Digite a data de vencimento (dd/mm/aaaa): ");
    mvwscanw(localw, 5, 43, "%d%d%d", &p.vencimento.dia, &p.vencimento.mes, &p.vencimento.ano);
    p.existe = 1;
    noecho();
    mvwprintw(localw, 6, 1, "Produto salvo!");
    mvwprintw(localw, 7, 1, "Pressione [KEY_UP] ou [KEY_DOWN]");
    fwrite(&p, sizeof(p), 1, arq);
    wrefresh(localw);
    fclose(arq);
}

int consultarEstoque(WINDOW *localw, int height, int width){
    FILE *arq;
    produto p;
    if((arq = fopen("estoque.txt", "r+b"))==NULL){
        mvwprintw(localw, 1, 1, "Nao foi possivel criar o arquivo");
        getch();
        return -1;
    }
    int i=0;
    while(fread(&p, sizeof(p), 1, arq)==1){
        if(p.existe){
            mvwprintw(localw, 1+i, 1, "Nome do produto: %s", p.nome);
            mvwprintw(localw, 2+i, 1, "Classificacao do produto: %s", p.classif);
            mvwprintw(localw, 3+i, 1, "ID do produto: %d", p.ID);
            mvwprintw(localw, 4+i, 1, "Data de producao: %d/%d/%d", p.producao.dia, p.producao.mes, p.producao.ano);
            mvwprintw(localw, 5+i, 1, "Data de vencimento: %d/%d/%d", p.vencimento.dia, p.vencimento.mes, p.vencimento.ano);
            wrefresh(localw);
            if(!(i%12) && i){
                i=0;
                getch();
                wclear(localw);
                localw = create_win(height, width, (LINES - height)/2, (COLS - width)/2);
            }
            else i+=6;
        }
    }
    mvwprintw(localw, height-1, 1, "Pressione [KEY_UP] ou [KEY_DOWN] para sair");
    wrefresh(localw);
    fclose(arq);
}

int alteraEstoque(WINDOW *localw, int height){
    FILE *arq;
    int id, achou=0;
    produto p;
    if((arq = fopen("estoque.txt", "r+b"))==NULL){
        mvwprintw(localw, 1, 1, "Nao foi possivel abrir o arquivo");
        getch();
        return -1;
    }
    mvwprintw(localw, 1, 1, "Digite o ID do produto: ");
    echo();
    mvwscanw(localw, 1, 25, "%d", &id);
    while(!achou && fread(&p, sizeof(produto), 1, arq)==1){
        if(p.existe && p.ID==id){
            mvwprintw(localw, 2, 1, "Digite o nome do produto: ");
            mvwscanw(localw, 2, 27, " %20[^\n]", p.nome);
            mvwprintw(localw, 3, 1, "Digite a classificacao (limpeza, comestivel, ...): ");
            mvwscanw(localw, 3, 52, " %50[^\n]", p.classif);
            mvwprintw(localw, 4, 1, "Digite o ID: ");
            mvwscanw(localw, 4, 14, "%d", &p.ID);
            mvwprintw(localw, 5, 1, "Digite a data de producao (dd/mm/aaaa): ");
            mvwscanw(localw, 5, 41, "%d%d%d", &p.producao.dia, &p.producao.mes, &p.producao.ano);
            mvwprintw(localw, 6, 1, "Digite a data de vencimento (dd/mm/aaaa): ");
            mvwscanw(localw, 6, 43, "%d%d%d", &p.vencimento.dia, &p.vencimento.mes, &p.vencimento.ano);
            p.existe = 1;
            fseek(arq, -sizeof(produto), SEEK_CUR);
            fwrite(&p, sizeof(produto), 1, arq);
            noecho();
            achou=1;
        }
    }
    if(!achou) mvwprintw(localw, height-1, 1, "ID nao encontrado");
    else mvwprintw(localw, height-1, 1, "Dados alterados!");
    fclose(arq);
    wrefresh(localw);
}

int excluirEstoque(WINDOW *localw){
    FILE *arq, *arq2;
    int id, achou=0;
    char resp;
    produto p;
    echo();
    if((arq = fopen("estoque.txt", "r+b"))==NULL){
        mvwprintw(localw, 1, 1, "Erro na abertura do arquivo");
        getch();
        return -1;;
    }    
    mvwprintw(localw, 1, 1, "ID do produto que deseja excluir: ");
    mvwscanw(localw, 1, 35, "%d", &id);
    while(!achou && fread(&p, sizeof(produto), 1, arq)==1){
        if(p.existe && p.ID==id){
            mvwprintw(localw, 2, 1, "Nome do produto: %s", p.nome);
            mvwprintw(localw, 3, 1, "Classificacao do produto: %s", p.classif);
            mvwprintw(localw, 4, 1, "ID do produto: %d", p.ID);
            mvwprintw(localw, 5, 1, "Data de producao: %d/%d/%d", p.producao.dia, p.producao.mes, p.producao.ano);
            mvwprintw(localw, 6, 1, "Data de vencimento: %d/%d/%d", p.vencimento.dia, p.vencimento.mes, p.vencimento.ano);
            p.existe = 0;
            fseek(arq, -sizeof(produto), SEEK_CUR);
            fwrite(&p, sizeof(p), 1, arq);
            mvwprintw(localw, 8, 1, "Produto deletado!");
            achou=1;
        }
    }
    if(!achou)
        mvwprintw(localw, 3, 1, "Produto nao encontrado");
    if(achou){
        mvwprintw(localw, 9, 1, "Deseja excluir fisicamente (S/N)?");
        do{
            mvwscanw(localw, 9, 36, " %c", &resp);
            resp = toupper(resp);
        }while(resp!='S' && resp!='N');
        if(resp=='S'){
            arq2 = fopen("copiahahahaNnNNNNNNn.txt", "a+b");
            rewind(arq);
            while(fread(&p, sizeof(p), 1, arq)==1){
                if(p.existe)
                    fwrite(&p, sizeof(p), 1, arq2);
            }
            fclose(arq);
            remove("estoque.txt");
            rename("copiahahahaNnNNNNNNn.txt", "estoque.txt");
            fclose(arq2);
            mvwprintw(localw, 11, 1, "Arquivo excluido fisicamente");
        }
        else
            mvwprintw(localw, 11, 1, "Arquivo excluido apenas logicamente.");
    }
    else{
        fclose(arq);
    }
    noecho();
    wrefresh(localw);
}
