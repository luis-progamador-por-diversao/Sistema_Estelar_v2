#ifndef ESTABILIDADE_ORBITAL_HPP_INCLUDED
#define ESTABILIDADE_ORBITAL_HPP_INCLUDED

#include <cmath>
#include <random>
#include <utility>
#include "constantes.hpp"
#include "orbita.hpp"
#include "estrela.hpp"
#include "subsistema_no.hpp"

namespace EstabilidadeOrbital {

// Critério analítico de estabilidade hierárquica de Mardling & Aarseth (2001)
// Determina o periastro mínimo do membro externo para garantir estabilidade a longo prazo
[[nodiscard]] inline double mardling_aarseth(double m_sub, double m_star, double e) noexcept {
    const double m = m_star / m_sub;
    const double termo_superior = (1.0 + m) * (1.0 + e);
    const double base = termo_superior / std::sqrt(1.0 - e);
    return 2.8 * std::pow(base, 0.4);
}

// Estrutura para retorno da partição de distâncias em torno do baricentro comum
struct ApoastrosBaricentro {
    double apoastro_corpo1 = 0.0;
    double apoastro_corpo2 = 0.0;
};

// Calcula a distância de apoastro de dois corpos em torno do baricentro comum
// respeitando a relação m1 * r1 = m2 * r2
[[nodiscard]] inline ApoastrosBaricentro calcular_apoastros_baricentro(double distancia_total, double m1, double m2) noexcept {
    if (m1 + m2 <= 0.0) {
        return {distancia_total * 0.5, distancia_total * 0.5};
    }
    if (m1 >= m2) {
        const double apo_leve = distancia_total * (m1 / (m1 + m2));
        const double apo_pesado = distancia_total - apo_leve;
        return {apo_pesado, apo_leve};
    } else {
        const double apo_leve = distancia_total * (m2 / (m1 + m2));
        const double apo_pesado = distancia_total - apo_leve;
        return {apo_leve, apo_pesado};
    }
}

// Sorteio de excentricidade na faixa padrão [0.01, 0.70]
[[nodiscard]] double sortear_excentricidade(std::mt19937 &prng) noexcept;

// Métodos de acoplamento orbital (monta_orbitas)
// Retornam a separação ou semieixo total em metros
double monta_orbitas(std::mt19937 &prng, Estrela &estrela1, Estrela &estrela2);
double monta_orbitas(std::mt19937 &prng, SubsistemaNo *sub, Estrela &estrela);
double monta_orbitas(std::mt19937 &prng, SubsistemaNo *sub1, SubsistemaNo *sub2);

// Sobrecargas seguras utilizando referências
inline double monta_orbitas(std::mt19937 &prng, SubsistemaNo &sub, Estrela &estrela) {
    return monta_orbitas(prng, &sub, estrela);
}
inline double monta_orbitas(std::mt19937 &prng, SubsistemaNo &sub1, SubsistemaNo &sub2) {
    return monta_orbitas(prng, &sub1, &sub2);
}

} // namespace EstabilidadeOrbital



#endif // ESTABILIDADE_ORBITAL_HPP_INCLUDED

