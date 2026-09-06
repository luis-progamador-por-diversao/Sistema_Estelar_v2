#include "sistema_arvore.hpp"

//No construtores.
No::No(Estrela &star1, const int &star1_id, Estrela &star2, const int &star2_id, const double &dist){

    this->membro_dist = dist;
    this->massa_sub = star1.get_massa() + star2.get_massa();

    if(star1.get_massa() >= star2.get_massa()){
        this->id_maior_estrela = star1_id;
        this->id_menor_estrela = star2_id;

        return;
    }

    this->id_maior_estrela = star2_id;
    this->id_menor_estrela = star1_id;
}
No::No(std::unique_ptr<No> &subconj, Estrela &star, const int &star_id, const double &dist){

    this->membro_dist = dist;
    this->massa_sub = subconj->massa_sub + star.get_massa();

    if(subconj->massa_sub > star.get_massa()){
        this->sub_maior = std::move(subconj);
        this->id_menor_estrela = star_id;

        return;
    }

    this->sub_menor = std::move(subconj);
    this->id_maior_estrela = star_id;
}
No::No(std::unique_ptr<No> &subconj1, std::unique_ptr<No> &subconj2, const double &dist){

    this->membro_dist = dist;
    this->massa_sub = subconj1->massa_sub + subconj2->massa_sub;

    if(subconj1->massa_sub > subconj2->massa_sub){
        this->sub_maior = std::move(subconj1);
        this->sub_menor = std::move(subconj2);

        return;
    }

    this->sub_maior = std::move(subconj2);
    this->sub_menor = std::move(subconj1);
}

void No::get_estrelas_id(int &major_id, int &menor_id)const{

    major_id = this->id_maior_estrela;
    menor_id = this->id_menor_estrela;

}

//Pseudo_no construtor.
Pseudo_no::Pseudo_no(std::unique_ptr<No> &original, const double massa){
    this->no_original = original.get();
    this->mass = massa;
}
