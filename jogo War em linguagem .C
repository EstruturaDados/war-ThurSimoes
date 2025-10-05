

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Estrutura dos Territórios ---
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// --- Banco de Missões Estratégicas ---
const char* missoes[] = {
    "Conquistar 3 territorios.",
    "Eliminar o exercito Vermelho.",
    "Controlar o territorio 'Brasil'.",
    "Acumular 12 tropas no total.",
    "Possuir um territorio com 7 ou mais tropas."
};
const int TOTAL_MISSOES = 5;

// --- Protótipos das Funções Modulares ---
void cadastrarTerritorios(struct Territorio *mapa, int tam);
void exibirMapa(const struct Territorio *mapa, int tam);
void atacar(struct Territorio *atacante, struct Territorio *defensor);
void atribuirMissao(char *destino, const char *todasMissoes[], int totalMissoes);
void exibirMissao(const char *corJogador, const char *missao);
int verificarMissao(const char *missao, const struct Territorio *mapa, int tam, const char *corJogador);
void liberarMemoria(struct Territorio *mapa, char *missoesJogadores[], int numJogadores);

int main() {
    srand(time(NULL));

    // --- Configuração dos Jogadores ---
    const int NUM_JOGADORES = 2;
    const char* CORES_JOGADORES[] = {"Azul", "Verde"};
    char *missoesJogadores[NUM_JOGADORES];

    int numTerritorios;
    printf("=============================================\n");
    printf("    WAR ESTRUTURADO - O DESAFIO FINAL\n");
    printf("=============================================\n\n");

    // --- Alocação de Memória para Territórios ---
    printf("Digite o numero total de territorios no mapa: ");
    scanf("%d", &numTerritorios);
    struct Territorio *mapaMundi = (struct Territorio*) calloc(numTerritorios, sizeof(struct Territorio));
    if (mapaMundi == NULL) {
        printf("Erro: Falha ao alocar memoria para o mapa.\n");
        return 1;
    }

    // --- Atribuição e Exibição das Missões ---
    printf("\n--- SORTEANDO MISSOES ESTRATEGICAS ---\n");
    for (int i = 0; i < NUM_JOGADORES; i++) {
        // Aloca memória para a string da missão de cada jogador
        missoesJogadores[i] = (char*) malloc(100 * sizeof(char));
        if (missoesJogadores[i] == NULL) {
            printf("Erro: Falha ao alocar memoria para missao.\n");
            return 1;
        }
        atribuirMissao(missoesJogadores[i], missoes, TOTAL_MISSOES);
        exibirMissao(CORES_JOGADORES[i], missoesJogadores[i]);
    }

    cadastrarTerritorios(mapaMundi, numTerritorios);

    // --- Loop Principal do Jogo ---
    int jogoAcabou = 0;
    while (!jogoAcabou) {
        exibirMapa(mapaMundi, numTerritorios);

        int idxAtacante, idxDefensor;
        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Digite o numero do territorio atacante (ou 0 para sair): ");
        scanf("%d", &idxAtacante);
        if (idxAtacante == 0) break;

        printf("Digite o numero do territorio defensor: ");
        scanf("%d", &idxDefensor);

        idxAtacante--;
        idxDefensor--;

        if (idxAtacante < 0 || idxAtacante >= numTerritorios || idxDefensor < 0 || idxDefensor >= numTerritorios) {
            printf("\nErro: Numero de territorio invalido!\n\n");
            continue;
        }

        struct Territorio *atacante = &mapaMundi[idxAtacante];
        struct Territorio *defensor = &mapaMundi[idxDefensor];

        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("\nErro: Voce nao pode atacar um territorio que ja eh seu!\n\n");
            continue;
        }
        if (atacante->tropas <= 1) {
            printf("\nErro: Voce precisa de mais de 1 tropa para atacar!\n\n");
            continue;
        }

        atacar(atacante, defensor);

        // --- Verificação de Missão Após o Ataque ---
        for (int i = 0; i < NUM_JOGADORES; i++) {
            if (verificarMissao(missoesJogadores[i], mapaMundi, numTerritorios, CORES_JOGADORES[i])) {
                printf("\n=============================================\n");
                printf("  VITORIA DO JOGADOR %s!\n", CORES_JOGADORES[i]);
                printf("  MISSAO CUMPRIDA: %s\n", missoesJogadores[i]);
                printf("=============================================\n");
                jogoAcabou = 1;
                break;
            }
        }
    }

    liberarMemoria(mapaMundi, missoesJogadores, NUM_JOGADORES);

    printf("\nJogo finalizado.\n");
    return 0;
}

/**
 * @brief Sorteia uma missão e a copia para o destino.
 * @param destino Ponteiro para a string que armazenará a missão (passagem por referência).
 * @param todasMissoes Vetor com todas as missões possíveis.
 * @param totalMissoes Tamanho do vetor de missões.
 */
void atribuirMissao(char* destino, const char* todasMissoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    strcpy(destino, todasMissoes[indiceSorteado]);
}

/**
 * @brief Exibe a missão de um jogador.
 * @param corJogador String com a cor do jogador.
 * @param missao String da missão (passagem por valor).
 */
void exibirMissao(const char *corJogador, const char *missao) {
    printf("Jogador %s, sua missao e: \"%s\"\n", corJogador, missao);
}

/**
 * @brief Verifica se a condição de vitória de uma missão foi atingida.
 * @param missao A missão a ser verificada.
 * @param mapa O estado atual do jogo.
 * @param tam O número de territórios.
 * @param corJogador A cor do jogador cuja missão está sendo verificada.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char* missao, const struct Territorio* mapa, int tam, const char* corJogador) {
    // Lógica para "Conquistar 3 territorios."
    if (strcmp(missao, "Conquistar 3 territorios.") == 0) {
        int contagem = 0;
        for (int i = 0; i < tam; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0) {
                contagem++;
            }
        }
        return contagem >= 3;
    }

    // Lógica para "Eliminar o exercito Vermelho."
    if (strcmp(missao, "Eliminar o exercito Vermelho.") == 0) {
        for (int i = 0; i < tam; i++) {
            if (strcmp((mapa + i)->cor, "Vermelho") == 0) {
                return 0; // Se encontrar ao menos um, a missão não foi cumprida.
            }
        }
        return 1; // Se o loop terminar, não há mais territórios vermelhos.
    }

    // Lógica para "Controlar o territorio 'Brasil'."
    if (strcmp(missao, "Controlar o territorio 'Brasil'.") == 0) {
        for (int i = 0; i < tam; i++) {
            if (strcmp((mapa + i)->nome, "Brasil") == 0) {
                return strcmp((mapa + i)->cor, corJogador) == 0;
            }
        }
    }
    
    // Lógica para "Acumular 12 tropas no total."
    if (strcmp(missao, "Acumular 12 tropas no total.") == 0) {
        int totalTropas = 0;
        for (int i = 0; i < tam; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0) {
                totalTropas += (mapa + i)->tropas;
            }
        }
        return totalTropas >= 12;
    }
    
    // Lógica para "Possuir um territorio com 7 ou mais tropas."
    if (strcmp(missao, "Possuir um territorio com 7 ou mais tropas.") == 0) {
        for (int i = 0; i < tam; i++) {
            if (strcmp((mapa + i)->cor, corJogador) == 0 && (mapa + i)->tropas >= 7) {
                return 1;
            }
        }
    }

    return 0; // Nenhuma condição de vitória foi atingida
}


// Funções do desafio anterior (reutilizadas)
void cadastrarTerritorios(struct Territorio *mapa, int tam) {
    printf("\n--- FASE DE CADASTRO DE TERRITORIOS ---\n");
    for (int i = 0; i < tam; i++) {
        printf("\n--- Territorio %d ---\n", i + 1);
        printf("Nome: "); scanf("%s", (mapa + i)->nome);
        printf("Cor do exercito: "); scanf("%s", (mapa + i)->cor);
        printf("Numero de tropas: "); scanf("%d", &(mapa + i)->tropas);
    }
}

void exibirMapa(const struct Territorio *mapa, int tam) {
    printf("\n=============================================\n");
    printf("              ESTADO ATUAL DO MAPA\n");
    printf("=============================================\n");
    for (int i = 0; i < tam; i++) {
        printf("%d. %s | Exercito: %s | Tropas: %d\n", i + 1, (mapa + i)->nome, (mapa + i)->cor, (mapa + i)->tropas);
    }
    printf("=============================================\n");
}

void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Atacante (%s) rolou: %d\n", atacante->nome, dadoAtaque);
    printf("Defensor (%s) rolou: %d\n", defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\n>> VITORIA DO ATACANTE! %s conquistou %s.\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        int tropasMovidas = atacante->tropas / 2;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;
    } else {
        printf("\n>> VITORIA DA DEFESA! %s repeliu o ataque.\n", defensor->nome);
        atacante->tropas--;
    }
}

/**
 * @brief Libera toda a memória alocada dinamicamente.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param missoesJogadores Vetor de ponteiros para as missões dos jogadores.
 * @param numJogadores Número de jogadores.
 */
void liberarMemoria(struct Territorio *mapa, char *missoesJogadores[], int numJogadores) {
    printf("\nLiberando memoria alocada...\n");
    // Libera a memória do mapa de territórios
    free(mapa);
    // Libera a memória de cada string de missão individualmente
    for (int i = 0; i < numJogadores; i++) {
        free(missoesJogadores[i]);
    }
    printf("Memoria liberada com sucesso.\n");
}
