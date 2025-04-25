#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define TAMANHO 8

// Cores para as peças
#define VERMELHO "\x1b[31m"
#define VERDE "\x1b[32m"
#define AZUL "\x1b[34m"
#define RESET "\x1b[0m"

typedef struct {
    char tipo;
    char cor;
} Peca;

Peca tabuleiro[TAMANHO][TAMANHO];

void inicializarTabuleiro() {
    // Inicializa o tabuleiro vazio
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            tabuleiro[i][j].tipo = ' ';
            tabuleiro[i][j].cor = ' ';
        }
    }

    // Peças brancas
    tabuleiro[0][0] = (Peca){'T', 'B'};
    tabuleiro[0][1] = (Peca){'C', 'B'};
    tabuleiro[0][2] = (Peca){'B', 'B'};
    tabuleiro[0][3] = (Peca){'D', 'B'};
    tabuleiro[0][4] = (Peca){'R', 'B'};
    tabuleiro[0][5] = (Peca){'B', 'B'};
    tabuleiro[0][6] = (Peca){'C', 'B'};
    tabuleiro[0][7] = (Peca){'T', 'B'};
    for (int i = 0; i < TAMANHO; i++) {
        tabuleiro[1][i] = (Peca){'P', 'B'};
    }

    // Peças pretas
    tabuleiro[7][0] = (Peca){'T', 'P'};
    tabuleiro[7][1] = (Peca){'C', 'P'};
    tabuleiro[7][2] = (Peca){'B', 'P'};
    tabuleiro[7][3] = (Peca){'D', 'P'};
    tabuleiro[7][4] = (Peca){'R', 'P'};
    tabuleiro[7][5] = (Peca){'B', 'P'};
    tabuleiro[7][6] = (Peca){'C', 'P'};
    tabuleiro[7][7] = (Peca){'T', 'P'};
    for (int i = 0; i < TAMANHO; i++) {
        tabuleiro[6][i] = (Peca){'P', 'P'};
    }
}

void exibirTabuleiro() {
    printf("\n   A B C D E F G H\n");
    for (int i = 0; i < TAMANHO; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < TAMANHO; j++) {
            if ((i + j) % 2 == 0) {
                printf("\x1b[47m"); // Fundo branco
            } else {
                printf("\x1b[40m"); // Fundo preto
            }

            if (tabuleiro[i][j].cor == 'B') {
                printf(VERDE);
            } else if (tabuleiro[i][j].cor == 'P') {
                printf(VERMELHO);
            }

            printf(" %c" RESET, tabuleiro[i][j].tipo);
        }
        printf(" %d\n", 8 - i);
    }
    printf("   A B C D E F G H\n\n");
}

bool posicaoValida(int x, int y) {
    return x >= 0 && x < TAMANHO && y >= 0 && y < TAMANHO;
}

bool moverTorre(int x1, int y1, int x2, int y2) {
    // Verifica se o movimento é na mesma linha ou coluna
    if (x1 != x2 && y1 != y2) return false;

    // Verifica se há peças no caminho
    if (x1 == x2) {
        int passo = (y2 > y1) ? 1 : -1;
        for (int y = y1 + passo; y != y2; y += passo) {
            if (tabuleiro[x1][y].tipo != ' ') return false;
        }
    } else {
        int passo = (x2 > x1) ? 1 : -1;
        for (int x = x1 + passo; x != x2; x += passo) {
            if (tabuleiro[x][y1].tipo != ' ') return false;
        }
    }

    // Verifica se a posição final é válida (não tem peça da mesma cor)
    if (tabuleiro[x2][y2].cor == tabuleiro[x1][y1].cor) return false;

    return true;
}

bool moverBispo(int x1, int y1, int x2, int y2) {
    // Verifica se o movimento é diagonal
    if (abs(x2 - x1) != abs(y2 - y1)) return false;

    // Verifica se há peças no caminho
    int passoX = (x2 > x1) ? 1 : -1;
    int passoY = (y2 > y1) ? 1 : -1;
    int x = x1 + passoX;
    int y = y1 + passoY;

    while (x != x2 && y != y2) {
        if (tabuleiro[x][y].tipo != ' ') return false;
        x += passoX;
        y += passoY;
    }

    // Verifica se a posição final é válida
    if (tabuleiro[x2][y2].cor == tabuleiro[x1][y1].cor) return false;

    return true;
}

bool moverRainha(int x1, int y1, int x2, int y2) {
    // A rainha combina os movimentos da torre e do bispo
    return moverTorre(x1, y1, x2, y2) || moverBispo(x1, y1, x2, y2);
}

bool moverCavalo(int x1, int y1, int x2, int y2) {
    // Verifica o movimento em L
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;

    // Verifica se a posição final é válida
    if (tabuleiro[x2][y2].cor == tabuleiro[x1][y1].cor) return false;

    return true;
}

bool moverRei(int x1, int y1, int x2, int y2) {
    // Verifica movimento de uma casa em qualquer direção
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    if (dx > 1 || dy > 1) return false;

    // Verifica se a posição final é válida
    if (tabuleiro[x2][y2].cor == tabuleiro[x1][y1].cor) return false;

    return true;
}

bool moverPeao(int x1, int y1, int x2, int y2) {
    int direcao = (tabuleiro[x1][y1].cor == 'B') ? 1 : -1;
    int dx = x2 - x1;
    int dy = y2 - y1;

    // Movimento para frente
    if (dy == 0) {
        // Movimento simples
        if (dx == direcao && tabuleiro[x2][y2].tipo == ' ') return true;
        // Movimento duplo no primeiro movimento
        if (dx == 2 * direcao && x1 == (tabuleiro[x1][y1].cor == 'B' ? 1 : 6) && 
            tabuleiro[x2][y2].tipo == ' ' && tabuleiro[x1 + direcao][y1].tipo == ' ') return true;
    } 
    // Captura diagonal
    else if (abs(dy) == 1 && dx == direcao && 
             tabuleiro[x2][y2].tipo != ' ' && tabuleiro[x2][y2].cor != tabuleiro[x1][y1].cor) {
        return true;
    }

    return false;
}

void converterPosicao(char *entrada, int *x, int *y) {
    if (entrada[0] >= 'a' && entrada[0] <= 'h') {
        *y = entrada[0] - 'a';
    } else if (entrada[0] >= 'A' && entrada[0] <= 'H') {
        *y = entrada[0] - 'A';
    } else {
        *y = -1;
    }

    if (entrada[1] >= '1' && entrada[1] <= '8') {
        *x = 8 - (entrada[1] - '0');
    } else {
        *x = -1;
    }
}

void realizarMovimento(int x1, int y1, int x2, int y2) {
    tabuleiro[x2][y2] = tabuleiro[x1][y1];
    tabuleiro[x1][y1].tipo = ' ';
    tabuleiro[x1][y1].cor = ' ';
}

void simularMovimento(char tipo, char cor) {
    char entrada[3];
    int x1, y1, x2, y2;
    bool movimentoValido = false;

    do {
        exibirTabuleiro();
        printf("Movendo %s%c" RESET ". Digite a posição de origem (ex: A2): ", 
               cor == 'B' ? VERDE : VERMELHO, tipo);
        scanf("%2s", entrada);
        converterPosicao(entrada, &x1, &y1);

        if (!posicaoValida(x1, y1) || tabuleiro[x1][y1].tipo != tipo || tabuleiro[x1][y1].cor != cor) {
            printf("Posição inválida ou peça incorreta. Tente novamente.\n");
            continue;
        }

        printf("Digite a posição de destino (ex: A4): ");
        scanf("%2s", entrada);
        converterPosicao(entrada, &x2, &y2);

        if (!posicaoValida(x2, y2)) {
            printf("Posição inválida. Tente novamente.\n");
            continue;
        }

        switch (tipo) {
            case 'T': movimentoValido = moverTorre(x1, y1, x2, y2); break;
            case 'B': movimentoValido = moverBispo(x1, y1, x2, y2); break;
            case 'D': movimentoValido = moverRainha(x1, y1, x2, y2); break;
            case 'C': movimentoValido = moverCavalo(x1, y1, x2, y2); break;
            case 'R': movimentoValido = moverRei(x1, y1, x2, y2); break;
            case 'P': movimentoValido = moverPeao(x1, y1, x2, y2); break;
        }

        if (movimentoValido) {
            realizarMovimento(x1, y1, x2, y2);
            printf("Movimento realizado com sucesso!\n");
        } else {
            printf("Movimento inválido para esta peça. Tente novamente.\n");
        }
    } while (!movimentoValido);
}

void menuPrincipal() {
    int opcao;
    do {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Mover Torre (Branca)\n");
        printf("2. Mover Bispo (Branco)\n");
        printf("3. Mover Rainha (Branca)\n");
        printf("4. Mover Cavalo (Branco)\n");
        printf("5. Mover Peão (Branco)\n");
        printf("6. Mover Rei (Branco)\n");
        printf("7. Mover Torre (Preta)\n");
        printf("8. Mover Bispo (Preto)\n");
        printf("9. Mover Rainha (Preta)\n");
        printf("10. Mover Cavalo (Preto)\n");
        printf("11. Mover Peão (Preto)\n");
        printf("12. Mover Rei (Preto)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: simularMovimento('T', 'B'); break;
            case 2: simularMovimento('B', 'B'); break;
            case 3: simularMovimento('D', 'B'); break;
            case 4: simularMovimento('C', 'B'); break;
            case 5: simularMovimento('P', 'B'); break;
            case 6: simularMovimento('R', 'B'); break;
            case 7: simularMovimento('T', 'P'); break;
            case 8: simularMovimento('B', 'P'); break;
            case 9: simularMovimento('D', 'P'); break;
            case 10: simularMovimento('C', 'P'); break;
            case 11: simularMovimento('P', 'P'); break;
            case 12: simularMovimento('R', 'P'); break;
            case 0: printf("Encerrando o programa...\n"); break;
            default: printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

int main() {
    inicializarTabuleiro();
    menuPrincipal();
    return 0;
}