#ifndef CONSTANTES_HPP_INCLUDED
#define CONSTANTES_HPP_INCLUDED

namespace Constantes {

// Constantes Matemáticas
inline constexpr double PI = 3.14159265358979323846;

// Constantes Físicas Fundamentais
inline constexpr double G = 6.67430e-11; // Constante gravitacional em m³ / (kg * s²)

// Constantes Solares de Referência
inline constexpr double MASSA_SOLAR_KG = 1.989e30;       // Massa Solar em kg (M☉)
inline constexpr double RAIO_SOLAR_M = 6.95e8;          // Raio Solar em metros (R☉)
inline constexpr double LUMINOSIDADE_SOLAR_W = 3.828e26; // Luminosidade Solar em Watts (L☉)
inline constexpr double TEMP_SOLAR_K = 5778.0;          // Temperatura efetiva do Sol em Kelvin (T☉)

// Constantes Astrofísicas e Evolução Estelar
inline constexpr double IDADE_UNIVERSO_ANOS = 13.0e9;   // Idade máxima estimada do Universo (anos)
inline constexpr double TEMPO_MS_MINIMO_ANOS = 3.0e6;   // Tempo mínimo na Sequência Principal (anos)
inline constexpr double LIMITE_EDDINGTON_FATOR = 3.2e4; // L_edd = 3.2e4 * M (em L☉)

// Constantes Temporais para Conversão de Períodos
inline constexpr double SEGUNDOS_POR_DIA = 86400.0;
inline constexpr double DIAS_POR_MES_MEDIO = 30.437;
inline constexpr double DIAS_POR_ANO_JULIANO = 365.25;

} // namespace Constantes

#endif // CONSTANTES_HPP_INCLUDED

