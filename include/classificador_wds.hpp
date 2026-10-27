#ifndef CLASSIFICADOR_WDS_HPP_INCLUDED
#define CLASSIFICADOR_WDS_HPP_INCLUDED

#include <vector>
#include "estrela.hpp"
#include "subsistema_no.hpp"

namespace ClassificadorWDS {

// Aplica nomenclatura WDS a um sistema binário (sufixos A e B ordenados por luminosidade)
void classificar_binario(Estrela &estrela1, Estrela &estrela2);

// Aplica nomenclatura WDS a um sistema trinário hierárquico
void classificar_trinario(std::vector<Estrela> &estrelas, SubsistemaNo *raiz);

// Aplica nomenclatura WDS a sistemas de 4 a 7 estrelas usando ordenação de luminosidade por pseudo-nós
void classificar_multiplo(std::vector<Estrela> &estrelas, std::vector<PseudoNo> &pseudo_lista);

// Despachante conveniente que aplica a classificação apropriada de acordo com o número de estrelas
void classificar(std::vector<Estrela> &estrelas, SubsistemaNo *raiz, std::vector<PseudoNo> &pseudo_lista);

} // namespace ClassificadorWDS

#endif // CLASSIFICADOR_WDS_HPP_INCLUDED

