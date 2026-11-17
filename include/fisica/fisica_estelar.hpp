#ifndef FISICA_ESTELAR_HPP_INCLUDED
#define FISICA_ESTELAR_HPP_INCLUDED

#include <array>
#include <string>
#include <string_view>
#include <random>
#include "nucleo/constantes.hpp"

namespace FisicaEstelar {

// Estágios de evolução estelar
enum class EstagioEvolutivo {
    SequenciaPrincipal,
    Subgigante,
    Gigante,
    Supergigante,
    Hipergigante
};

// Faixa de massa da IMF (Função de Massa Inicial)
// Probabilidade p(m) proporcional a m^(-alfa)
struct FaixaMassa {
    double min = 0.0;
    double max = 0.0;
    double alfa = 0.0;
};

// Resultado das propriedades físicas calculadas para uma estrela (em unidades solares e Kelvin)
struct PropriedadesFisicasEstrela {
    double luminosidade = 0.0; // Em L☉ (luminosidades solares)
    double raio = 0.0;         // Em R☉ (raios solares)
    double temperatura = 0.0;  // Em Kelvin
};

// 7 classes espectrais da IMF correspondentes a M, K, G, F, A, B, O
inline constexpr std::array<FaixaMassa, 7> FAIXAS_MASSA = {{
    {0.08, 0.45, 1.30},  // M
    {0.45, 0.80, 2.35},  // K
    {0.80, 1.04, 2.35},  // G
    {1.04, 1.40, 2.35},  // F
    {1.40, 2.10, 2.35},  // A
    {2.10, 16.0, 2.35},  // B
    {16.0, 50.0, 2.35}   // O
}};

// Pesos estatísticos para sorteio de cada classe na IMF
inline constexpr std::array<double, 7> PESOS_CLASSE_IMF = {{60.0, 15.0, 10.0, 7.0, 4.0, 3.0, 1.0}};

// Funções analíticas da Sequência Principal e Astrofísica
[[nodiscard]] double calcular_luminosidade_ms(double massa_solar);
[[nodiscard]] double calcular_raio_ms(double massa_solar);
[[nodiscard]] double calcular_temperatura_efetiva(double luminosidade_solar, double raio_solar);
[[nodiscard]] double calcular_raio_stefan_boltzmann(double luminosidade_solar, double temperatura_k);
[[nodiscard]] double calcular_tempo_sequencia_principal(double massa_solar);

// Evolução Estelar
[[nodiscard]] EstagioEvolutivo definir_estagio_evolutivo(double massa_solar, double idade_anos);

// Sorteio e Cálculo Estelar usando PRNG
[[nodiscard]] double sortear_massa(std::mt19937 &prng, const FaixaMassa &faixa);
[[nodiscard]] PropriedadesFisicasEstrela calcular_propriedades_estrela(std::mt19937 &prng, double massa_solar, EstagioEvolutivo estagio);

// Classificação Espectral e Visual
[[nodiscard]] char obter_letra_classe_espectral(double temperatura_k);
[[nodiscard]] std::string_view obter_cor_ansi(char letra_classe);
[[nodiscard]] std::string montar_descricao_tipo(int indice_classe, EstagioEvolutivo estagio, double temperatura_k);

} // namespace FisicaEstelar

#endif // FISICA_ESTELAR_HPP_INCLUDED

