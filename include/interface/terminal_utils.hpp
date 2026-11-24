#ifndef TERMINAL_UTILS_HPP_INCLUDED
#define TERMINAL_UTILS_HPP_INCLUDED

#include <filesystem>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <cstddef>

namespace Menus {
    
    // --- Helpers Reutilizáveis de Leitura de Entrada ---

    // Lê uma opção numérica garantidamente no intervalo fechado [min, max]
    [[nodiscard]] std::size_t lerOpcao(std::size_t min, std::size_t max, const std::string& prompt = "");

    // Lê um caractere garantidamente pertencente à string de opções válidas
    [[nodiscard]] char lerCharValido(const std::string& opcoesValidas, const std::string& prompt = "");

    // Lê um número inteiro (positivo ou negativo) com validação completa
    [[nodiscard]] int lerInteiro(const std::string& prompt = "");

    // --- Funções dos Menus ---

    void limparTela();

    // Função que renderiza a barra de carregamento animada
    void animacao_gerador();

} // namespace Menus

#endif  // TERMINAL_UTILS_HPP_INCLUDED