#include "interface/menu_gerador.hpp"
#include "interface/visualizador_sistema.hpp"
#include "geracao/gerador_sistema.hpp"
#include <random>
#include <fstream>

bool Menus::geracao_procedural() {

    while (true) {
        limparTela(); 

        int seed = 0;
   
        std::cout << "\n\nGerando Sistema Estelar:\n\n";
        std::cout << "\n[1] Escolher uma seed.";
        std::cout << "\n[2] Seed aleatoria.";
        std::cout << "\n[3] Voltar para menu principal.";
        std::cout << "\n[4] Fechar programa.\n";

        char escolha_seed = Menus::lerCharValido("1234", "\nEscolha uma opcao: ");
        auto opcao_seed = static_cast<OpcaoMenuSeed>(escolha_seed);

        if (opcao_seed == OpcaoMenuSeed::Manual) {
            seed = Menus::lerInteiro("\nDigite a seed (numero inteiro): ");
        } else if (opcao_seed == OpcaoMenuSeed::Aleatoria) {
            std::random_device random;
            seed = static_cast<int>(random());
        } else if (opcao_seed == OpcaoMenuSeed::Voltar) {
            return true;
        } else if (opcao_seed == OpcaoMenuSeed::Sair) {
            return false;
        }
        
        limparTela();
        // Geração procedural completa
        const SistemaEstelar sistema = GeradorSistema::gerar(seed);

        // Renderização do relatório astronômico no terminal
        VisualizadorSistema::relatorio(
            std::cout,
            sistema.seed,
            sistema.nome, 
            sistema.estrelas,
            sistema.no_raiz.get(),
            sistema.idade);

        std::cout << "\n[1] Salvar sistema como relatório .txt.";
        std::cout << "\n[2] Gerar outro sistema.";
        std::cout << "\n[3] Voltar para menu principal.";
        std::cout << "\n[4] Fechar programa.\n";

        char escolha = Menus::lerCharValido("1234", "\nEscolha uma opcao: ");
        auto opcao_pos = static_cast<OpcaoMenuPosGeracao>(escolha);

        switch (opcao_pos) {
            case OpcaoMenuPosGeracao::Salvar: {
                std::ofstream ofs;
                std::cout << "\nSalvando...\n";

                std::error_code ec;
                std::filesystem::create_directories("sistemas_gerados", ec);

                ofs.open("sistemas_gerados/Sistema " + sistema.nome + ".txt");

                if (ofs.is_open()) {
                    VisualizadorSistema::relatorio(
                        ofs,
                        sistema.seed,
                        sistema.nome,
                        sistema.estrelas,
                        sistema.no_raiz.get(),
                        sistema.idade);

                    std::cout << "\nSistema: " << sistema.nome << " salvo com sucesso.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1)); 
                } else {
                    std::cout << "\nOcorreu um erro ao salvar o sistema.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(1)); 
                }

                ofs.close();
                break;
            }

            case OpcaoMenuPosGeracao::GerarOutro:
                continue;

            case OpcaoMenuPosGeracao::Voltar:
                return true;

            case OpcaoMenuPosGeracao::Sair:
                return false;
        }
    }
}
