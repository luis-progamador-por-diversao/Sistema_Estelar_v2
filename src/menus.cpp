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
#include <random>

namespace {

// Remove espaços em branco do início e do final da string
std::string trim(const std::string& str) {
    const auto inicio = str.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return "";
    }
    const auto fim = str.find_last_not_of(" \t\r\n");
    return str.substr(inicio, fim - inicio + 1);
}

} // namespace anonymous

// --- Helpers Reutilizáveis de Entrada ---

std::size_t Menus::lerOpcao(std::size_t min, std::size_t max, const std::string& prompt) {
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }

        std::string linha;
        if (!std::getline(std::cin, linha)) {
            std::cin.clear();
            continue;
        }

        std::string limpa = trim(linha);
        if (limpa.empty()) {
            std::cout << "\n\033[31mEntrada vazia! Digite um valor entre "
                      << min << " e " << max << ".\033[0m\n";
            continue;
        }

        if (limpa[0] == '-') {
            std::cout << "\n\033[31mOpcao fora do intervalo! Digite um valor entre "
                      << min << " e " << max << ".\033[0m\n";
            continue;
        }

        try {
            std::size_t pos = 0;
            unsigned long long valor = std::stoull(limpa, &pos);
            if (pos == limpa.size()) {
                if (valor >= min && valor <= max) {
                    return static_cast<std::size_t>(valor);
                }
                std::cout << "\n\033[31mOpcao fora do intervalo! Digite um valor entre "
                          << min << " e " << max << ".\033[0m\n";
                continue;
            }
        } catch (const std::exception&) {
            // Entrada não pôde ser convertida para número
        }

        std::cout << "\n\033[31mEntrada invalida! Digite apenas numeros inteiros.\033[0m\n";
    }
}

char Menus::lerCharValido(const std::string& opcoesValidas, const std::string& prompt) {
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }

        std::string linha;
        if (!std::getline(std::cin, linha)) {
            std::cin.clear();
            continue;
        }

        std::string limpa = trim(linha);

        if (limpa.size() == 1 && opcoesValidas.find(limpa[0]) != std::string::npos) {
            return limpa[0];
        }

        std::cout << "\n\033[31mOpcao invalida! Opcoes validas: [";
        for (std::size_t i = 0; i < opcoesValidas.size(); ++i) {
            std::cout << opcoesValidas[i];
            if (i + 1 < opcoesValidas.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]\033[0m\n";
    }
}

int Menus::lerInteiro(const std::string& prompt) {
    while (true) {
        if (!prompt.empty()) {
            std::cout << prompt;
        }

        std::string linha;
        if (!std::getline(std::cin, linha)) {
            std::cin.clear();
            continue;
        }

        std::string limpa = trim(linha);
        if (limpa.empty()) {
            std::cout << "\n\033[31mEntrada vazia! Digite um numero inteiro.\033[0m\n";
            continue;
        }

        try {
            std::size_t pos = 0;
            int valor = std::stoi(limpa, &pos);
            if (pos == limpa.size()) {
                return valor;
            }
        } catch (const std::exception&) {
            // Conversão falhou ou número fora da faixa de int
        }

        std::cout << "\n\033[31mEntrada invalida! Digite apenas numeros inteiros.\033[0m\n";
    }
}

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
        float progresso = static_cast<float>(i) / larguraBarra;
        int porcentagem = static_cast<int>(progresso * 100);

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

        std::cout << "=====================================\n";
        std::cout << "   SISTEMA GERADO COM SUCESSO  \n";
        std::cout << "=====================================\n\n";

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

[[nodiscard]] static std::vector<std::string> extrair_nomes(const std::filesystem::path& pasta = "sistemas_gerados") {
    std::error_code ec;

    if (!std::filesystem::exists(pasta, ec)) {
        if (ec) {
            throw std::runtime_error("Erro ao acessar o sistema de arquivos: " + ec.message());
        }
        return {}; // Pasta ainda não existe, retorna lista vazia
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

static void exibir_sistemas(const std::vector<std::string>& sistemas) {
    std::cout << "=== SISTEMAS SALVOS ===\n\n";

    std::cout << "=============================================================================\n";
    for (std::size_t i = 0; i < sistemas.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << sistemas[i] << '\n';
        std::cout << "=============================================================================\n";
    }
}

[[nodiscard]] static bool carregar_sistema(const std::string& nome) {
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
                std::cout << "\n[Aviso] Nenhum arquivo de sistema (.txt) encontrado na pasta.\n";
                static_cast<void>(Menus::lerCharValido("vV", "\nDigite 'v' para voltar ao menu principal: "));
                limparTela();
                return true;
            }

            exibir_sistemas(arquivos);

        } catch (const std::exception& e) {
            std::cerr << "\n[Erro ao listar sistemas]: " << e.what() << '\n';
            std::cerr << "Verifique se a pasta existe ou se o programa tem permissao de leitura.\n\n";
            return true;
        }

        std::cout << "\nDigite o número entre colchetes para carregar um sistema.\n";
        std::cout << "[0] Para voltar ao menu principal.\n";

        std::size_t sistema_ID = Menus::lerOpcao(0, arquivos.size(), "\nEscolha uma opcao: ");

        if (sistema_ID == 0) {
            limparTela();
            return true;
        }

        limparTela();
        bool carregou = carregar_sistema(arquivos[sistema_ID - 1]);

        if (!carregou) {
            return true;
        }

        std::cout << "\n[1] Excluir sistema.";
        std::cout << "\n[2] Carregar outro sistema.";
        std::cout << "\n[3] Voltar ao menu principal.";
        std::cout << "\n[4] Fechar programa.\n";

        char sub_escolha = Menus::lerCharValido("1234", "\nEscolha uma opcao: ");
        auto opcao_acao = static_cast<OpcaoAcaoRelatorio>(sub_escolha);

        switch (opcao_acao) {
            case OpcaoAcaoRelatorio::Excluir: {
                std::filesystem::path arquivo_alvo = std::filesystem::path("sistemas_gerados") / arquivos[sistema_ID - 1];
                std::error_code ec;

                if (std::filesystem::remove(arquivo_alvo, ec)) {
                    std::cout << "\n\033[32mArquivo excluído com sucesso!\033[0m\n";
                } else {
                    std::cout << "\n\033[31mNão foi possível excluir o arquivo: " << ec.message() << "\033[0m\n";
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            }

            case OpcaoAcaoRelatorio::CarregarOutro:
                limparTela();
                continue;

            case OpcaoAcaoRelatorio::Voltar:
                limparTela();
                return true;

            case OpcaoAcaoRelatorio::Sair:
                return false;
        }

        limparTela();
    }
}
