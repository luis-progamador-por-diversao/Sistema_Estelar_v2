#ifndef GERADOR_SISTEMA_HPP_INCLUDED
#define GERADOR_SISTEMA_HPP_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include "estrela.hpp"
#include "subsistema_no.hpp"

// Representa a estrutura de dados completa de um sistema estelar gerado
struct SistemaEstelar {
    int seed = 0;
    std::string nome;
    unsigned int quantidade_estrelas = 0;
    std::vector<Estrela> estrelas;
    std::unique_ptr<SubsistemaNo> no_raiz;
    double distancia_binaria = 0.0; // Distância característica utilizada quando quanti == 2
};

// Classe orquestradora da geração procedural
class GeradorSistema {
public:
    // Gera um sistema estelar completo com base na seed informada
    [[nodiscard]] static SistemaEstelar gerar(int seed);
};

#endif // GERADOR_SISTEMA_HPP_INCLUDED

