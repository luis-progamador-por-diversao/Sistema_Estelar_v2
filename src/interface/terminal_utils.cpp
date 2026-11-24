#include "interface/terminal_utils.hpp"

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
    std::cout << "\033[H\033[2J\033[3J" << std::flush;
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
