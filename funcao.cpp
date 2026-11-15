#include <iostream>
#include <fstream> // Necessário para manipular arquivos
#include <string>

int main() {
    // 1. Abre o arquivo para leitura
    std::ifstream arquivo("dados.txt");

    // 2. Verifica se o arquivo foi aberto com sucesso
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return 1;
    }

    std::string linha;

    // 3. Lê o arquivo linha por linha até o final
    while (std::getline(arquivo, linha)) {
        std::cout << linha << std::endl;
    }

    // 4. Fecha o arquivo (boa prática)
    arquivo.close();

    return 0;
}
