#ifndef MENUS_GERADOR_HPP_INCLUDED
#define MENUS_GERADOR_HPP_INCLUDED

#include "terminal_utils.hpp"

namespace Menus {

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

    // Menu da geração procedural
    [[nodiscard]] bool geracao_procedural();

} // namespace Menus

#endif // MENUS_GERADOR_HPP_INCLUDED