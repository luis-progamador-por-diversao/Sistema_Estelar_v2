#include "menus.hpp"
#include "gerador_sistema.hpp"
#include "visualizador_sistema.hpp"
#include <fstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <system_error>

void Menus::limparTela() {

    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void Menus::animacao_gerador() {

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

bool Menus::geracao_procedural() {

    animacao_gerador();

    while(true){
        limparTela(); 

        int seed = 0;
   
        while(true) { 
            char escolha_seed = '\0';

            std::cout << "\n\nGerando Sistema Estelar:\n\n";
            std::cout << "\n[1] Escolher uma seed.";
            std::cout << "\n[2] Seed aleatoria.";
            std::cout << "\n[3] Voltar para menu principal.";
            std::cout << "\n[4] Fechar programa.";
            std::cin >> escolha_seed;

            if (escolha_seed == '1') {
                std::cout << "\nDigite a seed: ";
                while (!(std::cin >> seed)) {
                    std::cout << "\n\033[31mSeed invalida! Digite apenas números inteiros:\033[0m ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;

            } else if (escolha_seed == '2'){
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

        if (escolha == '1') {
            std::ofstream ofs;
            std::cout << "\nSalvando...\n";

            ofs.open("sistemas_gerados/Sistema " + sistema.nome + ".txt");

            if (ofs.is_open()) {

                VisualizadorSistema::relatorio(ofs,
                sistema.seed,
                sistema.nome,
                sistema.estrelas,
                sistema.no_raiz.get(),
                sistema.idade);

                std::cout << "\nSistema: " << sistema.nome << " salvo com sucesso.";
                std::this_thread::sleep_for(std::chrono::seconds(1)); 

            } else {
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
}

[[nodiscard]] std::vector<std::string> extrair_nomes(const std::filesystem::path& pasta = "sistemas_gerados") {
    std::error_code ec;

    if (!std::filesystem::exists(pasta, ec)) {
        if (ec) {
            throw std::runtime_error("Erro ao acessar o sistema de arquivos: " + ec.message());
        }
        throw std::runtime_error("A pasta '" + pasta.string() + "' nao foi encontrada.");
    }

    if (!std::filesystem::is_directory(pasta, ec)) {
        if (ec) {
            throw std::runtime_error("Erro ao verificar o diretorio: " + ec.message());
        }
        throw std::runtime_error("O caminho '" + pasta.string() + "' nao e um diretorio valido.");
    }

    std::filesystem::directory_iterator it(pasta, ec);
    if (ec) {
        throw std::runtime_error("Nao foi possivel ler o conteudo da pasta '" + pasta.string() + "': " + ec.message());
    }

    std::vector<std::string> nomes;

    for (const std::filesystem::directory_iterator end; it != end; it.increment(ec)) {
        if (ec) {
            throw std::runtime_error("Erro durante a leitura dos arquivos: " + ec.message());
        }

        const auto& entrada = *it;

        if (entrada.is_regular_file(ec) && !ec && entrada.path().extension() == ".txt") {
            nomes.emplace_back(entrada.path().filename().string());
        }
    }

    return nomes;
}

void exibir_sistemas(const std::vector<std::string> & sistemas) {
    std::cout << "=== SISTEMAS ===\n\n";

    std::cout << "=============================================================================\n";
    for (std::size_t i = 0; i < sistemas.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << sistemas[i] << '\n';
        std::cout << "=============================================================================\n";
    }
}

bool carregar_sistema(const std::string nome) {
    std::ifstream ifs;

    ifs.open("sistemas_gerados/" + nome);

    if (!ifs.is_open()) {
        std::cerr << nome << " não encontrado.\n";
        return false; 
    }

    std::string linha;
    
    while (std::getline(ifs, linha)) {
        std::cout << linha << '\n';
    }

    ifs.close(); 
    return true;
}

bool Menus::relatorios_salvos() {
    limparTela();
    
    while (true) {
        std::vector<std::string> arquivos;
        
        try {
            arquivos = extrair_nomes();

            if (arquivos.empty()) {
                char e = '\0';

                std::cout << "\n[Aviso] Nenhum arquivo de sistema (.txt) encontrado na pasta.";
                std::cout << "\nDigite qualquer tecla para voltar ao menu princial.";
                std::cin >> e;
                std::cin.clear();
                limparTela();
                return true;
            }

            exibir_sistemas(arquivos);

        } catch (const std::exception& e) {
            std::cerr << "\n[Erro ao listar sistemas]: " << e.what() << '\n';
            std::cerr << "Verifique se a pasta existe ou se o programa tem permissao de leitura.\n\n";
            return false;
        }

        std::size_t sistema_ID = 0ULL;

        std::cout << "\nDigite o número entre cochetes para carregar um sistema.\n";
        std::cout << "[0] Para voltar ao menu principal.\n";
         
        while (!(std::cin >> sistema_ID)) {
            std::cout << "\n\033[31mEscolha invalida! Digite apenas números inteiros:\033[0m ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (sistema_ID == 0) {
            limparTela();
            return true;

        } else if (sistema_ID >= 1 && sistema_ID <= arquivos.size()) { 
            limparTela();

            bool carregou = carregar_sistema(arquivos[sistema_ID - 1]);

            if (!carregou) {
                return true;
            }

            char sub_escolha = '\0';

            std::cout << "\n[1] Excluir sistema.";
            std::cout << "\n[2] Carregar outro sistema.";
            std::cout << "\n[3] Voltar ao menu principal.";
            std::cout << "\n[4] Fechar programa.\nEscolha: ";
            std::cin >> sub_escolha;

            if (sub_escolha == '1') {
                std::filesystem::path arquivo_alvo = std::filesystem::path("sistemas_gerados") / arquivos[sistema_ID - 1];
                
                if (std::filesystem::remove(arquivo_alvo)) {
                    std::cout << "\nArquivo excluído com sucesso!\n";
                } else {
                    std::cout << "\nO arquivo não foi encontrado.\n";
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));

            } else if (sub_escolha == '2') {
                continue;
            } else if (sub_escolha == '3') {
                limparTela();
                return true;
            } else if (sub_escolha == '4') {
                return false;
            }

        } else {
            std::cout << "\n\033[31mOpção fora do intervalo válido!\033[0m\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        limparTela();
    }
}
