#include "nucleo/subsistema_no.hpp"
#include <algorithm>

SubsistemaNo::SubsistemaNo(const Estrela &star1, int star1_id,
                           const Estrela &star2, int star2_id,
                           double dist)
    : massa_sub(star1.get_massa() + star2.get_massa()),
      membro_dist(dist),
      membro_exc(star1.get_orbita().excentricidade),
      soma_ext(star1.get_raio() + star2.get_raio()),
      extensao(dist * (1.0 + star1.get_orbita().excentricidade) + std::max(star1.get_raio(), star2.get_raio())) {
    if (star1.get_massa() >= star2.get_massa()) {
        id_maior_estrela = star1_id;
        id_menor_estrela = star2_id;
    } else {
        id_maior_estrela = star2_id;
        id_menor_estrela = star1_id;
    }
}

SubsistemaNo::SubsistemaNo(std::unique_ptr<SubsistemaNo> subconj,
                           const Estrela &star, int star_id,
                           double dist)
    : massa_sub((subconj ? subconj->massa_sub : 0.0) + star.get_massa()),
      membro_dist(dist),
      membro_exc(star.get_orbita().excentricidade) {
    const double ext_sub = subconj ? subconj->extensao : 0.0;
    soma_ext = ext_sub + star.get_raio();
    extensao = dist * (1.0 + membro_exc) + std::max(ext_sub, star.get_raio());

    if (!subconj) {
        id_maior_estrela = star_id;
        return;
    }

    if (subconj->massa_sub > star.get_massa()) {
        sub_maior = std::move(subconj);
        sub_maior->volta = this;
        id_menor_estrela = star_id;
    } else {
        sub_menor = std::move(subconj);
        sub_menor->volta = this;
        id_maior_estrela = star_id;
    }
}

SubsistemaNo::SubsistemaNo(std::unique_ptr<SubsistemaNo> subconj1,
                           std::unique_ptr<SubsistemaNo> subconj2,
                           double dist)
    : massa_sub((subconj1 ? subconj1->massa_sub : 0.0) + (subconj2 ? subconj2->massa_sub : 0.0)),
      membro_dist(dist) {
    if (!subconj1 && !subconj2) {
        return;
    }

    const double ext1 = subconj1 ? subconj1->extensao : 0.0;
    const double ext2 = subconj2 ? subconj2->extensao : 0.0;
    membro_exc = subconj1 ? subconj1->orbt.excentricidade : subconj2->orbt.excentricidade;
    soma_ext = ext1 + ext2;
    extensao = dist * (1.0 + membro_exc) + std::max(ext1, ext2);

    const double m1 = subconj1 ? subconj1->massa_sub : 0.0;
    const double m2 = subconj2 ? subconj2->massa_sub : 0.0;

    if (m1 >= m2) {
        sub_maior = std::move(subconj1);
        sub_menor = std::move(subconj2);
    } else {
        sub_maior = std::move(subconj2);
        sub_menor = std::move(subconj1);
    }

    if (sub_maior) {
        sub_maior->volta = this;
    }
    if (sub_menor) {
        sub_menor->volta = this;
    }
}

void SubsistemaNo::get_estrelas_id(int &major_id, int &menor_id) const noexcept {
    major_id = id_maior_estrela;
    menor_id = id_menor_estrela;
}

std::pair<int, int> SubsistemaNo::get_estrelas_id() const noexcept {
    return {id_maior_estrela, id_menor_estrela};
}

bool SubsistemaNo::eh_folha_binaria() const noexcept {
    return sub_maior == nullptr && sub_menor == nullptr;
}

bool SubsistemaNo::eh_raiz() const noexcept {
    return volta == nullptr;
}

PseudoNo::PseudoNo(SubsistemaNo* original, double luz) noexcept
    : no_original(original),
      lumi(luz),
      utilizado(false) {}

PseudoNo::PseudoNo(const std::unique_ptr<SubsistemaNo> &original, double luz) noexcept
    : no_original(original.get()),
      lumi(luz),
      utilizado(false) {}
