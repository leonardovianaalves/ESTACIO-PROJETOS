#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_CIDADES 100
#define TAM_NOME 50
#define TAM_ESTADO 3

typedef struct {
    char estado[TAM_ESTADO];
    int codigo;
    char nome[TAM_NOME];
    long populacao;
    double pib;
    double area;
    int pontos_turisticos;
    double densidade;
    double pib_per_capita;
} Cidade;

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

void cadastrarCidade(Cidade cidades[], int *total) {
    if (*total >= MAX_CIDADES) {
        printf("Limite máximo de cidades atingido!\n");
        return;
    }

    Cidade nova;

    printf("\n--- Cadastro de Nova Cidade ---\n");

    // Estado (sigla)
    printf("Estado (sigla, 2 letras): ");
    fgets(nova.estado, TAM_ESTADO, stdin);
    nova.estado[strcspn(nova.estado, "\n")] = '\0';
    limparBuffer();

    // Converter sigla do estado para maiúsculas
    for (int i = 0; nova.estado[i]; i++) {
        nova.estado[i] = toupper(nova.estado[i]);
    }

    // Código
    printf("Código da cidade: ");
    scanf("%d", &nova.codigo);
    limparBuffer();

    // Nome da cidade
    printf("Nome da cidade: ");
    fgets(nova.nome, TAM_NOME, stdin);
    nova.nome[strcspn(nova.nome, "\n")] = '\0';
    formatarNome(nova.nome);

    // População
    printf("População: ");
    scanf("%ld", &nova.populacao);
    limparBuffer();

    // PIB
    printf("PIB (em milhões): ");
    scanf("%lf", &nova.pib);
    limparBuffer();

    // Área
    printf("Área (em km²): ");
    scanf("%lf", &nova.area);
    limparBuffer();

    // Pontos turísticos
    printf("Número de pontos turísticos: ");
    scanf("%d", &nova.pontos_turisticos);
    limparBuffer();

    // Calcular propriedades derivadas
    nova.densidade = (nova.area > 0) ? nova.populacao / nova.area : 0;
    nova.pib_per_capita = (nova.populacao > 0) ? (nova.pib * 1000000) / nova.populacao : 0;

    cidades[(*total)++] = nova;
    printf("\nCidade cadastrada com sucesso!\n");
}

void listarCidades(Cidade cidades[], int total) {
    if (total == 0) {
        printf("\nNenhuma cidade cadastrada ainda.\n");
        return;
    }

    printf("\n--- Lista de Cidades Cadastradas ---\n");
    for (int i = 0; i < total; i++) {
        printf("\nCidade #%d\n", i + 1);
        printf("Estado: %s\n", cidades[i].estado);
        printf("Código: %d\n", cidades[i].codigo);
        printf("Nome: %s\n", cidades[i].nome);
        printf("População: %ld habitantes\n", cidades[i].populacao);
        printf("PIB: %.2f milhões\n", cidades[i].pib);
        printf("Área: %.2f km²\n", cidades[i].area);
        printf("Pontos Turísticos: %d\n", cidades[i].pontos_turisticos);
        printf("Densidade Populacional: %.2f hab/km²\n", cidades[i].densidade);
        printf("PIB per capita: %.2f\n", cidades[i].pib_per_capita);
    }
}

void pesquisarCidade(Cidade cidades[], int total) {
    if (total == 0) {
        printf("\nNenhuma cidade cadastrada ainda.\n");
        return;
    }

    int opcao;
    printf("\n--- Pesquisar Cidade ---\n");
    printf("1. Por código\n");
    printf("2. Por nome\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        int codigo;
        printf("Digite o código da cidade: ");
        scanf("%d", &codigo);
        limparBuffer();

        for (int i = 0; i < total; i++) {
            if (cidades[i].codigo == codigo) {
                printf("\nCidade encontrada:\n");
                printf("Estado: %s\n", cidades[i].estado);
                printf("Código: %d\n", cidades[i].codigo);
                printf("Nome: %s\n", cidades[i].nome);
                printf("População: %ld habitantes\n", cidades[i].populacao);
                printf("PIB: %.2f milhões\n", cidades[i].pib);
                printf("Área: %.2f km²\n", cidades[i].area);
                printf("Pontos Turísticos: %d\n", cidades[i].pontos_turisticos);
                printf("Densidade Populacional: %.2f hab/km²\n", cidades[i].densidade);
                printf("PIB per capita: %.2f\n", cidades[i].pib_per_capita);
                return;
            }
        }
        printf("\nCidade com código %d não encontrada.\n", codigo);
    } else if (opcao == 2) {
        char nome[TAM_NOME];
        printf("Digite o nome da cidade: ");
        fgets(nome, TAM_NOME, stdin);
        nome[strcspn(nome, "\n")] = '\0';
        formatarNome(nome);

        int encontradas = 0;
        for (int i = 0; i < total; i++) {
            if (strcmp(cidades[i].nome, nome) == 0) {
                printf("\nCidade encontrada:\n");
                printf("Estado: %s\n", cidades[i].estado);
                printf("Código: %d\n", cidades[i].codigo);
                printf("Nome: %s\n", cidades[i].nome);
                printf("População: %ld habitantes\n", cidades[i].populacao);
                printf("PIB: %.2f milhões\n", cidades[i].pib);
                printf("Área: %.2f km²\n", cidades[i].area);
                printf("Pontos Turísticos: %d\n", cidades[i].pontos_turisticos);
                printf("Densidade Populacional: %.2f hab/km²\n", cidades[i].densidade);
                printf("PIB per capita: %.2f\n", cidades[i].pib_per_capita);
                encontradas++;
            }
        }

        if (encontradas == 0) {
            printf("\nNenhuma cidade com nome '%s' encontrada.\n", nome);
        } else {
            printf("\nTotal de cidades encontradas: %d\n", encontradas);
        }
    } else {
        printf("\nOpção inválida.\n");
    }
}

int main() {
    Cidade cidades[MAX_CIDADES];
    int total = 0;
    int opcao;

    printf("=== Sistema de Cadastro de Cartas - Super Trunfo de Países ===\n");

    do {
        printf("\nMenu Principal:\n");
        printf("1. Cadastrar nova cidade\n");
        printf("2. Listar todas as cidades\n");
        printf("3. Pesquisar cidade\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarCidade(cidades, &total);
                break;
            case 2:
                listarCidades(cidades, total);
                break;
            case 3:
                pesquisarCidade(cidades, total);
                break;
            case 4:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}