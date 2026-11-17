#ifndef GERADOR_ALEATORIO_HPP_INCLUDED
#define GERADOR_ALEATORIO_HPP_INCLUDED

#include <string>
#include <string_view>
#include <vector>
#include <random>
#include "nucleo/estrela.hpp"

namespace GeradorAleatorio {

// Sorteia a quantidade de estrelas do sistema (1 a 7) baseado em probabilidades astrofísicas
[[nodiscard]] unsigned int sortear_quantidade_estrelas(std::mt19937 &prng);

// Gera proceduralmente o nome de catálogo alfanumérico do sistema (ex.: "HD 12345-67")
[[nodiscard]] std::string gerar_nome_sistema(std::mt19937 &prng);

// Sorteia um número real uniformemente distribuído no intervalo [min, max]
[[nodiscard]] double uniforme(std::mt19937 &prng, double min, double max);

// Sorteia um número real com distribuição log-uniforme no intervalo [min, max]
[[nodiscard]] double log_uniforme(std::mt19937 &prng, double min, double max);

// Gera um lote completo de estrelas aplicando a IMF, idade compartilhada e evolução astrofísica
[[nodiscard]] std::vector<Estrela> gerar_lote_estrelas(std::mt19937 &prng,
                                                        std::string_view nome_geral,
                                                        unsigned int quanti, double & _idade);

} // namespace GeradorAleatorio

#endif // GERADOR_ALEATORIO_HPP_INCLUDED

