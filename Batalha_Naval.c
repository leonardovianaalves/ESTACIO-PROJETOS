#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define TAMANHO 10
#define AGUA '~'
#define NAVIO 'N'
#define ACERTO 'X'
#define ERRO 'O'
#define HABILIDADE '*'

// Cores para o tabuleiro
#define AZUL "\x1b[34m"
#define VERMELHO "\x1b[31m"
#define VERDE "\x1b[32m"
#define AMARELO "\x1b[33m"
#define RESET "\x1b[0m"

typedef struct {
    char tabuleiro[TAMANHO][TAMANHO];
    int navios_restantes;
} Jogador;

void inicializarTabuleiro(Jogador *jogador) {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            jogador->tabuleiro[i][j] = AGUA;
        }
    }
    jogador->navios_restantes = 0;
}

void exibirTabuleiro(Jogador *jogador, bool mostrar_navios) {
    printf("\n  ");
    for (int i = 0; i < TAMANHO; i++) {
        printf(" %c", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < TAMANHO; i++) {
        printf("%2d", i + 1);
        for (int j = 0; j < TAMANHO; j++) {
            char celula = jogador->tabuleiro[i][j];
            if (celula == NAVIO && !mostrar_navios) {
                printf(" %c", AGUA);
            } else if (celula == AGUA) {
                printf(AZUL " %c" RESET, celula);
            } else if (celula == ACERTO) {
                printf(VERMELHO " %c" RESET, celula);
            } else if (celula == ERRO) {
                printf(" %c", celula);
            } else if (celula == HABILIDADE) {
                printf(AMARELO " %c" RESET, celula);
            } else {
                printf(VERDE " %c" RESET, celula);
            }
        }
        printf("\n");
    }
}

bool posicionarNavio(Jogador *jogador, int tamanho, int x, int y, char direcao) {
    // Verifica se a posição é válida
    if (x < 0 || x >= TAMANHO || y < 0 || y >= TAMANHO) {
        return false;
    }

    // Verifica se há espaço para o navio
    if (direcao == 'H') { // Horizontal
        if (y + tamanho > TAMANHO) return false;
        for (int i = 0; i < tamanho; i++) {
            if (jogador->tabuleiro[x][y + i] != AGUA) return false;
        }
    } else { // Vertical
        if (x + tamanho > TAMANHO) return false;
        for (int i = 0; i < tamanho; i++) {
            if (jogador->tabuleiro[x + i][y] != AGUA) return false;
        }
    }

    // Posiciona o navio
    if (direcao == 'H') {
        for (int i = 0; i < tamanho; i++) {
            jogador->tabuleiro[x][y + i] = NAVIO;
        }
    } else {
        for (int i = 0; i < tamanho; i++) {
            jogador->tabuleiro[x + i][y] = NAVIO;
        }
    }

    jogador->navios_restantes += tamanho;
    return true;
}

void posicionarNaviosAutomaticamente(Jogador *jogador) {
    int navios[5] = {5, 4, 3, 3, 2}; // Tamanhos dos navios
    char direcoes[2] = {'H', 'V'};

    for (int i = 0; i < 5; i++) {
        bool posicionado = false;
        while (!posicionado) {
            int x = rand() % TAMANHO;
            int y = rand() % TAMANHO;
            char dir = direcoes[rand() % 2];

            posicionado = posicionarNavio(jogador, navios[i], x, y, dir);
        }
    }
}

bool atacar(Jogador *jogador, int x, int y) {
    if (x < 0 || x >= TAMANHO || y < 0 || y >= TAMANHO) {
        return false;
    }

    if (jogador->tabuleiro[x][y] == NAVIO) {
        jogador->tabuleiro[x][y] = ACERTO;
        jogador->navios_restantes--;
        return true;
    } else if (jogador->tabuleiro[x][y] == AGUA) {
        jogador->tabuleiro[x][y] = ERRO;
    }

    return false;
}

void aplicarHabilidadeCruz(Jogador *jogador, int x, int y, int alcance) {
    for (int i = -alcance; i <= alcance; i++) {
        if (x + i >= 0 && x + i < TAMANHO) {
            if (jogador->tabuleiro[x + i][y] == NAVIO) {
                jogador->tabuleiro[x + i][y] = ACERTO;
                jogador->navios_restantes--;
            } else if (jogador->tabuleiro[x + i][y] == AGUA) {
                jogador->tabuleiro[x + i][y] = HABILIDADE;
            }
        }

        if (y + i >= 0 && y + i < TAMANHO && i != 0) {
            if (jogador->tabuleiro[x][y + i] == NAVIO) {
                jogador->tabuleiro[x][y + i] = ACERTO;
                jogador->navios_restantes--;
            } else if (jogador->tabuleiro[x][y + i] == AGUA) {
                jogador->tabuleiro[x][y + i] = HABILIDADE;
            }
        }
    }
}

void aplicarHabilidadeCone(Jogador *jogador, int x, int y, int direcao, int alcance) {
    // Direções: 0 = cima, 1 = direita, 2 = baixo, 3 = esquerda
    for (int i = 1; i <= alcance; i++) {
        for (int j = -(i - 1); j <= (i - 1); j++) {
            int nx = x, ny = y;

            switch (direcao) {
                case 0: // Cima
                    nx = x - i;
                    ny = y + j;
                    break;
                case 1: // Direita
                    nx = x + j;
                    ny = y + i;
                    break;
                case 2: // Baixo
                    nx = x + i;
                    ny = y + j;
                    break;
                case 3: // Esquerda
                    nx = x + j;
                    ny = y - i;
                    break;
            }

            if (nx >= 0 && nx < TAMANHO && ny >= 0 && ny < TAMANHO) {
                if (jogador->tabuleiro[nx][ny] == NAVIO) {
                    jogador->tabuleiro[nx][ny] = ACERTO;
                    jogador->navios_restantes--;
                } else if (jogador->tabuleiro[nx][ny] == AGUA) {
                    jogador->tabuleiro[nx][ny] = HABILIDADE;
                }
            }
        }
    }
}

void aplicarHabilidadeOctaedro(Jogador *jogador, int x, int y, int alcance) {
    for (int i = -alcance; i <= alcance; i++) {
        for (int j = -alcance; j <= alcance; j++) {
            if (abs(i) + abs(j) <= alcance) {
                int nx = x + i;
                int ny = y + j;

                if (nx >= 0 && nx < TAMANHO && ny >= 0 && ny < TAMANHO) {
                    if (jogador->tabuleiro[nx][ny] == NAVIO) {
                        jogador->tabuleiro[nx][ny] = ACERTO;
                        jogador->navios_restantes--;
                    } else if (jogador->tabuleiro[nx][ny] == AGUA) {
                        jogador->tabuleiro[nx][ny] = HABILIDADE;
                    }
                }
            }
        }
    }
}

void converterPosicao(char *entrada, int *x, int *y) {
    if (entrada[0] >= 'a' && entrada[0] <= 'j') {
        *y = entrada[0] - 'a';
    } else if (entrada[0] >= 'A' && entrada[0] <= 'J') {
        *y = entrada[0] - 'A';
    } else {
        *y = -1;
    }

    if (entrada[1] >= '1' && entrada[1] <= '9') {
        *x = entrada[1] - '1';
    } else if (entrada[1] == '0') {
        *x = 9;
    } else {
        *x = -1;
    }
}

void jogar(Jogador *humano, Jogador *computador) {
    int habilidades = 3;
    char entrada[3];
    int x, y;

    while (humano->navios_restantes > 0 && computador->navios_restantes > 0) {
        // Turno do jogador
        printf("\n=== SEU TURNO ===\n");
        printf("Seu tabuleiro:\n");
        exibirTabuleiro(humano, true);
        printf("\nTabuleiro do computador:\n");
        exibirTabuleiro(computador, false);
        printf("\nNavios restantes: Você %d x %d Computador\n", 
               humano->navios_restantes, computador->navios_restantes);
        printf("Habilidades especiais restantes: %d\n", habilidades);

        int opcao;
        printf("\n1. Atacar\n");
        printf("2. Usar habilidade especial\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("Digite a coordenada para atacar (ex: A1): ");
            scanf("%2s", entrada);
            converterPosicao(entrada, &x, &y);

            if (x == -1 || y == -1) {
                printf("Coordenada inválida!\n");
                continue;
            }

            if (computador->tabuleiro[x][y] == ACERTO || 
                computador->tabuleiro[x][y] == ERRO || 
                computador->tabuleiro[x][y] == HABILIDADE) {
                printf("Você já atacou esta posição!\n");
                continue;
            }

            if (atacar(computador, x, y)) {
                printf(VERMELHO "\nACERTOU!\n" RESET);
            } else {
                printf("\nERROU!\n");
            }
        } else if (opcao == 2 && habilidades > 0) {
            int habilidade;
            printf("\nEscolha a habilidade:\n");
            printf("1. Cruz (alcance 2)\n");
            printf("2. Cone (alcance 3)\n");
            printf("3. Octaedro (alcance 2)\n");
            printf("Escolha: ");
            scanf("%d", &habilidade);

            printf("Digite a coordenada central (ex: B2): ");
            scanf("%2s", entrada);
            converterPosicao(entrada, &x, &y);

            if (x == -1 || y == -1) {
                printf("Coordenada inválida!\n");
                continue;
            }

            switch (habilidade) {
                case 1:
                    aplicarHabilidadeCruz(computador, x, y, 2);
                    break;
                case 2: {
                    int direcao;
                    printf("Escolha a direção (0=Cima, 1=Direita, 2=Baixo, 3=Esquerda): ");
                    scanf("%d", &direcao);
                    aplicarHabilidadeCone(computador, x, y, direcao, 3);
                    break;
                }
                case 3:
                    aplicarHabilidadeOctaedro(computador, x, y, 2);
                    break;
                default:
                    printf("Habilidade inválida!\n");
                    continue;
            }

            habilidades--;
            printf(AMARELO "\nHABILIDADE ESPECIAL UTILIZADA!\n" RESET);
        } else {
            printf("Opção inválida ou sem habilidades disponíveis!\n");
            continue;
        }

        // Turno do computador (ataque simples)
        if (computador->navios_restantes > 0) {
            printf("\n=== TURNO DO COMPUTADOR ===\n");
            do {
                x = rand() % TAMANHO;
                y = rand() % TAMANHO;
            } while (humano->tabuleiro[x][y] == ACERTO || humano->tabuleiro[x][y] == ERRO);

            if (atacar(humano, x, y)) {
                printf("O computador ACERTOU em %c%d!\n", 'A' + y, x + 1);
            } else {
                printf("O computador ERROU em %c%d.\n", 'A' + y, x + 1);
            }
        }
    }

    // Resultado final
    printf("\n=== FIM DE JOGO ===\n");
    printf("Seu tabuleiro:\n");
    exibirTabuleiro(humano, true);
    printf("\nTabuleiro do computador:\n");
    exibirTabuleiro(computador, true);

    if (humano->navios_restantes == 0) {
        printf(VERMELHO "\nVOCÊ PERDEU!\n" RESET);
    } else {
        printf(VERDE "\nVOCÊ GANHOU!\n" RESET);
    }
}

int main() {
    srand(time(NULL));

    Jogador humano, computador;
    inicializarTabuleiro(&humano);
    inicializarTabuleiro(&computador);

    printf("=== BATALHA NAVAL COM HABILIDADES ESPECIAIS ===\n");

    // Posicionamento automático dos navios
    posicionarNaviosAutomaticamente(&humano);
    posicionarNaviosAutomaticamente(&computador);

    jogar(&humano, &computador);

    return 0;
}