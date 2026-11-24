#ifndef ESTABILIDADE_ORBITAL_HPP_INCLUDED
#define ESTABILIDADE_ORBITAL_HPP_INCLUDED

#include <algorithm>
#include <cmath>
#include <random>
#include <utility>
#include "nucleo/constantes.hpp"
#include "nucleo/orbita.hpp"
#include "nucleo/estrela.hpp"
#include "nucleo/subsistema_no.hpp"

namespace EstabilidadeOrbital {

// Constantes de estabilidade secular (Kozai-Lidov, limite quadrupolar)
inline constexpr double KOZAI_COS2_CRITICO = 0.6;        // cos²(39,23°) = 3/5 -> limite da janela de Kozai-Lidov
inline constexpr int MAX_TENTATIVAS_INCLINACAO = 20;     // Limite de re-sorteios antes do fallback
inline constexpr double FATOR_SEGURANCA_PERIASTRO = 3.0; // Periastro mínimo = 3x a soma das extensões dos membros
inline constexpr double INCLINACAO_SEGURA_MAX_GRAUS = 39.0; // Zona segura: [0°, 39°] e [141°, 180°]

// Critério analítico de estabilidade hierárquica de Mardling & Aarseth (2001)
// Determina o periastro mínimo do membro externo (em unidades do semieixo interno)
// e = excentricidade da órbita EXTERNA; inc_graus = inclinação mútua em [0°, 180°]
// O termo (1 - 0.3 * i/π) torna órbitas retrógradas mais estáveis (permite maior proximidade)
[[nodiscard]] inline double mardling_aarseth(double m_sub, double m_star, double e,
                                             double inc_graus = 0.0) noexcept {
    const double m = m_star / m_sub;
    const double termo_superior = (1.0 + m) * (1.0 + e);
    const double base = termo_superior / std::sqrt(1.0 - e);
    const double correcao_inclinacao = 1.0 - 0.3 * (inc_graus / 180.0); // i_rad / π == i_graus / 180
    return 2.8 * std::pow(base, 0.4) * correcao_inclinacao;
}

// Indica se a inclinação mútua está na janela de Kozai-Lidov (39,23° < i < 140,77°)
[[nodiscard]] inline bool em_janela_kozai(double inc_graus) noexcept {
    const double c = std::cos(inc_graus * Constantes::PI / 180.0);
    return (c * c) < KOZAI_COS2_CRITICO;
}

// Excentricidade máxima atingida pela órbita interna durante os ciclos de Kozai-Lidov
// e_max = max(e0, sqrt(1 - (5/3)(1 - e0²) cos²(i))); fora da janela retorna e0
[[nodiscard]] inline double excentricidade_maxima_kozai(double e0, double inc_graus) noexcept {
    if (!em_janela_kozai(inc_graus)) {
        return e0;
    }
    const double c = std::cos(inc_graus * Constantes::PI / 180.0);
    const double radicando = 1.0 - (5.0 / 3.0) * (1.0 - e0 * e0) * (c * c);
    const double e_kozai = std::sqrt(std::max(0.0, radicando));
    return std::max(e0, e_kozai);
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

// Semente derivada para o gerador dedicado às inclinações.
// Usar um gerador separado preserva a sequência do gerador principal (estrelas, excentricidades, estrutura).
[[nodiscard]] inline unsigned int semente_inclinacoes(int seed) noexcept {
    return static_cast<unsigned int>(seed) ^ 0x9E3779B9u;
}

// Sorteia uma inclinação mútua em [0°, 180°]:
// 40% quase coplanar (|N(0°, 10°)|, sendo 10% desses espelhados para perto de 180°) + 60% isotrópica em cos(i)
[[nodiscard]] double sortear_inclinacao_mutua(std::mt19937 &prng_inc);

// Sorteia uma inclinação na zona segura fora da janela de Kozai-Lidov ([0°, 39°] ∪ [141°, 180°]), isotrópica em cos(i)
[[nodiscard]] double sortear_inclinacao_segura(std::mt19937 &prng_inc);

// Sorteia uma inclinação mútua estável em relação ao plano orbital interno de 'interno':
// dentro da janela de Kozai-Lidov exige a_in * (1 - e_max) > FATOR_SEGURANCA_PERIASTRO * soma_ext.
// Após MAX_TENTATIVAS_INCLINACAO falhas, recorre a sortear_inclinacao_segura.
[[nodiscard]] double sortear_inclinacao_estavel(std::mt19937 &prng_inc, const SubsistemaNo &interno);

// Métodos de acoplamento orbital (monta_orbitas)
// Exclusivo para sistemas binarios.
void monta_orbitas_binarias(std::mt19937 &prng, Estrela &estrela1, Estrela &estrela2);

// Retornam o semieixo maior RELATIVO (a1 + a2) da órbita montada, em metros.
// O par estelar mais interno define o plano de referência (inclinação 0°).
double monta_orbitas(std::mt19937 &prng, Estrela &estrela1, Estrela &estrela2);

// As sobrecargas hierárquicas sorteiam inclinações mútuas com o gerador dedicado 'prng_inc'.
// Sub + Estrela: inclinação relativa ao plano interno de 'sub' (gravada nas duas órbitas).
// Sub + Sub: uma inclinação independente para cada subsistema, relativa ao seu próprio plano interno.
double monta_orbitas(std::mt19937 &prng, std::mt19937 &prng_inc, SubsistemaNo *sub, Estrela &estrela);
double monta_orbitas(std::mt19937 &prng, std::mt19937 &prng_inc, SubsistemaNo *sub1, SubsistemaNo *sub2);

// Sobrecargas seguras utilizando referências
inline double monta_orbitas(std::mt19937 &prng, std::mt19937 &prng_inc, SubsistemaNo &sub, Estrela &estrela) {
    return monta_orbitas(prng, prng_inc, &sub, estrela);
}
inline double monta_orbitas(std::mt19937 &prng, std::mt19937 &prng_inc, SubsistemaNo &sub1, SubsistemaNo &sub2) {
    return monta_orbitas(prng, prng_inc, &sub1, &sub2);
}

} // namespace EstabilidadeOrbital

#endif // ESTABILIDADE_ORBITAL_HPP_INCLUDED

