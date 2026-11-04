#ifndef MENUS_HPP_INCLUDED
#define MENUS_HPP_INCLUDED

#include <iostream>
#include <limits>

namespace Menus {
    void limparTela();

    // Função que renderiza a barra de carregamento animada
    void animacao_gerador();

    // Menu da geração procedural
    [[nodiscard]] bool geracao_procedural() noexcept;

} // namespace Menus 

#endif // MENUS_HPP_INCLUDED