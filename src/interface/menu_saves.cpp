#include "interface/menu_saves.hpp"
#include <fstream>
#include <stdexcept>
#include <vector>
#include <system_error>

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
