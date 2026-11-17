#ifndef ESTRELA_HPP_INCLUDED
#define ESTRELA_HPP_INCLUDED

#include <string>
#include <string_view>
#include "constantes.hpp"
#include "orbita.hpp"
#include "fisica/fisica_estelar.hpp"

// Representa uma estrela com suas propriedades astrofísicas, orbitais e identificação
class Estrela {
public:
    // Construtores
    Estrela() = default;

    // Construtor a partir de grandezas solares e temperatura (compatibilidade com gerador de classes)
    Estrela(std::string_view nome, std::string_view tipo,
            double m, double r, int tem, double l,
            char t);

    // Construtor integrado com as estruturas do módulo FisicaEstelar
    Estrela(std::string_view nome, std::string_view tipo,
            double m,char t, const FisicaEstelar::PropriedadesFisicasEstrela &props,
            FisicaEstelar::EstagioEvolutivo estagio = FisicaEstelar::EstagioEvolutivo::SequenciaPrincipal 
            );

    // Getters de Identificação e Classificação
    [[nodiscard]] char get_tipo_spectral() const noexcept;
    [[nodiscard]] const std::string& get_nome() const noexcept;
    [[nodiscard]] const std::string& get_tipo() const noexcept;
    [[nodiscard]] FisicaEstelar::EstagioEvolutivo get_estagio() const noexcept;

    // Getters Orbitais
    [[nodiscard]] const Orbita& get_orbita() const noexcept;
    [[nodiscard]] Orbita& get_orbita() noexcept;

    // Getters Físicos no Sistema Internacional (SI: kg, metros, Watts, Kelvin)
    [[nodiscard]] double get_massa() const noexcept;
    [[nodiscard]] double get_raio() const noexcept;
    [[nodiscard]] double get_luminosidade() const noexcept;
    [[nodiscard]] int get_temperatura() const noexcept;

    // Getters Físicos em Unidades Solares de Referência (M☉, R☉, L☉)
    [[nodiscard]] double get_massa_solar() const noexcept;
    [[nodiscard]] double get_raio_solar() const noexcept;
    [[nodiscard]] double get_luminosidade_solar() const noexcept;

    // Métodos de Atualização de Estado (Setters)
    void set_tipo_spectral(char novo_tipo_spectral) noexcept;
    void set_nome(std::string_view novo_nome);
    void set_tipo(std::string_view novo_tipo);
    void set_orbita(const Orbita &nova_orbita) noexcept;
    void set_estagio(FisicaEstelar::EstagioEvolutivo estagio) noexcept;

    void definir_massa_solar(double massa_solar) noexcept;
    void definir_raio_solar(double raio_solar) noexcept;
    void definir_luminosidade_solar(double lumi_solar) noexcept;
    void definir_temperatura(int temperatura_k) noexcept;

    // Atualiza massa, raio, temperatura e luminosidade a partir de PropriedadesFisicasEstrela
    void atualizar_propriedades_fisicas(double massa_solar, const FisicaEstelar::PropriedadesFisicasEstrela &props);

private:
    char m_tipo_spectral = '\0';
    std::string m_nome;
    std::string m_tipo;
    Orbita m_orbita;

    // Propriedades físicas armazenadas em unidades SI
    double m_massa_kg = 0.0;
    double m_raio_m = 0.0;
    double m_luminosidade_w = 0.0;
    int m_temperatura = 0;

    FisicaEstelar::EstagioEvolutivo m_estagio = FisicaEstelar::EstagioEvolutivo::SequenciaPrincipal;
};

#endif // ESTRELA_HPP_INCLUDED

