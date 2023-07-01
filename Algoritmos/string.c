#include <ncurses.h>
#include <string.h>

void print_centered(WINDOW *win, int start_row, char text[]);
char *strrev(char *str);
void cheque(WINDOW *win);
void saque(WINDOW *win);

int main()
{
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_YELLOW);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(2));
    noecho();
    cbreak();
    curs_set(0);
    int xmax, ymax, escolha, destaque;
    getmaxyx(stdscr, ymax, xmax);
    char opcoes[4][7] = {"CHEQUE", "SAQUES", "HELP", "SAIR"};
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    WINDOW *janela = newwin(ymax / 2 + 4, xmax / 2, ymax / 4, xmax / 4);
    WINDOW *sombra = newwin(ymax / 2 + 4, xmax / 2, ymax / 4 + 1, xmax / 4 + 3);
    box(janela, 0, 0);
    wbkgd(janela, COLOR_PAIR(1));
    wbkgd(sombra, COLOR_PAIR(3));
    refresh();
    wrefresh(sombra);

    keypad(janela, true);

    while (1)
    {
        attron(COLOR_PAIR(1));
        wattron(janela, COLOR_PAIR(3));
        print_centered(stdscr, 3, "TRABALHO DE ALGORITMOS II");
        print_centered(janela, 0, "BANCO UNESP-BAURU");
        attroff(COLOR_PAIR(1));
        wattroff(janela, COLOR_PAIR(3));
        // senha: lepec22;
        refresh();
        char senha[8];
        while (strcmp(senha, "lepec22"))
        {
            print_centered(janela, 5, "DIGITE A SENHA:");
            echo();
            mvwscanw(janela, 6, xmax / 4 - 2, "%7[^\n]", senha);
            noecho();
            if (strcmp(senha, "lepec22"))
            {
                print_centered(janela, 8, "SENHA INCORRETA");
                print_centered(janela, 9, "ENCERRANDO O PROGRAMA");
                wrefresh(janela);
                goto fim;
            }
        }
    inicio:
        wclear(janela);
        box(janela, 0, 0);
        wattron(janela, COLOR_PAIR(3));
        print_centered(janela, 0, "BANCO UNESP-BAURU");
        wattroff(janela, COLOR_PAIR(3));
        wrefresh(janela);
        refresh();

        for (int i = 0; i < 4; i++)
        {
            if (i == destaque)
                wattron(janela, A_REVERSE);
            print_centered(janela, ymax / 4 + i - 2, opcoes[i]);
            wattroff(janela, A_REVERSE);
        }
        escolha = wgetch(janela);
        switch (escolha)
        {
        case KEY_UP:
            destaque--;
            if (destaque == -1)
                destaque = 3;
            break;
        case KEY_DOWN:
            destaque++;
            if (destaque == 4)
                destaque = 0;
            break;
        default:
            break;
        }
        if (escolha == 10)
            break;
    }
    while (1)
    {
        if (destaque == 0)
        {
            cheque(janela);
            refresh();
            wrefresh(janela);
            getch();
            goto inicio;
        }
        else if (destaque == 1)
        {
            saque(janela);
            getch();
            goto inicio;
        }
        else if (destaque == 2)
        {
            wclear(janela);
            box(janela, 0, 0);
            print_centered(janela, 1, "---MENU DE AJUDA---");
            print_centered(janela, 4, "1. Teclas UP and DOWN movem e ENTER seleciona");
            print_centered(janela, 5, "2. CTRL + C (^C) sai do programa");
            print_centered(janela, 6, "3. Para usar o cheque, nao esqueca de colocar '.00'");
            wrefresh(janela);
            getch();
            goto inicio;
        }
        else if (destaque == 3)
        {
            print_centered(janela, 10, "VOCE ESCOLHEU SAIR!");
            wrefresh(janela);
            break;
        }
    }
fim:
    getch();
    endwin();
}

void print_centered(WINDOW *win, int start_row, char text[])
{
    int center_col = win->_maxx / 2;
    int half_length = strlen(text) / 2;
    int adjusted_col = center_col - half_length;
    mvwprintw(win, start_row, adjusted_col, text);
}

char *strrev(char *str)
{
    char *p1, *p2;
    if (!str || !*str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

void cheque(WINDOW *win)
{
    wclear(win);
    box(win, 0, 0);
    wrefresh(win);
    int escolha, xmax, ymax;
    getmaxyx(win, ymax, xmax);
    int cont = 0, i = 0;
    long int n = 0;
    char cent[3], r[50], s[18], t[18], u[18], num[50], numCat[200] = "";
    for (int i = 0; i < 50; i++)
    {
        r[i] = 0;
        num[i] = 0;
    }
    for (int i = 0; i < 18; i++)
    {
        t[i] = 0;
        s[i] = 0;
        u[i] = 0;
    }
    print_centered(win, 1, "DIGITE A QUANTIDADE DE BITS (B$):");
    echo();
    mvwscanw(win, 3, xmax / 2 - 3, " %49[^\n]", r);
    noecho();
    // printf("%s\n", r); //talvez excluir
    strcpy(r, strtok(r, "."));
    // printf("%s\n", r); //excluir
    strcpy(cent, strtok(NULL, "\0"));
    cent[2] = 0;
    // adiciona zeros para os bits;
    strrev(r);
    while (strlen(r) % 3)
        strcat(r, "0");
    strrev(r);
    cont = strlen(r) / 3 + 1;
    // adiciona zeros para os bitscents
    strrev(cent);
    while (strlen(cent) < 3)
        strcat(cent, "0");
    strrev(cent);
    strcat(r, cent);
    // printf("%s\n", cent); // excluir
    while (cont--)
    {
        if (r[0 + i] == '1')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "CEM");
            }
            else
                strcpy(s, "CENTO E ");
        }
        else if (r[0 + i] == '2')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "DUZENTOS");
            }
            else
            {
                strcpy(s, "DUZENTOS E ");
            }
        }
        else if (r[0 + i] == '3')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "TREZENTOS");
            }
            else
            {
                strcpy(s, "TREZENTOS E ");
            }
        }
        else if (r[0 + i] == '4')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "QUATROCENTOS");
            }
            else
            {
                strcpy(s, "QUATROCENTOS E ");
            }
        }
        else if (r[0 + i] == '5')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "QUINHENTOS");
            }
            else
            {
                strcpy(s, "QUINHENTOS E ");
            }
        }
        else if (r[0 + i] == '6')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "SEISCENTOS");
            }
            else
            {
                strcpy(s, "SEISCENTOS E ");
            }
        }
        else if (r[0 + i] == '7')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "SETECENTOS");
            }
            else
            {
                strcpy(s, "SETECENTOS E ");
            }
        }
        else if (r[0 + i] == '8')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "OITOCENTOS");
            }
            else
            {
                strcpy(s, "OITOCENTOS E ");
            }
        }
        else if (r[0 + i] == '9')
        {
            if (r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(s, "NOVECENTOS");
            }
            else
            {
                strcpy(s, "NOVECENTOS E ");
            }
        }
        if (r[1 + i] == '1')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "DEZ");
            }
            else if (r[2 + i] == '1')
            {
                strcpy(t, "ONZE");
            }
            else if (r[2 + i] == '2')
            {
                strcpy(t, "DOZE");
            }
            else if (r[2 + i] == '3')
            {
                strcpy(t, "TREZE");
            }
            else if (r[2 + i] == '4')
            {
                strcpy(t, "QUATORZE");
            }
            else if (r[2 + i] == '5')
            {
                strcpy(t, "QUINZE");
            }
            else if (r[2 + i] == '6')
            {
                strcpy(t, "DEZESSEIS");
            }
            else if (r[2 + i] == '7')
            {
                strcpy(t, "DEZESSETE");
            }
            else if (r[2 + i] == '8')
            {
                strcpy(t, "DEZOITO");
            }
            else if (r[2 + i] == '9')
            {
                strcpy(t, "DEZENOVE");
            }
        }
        else if (r[1 + i] == '2')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "VINTE");
            }
            else
            {
                strcpy(t, "VINTE E ");
            }
        }
        else if (r[1 + i] == '3')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "TRINTA");
            }
            else
            {
                strcpy(t, "TRINTA E ");
            }
        }
        else if (r[1 + i] == '4')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "QUARENTA");
            }
            else
            {
                strcpy(t, "QUARENTA E ");
            }
        }
        else if (r[1 + i] == '5')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "CINQUENTA");
            }
            else
            {
                strcpy(t, "CINQUENTA E ");
            }
        }
        else if (r[1 + i] == '6')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "SESSENTA");
            }
            else
            {
                strcpy(t, "SESSENTA E ");
            }
        }
        else if (r[1 + i] == '7')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "SETENTA");
            }
            else
            {
                strcpy(t, "SETENTA E ");
            }
        }
        else if (r[1 + i] == '8')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "OITENTA");
            }
            else
            {
                strcpy(t, "OITENTA E ");
            }
        }
        else if (r[1 + i] == '9')
        {
            if (r[2 + i] == '0')
            {
                strcpy(t, "NOVENTA");
            }
            else
            {
                strcpy(t, "NOVENTA E ");
            }
        }
        if (r[1 + i] != '1')
        {
            if (r[2 + i] == '1')
            {
                strcpy(u, "UM");
            }
            else if (r[2 + i] == '2')
            {
                strcpy(u, "DOIS");
            }
            else if (r[2 + i] == '3')
            {
                strcpy(u, "TRES");
            }
            else if (r[2 + i] == '4')
            {
                strcpy(u, "QUATRO");
            }
            else if (r[2 + i] == '5')
            {
                strcpy(u, "CINCO");
            }
            else if (r[2 + i] == '6')
            {
                strcpy(u, "SEIS");
            }
            else if (r[2 + i] == '7')
            {
                strcpy(u, "SETE");
            }
            else if (r[2 + i] == '8')
            {
                strcpy(u, "OITO");
            }
            else if (r[2 + i] == '9')
            {
                strcpy(u, "NOVE");
            }
        }
        if (r[0 + i] == '0' && r[1 + i] == '0' && r[2 + i] == '0')
        {
            strcpy(num, "ZERO");
        }
        else if (r[0 + i] == '0' && r[1 + i] == '0' && r[2 + i] != '0')
        {
            strcpy(num, u);
        }
        else if (r[0 + i] == '0' && r[1 + i] != '0')
        {
            if (r[1 + i] == '1')
            {
                strcpy(num, t);
            }
            else if (r[2 + i] == '0')
            {
                strcpy(num, t);
            }
            else
            {
                strcpy(num, t);
                strcat(num, u);
            }
        }
        else if (r[0 + i] != '0')
        {
            if (r[0 + i] == '1' && r[1 + i] == '0' && r[2 + i] == '0')
            {
                strcpy(num, s);
            }
            else if (r[1 + i] == '1')
            {
                strcpy(num, s);
                strcat(num, t);
            }
            else if (r[2 + i] == '0')
            {
                strcpy(num, s);
                strcat(num, t);
            }
            else
            {
                strcpy(num, s);
                strcat(num, t);
                strcat(num, u);
            }
        }
        if (strcmp(num, "ZERO"))
            strcat(numCat, num);
        if (cont == 4)
        {
            if (r[2 + i] == '1' && r[1 + i] == '0' && r[i] == '0')
                strcat(numCat, " BILHAO ");
            else if (r[2 + i] == '0' && r[1 + i] == '0' && r[i] == '0')
                ;
            else
                strcat(numCat, " BILHOES ");
        }
        else if (cont == 3)
        {
            if (r[2 + i] == '1' && r[i + 1] == '0' && r[i] == '0')
                strcat(numCat, " MILHAO ");
            else if (r[2 + i] == '0' && r[1 + i] == '0' && r[i] == '0')
                ;
            else
                strcat(numCat, " MILHOES ");
        }
        else if (cont == 2)
        {
            if (r[2 + i] == '0' && r[i + 1] == '0' && r[i] == '0')
                ;
            else
                strcat(numCat, " MIL ");
        }
        else if (cont == 1)
            strcat(numCat, " BITS ");
        else if (!cont)
        {
            if (!strcmp(num, "ZERO"))
                ;
            else
                strcat(numCat, " BITCENTS");
        }
        i += 3;
    }
    if (strcmp(numCat, " BITS "))
        mvwprintw(win, 5, 1, numCat);
    else
        print_centered(win, 5, "ZERO BITS");
}

void saque(WINDOW *win)
{
    wclear(win);
    box(win, 0, 0);
    wrefresh(win);
    double num;
    int xmax, ymax, inteiro, decimal, notas[6] = {100, 50, 20, 10, 5, 2}, cont = 0, moedas[4] = {50, 25, 10, 5};
    getmaxyx(stdscr, ymax, xmax);
    print_centered(win, 1, "DIGITE A QUANTIDADE DE BITS:");
    echo();
    mvwscanw(win, 3, xmax / 4 - 3, "%lf", &num);
    noecho();
    inteiro = num;
    decimal = num * 100 - inteiro * 100;
    print_centered(win, 5, "NOTAS:");
    for (int i = 0; i < 6; i++)
    {
        if (inteiro / notas[i])
        {
            mvwprintw(win, 6 + cont, xmax / 4 - 10, "%d nota(s) de B$ %d.00", inteiro / notas[i], notas[i]);
            cont++;
        }
        inteiro %= notas[i];
        // std::cout << std::fixed << inteiro/notas[i] << " nota(s) de R$ " << notas[i] << ".00\n";
    }
    cont = 6 + cont;
    print_centered(win, cont++, "MOEDAS:");
    mvwprintw(win, cont++, xmax / 4 - 10, "%d moeda(s) de B$ 1.00", inteiro);
    for (int i = 0; i < 4; i++)
    {
        if (decimal / moedas[i])
            mvwprintw(win, cont++, xmax / 4 - 10, "%d moeda(s) de B$ %.2f", decimal / moedas[i], (float)moedas[i] / 100);
        decimal %= moedas[i];
    }
    mvwprintw(win, cont++, xmax / 4 - 10, "%d moeda(s) de B$ 0.01", decimal);
    wrefresh(win);
}