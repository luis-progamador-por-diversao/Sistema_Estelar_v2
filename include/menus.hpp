#ifndef MENUS_HPP_INCLUDED
#define MENUS_HPP_INCLUDED

#include <iostream>
#include <string>
#include <cstddef>

namespace Menus {

    // --- Enumerações Fortemente Tipadas (enum class) dos Menus ---

    // Opções do menu principal
    enum class OpcaoMenuPrincipal : char {
        Relatorios   = '1',
        GerarSistema = '2',
        Sair         = '3'
    };

    // Opções do menu de configuração de seed
    enum class OpcaoMenuSeed : char {
        Manual    = '1',
        Aleatoria = '2',
        Voltar    = '3',
        Sair      = '4'
    };

    // Opções do menu exibido após a geração procedural de um sistema
    enum class OpcaoMenuPosGeracao : char {
        Salvar     = '1',
        GerarOutro = '2',
        Voltar     = '3',
        Sair       = '4'
    };

    // Opções do menu de ações para um relatório salvo carregado
    enum class OpcaoAcaoRelatorio : char {
        Excluir       = '1',
        CarregarOutro = '2',
        Voltar        = '3',
        Sair          = '4'
    };

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

    // Menu da geração procedural
    [[nodiscard]] bool geracao_procedural();

    // Menu para visualizar os relatórios salvos
    [[nodiscard]] bool relatorios_salvos();

} // namespace Menus 

#endif // MENUS_HPP_INCLUDED
