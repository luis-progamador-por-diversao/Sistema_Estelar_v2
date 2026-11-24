#ifndef MENUS_SAVES_HPP_INCLUDED
#define MENUS_SAVES_HPP_INCLUDED

#include "terminal_utils.hpp"

namespace Menus {

    // Opções do menu de ações para um relatório salvo carregado
    enum class OpcaoAcaoRelatorio : char {
        Excluir       = '1',
        CarregarOutro = '2',
        Voltar        = '3',
        Sair          = '4'
    };

    // Menu para visualizar os relatórios salvos
    [[nodiscard]] bool relatorios_salvos();

} // namespace Menus 

#endif // MENUS_SAVES_HPP_INCLUDED