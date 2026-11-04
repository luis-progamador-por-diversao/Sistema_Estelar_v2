#include "estrela.hpp"

Estrela::Estrela(std::string_view nome, std::string_view tipo,
                 double m, double r, int tem, double l,
                 char t)
    : m_tipo_spectral(t),
      m_nome(nome),
      m_tipo(tipo),
      m_temperatura(tem)
      {
      definir_massa_solar(m);
      definir_raio_solar(r);
      definir_luminosidade_solar(l);
}

Estrela::Estrela(std::string_view nome, std::string_view tipo,
                 double massa_solar, char t, const FisicaEstelar::PropriedadesFisicasEstrela &props,
                 FisicaEstelar::EstagioEvolutivo estagio)
    : m_tipo_spectral(t),
      m_nome(nome),
      m_tipo(tipo),
      m_estagio(estagio)
      {
          atualizar_propriedades_fisicas(massa_solar, props);
      }

char Estrela::get_tipo_spectral() const noexcept{
    return m_tipo_spectral;
}

const std::string& Estrela::get_nome() const noexcept {
    return m_nome;
}

const std::string& Estrela::get_tipo() const noexcept {
    return m_tipo;
}

FisicaEstelar::EstagioEvolutivo Estrela::get_estagio() const noexcept {
    return m_estagio;
}

const Orbita& Estrela::get_orbita() const noexcept {
    return m_orbita;
}

Orbita& Estrela::get_orbita() noexcept {
    return m_orbita;
}

double Estrela::get_massa() const noexcept {
    return m_massa_kg;
}

double Estrela::get_raio() const noexcept {
    return m_raio_m;
}

double Estrela::get_luminosidade() const noexcept {
    return m_luminosidade_w;
}

int Estrela::get_temperatura() const noexcept {
    return m_temperatura;
}

double Estrela::get_massa_solar() const noexcept {
    return m_massa_kg / Constantes::MASSA_SOLAR_KG;
}

double Estrela::get_raio_solar() const noexcept {
    return m_raio_m / Constantes::RAIO_SOLAR_M;
}

double Estrela::get_luminosidade_solar() const noexcept {
    return m_luminosidade_w / Constantes::LUMINOSIDADE_SOLAR_W;
}

void Estrela::set_tipo_spectral(char novo_tipo_spectral) noexcept {
    m_tipo_spectral = novo_tipo_spectral;
}

void Estrela::set_nome(std::string_view novo_nome) {
    m_nome = novo_nome;
}

void Estrela::set_tipo(std::string_view novo_tipo) {
    m_tipo = novo_tipo;
}

void Estrela::set_orbita(const Orbita &nova_orbita) noexcept {
    m_orbita = nova_orbita;
}

void Estrela::set_estagio(FisicaEstelar::EstagioEvolutivo estagio) noexcept {
    m_estagio = estagio;
}

void Estrela::definir_massa_solar(double massa_solar) noexcept {
    m_massa_kg = massa_solar * Constantes::MASSA_SOLAR_KG;
}

void Estrela::definir_raio_solar(double raio_solar) noexcept {
    m_raio_m = raio_solar * Constantes::RAIO_SOLAR_M;
}

void Estrela::definir_luminosidade_solar(double lumi_solar) noexcept {
    m_luminosidade_w = lumi_solar * Constantes::LUMINOSIDADE_SOLAR_W;
}

void Estrela::definir_temperatura(int temperatura_k) noexcept {
    m_temperatura = temperatura_k;
}

void Estrela::atualizar_propriedades_fisicas(double massa_solar, const FisicaEstelar::PropriedadesFisicasEstrela &props) {
    definir_massa_solar(massa_solar);
    definir_raio_solar(props.raio);
    definir_luminosidade_solar(props.luminosidade);
    definir_temperatura(static_cast<int>(props.temperatura));
}

