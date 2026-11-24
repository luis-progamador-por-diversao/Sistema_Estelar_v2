#include "fisica/estabilidade_orbital.hpp"
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

double sortear_inclinacao_mutua(std::mt19937 &prng_inc) {
    std::uniform_real_distribution<double> u01(0.0, 1.0);

    if (u01(prng_inc) < 0.40) {
        // Ramo quase coplanar: |N(0°, 10°)|, limitado a 180°
        std::normal_distribution<double> quase_coplanar(0.0, 10.0);
        double inc = std::min(std::abs(quase_coplanar(prng_inc)), 180.0);
        if (u01(prng_inc) < 0.10) {
            inc = 180.0 - inc; // quase coplanar retrógrada
        }
        return inc;
    }

    // Ramo isotrópico: cos(i) uniforme em [-1, 1]
    std::uniform_real_distribution<double> cos_uniforme(-1.0, 1.0);
    return std::acos(cos_uniforme(prng_inc)) * 180.0 / Constantes::PI;
}

double sortear_inclinacao_segura(std::mt19937 &prng_inc) {
    // Isotrópica em cos(i) restrita a |cos(i)| >= cos(39°)
    const double cos_limite = std::cos(INCLINACAO_SEGURA_MAX_GRAUS * Constantes::PI / 180.0);
    std::uniform_real_distribution<double> cos_dist(cos_limite, 1.0);
    std::bernoulli_distribution retrograda(0.5);

    const double inc = std::acos(cos_dist(prng_inc)) * 180.0 / Constantes::PI;
    return retrograda(prng_inc) ? 180.0 - inc : inc;
}

double sortear_inclinacao_estavel(std::mt19937 &prng_inc, const SubsistemaNo &interno) {
    const double a_in = interno.membro_dist;
    const double e0 = interno.membro_exc;
    const double periastro_seguro = FATOR_SEGURANCA_PERIASTRO * interno.soma_ext;

    for (int tentativa = 0; tentativa < MAX_TENTATIVAS_INCLINACAO; ++tentativa) {
        const double inc = sortear_inclinacao_mutua(prng_inc);

        // Fora da janela de Kozai-Lidov a excentricidade interna não cresce secularmente
        if (!em_janela_kozai(inc)) {
            return inc;
        }

        // Dentro da janela: periastro interno no pico do ciclo deve evitar colisão/ruptura
        const double e_max = excentricidade_maxima_kozai(e0, inc);
        if (a_in * (1.0 - e_max) > periastro_seguro) {
            return inc;
        }
    }

    return sortear_inclinacao_segura(prng_inc);
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

double monta_orbitas(std::mt19937 &prng, std::mt19937 &prng_inc, SubsistemaNo *sub, Estrela &estrela) {
    if (!sub) {
        std::cerr << "\nErro: A funcao monta_orbitas(No + estrela) falhou por ponteiro nulo.\n";
        return 0.1;
    }

    const double excent = sortear_excentricidade(prng);

    // Inclinação mútua relativa ao plano orbital interno de 'sub', validada contra Kozai-Lidov
    const double inc = sortear_inclinacao_estavel(prng_inc, *sub);
    const double fator = mardling_aarseth(sub->massa_sub, estrela.get_massa(), excent, inc);

    const double peri_minimo = sub->membro_dist * fator;

    const double apo_minimo = peri_minimo * (1.0 + excent) / (1.0 - excent);
    const double apo_maximo = apo_minimo * 5.0;

    const double distancia = log_uniforme(prng, apo_minimo, apo_maximo);

    const auto [apo_sub, apo_star] = calcular_apoastros_baricentro(distancia, sub->massa_sub, estrela.get_massa());

    Orbita orbita_sub(apo_sub, excent, true);
    Orbita orbita_star(apo_star, excent, true);
    orbita_sub.inclinacao = inc;
    orbita_star.inclinacao = inc;

    sub->orbt = orbita_sub;
    estrela.set_orbita(orbita_star);

    return orbita_sub.semieixo_maior + orbita_star.semieixo_maior;
}

double monta_orbitas(std::mt19937 &prng, std::mt19937 &prng_inc, SubsistemaNo *sub1, SubsistemaNo *sub2) {
    if (!sub1 || !sub2) {
        std::cerr << "\nErro: A funcao monta_orbitas(No + No) falhou por ponteiro nulo.\n";
        return 0.1;
    }

    const double exc = sortear_excentricidade(prng);
    const double a_interno = std::max(sub1->membro_dist, sub2->membro_dist);

    // Uma inclinação mútua independente para cada plano interno, cada uma validada contra seu próprio Kozai-Lidov
    const double inc1 = sortear_inclinacao_estavel(prng_inc, *sub1);
    const double inc2 = sortear_inclinacao_estavel(prng_inc, *sub2);

    // Critério de MA com a inclinação menos favorável (mais prógrada => maior fator => mais conservador)
    const double fator = mardling_aarseth(std::min(sub1->massa_sub, sub2->massa_sub),
                                          std::max(sub1->massa_sub, sub2->massa_sub),
                                          exc, std::min(inc1, inc2));

    const double peri_minimo = a_interno * fator;
    const double apo_minimo = peri_minimo * (1.0 + exc) / (1.0 - exc);
    const double apo_maximo = apo_minimo * 10.0;

    const double dist_entre_sub = log_uniforme(prng, apo_minimo, apo_maximo);

    const auto [apo1, apo2] = calcular_apoastros_baricentro(dist_entre_sub, sub1->massa_sub, sub2->massa_sub);

    Orbita orbita1(apo1, exc, true);
    Orbita orbita2(apo2, exc, true);
    orbita1.inclinacao = inc1;
    orbita2.inclinacao = inc2;

    sub1->orbt = orbita1;
    sub2->orbt = orbita2;

    return orbita1.semieixo_maior + orbita2.semieixo_maior;
}

} // namespace EstabilidadeOrbital

