#ifndef ORBITA_HPP_INCLUDED
#define ORBITA_HPP_INCLUDED

#include <string>
#include "constantes.hpp"

// Representa uma órbita kepleriana e seus parâmetros clássicos
struct Orbita {
    // Parâmetros de dimensão e geometria da órbita
    double semieixo_maior = 0.0; // Em metros (a)
    double excentricidade = 0.0; // Adimensional, 0 <= e < 1 (e)
    double apoastro = 0.0;       // Distância do apoastro em metros (r_a)
    double periastro = 0.0;      // Distância do periastro em metros (r_p)

    // Elementos keplerianos angulares clássicos (em graus)
    double inclinacao = 0.0;              // Inclinação orbital (i)
    double argumento_periastro = 0.0;     // Argumento do periastro (ω)
    double longitude_no_ascendente = 0.0; // Longitude do nó ascendente (Ω)
    double anomalia_media = 0.0;          // Anomalia média na época t0 (M0)

    // Construtores
    Orbita() = default;

    // Construtor a partir de apoastro ou periastro com excentricidade
    // Se calcula_periastro == true, apo_ou_peri é interpretado como apoastro
    // Se calcula_periastro == false, apo_ou_peri é interpretado como periastro
    Orbita(double apo_ou_peri, double exc, bool calcula_periastro);

    // Construtor a partir de semieixo maior e excentricidade
    Orbita(double semieixo_maior_m, double exc);

    // Construtor completo com todos os parâmetros orbitais clássicos
    Orbita(double semieixo_maior_m, double exc, double inc_graus,
           double arg_peri_graus, double long_no_asc_graus, double anomalia_med_graus);

    // Propriedades geométricas derivadas
    [[nodiscard]] double semieixo_menor() const noexcept;
    [[nodiscard]] double distancia_focal() const noexcept;

    // Funções de mecânica celeste (3ª Lei de Kepler modificada por Newton)
    [[nodiscard]] static double calcular_periodo_segundos(double semieixo_total_m, double massa_total_kg);
    [[nodiscard]] static double calcular_periodo_segundos(const Orbita &o1, const Orbita &o2, double m1_kg, double m2_kg);

    // Funções de conversão e formatação textual do período orbital
    [[nodiscard]] static std::string formatar_periodo(double periodo_segundos);
    [[nodiscard]] static std::string calcular_periodo_formatado(const Orbita &o1, const Orbita &o2, double m1_kg, double m2_kg);
};

#endif // ORBITA_HPP_INCLUDED

