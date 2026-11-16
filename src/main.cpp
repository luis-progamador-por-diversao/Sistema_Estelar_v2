#include <windows.h>
#include <iostream>
#include "menus.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Menus::limparTela();
    bool rodando = true;

    while (rodando) {
        std::cout << "\n=== Menu Principal ===\n";
        std::cout << "\n[1] Relatorios .txt.";
        std::cout << "\n[2] Gerar novo sistema.";
        std::cout << "\n[3] Fechar programa.\n";

        char escolha = Menus::lerCharValido("123", "\nEscolha uma opcao: ");
        auto opcao = static_cast<Menus::OpcaoMenuPrincipal>(escolha);

        switch (opcao) {
            case Menus::OpcaoMenuPrincipal::Relatorios:
                Menus::limparTela();
                rodando = Menus::relatorios_salvos();
                break;

            case Menus::OpcaoMenuPrincipal::GerarSistema:
                Menus::limparTela();
                rodando = Menus::geracao_procedural();
                break;

            case Menus::OpcaoMenuPrincipal::Sair:
                rodando = false;
                break;
        }

        Menus::limparTela();
    }

    Menus::limparTela();
    std::cout << "\n\033[32mPrograma finalizado\033[0m\n";
    return 0;
}
