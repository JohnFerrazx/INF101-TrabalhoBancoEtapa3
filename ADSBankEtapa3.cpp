
/******************************************************************************
INF101
Trabalho – Implementação de um Sistema de
Registro e Gestão de Contas Bancarias (ETAPA 3)

Aluno: Joao Pedro Ferraz Barreto
Matricula: 29059
******************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int MAX_CONTAS = 100;
const int MAX_MOVIMENTACOES = 500;

int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];

int quantidadeContas = 0;

int contaMovimentacao[MAX_MOVIMENTACOES];
char tipoMovimentacao[MAX_MOVIMENTACOES];
double valorMovimentacao[MAX_MOVIMENTACOES];

int quantidadeMovimentacoes = 0;

// Limpa os dados que ficaram na entrada do teclado
void limparEntrada() {
    cin.clear();
    cin.ignore(1000, '\n');
}

// Procura uma conta pelo numero
int localizarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) {
            return i;
        }
    }

    return -1;
}

// Carrega as contas armazenadas no arquivo
void carregarContas() {
    ifstream arquivo("contas.txt");

    if (arquivo.is_open()) {
        quantidadeContas = 0;

        while (quantidadeContas < MAX_CONTAS &&
               arquivo >> numeroConta[quantidadeContas]) {

            char separador;

            arquivo >> separador;

            getline(arquivo, nomeCliente[quantidadeContas], ';');

            getline(arquivo, cpf[quantidadeContas], ';');

            arquivo >> tipoConta[quantidadeContas];
            arquivo >> separador;

            arquivo >> saldo[quantidadeContas];
            arquivo >> separador;

            arquivo >> contaAtiva[quantidadeContas];

            quantidadeContas++;
        }

        arquivo.close();
    }
}

// Salva todas as contas no arquivo
void salvarContas() {
    ofstream arquivo("contas.txt");

    if (arquivo.is_open()) {
        for (int i = 0; i < quantidadeContas; i++) {
            arquivo << numeroConta[i] << ";"
                    << nomeCliente[i] << ";"
                    << cpf[i] << ";"
                    << tipoConta[i] << ";"
                    << saldo[i] << ";"
                    << contaAtiva[i] << endl;
        }

        arquivo.close();
    } else {
        cout << "Erro ao salvar as contas.\n";
    }
}

// Carrega o historico de movimentacoes
void carregarMovimentacoes() {
    ifstream arquivo("movimentacoes.txt");

    if (arquivo.is_open()) {
        quantidadeMovimentacoes = 0;

        while (quantidadeMovimentacoes < MAX_MOVIMENTACOES &&
               arquivo >> contaMovimentacao[quantidadeMovimentacoes]) {

            char separador;

            arquivo >> separador;
            arquivo >> tipoMovimentacao[quantidadeMovimentacoes];
            arquivo >> separador;
            arquivo >> valorMovimentacao[quantidadeMovimentacoes];

            quantidadeMovimentacoes++;
        }

        arquivo.close();
    }
}

// Salva todas as movimentacoes no arquivo
void salvarMovimentacoes() {
    ofstream arquivo("movimentacoes.txt");

    if (arquivo.is_open()) {
        for (int i = 0; i < quantidadeMovimentacoes; i++) {
            arquivo << contaMovimentacao[i] << ";"
                    << tipoMovimentacao[i] << ";"
                    << valorMovimentacao[i] << endl;
        }

        arquivo.close();
    } else {
        cout << "Erro ao salvar as movimentacoes.\n";
    }
}

// Registra uma movimentacao no historico
void registrarMovimentacao(int conta, char tipo, double valor) {
    if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
        contaMovimentacao[quantidadeMovimentacoes] = conta;
        tipoMovimentacao[quantidadeMovimentacoes] = tipo;
        valorMovimentacao[quantidadeMovimentacoes] = valor;

        quantidadeMovimentacoes++;

        salvarMovimentacoes();
    }
}

int main() {

    int numero, numeroPesquisa, numeroDestino;
    int indice, indiceOrigem, indiceDestino;
    double valor;

    // Recupera os dados salvos anteriormente
    carregarContas();
    carregarMovimentacoes();

    do {

        cout << "\n********************************\n";
        cout << "********** ADS BANK ************\n";
        cout << "********************************\n";
        cout << "**********   MENU   ************\n";
        cout << "********************************\n";

        cout << "1 - Cadastrar conta\n";
        cout << "2 - Listar contas\n";
        cout << "3 - Consultar conta\n";
        cout << "4 - Realizar deposito\n";
        cout << "5 - Realizar saque\n";
        cout << "6 - Realizar transferencia\n";
        cout << "7 - Exibir extrato\n";
        cout << "8 - Sair\n";

        cout << "********************************\n";
        cout << "Digite o numero da opcao desejada: ";
        cin >> numero;

        if (cin.fail()) {
            limparEntrada();
            cout << "\nErro: digite uma opcao numerica.\n";
            numero = 0;
            continue;
        }

        switch (numero) {

            // CADASTRAR CONTA
            case 1: {

                cout << "\n--- CADASTRO DE CONTA ---\n";

                if (quantidadeContas >= MAX_CONTAS) {
                    cout << "Limite maximo de contas atingido.\n";
                    break;
                }

                int novoNumero;

                do {
                    cout << "Numero da conta: ";
                    cin >> novoNumero;

                    if (cin.fail()) {
                        limparEntrada();
                        cout << "Erro: digite apenas numeros.\n";
                        novoNumero = 0;
                    } else if (novoNumero <= 0) {
                        cout << "Erro: o numero deve ser maior que zero.\n";
                    } else if (localizarConta(novoNumero) != -1) {
                        cout << "Erro: ja existe uma conta com esse numero.\n";
                        novoNumero = 0;
                    }

                } while (novoNumero <= 0);

                numeroConta[quantidadeContas] = novoNumero;

                // Limpa a quebra de linha antes de usar getline
                limparEntrada();

                cout << "Nome do titular: ";
                getline(cin, nomeCliente[quantidadeContas]);

                cout << "CPF do titular: ";
                getline(cin, cpf[quantidadeContas]);

                do {
                    cout << "Tipo da conta (1 - Corrente / 2 - Poupanca): ";
                    cin >> tipoConta[quantidadeContas];

                    if (cin.fail()) {
                        limparEntrada();
                        tipoConta[quantidadeContas] = 0;
                        cout << "Erro: digite 1 ou 2.\n";
                    } else if (tipoConta[quantidadeContas] != 1 &&
                               tipoConta[quantidadeContas] != 2) {
                        cout << "Erro: escolha 1 ou 2.\n";
                    }

                } while (tipoConta[quantidadeContas] != 1 &&
                         tipoConta[quantidadeContas] != 2);

                do {
                    cout << "Saldo inicial: R$ ";
                    cin >> saldo[quantidadeContas];

                    if (cin.fail()) {
                        limparEntrada();
                        saldo[quantidadeContas] = -1;
                        cout << "Erro: digite um valor numerico.\n";
                    } else if (saldo[quantidadeContas] < 0) {
                        cout << "Erro: o saldo nao pode ser negativo.\n";
                    }

                } while (saldo[quantidadeContas] < 0);

                contaAtiva[quantidadeContas] = true;

                quantidadeContas++;

                salvarContas();

                cout << "\nConta cadastrada com sucesso!\n";

                break;
            }

            // LISTAR CONTAS
            case 2: {

                cout << "\n--- LISTA DE CONTAS ---\n";

                if (quantidadeContas == 0) {
                    cout << "Nenhuma conta foi cadastrada.\n";
                } else {

                    for (int i = 0; i < quantidadeContas; i++) {

                        cout << "\n----------------------------\n";
                        cout << "Numero da conta: " << numeroConta[i] << endl;
                        cout << "Titular: " << nomeCliente[i] << endl;
                        cout << "CPF: " << cpf[i] << endl;

                        if (tipoConta[i] == 1) {
                            cout << "Tipo: Corrente\n";
                        } else {
                            cout << "Tipo: Poupanca\n";
                        }

                        cout << "Saldo: R$ " << saldo[i] << endl;

                        if (contaAtiva[i]) {
                            cout << "Situacao: Ativa\n";
                        } else {
                            cout << "Situacao: Inativa\n";
                        }
                    }
                }

                break;
            }

            // CONSULTAR CONTA
            case 3: {

                cout << "\n--- CONSULTA DE CONTA ---\n";

                cout << "Digite o numero da conta: ";
                cin >> numeroPesquisa;

                if (cin.fail()) {
                    limparEntrada();
                    cout << "Erro: numero de conta invalido.\n";
                    break;
                }

                indice = localizarConta(numeroPesquisa);

                if (indice == -1) {
                    cout << "Conta nao encontrada.\n";
                } else {

                    cout << "\nNumero da conta: " << numeroConta[indice] << endl;
                    cout << "Nome do titular: " << nomeCliente[indice] << endl;
                    cout << "CPF: " << cpf[indice] << endl;

                    if (tipoConta[indice] == 1) {
                        cout << "Tipo: Corrente\n";
                    } else {
                        cout << "Tipo: Poupanca\n";
                    }

                    cout << "Saldo: R$ " << saldo[indice] << endl;

                    if (contaAtiva[indice]) {
                        cout << "Situacao: Ativa\n";
                    } else {
                        cout << "Situacao: Inativa\n";
                    }
                }

                break;
            }

            // DEPOSITO
            case 4: {

                cout << "\n--- REALIZAR DEPOSITO ---\n";

                cout << "Numero da conta: ";
                cin >> numeroPesquisa;

                if (cin.fail()) {
                    limparEntrada();
                    cout << "Erro: numero de conta invalido.\n";
                    break;
                }

                indice = localizarConta(numeroPesquisa);

                if (indice == -1) {
                    cout << "Conta nao encontrada.\n";
                } else if (!contaAtiva[indice]) {
                    cout << "Esta conta esta inativa.\n";
                } else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
                    cout << "Limite de movimentacoes atingido.\n";
                } else {

                    do {
                        cout << "Valor do deposito: R$ ";
                        cin >> valor;

                        if (cin.fail()) {
                            limparEntrada();
                            valor = 0;
                            cout << "Erro: digite um valor numerico.\n";
                        } else if (valor <= 0) {
                            cout << "Erro: o valor deve ser maior que zero.\n";
                        }

                    } while (valor <= 0);

                    saldo[indice] = saldo[indice] + valor;

                    registrarMovimentacao(numeroConta[indice], 'D', valor);
                    salvarContas();

                    cout << "Deposito realizado com sucesso!\n";
                    cout << "Novo saldo: R$ " << saldo[indice] << endl;
                }

                break;
            }

            // SAQUE
            case 5: {

                cout << "\n--- REALIZAR SAQUE ---\n";

                cout << "Numero da conta: ";
                cin >> numeroPesquisa;

                if (cin.fail()) {
                    limparEntrada();
                    cout << "Erro: numero de conta invalido.\n";
                    break;
                }

                indice = localizarConta(numeroPesquisa);

                if (indice == -1) {
                    cout << "Conta nao encontrada.\n";
                } else if (!contaAtiva[indice]) {
                    cout << "Esta conta esta inativa.\n";
                } else {

                    do {
                        cout << "Valor do saque: R$ ";
                        cin >> valor;

                        if (cin.fail()) {
                            limparEntrada();
                            valor = 0;
                            cout << "Erro: digite um valor numerico.\n";
                        } else if (valor <= 0) {
                            cout << "Erro: o valor deve ser maior que zero.\n";
                        }

                    } while (valor <= 0);

                    if (valor > saldo[indice]) {
                        cout << "Saldo insuficiente para realizar o saque.\n";
                    } else if (quantidadeMovimentacoes >= MAX_MOVIMENTACOES) {
                        cout << "Limite de movimentacoes atingido.\n";
                    } else {

                        saldo[indice] = saldo[indice] - valor;

                        registrarMovimentacao(numeroConta[indice], 'S', valor);
                        salvarContas();

                        cout << "Saque realizado com sucesso!\n";
                        cout << "Novo saldo: R$ " << saldo[indice] << endl;
                    }
                }

                break;
            }

            // TRANSFERENCIA
            case 6: {

                cout << "\n--- REALIZAR TRANSFERENCIA ---\n";

                cout << "Numero da conta de origem: ";
                cin >> numeroPesquisa;

                if (cin.fail()) {
                    limparEntrada();
                    cout << "Erro: numero de conta invalido.\n";
                    break;
                }

                cout << "Numero da conta de destino: ";
                cin >> numeroDestino;

                if (cin.fail()) {
                    limparEntrada();
                    cout << "Erro: numero de conta invalido.\n";
                    break;
                }

                indiceOrigem = localizarConta(numeroPesquisa);
                indiceDestino = localizarConta(numeroDestino);

                if (indiceOrigem == -1 || indiceDestino == -1) {
                    cout << "Uma ou ambas as contas nao foram encontradas.\n";
                } else if (indiceOrigem == indiceDestino) {
                    cout << "Nao e possivel transferir para a mesma conta.\n";
                } else if (!contaAtiva[indiceOrigem] || !contaAtiva[indiceDestino]) {
                    cout << "Uma das contas esta inativa.\n";
                } else if (quantidadeMovimentacoes > MAX_MOVIMENTACOES - 2) {
                    cout << "Espaco insuficiente no historico de movimentacoes.\n";
                } else {

                    do {
                        cout << "Valor da transferencia: R$ ";
                        cin >> valor;

                        if (cin.fail()) {
                            limparEntrada();
                            valor = 0;
                            cout << "Erro: digite um valor numerico.\n";
                        } else if (valor <= 0) {
                            cout << "Erro: o valor deve ser maior que zero.\n";
                        }

                    } while (valor <= 0);

                    if (valor > saldo[indiceOrigem]) {
                        cout << "Saldo insuficiente para realizar a transferencia.\n";
                    } else {

                        saldo[indiceOrigem] = saldo[indiceOrigem] - valor;
                        saldo[indiceDestino] = saldo[indiceDestino] + valor;

                        // Saida da conta de origem
                        registrarMovimentacao(numeroConta[indiceOrigem], 'S', valor);

                        // Entrada na conta de destino
                        registrarMovimentacao(numeroConta[indiceDestino], 'D', valor);

                        salvarContas();

                        cout << "Transferencia realizada com sucesso!\n";
                        cout << "Novo saldo da origem: R$ " << saldo[indiceOrigem] << endl;
                        cout << "Novo saldo do destino: R$ " << saldo[indiceDestino] << endl;
                    }
                }

                break;
            }

            // EXTRATO
            case 7: {

                cout << "\n--- EXTRATO DA CONTA ---\n";

                cout << "Numero da conta: ";
                cin >> numeroPesquisa;

                if (cin.fail()) {
                    limparEntrada();
                    cout << "Erro: numero de conta invalido.\n";
                    break;
                }

                indice = localizarConta(numeroPesquisa);

                if (indice == -1) {
                    cout << "Conta nao encontrada.\n";
                } else {

                    cout << "\nTitular: " << nomeCliente[indice] << endl;
                    cout << "Numero da conta: " << numeroConta[indice] << endl;

                    cout << "\n--- MOVIMENTACOES ---\n";

                    bool encontrouMovimentacao = false;

                    for (int i = 0; i < quantidadeMovimentacoes; i++) {

                        if (contaMovimentacao[i] == numeroPesquisa) {

                            encontrouMovimentacao = true;

                            if (tipoMovimentacao[i] == 'D') {
                                cout << "Deposito/Transferencia recebida: R$ "
                                     << valorMovimentacao[i] << endl;
                            } else if (tipoMovimentacao[i] == 'S') {
                                cout << "Saque/Transferencia enviada: R$ "
                                     << valorMovimentacao[i] << endl;
                            }
                        }
                    }

                    if (!encontrouMovimentacao) {
                        cout << "Nenhuma movimentacao registrada.\n";
                    }

                    cout << "\nSaldo atual: R$ " << saldo[indice] << endl;
                }

                break;
            }

            // SAIR
            case 8:

                cout << "\nSistema encerrado. Obrigado por utilizar o ADS BANK!\n";
                break;

            default:

                cout << "\nOpcao invalida! Escolha uma opcao de 1 a 8.\n";
        }

    } while (numero != 8);

    return 0;
}
