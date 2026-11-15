#include <windows.h>
#include "menu.hpp"
#include "gerador_sistema.hpp"
#include "visualizador_sistema.hpp"
//||\n\033[0m?
//  .\output/Gerador.exe

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Menus::limparTela();
    bool rodando = true;

    while(rodando){
        char menu_principal = '\0';

        std::cout << "\nMenu_principal\n";

        std::cout << "\n[1] Relatorios .txt.";
        std::cout << "\n[2] Gerar novo sistema.";
        std::cout << "\n[3] Fechar programa.";

        std::cin >> menu_principal;

        if (menu_principal == '1'){

            Menus::limparTela();
            rodando = Menus::relatorios_salvos();

        } else if (menu_principal == '2'){
            Menus::limparTela();
            rodando = Menus::geracao_procedural();

        } else if (menu_principal == '3'){

            rodando = false;
        } 
         
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        //BMenus::limparTela();
    }
    
    //Menus::limparTela();
    
    std::cout << "\n\n\033[32mPrograma finalizado\033[0m\n";
    return 0;
}

