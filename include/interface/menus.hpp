#ifndef MENUS_HPP_INCLUDED
#define MENUS_HPP_INCLUDED

#include "menu_gerador.hpp"
#include "menu_saves.hpp"
namespace Menus {

    // Opções do menu principal
    enum class OpcaoMenuPrincipal : char {
        Relatorios   = '1',
        GerarSistema = '2',
        Sair         = '3'
    };

} // namespace Menus 

#endif // MENUS_HPP_INCLUDED
