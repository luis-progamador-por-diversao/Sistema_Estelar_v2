#ifndef SUBSISTEMA_NO_HPP_INCLUDED
#define SUBSISTEMA_NO_HPP_INCLUDED

#include <memory>
#include <string>
#include <utility>
#include "orbita.hpp"
#include "estrela.hpp"

// Representa um nó na árvore hierárquica de subsistemas múltiplos
struct SubsistemaNo {
    std::string nome;
    Orbita orbt;
    SubsistemaNo* volta = nullptr; // Ponteiro observador para o nó ascendente (pai)

    std::unique_ptr<SubsistemaNo> sub_maior; // Subsistema filho de maior massa
    std::unique_ptr<SubsistemaNo> sub_menor; // Subsistema filho de menor massa

    double massa_sub = 0.0;   // Massa total combinada deste subsistema em kg
    double membro_dist = 0.0; // Distância orbital característica entre os membros

    // Construtor para par estelar folha (duas estrelas individuais)
    SubsistemaNo(const Estrela &star1, int star1_id,
                 const Estrela &star2, int star2_id,
                 double dist);

    // Construtor para subsistema hierárquico + estrela individual
    SubsistemaNo(std::unique_ptr<SubsistemaNo> subconj,
                 const Estrela &star, int star_id,
                 double dist);

    // Construtor para dois subsistemas hierárquicos
    SubsistemaNo(std::unique_ptr<SubsistemaNo> subconj1,
                 std::unique_ptr<SubsistemaNo> subconj2,
                 double dist);

    // Obtenção dos IDs das estrelas folhas
    void get_estrelas_id(int &major_id, int &menor_id) const noexcept;
    [[nodiscard]] std::pair<int, int> get_estrelas_id() const noexcept;

    // Métodos de consulta de estrutura
    [[nodiscard]] bool eh_folha_binaria() const noexcept;
    [[nodiscard]] bool eh_raiz() const noexcept;

private:
    int id_maior_estrela = -1;
    int id_menor_estrela = -1;
};

// Estrutura auxiliar para classificação de luminosidade no padrão WDS
struct PseudoNo {
    SubsistemaNo* no_original = nullptr;
    double lumi = 0.0;
    bool utilizado = false;

    PseudoNo() = default;
    PseudoNo(SubsistemaNo* original, double luz) noexcept;
    PseudoNo(const std::unique_ptr<SubsistemaNo> &original, double luz) noexcept;
};

#endif // SUBSISTEMA_NO_HPP_INCLUDED

