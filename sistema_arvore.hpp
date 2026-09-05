#ifndef SISTEMA_EM_ARVORE_HPP
#define SISTEMA_EM_ARVORE_HPP

//g++ *.cpp -o main.exe
//.\main.exe

#include <memory>
#include "objetos_celestes.hpp"

//No, alem de comprir sua função de ser o no de uma árvore,
//também representa um subsistema de sistemas estelares multiplos.
struct No{

    std::string nome = "";
    Orbita orbt;
    No * volta = nullptr;
    std::unique_ptr<No> sub_menor;
    std::unique_ptr<No> sub_maior;
    double massa_sub = 0.0;
    double membro_dist = 0.0;

    //Construtores, impedem que a árvore seja montada de forma inadequada.
    No(Estrela &star1, const int &star1_id, Estrela &star2, const int &star2_id);
    No(std::unique_ptr<No> &subconj, Estrela &star, const int &star_id);
    No(std::unique_ptr<No> &subconj1, std::unique_ptr<No> &subconj2);

    void get_estrelas_id(int &major_id, int &menor_id)const;

private:
    int id_maior_estrela = -1;
    int id_menor_estrela = -1;
};

//Auxiliar, e com base nele que o código consegue classificar o sistema.
struct Pseudo_no{
    No * no_original = nullptr;
    double mass;
    bool utilizado = false;

    Pseudo_no(std::unique_ptr<No> &original, const double massa);
};
#endif
