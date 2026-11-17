#ifndef VISUALIZADOR_SISTEMA_HPP_INCLUDED
#define VISUALIZADOR_SISTEMA_HPP_INCLUDED

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "nucleo/estrela.hpp"
#include "nucleo/orbita.hpp"
#include "nucleo/subsistema_no.hpp"

namespace VisualizadorSistema {

// Formata a idade do sistema estelar 
[[nodiscard]] std::string formata_idade(const double idade);

// Converte valores numéricos para notação científica formatada (ex.: 1.99 x 10^30)
[[nodiscard]] std::string formato_cientifico(double valor);

// Renderiza os parâmetros de uma órbita referenciada ao baricentro indicado
void imprimir_orbita(std::ostream &os, const Orbita &orbit, std::string_view baricentro);

// Renderiza a ficha técnica detalhada de uma estrela
void imprimir_estrela(std::ostream &os, const Estrela &estrela, std::string_view baricentro = "");

// Renderiza recursivamente a árvore hierárquica de subsistemas
void imprimir_sistema(std::ostream &os, const std::vector<Estrela> &lista, const SubsistemaNo *sub);

// Renderiza o relatório completo formatado para 1 até 7 estrelas
void relatorio(std::ostream &os,
                        const int Seed,
                        std::string_view nome_geral,
                        const std::vector<Estrela> &sistema,
                        const SubsistemaNo *no_raiz = nullptr,
                        double idade = 0.0);

} // namespace VisualizadorSistema

#endif // VISUALIZADOR_SISTEMA_HPP_INCLUDED

