#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h> // Para adicionar caracteres especiais do PT-BR
#include <time.h> // Para a funcao srand/time no modo adivinhar - RANDOM

// --- Estruturas e Constantes ---

#define MAX_MONSTROS 5
#define TAM_NOME 50

// Estrutura para as opcoes do catalogo
typedef struct {
    char tipo[TAM_NOME];
    char familia[TAM_NOME];
} Caracteristica;

// Estrutura para um monstro completo (no catalogo)
typedef struct {
    char nome[TAM_NOME];
    char cabeca[TAM_NOME];
    char olhos[TAM_NOME];
    char boca[TAM_NOME];
    char membros[TAM_NOME];
} Monstro;

// Funcao para tratamento de dados do chute
void para_minusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// --- CATAOGO COMPLETO DE OPCOES ---

// Cabeca (3 opcoes)
Caracteristica opcoes_cabeca[] = {
    {"Parece uma caveira", "Zombos"},
    {"Quadrada com parafusos", "Franken"},
    {"Redonda", "Happy"},
    {"Nenhum", "Nula"}
};

// Olhos (3 opcoes)
Caracteristica opcoes_olhos[] = {
    {"Olhos grandes e separados", "Vegitas"},
    {"Dois desiguais e juntos", "Wackus"},
    {"Simpáticos, como quando sorrimos", "Spritem"},
};

// Boca (4 opcoes: incluindo uma nula)
Caracteristica opcoes_boca[] = {
    {"Boca faltando dentes", "Vegitas"},
    {"Sorrindo, com dentes apenas em cima", "Wackus"},
    {"Sorrindo, sem dentes", "Spritem"},
    {"Nenhum", "Nula"}
};
int num_opcoes_boca = 4;

// Membros (4 opcoes: incluindo uma nula)
Caracteristica opcoes_membros[] = {
    {"Quatro braços finos", "Vegitas"},
    {"Dois braços grossos e curtos", "Wackus"},
    {"Seis tentáculos", "Spritem"},
    {"Nenhum", "Nula"}
};
int num_opcoes_membros = 4;

// --- CATALOGO DOS 5 MONSTROS CRIADOS ---
Monstro catalogo_monstros[MAX_MONSTROS] = {
    // 1. Happy Vegitas
    {"Happy Vegitas", "Redonda", "Olhos grandes e separados", "Boca faltando dentes", "nula"},
    // 2. Zombos Spriten
    {"Zombos Spritem", "Parece uma caveira", "Simpáticos, como quando sorrimos", "Sorrindo, sem dentes", "Seis tentáculos"},
    // 3. Franken Vegitas
    {"Franken Vegitas", "Quadrada com parafusos", "Olhos grandes e separados", "Boca faltando dentes", "Quatro braços finos"},
    // 4.Franken Wackus
    {"Franken Wackus", "Quadrada com parafusos", "Dois desiguais e juntos", "Sorrindo, com dentes apenas em cima", "Dois braços grossos e curtos"},
    // 5. Zombos Spritem (Desempate - O nome e igual ao monstro 2, mas as caracteristicas sao diferentes)
    {"Zombos Spritem", "Parece uma caveira", "Simpáticos, como quando sorrimos", "Sorrindo, sem dentes", "Quatro braços finos"}
};

// --- Funcoes Auxiliares ---

// Funco para exibir o catalogo completo para auxiliar no chute
void exibir_catalogo_para_chute() {
    printf("\n--- CATÁLOGO DE CARACTERÍSTICAS PARA AUXÍLIO ---\n");

    // As caracteristicas sao exibidas de forma clara com sua FAMILIA (que forma o nome)

    // Cabeca (Primeira Metade do Nome)
    printf("\n=== FAMÍLIA DA CABEÇA (PRIMEIRA METADE DO NOME) ===\n");
    for (int i = 0; i < 3; i++) {
        printf("- [%s]: %s\n", opcoes_cabeca[i].familia, opcoes_cabeca[i].tipo);
    }

    // Familias da Segunda Metade
    printf("\n=== FAMÍLIAS DE CARACTERÍSTICAS (SEGUNDA METADE DO NOME) ===\n");

    // Olhos
    printf(">> OLHOS (Importante para o desempate):\n");
    for (int i = 0; i < 3; i++) {
        printf("   - [%s]: %s\n", opcoes_olhos[i].familia, opcoes_olhos[i].tipo);
    }

    // Boca
    printf(">> BOCA:\n");
    for (int i = 0; i < num_opcoes_boca; i++) {
        printf("   - [%s]: %s\n", opcoes_boca[i].familia, opcoes_boca[i].tipo);
    }

    // Membros
    printf(">> MEMBROS:\n");
    for (int i = 0; i < num_opcoes_membros; i++) {
        printf("   - [%s]: %s\n", opcoes_membros[i].familia, opcoes_membros[i].tipo);
    }
    printf("-----------------------------------------------------------\n");
}

// Funcao para exibir opcoes e obter a escolha do usuario
int escolher_caracteristica(const char *titulo, Caracteristica opcoes[], int num_opcoes, char *escolha) {
    int opcao;
    printf("\n--- Escolha: %s ---\n", titulo);
    for (int i = 0; i < num_opcoes; i++) {
        printf("[%d] %s (Familia: %s)\n", i + 1, opcoes[i].tipo, opcoes[i].familia);
    }
    printf("Sua opção: ");

    // Verifica se a entrada eh um numero
    if (scanf("%d", &opcao) != 1) {
        printf("Entrada inválida. Por favor, insira um número.\n");
        while (getchar() != '\n'); // Limpa o buffer de entrada
        return -1;
    }
    while (getchar() != '\n'); // Limpa o buffer apos o numero

    if (opcao >= 1 && opcao <= num_opcoes) {
        strcpy(escolha, opcoes[opcao - 1].tipo);
        return opcao - 1;
    }
    printf("Opção invalida.\n");
    return -1;
}


// Funcao auxiliar para encontrar a familia de uma caracteristica
const char* encontrar_familia(const char* escolha, Caracteristica opcoes[], int num_opcoes) {
    for (int i = 0; i < num_opcoes; i++) {
        if (strcmp(escolha, opcoes[i].tipo) == 0) {
            return opcoes[i].familia;
        }
    }
    return "Desconhecida";
}


// Funcao principal de classificacao do monstro montado (LOGICA DE MAIORIA/DESEMPATE)
void classificar_monstro(const Monstro *novo_monstro) {
    printf("\n\n--- CLASSIFICANDO SEU MONSTRO ---\n");

    // OBTENCAO DAS FAMILIAS E PRIMEIRA METADE

    const char *familia_cabeca = encontrar_familia(novo_monstro->cabeca, opcoes_cabeca, 3);
    const char *primeira_metade = familia_cabeca;

    const char *familia_olhos = encontrar_familia(novo_monstro->olhos, opcoes_olhos, 3);
    const char *familia_boca = encontrar_familia(novo_monstro->boca, opcoes_boca, num_opcoes_boca);
    const char *familia_membros = encontrar_familia(novo_monstro->membros, opcoes_membros, num_opcoes_membros);

    // CONTAGEM DAS FAMILIAS (Exclui a cabeca)

    int contagem_vegitas = 0;
    int contagem_wackus = 0;
    int contagem_spritem = 0;

    const char *familias_a_contar[] = {familia_olhos, familia_boca, familia_membros};
    int total_caracteristicas = 3;

    for (int i = 0; i < total_caracteristicas; i++) {
        if (strcmp(familias_a_contar[i], "Vegitas") == 0) {
            contagem_vegitas++;
        } else if (strcmp(familias_a_contar[i], "Wackus") == 0) {
            contagem_wackus++;
        } else if (strcmp(familias_a_contar[i], "Spritem") == 0) {
            contagem_spritem++;
        }
    }

    printf("\nContagem das Famílias (sem incluir a cabeca):\n");
    printf("Vegitas: %d, Wackus: %d, Spritem: %d\n", contagem_vegitas, contagem_wackus, contagem_spritem);

    // APLICACAO DA REGRA DE MAIORIA/DESEMPATE

    const char *segunda_metade = NULL;
    int maior_contagem = contagem_vegitas;

    if (contagem_wackus > maior_contagem) maior_contagem = contagem_wackus;
    if (contagem_spritem > maior_contagem) maior_contagem = contagem_spritem;

    // Verificar empates (Regra de Desempate)
    int empates = 0;
    if (contagem_vegitas == maior_contagem) empates++;
    if (contagem_wackus == maior_contagem) empates++;
    if (contagem_spritem == maior_contagem) empates++;

    if (maior_contagem == 0) {
        // Se todas forem nulas ou desconhecidas (contagem 0), assume o olhos
        segunda_metade = familia_olhos;
    } else if (empates > 1) {
        // HOUVE EMPATE: Usa a regra dos Olhos
        segunda_metade = familia_olhos;
        printf("\nRESULTADO: EMPATE na contagem de características! Usando a regra de **DESEMPATE PELOS OLHOS**.\n");
    } else {
        // SEM EMPATE: Usa a regra da Maioria
        if (contagem_vegitas == maior_contagem) segunda_metade = "Vegitas";
        else if (contagem_wackus == maior_contagem) segunda_metade = "Wackus";
        else if (contagem_spritem == maior_contagem) segunda_metade = "Spritem";
    }

    // RESULTADO FINAL

    printf("\n--- CLASSIFICACAO FINAL ---\n");
    printf("Primeira Metade (Cabeça): **%s**\n", primeira_metade);
    printf("Segunda Metade (Maioria Escolhida): **%s**\n", segunda_metade);
    printf("=============================================\n");
    printf("Nome do Monstro Descrito: **%s %s**\n", primeira_metade, segunda_metade);
    printf("=============================================\n");
}

// --- Modos de Jogo ---

void modo_descrever() {
    Monstro novo_monstro = {"Monstro Descrito", "", "", "", ""};
    int sucesso_cabeca = -1, sucesso_olhos = -1, sucesso_boca = -1, sucesso_membros = -1;

    printf("\n--- MODO DESCREVER UM MONSTRO ---\n");

    // Escolher Cabeca
    while(sucesso_cabeca == -1)
        sucesso_cabeca = escolher_caracteristica("Formato da Cabeça", opcoes_cabeca, 3, novo_monstro.cabeca);

    // Escolher Olhos
    while(sucesso_olhos == -1)
        sucesso_olhos = escolher_caracteristica("Olhos", opcoes_olhos, 3, novo_monstro.olhos);

    // Escolher Boca
    while(sucesso_boca == -1)
        sucesso_boca = escolher_caracteristica("Boca", opcoes_boca, num_opcoes_boca, novo_monstro.boca);

    // Escolher Membros
    while(sucesso_membros == -1)
        sucesso_membros = escolher_caracteristica("Membros", opcoes_membros, num_opcoes_membros, novo_monstro.membros);

    printf("\nVocê descreveu o monstro:\n");
    printf("Cabeça: %s\nOlhos: %s\nBoca: %s\nMembros: %s\n",
           novo_monstro.cabeca, novo_monstro.olhos, novo_monstro.boca, novo_monstro.membros);

    classificar_monstro(&novo_monstro);
}


void modo_adivinhar() {
    printf("\n--- MODO ADIVINHAR O MONSTRO ---\n");

    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL));
    int indice_sorteado = rand() % MAX_MONSTROS;
    Monstro monstro_secreto = catalogo_monstros[indice_sorteado];

    // CHAMA O MENU DE AUXILIO para dar base ao chute do usuario
    exibir_catalogo_para_chute();

    printf("Descrição do Monstro Secreto:\n");
    printf("Cabeça: %s\n", monstro_secreto.cabeca);
    printf("Olhos: %s\n", monstro_secreto.olhos);
    printf("Boca: %s\n", monstro_secreto.boca);
    printf("Membros: %s\n", monstro_secreto.membros);

    char palpite[TAM_NOME];
    printf("\nQual o nome deste monstro?\n");
    printf("Seu Palpite: ");

    // Le a string completa (incluindo espacos)
    if (scanf(" %[^\n]", palpite) != 1) {
        printf("Entrada invalida.\n");
        return;
    }
    //---TRATAMENTO DE DADOS DO CHUTE---
    // Copia do nome secreto para nao alterar o catalogo original
    char nome_secreto_copia[TAM_NOME];
    strcpy(nome_secreto_copia, monstro_secreto.nome);
    // Converte o nome secreto para minusculas
    para_minusculas(nome_secreto_copia);
    // Converte o palpite do usuÃ¡rio para minusculas
    para_minusculas(palpite);

    if (strcmp(palpite, nome_secreto_copia) == 0) {
        printf("==========================================\n");
        printf("\nPARABÉNS! Você acertou o nome: %s\n", monstro_secreto.nome);
        printf("==========================================\n");
    } else {
        printf("\nQue pena! Você errou :(\n");
        printf("Seu palpite foi: %s\n", palpite);
        printf("O nome correto era: **%s**\n", monstro_secreto.nome);
    }
}


// --- Funcao Principal (Main) ---

int main() {
    setlocale(LC_ALL, "Portuguese"); // Para adicionar caracteres especiais do PT-BR
    int escolha_modo;

    printf("==========================================\n");
    printf("         FÁBRICA DE MONSTROS V1.0         \n");
    printf("==========================================\n");

    // Loop principal para garantir uma escolha de modo valida
    do {
        printf("\nEscolha o modo de jogo:\n");
        printf("[1] Descrever um monstro que eu vi!\n");
        printf("[2] Adivinhar o nome de um monstro!\n");
        printf("Sua opção: ");

        if (scanf("%d", &escolha_modo) != 1) {
            printf("Entrada inválida. Por favor, digite 1 ou 2.\n");
            while (getchar() != '\n'); // Limpa o buffer
            escolha_modo = 0; // Forca o loop a continuar
        }
         while (getchar() != '\n'); // Limpa o buffer apos o numero
    } while (escolha_modo != 1 && escolha_modo != 2);


    switch (escolha_modo) {
        case 1:
            modo_descrever();
            break;
        case 2:
            modo_adivinhar();
            break;
    }

    return 0;
}
