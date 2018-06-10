#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<unistd.h>
#include<time.h>

#define MAX 20 /*para fazer as linhas e colunas da matriz*/

void imprime(char matriz[MAX][MAX+1]); /*funcao usada para imprimir o labirinto*/
void anda(char matriz[MAX][MAX+1],int l,int c, int pontos[1]); /*funcao usada para andar no labirinto*/
void bomba(char matriz[MAX][MAX+1], int contador); /*funcao que coloca as bombas nos locais*/
void jogonov(char matriz[MAX][MAX+1]); /*funcao quando o cara pisa na bomba*/
void terminou(char matriz[MAX][MAX+1], int pontos[1]); /*pra quando o jogador ganhar*/
void imprimedps(char matriz[MAX][MAX+1]);

int main(void)
{
    int i,j,c=1,l=1,a,b,y,x;
    int pontos[1] = {0};
    char z;
    srand((unsigned)time(NULL));

    /*char matriz[MAX][MAX+1] = { "##########",
      "#x       #",
      "# # # ## #",
      "# # # ## #",
      "# # # ####",
      "# ### #C #",
      "#  #  ## #",
      "# ### ## #",
      "#   #    #",
      "##########" };*/

    char matriz[MAX][MAX+1] = { "####################",
        "#x                 #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                  #",
        "#                 C#",
        "####################" };




    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr,TRUE);
    getmaxyx(stdscr,y,x); /*ainda com duvida sobre como usar isso*/

    init_pair(1,COLOR_WHITE,COLOR_BLUE);
    init_pair(2,COLOR_RED,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);

    matriz[10][1] = 'b';   /*esses dois servem para completar as bombas, pois essa regiao do mapa sempre ficava sem bombas*/
    matriz[18][2] = 'b';
    matriz[5][18] = 'b';

    bomba(matriz,0);
    attron(COLOR_PAIR(1) | A_UNDERLINE);

    mvprintw(18,70,"BEM-VINDO AO JOGO!");
    refresh();
    mvprintw(20,70,"1 -> JOGAR");
    mvchgat(20,70,1,A_BLINK,4,NULL); /*para deixar o "1" com outra cor*/

    mvprintw(22,70,"2 -> ARREGAR");
    mvchgat(22,70,1,A_BLINK,4,NULL); /*para deixar o "2" com outra cor*/
    z = getch();

    if(z == '2')
    {
        endwin();
        getch();
    }
    else
    {
        if(z == '1')
        {
            clear();
            anda(matriz,1,1,pontos);
            endwin();
        }
        else
        {
            refresh();
            main();
        }
    }
    return EXIT_SUCCESS;
}
void bomba(char matriz[MAX][MAX+1],int contador)
{
    int i,a,b;
    if(contador == 17)
        return;
    else
    {
        a = rand()%15+2;
        b = rand()%15+2;

        if(a != 18 && b != 18)
        {
            matriz[a][b] = 'b';
            contador++;
            bomba(matriz,contador++);
        }
        else
        {
            bomba(matriz,contador);

        }
    }
}
void imprime(char matriz[MAX][MAX+1]) /*DUVIDA:COMO FAZER ESSE LAB FICAR MAIOR NA TELA SEM PRECISAR AUMENTAR A MATRIZ <-<-<-<-<-<-<-<-<*/
{
    int z,i,j;

    for(i=0; i<MAX; i++) /*esse 10,20 e 70,81 são uma tentativa de ficar no "meio" da tela sem usar o getmaxyx*/
    {
        for(j = 0;j < MAX+1;j++)
        {
            if(matriz[i][j] == 'x') /*esse if so serve para que o x fique com uma cor diferente */
            {
                attron(COLOR_PAIR(3));
                mvprintw(i, j, "%c", matriz[i][j]);  /*[i-10]  pois na matriz, ele irá chamar, [0][1][2]... e assim vai*/
            }
            else
            {
                if(matriz[i][j] == 'b')
                {
                    matriz[i][j] = ' ';  /*pra deixar o b invisivel*/
                    mvprintw(i,j,"%c",matriz[i][j]);
                    matriz[i][j] = 'b'; /*olha ele aq*/
                }
                else
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(i, j, "%c",matriz[i][j]);
                }

            }
        }
    }
    return;
}
void anda(char matriz[MAX][MAX+1],int l, int c,int pontos[1])
{
    int z,i,aux = 1;
    char r;
    attroff(A_UNDERLINE);
    attron(COLOR_PAIR(2));
    clear();
    matriz[18][18] = 'C';
    imprime(matriz);
    while(aux == 1)       /*while(read!='q')*/
    {
        if(l == 18 && c == 18) /*condicao de vitoria*/
        {
            aux = 0;
            break;
        }
        else
        {
            imprime(matriz);
            mvprintw(2,52,"USE AS SETAS PARA SE MOVER E TENTE CHEGAR AO PONTO C");
            mvprintw(7,69,"CUIDADO COM AS BOMBAS!");
            z = getch();

            switch(z)
            {
                case KEY_DOWN:    /*setinha pra baixo*/
                    if(matriz[l+1][c] == '#') /* @-> caso tenha uma parede nao faca nada e fique no mesmo lugar*/
                    {
                        refresh();
                        break;
                    }
                    else
                    {

                        if(matriz[l+1][c] == 'b')
                        {
                            jogonov(matriz);

                        }
                        else
                        {
                            matriz[l+1][c] = 'x';
                            matriz[l][c] = ' ';
                            l = l+1; /*pro valor da variavel l mudar ao ser alterada*/
                            refresh();
                            break;
                        }
                    }
                case KEY_UP: /*setinha pra cima*/
                    if(matriz[l-1][c] == '#') /* @*/
                    {
                        refresh();
                        break;
                    }
                    else
                    {

                        if(matriz[l-1][c] == 'b')
                        {
                            jogonov(matriz);
                        }
                        else
                        {
                            matriz[l-1][c] = 'x';
                            matriz[l][c] = ' ';
                            l = l -1;
                            refresh();
                            break;
                        }
                    }
                case KEY_LEFT: /*setinha pra esquerda*/
                    if(matriz[l][c-1] == '#') /*@*/
                    {
                        refresh();
                        break;
                    }
                    else
                    {

                        if(matriz[l][c-1] == 'b')
                        {
                            jogonov(matriz);

                        }
                        else
                        {
                            matriz[l][c-1] = 'x';
                            matriz[l][c] = ' ';
                            c = c -1;
                            refresh();
                            break;
                        }
                    }
                case KEY_RIGHT: /*setinha pra diretira*/
                    if(matriz[l][c+1] == '#') /*@*/
                    {
                        refresh();
                        break;
                    }
                    else
                    {
                        if(matriz[l][c+1] == 'b')
                        {
                            jogonov(matriz);
                        }
                        else
                        {
                            matriz[l][c+1] = 'x';
                            matriz[l][c] = ' ';
                            c = c + 1;
                            refresh();
                            break;
                        }
                    }
            }
        }
    }
    terminou(matriz,pontos);
    return;
}
void terminou(char matriz[MAX][MAX+1], int pontos[1])
{
    char r;

    clear(); /*limpando a tela*/

    pontos[0] = pontos[0] + 1; /*USA CONTADOR MESMO!MUDAR DEPOIS!*/

    mvprintw(20,57,"SUA PONTUACAO:  %d",pontos[0]);

    mvprintw(22,57,"TERMINOU!PARABENS!VOCE EH MTO BOM!");
    mvchgat(22,57,8,A_BLINK,1,NULL);

    mvprintw(24,57,"S PARA RECOMECAR");
    mvchgat(24,57,1,A_BLINK,1,NULL);

    mvprintw(26,57,"N PARA SAIR");
    mvchgat(26,57,1,A_BLINK,1,NULL);

    r = getch();
    if(r == 's')
    {
        clear();
        refresh();
        anda(matriz,1,1,pontos);
    }
    else
    {
        if(r == 'n')
        {
            endwin();
        }
        else
        {
            refresh();
            terminou(matriz,pontos);
        }
    }


}
void jogonov(char matriz[MAX][MAX+1])
{
    char r;
    clear();
    attroff(A_UNDERLINE);
    imprimedps(matriz);

    attron(COLOR_PAIR(1) | A_UNDERLINE);
    mvprintw(23,55,"BOOOOOOOOOOOOOMMMMMMM!");

    mvprintw(25,55,"VOCE PERDEU!!!VOCE EH RUIM DMS!!MELHOR SAIR DO JOGO!!LAMENTAVEL!");
    mvprintw(27,55,"S -> JOGAR NOVAMENTE");
    mvchgat(27,55,1,A_BLINK,1,NULL);

    mvprintw(29,55,"N -> SAIR (SAI LOGO FDM!)");
    mvchgat(29,55,1,A_BLINK,1,NULL);

    r = getch();
    if(r == 's')
    {
        clear();
        refresh();
        main();
    }
    else
    {
        if(r == 'n')
        {
            endwin();
            getch();
            return;
        }
        else
        {
            refresh();
            jogonov(matriz);
        }
    }

}
void imprimedps(char matriz[MAX][MAX+1]) /*DUVIDA:COMO FAZER ESSE LAB FICAR MAIOR NA TELA SEM PRECISAR AUMENTAR A MATRIZ <-<-<-<-<-<-<-<-<*/
{
    int z,i,j;

    for(i=0; i<MAX; i++) /*esse 10,20 e 70,81 são uma tentativa de ficar no "meio" da tela sem usar o getmaxyx*/
    {
        for(j = 0;j < MAX+1;j++)
        {
            if(matriz[i][j] == 'x') /*esse if so serve para que o x fique com uma cor diferente */
            {
                attron(COLOR_PAIR(2));
                mvprintw(i, j, "%c", matriz[i][j]); /*[i - 10] pois na matriz, ele irá chamar, [0][1][2]... e assim vai*/
            }
            else
            {
                if(matriz[i][j] == 'b')
                {
                  //matriz[i-10][j-70] = ' ';  /*pra deixar o b invisivel*/
                    mvprintw(i,j,"%c",matriz[i][j]);
                    matriz[i][j] = 'b'; /*olha ele aq*/
                }
                else
                {
                    attron(COLOR_PAIR(2));
                    mvprintw(i, j, "%c",matriz[i][j]);
                }
            }
        }
    }
    return;
}
