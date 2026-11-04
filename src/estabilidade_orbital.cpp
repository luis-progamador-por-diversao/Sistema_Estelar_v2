#include "estabilidade_orbital.hpp"
#include <iostream>
#include <algorithm>

namespace EstabilidadeOrbital {

namespace {

double log_uniforme(std::mt19937 &prng, double min, double max) {
    std::uniform_real_distribution<double> dist(std::log(min), std::log(max));
    return std::exp(dist(prng));
}

} // namespace

double sortear_excentricidade(std::mt19937 &prng) noexcept {
    std::uniform_int_distribution<int> numeros(1, 70);
    return static_cast<double>(numeros(prng)) / 100.0;
}

void monta_orbitas_binarias(std::mt19937 &prng, Estrela &estrela1, Estrela &estrela2) {
    const double peri_minimo = 3.0 * (estrela1.get_raio() + estrela2.get_raio());
    const double exc = sortear_excentricidade(prng);

    const double apo_minimo = peri_minimo * (1.0 + exc) / (1.0 - exc);
    const double apo_maximo = apo_minimo * 90.0;
    const double apo_maior = log_uniforme(prng, apo_minimo, apo_maximo);

    const auto [apo1, apo2] = calcular_apoastros_baricentro(apo_maior, estrela1.get_massa(), estrela2.get_massa());

    const Orbita o1(apo1, exc, true);
    const Orbita o2(apo2, exc, true);

    estrela1.set_orbita(o1);
    estrela2.set_orbita(o2);

}

double monta_orbitas(std::mt19937 &prng, Estrela &estrela1, Estrela &estrela2) {
    const double peri_minimo = 3.0 * (estrela1.get_raio() + estrela2.get_raio());
    const double exc = sortear_excentricidade(prng);

    const double apo_minimo = peri_minimo * (1.0 + exc) / (1.0 - exc);
    const double apo_maximo = apo_minimo * 90.0;
    const double apo_maior = log_uniforme(prng, apo_minimo, apo_maximo);

    const auto [apo1, apo2] = calcular_apoastros_baricentro(apo_maior, estrela1.get_massa(), estrela2.get_massa());

    const Orbita o1(apo1, exc, true);
    const Orbita o2(apo2, exc, true);

    estrela1.set_orbita(o1);
    estrela2.set_orbita(o2);

    return o1.semieixo_maior + o2.semieixo_maior;
}

double monta_orbitas(std::mt19937 &prng, SubsistemaNo *sub, Estrela &estrela) {
    if (!sub) {
        std::cerr << "\nErro: A funcao monta_orbitas(No + estrela) falhou por ponteiro nulo.\n";
        return 0.1;
    }

    const double excent = sortear_excentricidade(prng);
    const double peri_minimo = mardling_aarseth(sub->massa_sub, estrela.get_massa(), excent);

    const double apo_minimo = peri_minimo * (1.0 + excent) / (1.0 - excent);
    const double distancia = sub->membro_dist * log_uniforme(prng, peri_minimo, apo_minimo);

    const auto [apo_sub, apo_star] = calcular_apoastros_baricentro(distancia, sub->massa_sub, estrela.get_massa());

    sub->orbt = Orbita(apo_sub, excent, true);
    estrela.set_orbita(Orbita(apo_star, excent, true));

    return distancia;
}

double monta_orbitas(std::mt19937 &prng, SubsistemaNo *sub1, SubsistemaNo *sub2) {
    if (!sub1 || !sub2) {
        std::cerr << "\nErro: A funcao monta_orbitas(No + No) falhou por ponteiro nulo.\n";
        return 0.1;
    }

    const double exc = sortear_excentricidade(prng);
    const double a_interno = std::max(sub1->membro_dist, sub2->membro_dist);

    const double fator = mardling_aarseth(std::min(sub1->massa_sub, sub2->massa_sub),
                                          std::max(sub1->massa_sub, sub2->massa_sub),
                                          exc);

    const double peri_minimo = a_interno * fator;
    const double apo_minimo = peri_minimo * (1.0 + exc) / (1.0 - exc);
    const double apo_maximo = apo_minimo * 10.0;

    const double dist_entre_sub = log_uniforme(prng, apo_minimo, apo_maximo);

    const auto [apo1, apo2] = calcular_apoastros_baricentro(dist_entre_sub, sub1->massa_sub, sub2->massa_sub);

    sub1->orbt = Orbita(apo1, exc, true);
    sub2->orbt = Orbita(apo2, exc, true);

    return dist_entre_sub;
}

} // namespace EstabilidadeOrbital

