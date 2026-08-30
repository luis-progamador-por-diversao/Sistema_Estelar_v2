#ifndef SISTEMA_EM_ARVORE_HPP
#define SISTEMA_EM_ARVORE_HPP

#include <string>
#include "objetos_celestes.hpp"

//No, ele representa os subsistemas de um sistema planetario de multiplas estrelas.
struct No{
    
    std::string subs_nome;
    Orbita orbt;
    No * subsistema_menor;
    No * subsistema_maior;
    Estrela * estrela_maior = nullptr;
    Estrela * estrela_menor = nullptr;
    double massa_subsistema = 0.0;
    double membro_dist = 0.0; 
    bool modificado = false;

    //Construtores, impedem que a árvore seja montada de forma inadequada.
    No();
    No(Estrela & star1, Estrela & star2,const double & dist);
    No(No & subconj, Estrela & star, const double & dist);
    No(No & subconj1, No & subconj2, const double & dist);
};
    
#endif
