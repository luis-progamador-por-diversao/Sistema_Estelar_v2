#include <iostream>
#include <random>
#include <cstdlib>
#include "nlohmann/json.hpp"
#include "gerador_sistema.hpp"
#include "visualizador_sistema.hpp"

//.\output/Gerador.exe

int main() {
    int seed = 0;

    while (true) {
        char escolha = '\0';

        std::cout << "\n\nPrograma para criar sistemas de estrelas:\n\n";
        std::cout << "\nDigite [1] para escolher uma seed.\nDigite [2] para uma seed aleatoria.\n";
        std::cin.get(escolha);
        std::cin.ignore();

        if (escolha == '1') {
            std::cout << "\nDigite a seed: ";
            if (std::cin >> seed) {
                break;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        } else if (escolha == '2') {
            std::random_device rd;
            seed = static_cast<int>(rd());
            break;
        }

        std::system("cls");
        std::cout << "\n\033[31mNumero informado e invalido\033[0m";
        std::cout << "\nTente outra vez:\n";
    }

    std::cout << "\nSeed = " << seed << "\n";

    // Geração procedural completa
    const SistemaEstelar sistema = GeradorSistema::gerar(seed);

    // Renderização do relatório astronômico no terminal
    VisualizadorSistema::imprimir_relatorio(
        std::cout,
        sistema.nome,
        sistema.estrelas,
        sistema.no_raiz.get(),
        sistema.distancia_binaria);

    std::cout << "\n\n\033[32mPrograma finalizado\033[0m\n";
    return 0;
}

