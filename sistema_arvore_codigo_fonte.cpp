#include "sistema_arvore.hpp"

//No construtores.
No::No(){}
No::No(Estrela & star1, Estrela & star2, const double & dist){

    this->massa_subsistema = star1.get_massa() + star2.get_massa();
    this->membro_dist = dist;
    this->modificado = false;

    if(star1.get_massa() >= star2.get_massa()){
        this->estrela_maior = &star1;
        this->estrela_menor = &star2;

        return;
    }

    this->estrela_maior = &star2;
    this->estrela_menor = &star1;
}
No::No(No & subconj, Estrela & star, const double & dist){

    this->massa_subsistema = subconj.massa_subsistema + star.get_massa();
    this->membro_dist = dist;
    this->modificado = false;

    if(subconj.massa_subsistema > star.get_massa()){
        this->subsistema_maior = &subconj;
        this->estrela_menor = &star;

        return;
    }

    this->subsistema_menor = &subconj;
    this->estrela_maior = &star;
}
No::No(No & subconj1, No & subconj2, const double & dist){

    this->massa_subsistema = subconj1.massa_subsistema + subconj2.massa_subsistema;
    this->membro_dist = dist;
    this->modificado = false;

    if(subconj1.massa_subsistema > subconj2.massa_subsistema){
        this->subsistema_maior = &subconj1;
        this->subsistema_menor = &subconj2;

        return;
    }

    this->subsistema_maior = &subconj2;
    this->subsistema_menor = &subconj1;
}
