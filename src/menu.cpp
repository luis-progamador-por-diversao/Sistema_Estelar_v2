#include "menu.hpp"
#include "gerador_sistema.hpp"
#include "visualizador_sistema.hpp"
#include <fstream>
#include <cstdlib>
#include <thread>
#include <chrono>

void Menus::limparTela(){

    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Menus::animacao_gerador(){

    limparTela();
    std::cout << "\n\033[36m[SISTEMA DE ESTRELAS]\033[0m Inicializando motores de geracao...\n\n";

    int larguraBarra = 30; 

    for (int i = 0; i <= larguraBarra; ++i) {
        float progresso = (float)i / larguraBarra;
        int porcentagem = progresso * 100;

        std::cout << "\rMapeando quadrante galactico: [";
        
        for (int j = 0; j < i; ++j) {
            std::cout << "\033[32m#\033[0m"; 
        }
      
        for (int j = i; j < larguraBarra; ++j) {
            std::cout << " ";
        }
        
        std::cout << "] " << porcentagem << "%" << std::flush;

        // Pausa a execução por 60 milissegundos para dar o efeito visual
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
    
    std::cout << "\n\n\033[32m[SUCESSO]\033[0m Universo gerado com estabilidade.\n\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); 
}

bool Menus::geracao_procedural() noexcept {

    animacao_gerador();

    while(true){
        limparTela(); 

        int seed = 0;
   
        while(true){ 
            char escolha_seed = '\0';

            std::cout << "\n\nGerando Sistema Estelar:\n\n";
            std::cout << "\n[1] Escolher uma seed.";
            std::cout << "\n[2] Seed aleatoria.";
            std::cout << "\n[3] Voltar para menu principal.";
            std::cout << "\n[4] Fechar programa.";
            std::cin >> escolha_seed;

            if(escolha_seed == '1'){
                std::cout << "\nDigite a seed: ";
                while (!(std::cin >> seed)) {
                    std::cout << "\n\033[31mSeed invalida! Digite apenas números inteiros:\033[0m ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;

            } else if(escolha_seed == '2'){
                std::random_device random;
                seed = random();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;

            } else if (escolha_seed == '3'){
                return true;
            } else if (escolha_seed == '4'){
                return false;
            }
            
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            limparTela();
            
            std::cout << "\n\033[31mNúmero informado e inválido\033[0m";
            std::cout << "\nTente outra vez:\n";
        }

        std::cout << "=====================================\n";
        std::cout << "   SISTEMA GERADO COM SUCESSO  \n";
        std::cout << "=====================================\n\n";

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

        char escolha = '\0';

        std::cout << "\n[1] Salvar sistema como relatório .txt.";
        std::cout << "\n[2] Gerar outro sistema.";
        std::cout << "\n[3] Voltar para menu principal.";
        std::cout << "\n[4] Fechar programa.";
        std::cin >> escolha;

        if (escolha == '1'){
            std::ofstream ofs;
            std::cout << "\nSalvando...\n";

            ofs.open("sistemas_gerados/Sistema " + sistema.nome + ".txt");

            if(ofs.is_open()){

                VisualizadorSistema::relatorio(ofs,
                sistema.seed,
                sistema.nome,
                sistema.estrelas,
                sistema.no_raiz.get(),
                sistema.idade);

                std::cout << "\nSistema: " << sistema.nome << " salvo com sucesso.";
                std::this_thread::sleep_for(std::chrono::seconds(1)); 

            }else{
                std::cout << "\nOcorreu um erro ao salvar o sistema";
                std::this_thread::sleep_for(std::chrono::seconds(1)); 

            }

            ofs.close();
        } else if (escolha == '2'){
            continue;

        } else if (escolha == '3'){
            return true;

        } else if (escolha == '4'){
            return false;
        } 

        escolha = '\0';
    }

    return true;
}

