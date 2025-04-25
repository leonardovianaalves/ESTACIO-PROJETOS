#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CARTAS 50
#define TAM_NOME 50
#define TAM_PAIS 30

typedef struct {
    char pais[TAM_PAIS];
    char capital[TAM_NOME];
    double area;
    long populacao;
    double pib;
    double idh;
    int pontos_turisticos;
} Carta;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void formatarNome(char *nome) {
    int i;
    int proxima_maiuscula = 1;

    for (i = 0; nome[i]; i++) {
        if (proxima_maiuscula) {
            nome[i] = toupper(nome[i]);
            proxima_maiuscula = 0;
        } else {
            nome[i] = tolower(nome[i]);
        }

        if (nome[i] == ' ') {
            proxima_maiuscula = 1;
        }
    }
}

void carregarCartasPadrao(Carta cartas[], int *total) {
    // Cartas pré-definidas para o jogo
    Carta padroes[] = {
        {"Brasil", "Brasília", 8515767.049, 213317639, 1.445, 0.754, 32},
        {"Estados Unidos", "Washington", 9833517.0, 331002651, 20.94, 0.926, 78},
        {"China", "Pequim", 9596961.0, 1411778724, 14.72, 0.761, 55},
        {"Japão", "Tóquio", 377975.0, 126476461, 5.065, 0.919, 42},
        {"Alemanha", "Berlim", 357022.0, 83149300, 3.806, 0.947, 38},
        {"Índia", "Nova Delhi", 3287263.0, 1380004385, 2.623, 0.645, 29},
        {"Rússia", "Moscou", 17098242.0, 146748590, 1.483, 0.824, 27},
        {"França", "Paris", 643801.0, 67391582, 2.716, 0.901, 47},
        {"Reino Unido", "Londres", 242495.0, 67215293, 2.638, 0.932, 51},
        {"Itália", "Roma", 301340.0, 60359546, 1.989, 0.895, 53}
    };

    int qtd_padroes = sizeof(padroes) / sizeof(padroes[0]);
    
    for (int i = 0; i < qtd_padroes; i++) {
        cartas[(*total)++] = padroes[i];
    }
}

void exibirCarta(Carta carta) {
    printf("\n=== Carta ===\n");
    printf("País: %s\n", carta.pais);
    printf("Capital: %s\n", carta.capital);
    printf("Área: %.2f km²\n", carta.area);
    printf("População: %ld habitantes\n", carta.populacao);
    printf("PIB: US$ %.3f trilhões\n", carta.pib);
    printf("IDH: %.3f\n", carta.idh);
    printf("Pontos Turísticos: %d\n", carta.pontos_turisticos);
}

void compararAtributo(Carta carta1, Carta carta2, int atributo) {
    char *nomesAtributos[] = {
        "Área",
        "População",
        "PIB",
        "IDH",
        "Pontos Turísticos"
    };

    printf("\nComparando %s:\n", nomesAtributos[atributo - 1]);

    double valor1, valor2;
    char *resultado;

    switch (atributo) {
        case 1: // Área
            valor1 = carta1.area;
            valor2 = carta2.area;
            break;
        case 2: // População
            valor1 = carta1.populacao;
            valor2 = carta2.populacao;
            break;
        case 3: // PIB
            valor1 = carta1.pib;
            valor2 = carta2.pib;
            break;
        case 4: // IDH
            valor1 = carta1.idh;
            valor2 = carta2.idh;
            break;
        case 5: // Pontos Turísticos
            valor1 = carta1.pontos_turisticos;
            valor2 = carta2.pontos_turisticos;
            break;
    }

    // Operador ternário para determinar o resultado
    resultado = (valor1 > valor2) ? "VENCE" : 
                (valor1 < valor2) ? "PERDE" : "EMPATE";

    printf("%s: %.2f\n", carta1.pais, valor1);
    printf("%s: %.2f\n", carta2.pais, valor2);
    printf("Resultado: %s %s!\n", carta1.pais, resultado);
}

void compararDoisAtributos(Carta carta1, Carta carta2, int atributo1, int atributo2) {
    printf("\nComparação Combinada:\n");
    
    // Comparar primeiro atributo
    compararAtributo(carta1, carta2, atributo1);
    
    // Se empate no primeiro, comparar o segundo
    double valor1_1, valor1_2, valor2_1, valor2_2;
    
    // Obter valores do primeiro atributo
    switch (atributo1) {
        case 1: valor1_1 = carta1.area; valor2_1 = carta2.area; break;
        case 2: valor1_1 = carta1.populacao; valor2_1 = carta2.populacao; break;
        case 3: valor1_1 = carta1.pib; valor2_1 = carta2.pib; break;
        case 4: valor1_1 = carta1.idh; valor2_1 = carta2.idh; break;
        case 5: valor1_1 = carta1.pontos_turisticos; valor2_1 = carta2.pontos_turisticos; break;
    }
    
    // Obter valores do segundo atributo
    switch (atributo2) {
        case 1: valor1_2 = carta1.area; valor2_2 = carta2.area; break;
        case 2: valor1_2 = carta1.populacao; valor2_2 = carta2.populacao; break;
        case 3: valor1_2 = carta1.pib; valor2_2 = carta2.pib; break;
        case 4: valor1_2 = carta1.idh; valor2_2 = carta2.idh; break;
        case 5: valor1_2 = carta1.pontos_turisticos; valor2_2 = carta2.pontos_turisticos; break;
    }
    
    // Estrutura de decisão aninhada para determinar o vencedor
    if (valor1_1 > valor2_1) {
        printf("%s vence no critério principal!\n", carta1.pais);
    } else if (valor1_1 < valor2_1) {
        printf("%s vence no critério principal!\n", carta2.pais);
    } else {
        printf("Empate no primeiro critério. Desempate pelo segundo...\n");
        
        if (valor1_2 > valor2_2) {
            printf("%s vence no critério secundário!\n", carta1.pais);
        } else if (valor1_2 < valor2_2) {
            printf("%s vence no critério secundário!\n", carta2.pais);
        } else {
            printf("Empate total! As cartas são muito parecidas.\n");
        }
    }
}

void jogarSuperTrunfo(Carta cartas[], int total) {
    if (total < 2) {
        printf("É necessário ter pelo menos 2 cartas para jogar.\n");
        return;
    }

    srand(time(NULL));
    int idx1 = rand() % total;
    int idx2;
    
    do {
        idx2 = rand() % total;
    } while (idx2 == idx1);

    Carta cartaJogador = cartas[idx1];
    Carta cartaComputador = cartas[idx2];

    printf("\n=== SUA CARTA ===\n");
    exibirCarta(cartaJogador);

    int opcao;
    do {
        printf("\nEscolha como comparar:\n");
        printf("1. Por Área\n");
        printf("2. Por População\n");
        printf("3. Por PIB\n");
        printf("4. Por IDH\n");
        printf("5. Por Pontos Turísticos\n");
        printf("6. Comparar por dois atributos (desempate)\n");
        printf("0. Voltar\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao >= 1 && opcao <= 5) {
            printf("\n=== CARTA DO COMPUTADOR ===\n");
            exibirCarta(cartaComputador);
            compararAtributo(cartaJogador, cartaComputador, opcao);
        } else if (opcao == 6) {
            int atributo1, atributo2;
            printf("\nEscolha o primeiro atributo (1-5): ");
            scanf("%d", &atributo1);
            limparBuffer();
            
            printf("Escolha o segundo atributo (1-5, diferente do primeiro): ");
            scanf("%d", &atributo2);
            limparBuffer();
            
            if (atributo1 < 1 || atributo1 > 5 || atributo2 < 1 || atributo2 > 5 || atributo1 == atributo2) {
                printf("Atributos inválidos!\n");
            } else {
                printf("\n=== CARTA DO COMPUTADOR ===\n");
                exibirCarta(cartaComputador);
                compararDoisAtributos(cartaJogador, cartaComputador, atributo1, atributo2);
            }
        } else if (opcao != 0) {
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void adicionarCarta(Carta cartas[], int *total) {
    if (*total >= MAX_CARTAS) {
        printf("Limite máximo de cartas atingido!\n");
        return;
    }

    Carta nova;

    printf("\n--- Adicionar Nova Carta ---\n");

    // País
    printf("País: ");
    fgets(nova.pais, TAM_PAIS, stdin);
    nova.pais[strcspn(nova.pais, "\n")] = '\0';
    formatarNome(nova.pais);

    // Capital
    printf("Capital: ");
    fgets(nova.capital, TAM_NOME, stdin);
    nova.capital[strcspn(nova.capital, "\n")] = '\0';
    formatarNome(nova.capital);

    // Área
    printf("Área (km²): ");
    scanf("%lf", &nova.area);
    limparBuffer();

    // População
    printf("População: ");
    scanf("%ld", &nova.populacao);
    limparBuffer();

    // PIB
    printf("PIB (trilhões de US$): ");
    scanf("%lf", &nova.pib);
    limparBuffer();

    // IDH
    printf("IDH (0-1): ");
    scanf("%lf", &nova.idh);
    limparBuffer();

    // Pontos turísticos
    printf("Pontos turísticos: ");
    scanf("%d", &nova.pontos_turisticos);
    limparBuffer();

    cartas[(*total)++] = nova;
    printf("\nCarta adicionada com sucesso!\n");
}

void listarCartas(Carta cartas[], int total) {
    if (total == 0) {
        printf("\nNenhuma carta cadastrada ainda.\n");
        return;
    }

    printf("\n--- Lista de Cartas ---\n");
    for (int i = 0; i < total; i++) {
        printf("\nCarta #%d\n", i + 1);
        printf("País: %s\n", cartas[i].pais);
        printf("Capital: %s\n", cartas[i].capital);
        printf("Área: %.2f km²\n", cartas[i].area);
        printf("População: %ld\n", cartas[i].populacao);
        printf("PIB: %.3f trilhões\n", cartas[i].pib);
        printf("IDH: %.3f\n", cartas[i].idh);
        printf("Pontos Turísticos: %d\n", cartas[i].pontos_turisticos);
    }
}

int main() {
    Carta cartas[MAX_CARTAS];
    int total = 0;
    int opcao;

    // Carregar algumas cartas padrão
    carregarCartasPadrao(cartas, &total);

    printf("=== SUPER TRUNFO DE PAÍSES ===\n");

    do {
        printf("\nMenu Principal:\n");
        printf("1. Jogar Super Trunfo\n");
        printf("2. Adicionar nova carta\n");
        printf("3. Listar todas as cartas\n");
        printf("4. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                jogarSuperTrunfo(cartas, total);
                break;
            case 2:
                adicionarCarta(cartas, &total);
                break;
            case 3:
                listarCartas(cartas, total);
                break;
            case 4:
                printf("\nObrigado por jogar Super Trunfo!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}