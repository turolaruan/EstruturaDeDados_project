// Projeto Final de CC4652 - Estrutura de Dados
// Aplicativo para gerenciamento de serviços de saúde

// Desenvolvido por
// Gustavo Garcia Bagio - RA: 24.122.012-8
// Ruan Pastrelo Turola - RA: 24.122.050-8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================================================================
// ========================== Structs e métodos de Registro
// ================================================================================

typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Registro {
    char nome[100];
    int idade;
    char rg[10];
    Data entrada;
} Registro;

// Função que solicita ao usuário a inserção de um RG válido (9 dígitos ou menos)
// Essa função é utilizada em diversos momentos do programa
char *solicita_rg() {
    char *rg = malloc(sizeof(char) * 10);
    char aux[100];
    while (1) {
        printf("RG do paciente>");
        fgets(aux, 100, stdin);
        sscanf(aux, "%[^\n]s", aux);

        // Verifica se o RG inserido é válido
        if (strlen(aux) <= 9) {
            strcpy(rg, aux);
            break;
        } else {
            printf("RG invalido!\n");
        }
    }
    return rg;
}

// Função que solicita ao usuário a inserção de uma opção
// Essa função é utilizada em diversos momentos do programa (seleção de menus)
int solicita_opcao() {
    int opcao;
    char aux[100];
    fgets(aux, 100, stdin);
    sscanf(aux, "%[^\n]s", aux);
    opcao = strtol(aux, NULL, 10);
    return opcao;
}

// Função que cria um novo registro com os dados inseridos pelo usuário
Registro novoRegistro(char nome[100], int idade, char rg[10], int dia, int mes,int ano) {

    Registro registro;
    strcpy(registro.nome, nome);
    strcpy(registro.rg, rg);
    registro.idade = idade;
    registro.entrada.dia = dia;
    registro.entrada.mes = mes;
    registro.entrada.ano = ano;

    return registro;
}

// ================================================================================
// ========================== Structs e métodos da Lista
// ================================================================================

typedef struct Elista {
    Registro dados;
    struct Elista *proximo;
} Elista;

typedef struct Lista {
    Elista *primeiro;
    int qtde;
} Lista;

// Função que inicializa uma lista
Lista *inicializa_lista() {
    Lista *l = malloc(sizeof(Lista));
    l->primeiro = NULL;
    l->qtde = 0;
    return l;
}

// Função que busca um registro na lista a partir de um RG
Elista *buscar_na_lista(Lista *l, char rg[10]) {
    Elista *atual = l->primeiro;

    // Percorre a lista até encontrar o registro com o RG desejado
    while (atual != NULL && (strcmp(atual->dados.rg, rg) != 0)) {
        atual = atual->proximo;
    }

    return atual;
}

// Função que insere um registro no início da lista (utilizada para cadastrar um novo paciente)
void inserir_inicio_da_lista(Lista *l, char nome[100], int idade, char rg[10], int dia,int mes, int ano) {
    Elista *novo = malloc(sizeof(Elista));
    novo->dados = novoRegistro(nome, idade, rg, dia, mes, ano);
    novo->proximo = NULL;

    if (l->primeiro == NULL) {
        l->primeiro = novo;
    } else {
        novo->proximo = l->primeiro;
        l->primeiro = novo;
    }
    l->qtde++;
}

// Função que insere um registro no final da lista (utilizada para carregar os dados salvos no arquivo mantendo a ordem original)
void inserir_final_da_lista(Lista *l, char nome[100], int idade, char rg[10], int dia,int mes, int ano) {
    Elista *novo = malloc(sizeof(Elista));
    novo->dados = novoRegistro(nome, idade, rg, dia, mes, ano);
    novo->proximo = NULL;

    if (l->primeiro == NULL) {
        l->primeiro = novo;
    } else {
        Elista *atual = l->primeiro;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
    l->qtde++;
}

// Função que remove um registro da lista a partir de um RG
void remover_da_lista(Lista *l, char rg[10]) {
    Elista *atual = l->primeiro;
    Elista *anterior = NULL;

    // Percorre a lista até encontrar o registro com o RG desejado
    while (atual != NULL && (strcmp(atual->dados.rg, rg) != 0)) {
        anterior = atual;
        atual = atual->proximo;
    }
    if (atual == NULL) {
        printf("\nPaciente nao encontrado!\n");
        return;
    }
    if (anterior == NULL && atual->proximo == NULL) {
        l->primeiro = NULL;
    }
    if (anterior == NULL && atual->proximo != NULL) {
        l->primeiro = atual->proximo;
    }
    if (anterior != NULL && atual->proximo == NULL) {
        anterior->proximo = NULL;
    }
    if (anterior != NULL && atual->proximo != NULL) {
        anterior->proximo = atual->proximo;
    }
    free(atual);
    l->qtde--;

    printf("\nPaciente removido com sucesso!\n");
}

// ================================================================================
// ========================== Structs e métodos da Fila
// ================================================================================

typedef struct Efila {
    Registro dados;
    struct Efila *proximo;
} Efila;

typedef struct Fila {
    Efila *head;
    Efila *tail;
    int qtde;
} Fila;


// Função que inicializa uma fila
Fila *inicializa_fila() {
    Fila *f = malloc(sizeof(Fila));
    f->head = NULL;
    f->tail = NULL;
    f->qtde = 0;
    return f;
}

// Função que insere um registro na fila (utilizada para enfileirar um paciente)
void enfileirar(Fila *f, Registro dados) {
    Efila *novo = malloc(sizeof(Efila));
    novo->dados = dados;
    novo->proximo = NULL;

    if (f->qtde == 0) {
        f->head = novo;
        f->tail = novo;
    } else {
        f->tail->proximo = novo;
        f->tail = novo;
    }
    f->qtde++;
}


// Função que remove um registro da fila (utilizada para desenfileirar um paciente)
int desenfileirar(Fila *f) {

    Efila *atual = f->head;
    if (f->head == NULL || f->tail == NULL || f->qtde == 0) {
        return -1;
    }

    if (f->head == f->tail || f->qtde == 1) {
        f->head = NULL;
        f->tail = NULL;
    } else {
        f->head = atual->proximo;
    }

    free(atual);
    f->qtde--;
    return 0;

}

// ================================================================================
// ========================== Structs e métodos da ABB
// ================================================================================

typedef struct EABB {
    Registro dados;
    struct EABB *esq;
    struct EABB *dir;
    struct EABB *pai;
} EABB;

typedef struct ABB {
    EABB *raiz;
    int qtde;
} ABB;

// Função que inicializa uma ABB (utilizada para mostrar os pacientes em ordem)
ABB *inicializa_arvore() {
    ABB *arvore = malloc(sizeof(ABB));
    arvore->raiz = NULL;
    arvore->qtde = 0;
    return arvore;
}

// Função que insere um registro na ABB
// Essa função recebe como parâmetro a opção selecionada no menu, para que a árvore seja construída de acordo com o critério escolhido
// 1- Ano de entrada
// 2- Mês de entrada
// 3- Dia de entrada
// 4- Idade
void inserir_na_arvore(ABB *arvore, Registro dados, int opcao) {
    EABB *novo = malloc(sizeof(EABB));
    novo->dir = NULL;
    novo->esq = NULL;
    novo->pai = NULL;
    novo->dados = dados;

    if (arvore->qtde == 0) {
        arvore->raiz = novo;
        arvore->qtde++;
    } else {
        EABB *atual = arvore->raiz;
        EABB *anterior = NULL;
        long valor;
        long atual_valor;

        while(atual != NULL) {
            anterior = atual;

            // Compara os valores de acordo com a opção selecionada
            switch (opcao) {
                case 1:
                    // Utiliza como valor de comparação o ano de entrada
                    valor = novo->dados.entrada.ano;
                    atual_valor = atual->dados.entrada.ano;
                    break;
                case 2:
                    // Utiliza como valor de comparação o mês de entrada
                    valor = novo->dados.entrada.mes;
                    atual_valor = atual->dados.entrada.mes;
                    break;
                case 3:
                    // Utiliza como valor de comparação o dia de entrada
                    valor = novo->dados.entrada.dia;
                    atual_valor = atual->dados.entrada.dia;
                    break;
                case 4:
                    // Utiliza como valor de comparação a idade
                    valor = novo->dados.idade;
                    atual_valor = atual->dados.idade;
                    break;
                default:
                    valor = strtol(novo->dados.rg, NULL, 10);
                    atual_valor = strtol(atual->dados.rg, NULL, 10);
            }

            if (valor < atual_valor){
                atual = atual->esq;
            } else {
                atual = atual->dir;
            }
        }

        if (valor < atual_valor) {
            anterior->esq = novo;
        } else{
            anterior->dir = novo;
        }
        novo->pai = anterior;
    }
}

// Função que mostra os pacientes em ordem (critério definido na construção da árvore)
void mostrar_arvore_em_ordem(EABB *raiz) {
    if (raiz != NULL) {
        mostrar_arvore_em_ordem(raiz->esq);
        printf("Nome: %s\n", raiz->dados.nome);
        printf("Idade: %d\n", raiz->dados.idade);
        printf("RG: %s\n", raiz->dados.rg);
        printf("Entrada: %02d/%02d/%04d\n", raiz->dados.entrada.dia,
               raiz->dados.entrada.mes, raiz->dados.entrada.ano);
        printf("------------------------------------------\n");
        mostrar_arvore_em_ordem(raiz->dir);
    }
}

// Função que limpa a memória alocada para todos os nós da árvore
void limpa_arvore(EABB *raiz) {
    if (raiz != NULL) {
        limpa_arvore(raiz->esq);
        limpa_arvore(raiz->dir);
        free(raiz);
    }
}

// ================================================================================
// ========================== Métodos Cadastrar
// ================================================================================

// Função que cadastra um novo paciente
void cadastrar_paciente(Lista *l) {

    // Declara as variáveis que serão utilizadas
    char aux[100];
    char nome[100];
    int idade, dia, mes, ano;
    char rg[10];

    printf("\n----------- Cadastrar Paciente -----------\n");

    // Solicita ao usuário a inserção do nome do paciente
    printf("Nome>");
    fgets(aux, 100, stdin);
    sscanf(aux, "%[^\n]s", nome);

    // Solicita ao usuário a inserção da idade do paciente
    while(1) {
        printf("Idade>");
        fgets(aux, 100, stdin);
        sscanf(aux, "%[^\n]s", aux);
        idade = strtol(aux, NULL, 10);

        // Verifica se a idade inserida é válida
        if (idade >= 0) {
            break;
        } else {
            printf("Idade invalida!\n");
        }
    }

    // Solicita ao usuário a inserção do RG do paciente
    while(1) {
        printf("RG>");
        fgets(aux, 100, stdin);
        sscanf(aux, "%[^\n]s", aux);

        // Verifica se o RG inserido é válido
        if (strlen(aux) <= 9) {
            strcpy(rg, aux);
            break;
        } else {
            printf("RG invalido!\n");
        }
    }

    // Solicita ao usuário a inserção da data de entrada do paciente
    while(1) {
        printf("Data de entrada (xx/yy/zzzz)>");
        fgets(aux, 100, stdin);
        int result = sscanf(aux, "%[^\n]s", aux);

        // Verifica se a data inserida é válida (formato xx/yy/zzzz)
        if (result != 0 && strlen(aux) == 10 && aux[2] == '/' && aux[5] == '/') {
            dia = strtol(aux, NULL, 10);
            mes = strtol(aux + 3, NULL, 10);
            ano = strtol(aux + 6, NULL, 10);

            // Verifica se a data inserida é válida (dia, mês e ano)
            if (dia > 0 && dia <= 31 && mes > 0 && mes <= 12 && ano > 0) {
                break;
            } else {
                printf("Data invalida!\n");
            }
        } else {
            printf("Data invalida!\n");
        }

    }

    // Insere o paciente no início da lista
    inserir_inicio_da_lista(l, nome, idade, rg, dia, mes, ano);
    printf("\nPaciente cadastrado com sucesso!\n");
}

// Função que mostra a lista completa de pacientes
void mostrar_lista_completa(Lista *l) {

    Elista *atual = l->primeiro;
    printf("\n----------- Lista de pacientes -----------\n");

    // Percorre a lista e imprime os dados de cada paciente
    while (atual != NULL) {
        printf("Nome: %s\n", atual->dados.nome);
        printf("Idade: %d\n", atual->dados.idade);
        printf("RG: %s\n", atual->dados.rg);
        printf("Entrada: %02d/%02d/%04d\n", atual->dados.entrada.dia,
               atual->dados.entrada.mes, atual->dados.entrada.ano);
        printf("------------------------------------------\n");

        atual = atual->proximo;
    }
}

// Função que consulta um paciente cadastrado
void consultar_paciente(Lista *l, char rg[10]) {

    // Busca o paciente na lista a partir do RG
    Elista *atual = buscar_na_lista(l, rg);

    // Verifica se o paciente foi encontrado e imprime seus dados
    if (atual != NULL) {
        printf("\n----------- Paciente Encontrado ----------\n");
        printf("Nome: %s\n", atual->dados.nome);
        printf("Idade: %d\n", atual->dados.idade);
        printf("RG: %s\n", atual->dados.rg);
        printf("Entrada: %02d/%02d/%04d\n", atual->dados.entrada.dia,
               atual->dados.entrada.mes, atual->dados.entrada.ano);
        printf("------------------------------------------\n");
    } else {
        printf("\nPaciente nao encontrado!\n");
    }
}

// Função que atualiza os dados de um paciente cadastrado
void atualizar_dados_paciente(Lista *l, char rg[10]) {

    // Busca o paciente na lista a partir do RG
    Elista *atual = buscar_na_lista(l, rg);

    // Verifica se o paciente foi encontrado
    if (atual == NULL) {
        printf("\nPaciente nao encontrado!\n");
        return;
    } else {

        // Imprime os dados do paciente
        consultar_paciente(l, rg);
    }

    // Loop que solicita ao usuário a inserção de um novo dado
    while (1) {

        printf("\n------- Atualizar dados do Paciente ------\n");

        // Solicita ao usuário qual dado deseja atualizar
        printf("Qual dado deseja atualizar?\n");
        printf("1 - Nome\n");
        printf("2 - Idade\n");
        printf("3 - RG\n");
        printf("4 - Data de entrada\n");
        printf("------------------------------------------\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao;
        opcao = solicita_opcao();

        char aux[100];

        // Verifica qual dado será atualizado
        switch (opcao) {

            case 1:
                // Solicita ao usuário a inserção do novo nome
                printf("Novo Nome>");
                fgets(aux, 100, stdin);
                sscanf(aux, "%[^\n]s", atual->dados.nome);
                break;
            case 2:
                // Solicita ao usuário a inserção da nova idade
                while(1) {
                    int idade;
                    printf("Idade>");
                    fgets(aux, 100, stdin);
                    sscanf(aux, "%[^\n]s", aux);
                    idade = strtol(aux, NULL, 10);
                    if (idade >= 0) {
                        atual->dados.idade = idade;
                        break;
                    } else {
                        printf("Idade invalida!\n");
                    }
                }
                break;
            case 3:
                // Solicita ao usuário a inserção do novo RG
                while(1) {
                    printf("Novo RG>");
                    fgets(aux, 100, stdin);
                    sscanf(aux, "%[^\n]s", aux);
                    if (strlen(aux) <= 9) {
                        strcpy(atual->dados.rg, aux);
                        break;
                    } else {
                        printf("RG invalido!\n");
                    }
                }
                break;
            case 4:
                // Solicita ao usuário a inserção da nova data de entrada
                while (1) {
                    printf("Nova data (xx/yy/zzzz)>");
                    fgets(aux, 100, stdin);
                    int result = sscanf(aux, "%[^\n]s", aux);
                    if (result != 0 && strlen(aux) == 10 && aux[2] == '/' && aux[5] == '/') {
                        int dia = strtol(aux, NULL, 10);
                        int mes = strtol(aux + 3, NULL, 10);
                        int ano = strtol(aux + 6, NULL, 10);
                        if (dia > 0 && dia <= 31 && mes > 0 && mes <= 12 && ano > 0) {
                            atual->dados.entrada.dia = dia;
                            atual->dados.entrada.mes = mes;
                            atual->dados.entrada.ano = ano;
                            break;
                        } else {
                            printf("Data invalida!\n");
                        }
                    } else {
                        printf("Data invalida!\n");
                    }
                }
                break;
            default:
                printf("Opcao invalida!1\n");
                continue;
        }

        printf("------------------------------------------\n");
        printf("Dados atualizados com sucesso!\n\n");

        // Verifica se o usuário deseja atualizar mais algum dado
        printf("Deseja atualizar mais algum dado?\n");
        printf("1 - Sim\n");
        printf("2 - Nao\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao2;
        fgets(aux, 100, stdin);
        sscanf(aux, "%[^\n]s", aux);
        opcao2 = strtol(aux, NULL, 10);

        // Se não, retorna ao menu principal
        // Se sim, retorna ao loop
        if (opcao2 == 2) {
            break;
        } else if (opcao2 != 1) {
            printf("Opcao invalida!2\n");
        } else {
            printf("\n");
        }
    }
}

// ================================================================================
// ========================== Métodos Atendimento
// ================================================================================

// Função que enfileira um paciente
void enfileirar_paciente(Fila *f, Lista *l, char rg[10]) {

    // Busca o paciente na lista a partir do RG
    Elista *atual = buscar_na_lista(l, rg);

    // Verifica se o paciente foi encontrado
    if (atual != NULL) {

        // Enfileira o paciente
        enfileirar(f, atual->dados);
        printf("\nPaciente enfileirado com sucesso!\n");
    } else {
        printf("\nPaciente nao encontrado!\n");
    }
}

// Função que desenfileira um paciente
void desenfileirar_paciente(Fila *f) {

    // Desenfileira o paciente
    int value = desenfileirar(f);

    // Se retornar -1, a fila está vazia
    if (value == -1) {
        printf("\nFila vazia!\n");
    } else {
        printf("\nPaciente desenfileirado com sucesso!\n");
    }
}

// Função que mostra a fila de pacientes
void mostrar_fila(Fila *f){
    Efila *atual = f->head;
    printf("\n------------ Fila de pacientes -----------\n");

    // Percorre a fila e imprime os dados de cada paciente
    while (atual != NULL) {
        printf("Nome: %s\n", atual->dados.nome);
        printf("Idade: %d\n", atual->dados.idade);
        printf("RG: %s\n", atual->dados.rg);
        printf("Entrada: %02d/%02d/%04d\n", atual->dados.entrada.dia,atual->dados.entrada.mes, atual->dados.entrada.ano);
        printf("------------------------------------------\n");

        atual = atual->proximo;
    }
    printf("\n");
}

// ================================================================================
// ========================== Métodos Pesquisar
// ================================================================================

// Função que mostra os pacientes em ordem
void mostrar_pacientes_em_ordem(Lista *l, int opcao) {
    ABB *arvore = inicializa_arvore();
    Elista *atual = l->primeiro;

    // Percorre a lista e insere os pacientes na árvore (de acordo com o critério selecionado)
    while (atual != NULL) {
        inserir_na_arvore(arvore, atual->dados, opcao);
        atual = atual->proximo;
    }

    // Imprime o header de acordo com o critério selecionado
    switch (opcao) {
        case 1:
            printf("\n----------- Pacientes por ano ------------\n");
            break;
        case 2:
            printf("\n----------- Pacientes por mes ------------\n");
            break;
        case 3:
            printf("\n----------- Pacientes por dia ------------\n");
            break;
        case 4:
            printf("\n----------- Pacientes por idade ----------\n");
            break;
        default:
            printf("\n----------- Pacientes por RG -------------\n");
    }

    // Imprime os pacientes em ordem
    mostrar_arvore_em_ordem(arvore->raiz);

    // Limpa a memória alocada para os nós da árvore
    limpa_arvore(arvore->raiz);

    // Limpa a memória alocada para a árvore
    arvore->raiz = NULL;
    free(arvore);

}

// ================================================================================
// ========================== Métodos Carregar / Salvar
// ================================================================================

// Função que salva os dados da lista e da fila em arquivos binários
void salvar_dados(Lista *l, Fila *f) {

    // Abre os arquivos para escrita
    FILE *dados_lista;
    dados_lista = fopen("dados_lista.bin", "wb");

    FILE *dados_fila;
    dados_fila = fopen("dados_fila.bin", "wb");

    // Verifica se os arquivos foram abertos corretamente
    if (dados_fila == NULL || dados_lista == NULL) {
        printf("\nErro ao abrir o arquivo\n");
        return;
    }

    // Percorre a lista e salva os dados de registro no arquivo
    Elista *atual_lista = l->primeiro;
    while (atual_lista != NULL) {
        fwrite(&atual_lista->dados, sizeof(Registro), 1, dados_lista);
        atual_lista = atual_lista->proximo;
    }

    // Fecha o arquivo
    fclose(dados_lista);

    // Percorre a fila e salva os dados de registro no arquivo
    Efila *atual_fila = f->head;
    while (atual_fila != NULL) {
        fwrite(&atual_fila->dados, sizeof(Registro), 1, dados_fila);
        atual_fila = atual_fila->proximo;
    }

    // Fecha o arquivo
    fclose(dados_fila);

    printf("\nDados salvos com sucesso!\n");

}

// Função que carrega os dados da lista e da fila a partir dos arquivos binários
void carregar_dados(Lista *l, Fila *f) {

    // Abre os arquivos para leitura
    FILE *dados_lista;
    dados_lista = fopen("dados_lista.bin", "rb");

    FILE *dados_fila;
    dados_fila = fopen("dados_fila.bin", "rb");

    // Verifica se os arquivos foram abertos corretamente
    if (dados_fila == NULL || dados_lista == NULL) {
        printf("\nErro ao abrir o arquivo\n");
        return;
    }

    // Declara a variável que será utilizada para ler os dados de registro
    Registro dados;

    // Percorre o arquivo da lista, lê os dados de registro e insere no final da lista (para manter a ordem original)
    while (fread(&dados, sizeof(Registro), 1, dados_lista) != 0) {
        inserir_final_da_lista(l, dados.nome, dados.idade, dados.rg, dados.entrada.dia, dados.entrada.mes, dados.entrada.ano);
    }

    // Fecha o arquivo
    fclose(dados_lista);

    // Percorre o arquivo fila, lê os dados de registro e enfileira os pacientes
    while (fread(&dados, sizeof(Registro), 1, dados_fila) != 0) {
        enfileirar(f, dados);
    }

    printf("\nDados carregados com sucesso!\n");

}

// ================================================================================
// ========================== Menu Cadastrar
// ================================================================================

void menu_cadastrar(Lista *l) {
    while (1) {
        printf("\n------------- Menu Cadastrar -------------\n");
        printf("1 - Cadastrar novo paciente\n");
        printf("2 - Consultar paciente cadastrado\n");
        printf("3 - Mostrar lista de pacientes\n");
        printf("4 - Atualizar dados de paciente\n");
        printf("5 - Remover paciente\n");
        printf("6 - Voltar\n");
        printf("------------------------------------------\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao;
        opcao = solicita_opcao();

        // Caso a opção seja 2, 3, 4 ou 5, verifica se a lista está vazia
        // Se estiver vazia, não é possível realizar a operação
        if (opcao >= 2 && opcao <= 5 && l->qtde == 0) {
            printf("\nNao ha pacientes cadastrados!\n");
            continue;
        }

        char *rg;

        switch (opcao) {
            case 1:
                // Cadastra um novo paciente
                cadastrar_paciente(l);
                break;
            case 2:
                // Solicita o RG e consulta o paciente
                rg = solicita_rg();
                consultar_paciente(l, rg);
                break;
            case 3:
                // Mostra a lista completa de pacientes
                mostrar_lista_completa(l);
                break;
            case 4:
                // Solicita o RG e atualiza os dados do paciente
                rg = solicita_rg();
                atualizar_dados_paciente(l, rg);
                break;
            case 5:
                // Solicita o RG e remove o paciente
                rg = solicita_rg();
                remover_da_lista(l, rg);
                break;
            case 6:
                // Retorna ao menu principal
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

// ================================================================================
// ========================== Menu Atendimento
// ================================================================================

void menu_atendimento(Lista *l, Fila *f) {
    while (1) {
        printf("\n------------ Menu Atendimento ------------\n");
        printf("1 - Enfileirar paciente\n");
        printf("2 - Desenfileirar paciente\n");
        printf("3 - Mostrar fila de pacientes\n");
        printf("4 - Voltar\n");
        printf("------------------------------------------\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao;
        opcao = solicita_opcao();

        // Caso a opção seja 1, verifica se a lista está vazia
        // Se estiver vazia, não é possível realizar a operação
        if (opcao == 1 && l->qtde == 0) {
            printf("\nNao ha pacientes cadastrados!\n");
            continue;

        // Caso a opção seja 2 ou 3, verifica se a fila está vazia
        // Se estiver vazia, não é possível realizar a operação
        } else if (opcao >= 2 && opcao <= 3 && f->qtde == 0) {
            printf("\nNao ha pacientes na fila!\n");
            continue;
        }

        char *rg;

        switch (opcao) {
            case 1:
                // Solicita o RG e enfileira o paciente
                rg = solicita_rg();
                enfileirar_paciente(f, l, rg);
                break;
            case 2:
                // Desenfileira um paciente
                desenfileirar_paciente(f);
                break;
            case 3:
                // Mostra a fila de pacientes
                mostrar_fila(f);
                break;
            case 4:
                // Retorna ao menu principal
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

// ================================================================================
// ========================== Menu Pesquisa
// ================================================================================

void menu_pesquisa(Lista *l) {
    while (1) {
        printf("\n------------- Menu Pesquisa ------------\n");
        printf("1 - Mostrar pacientes por ano\n");
        printf("2 - Mostrar pacientes por mes\n");
        printf("3 - Mostrar pacientes por dia\n");
        printf("4 - Mostrar pacientes por idade\n");
        printf("5 - Voltar\n");
        printf("-----------------------------------------\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao;
        opcao = solicita_opcao();

        switch (opcao) {
            case 1:
                // Mostra os pacientes em ordem (ano de entrada)
                mostrar_pacientes_em_ordem(l, 1);
                break;
            case 2:
                // Mostra os pacientes em ordem (mês de entrada)
                mostrar_pacientes_em_ordem(l, 2);
                break;
            case 3:
                // Mostra os pacientes em ordem (dia de entrada)
                mostrar_pacientes_em_ordem(l, 3);
                break;
            case 4:
                // Mostra os pacientes em ordem (idade)
                mostrar_pacientes_em_ordem(l, 4);
                break;
            case 5:
                // Retorna ao menu principal
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

// ================================================================================
// ========================== Menu Sobre
// ================================================================================

void menu_sobre() {

    printf("\n--------- Sobre os Desenvolvedores -------\n");
    printf("Nome: Gustavo Garcia Bagio\n");
    printf("R.A: 24.122.012-8\n");
    printf("Nome: Ruan Pastrelo Turola\n");
    printf("R.A: 24.122.050-8\n");
    printf("------------------------------------------\n");
    printf("Ciclo: 4\n");
    printf("Curso: Ciencia da Computacao\n");
    printf("Disciplina: Estrutura de Dados (CC4652)\n");
    printf("Data de lancamento: 09/11/2023\n");
    printf("---------------------------------------------------\n");

}

// ================================================================================
// ========================== Menu Carregar / Salvar
// ================================================================================

void menu_carregar_salvar(Lista *l, Fila *f) {

    while(1) {
        printf("\n--------- Menu Carregar / Salvar ---------\n");
        printf("1 - Carregar dados de pacientes\n");
        printf("2 - Salvar dados de pacientes\n");
        printf("3 - Voltar\n");
        printf("------------------------------------------\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao;
        opcao = solicita_opcao();

        // Caso a opção seja 2, verifica se a lista está vazia
        // Se estiver vazia, não é possível realizar a operação
        if (opcao == 2 && l->qtde == 0) {
            printf("\nNao ha dados para salvar!\n");
            continue;
        }

        switch (opcao) {
            case 1:
                // Carrega os dados da lista e da fila a partir dos arquivos binários
                carregar_dados(l, f);
                break;
            case 2:
                // Salva os dados da lista e da fila em arquivos binários
                salvar_dados(l, f);
                break;
            case 3:
                // Retorna ao menu principal
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }

}

// ================================================================================
// ========================== Menu Principal
// ================================================================================

void menu_principal(Lista *l, Fila *f) {
    while (1) {
        printf("\n------------- Menu Principal -------------\n");
        printf("1 - Cadastrar\n");
        printf("2 - Atendimento\n");
        printf("3 - Pesquisa\n");
        printf("4 - Carregar / Salvar\n");
        printf("5 - Sobre\n");
        printf("6 - Sair\n");
        printf("------------------------------------------\n");
        printf("Insira a opcao desejada>");

        // Solicita ao usuário a inserção de uma opção
        int opcao;
        opcao = solicita_opcao();

        switch (opcao) {
            case 1:
                // Chama o menu cadastrar
                menu_cadastrar(l);
                break;
            case 2:
                // Chama o menu atendimento
                menu_atendimento(l, f);
                break;
            case 3:
                // Chama o menu pesquisa
                menu_pesquisa(l);
                break;
            case 4:
                // Chama o menu carregar / salvar
                menu_carregar_salvar(l, f);
                break;
            case 5:
                // Chama o menu sobre
                menu_sobre();
                break;
            case 6:
                // Encerra o programa
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

// ================================================================================
// ========================== Main
// ================================================================================

int main() {

    // Inicializa a lista e a fila
    Lista *l = inicializa_lista();
    Fila *f = inicializa_fila();

    // Chama o menu principal
    menu_principal(l, f);

    return 0;
}