#include "fisica_estelar.hpp"
#include <cmath>
#include <algorithm>

namespace FisicaEstelar {

namespace {

// Distribuições auxiliares restritas à unidade de compilação
double uniforme(std::mt19937 &prng, double min, double max) {
    return std::uniform_real_distribution<double>(min, max)(prng);
}

double log_uniforme(std::mt19937 &prng, double min, double max) {
    return std::exp(uniforme(prng, std::log(min), std::log(max)));
}

} // namespace

double calcular_luminosidade_ms(double massa_solar) {
    if (massa_solar < 0.43) {
        return 0.23 * std::pow(massa_solar, 2.3);
    }
    if (massa_solar < 2.0) {
        return std::pow(massa_solar, 4.0);
    }
    return 1.4 * std::pow(massa_solar, 3.5);
}

double calcular_raio_ms(double massa_solar) {
    if (massa_solar < 1.0) {
        return std::pow(massa_solar, 0.9);
    }
    if (massa_solar < 2.0) {
        return std::pow(massa_solar, 0.8);
    }
    return 1.74 * std::pow(massa_solar / 2.0, 0.57);
}

double calcular_temperatura_efetiva(double luminosidade_solar, double raio_solar) {
    return Constantes::TEMP_SOLAR_K * std::pow(luminosidade_solar / (raio_solar * raio_solar), 0.25);
}

double calcular_raio_stefan_boltzmann(double luminosidade_solar, double temperatura_k) {
    const double razao_temp = Constantes::TEMP_SOLAR_K / temperatura_k;
    return std::sqrt(luminosidade_solar) * (razao_temp * razao_temp);
}

double calcular_tempo_sequencia_principal(double massa_solar) {
    return std::max(1.0e10 * std::pow(massa_solar, -2.5), Constantes::TEMPO_MS_MINIMO_ANOS);
}

EstagioEvolutivo definir_estagio_evolutivo(double massa_solar, double idade_anos) {
    const double fracao_vida = idade_anos / calcular_tempo_sequencia_principal(massa_solar);
    if (fracao_vida < 1.0) {
        return EstagioEvolutivo::SequenciaPrincipal;
    }

    const double progresso_pos_sp = (fracao_vida - 1.0) / 0.10; // Progresso relativo (0.0 a 1.0)
    if (progresso_pos_sp < 0.20) {
        return EstagioEvolutivo::Subgigante;
    }
    if (massa_solar < 8.0) {
        return EstagioEvolutivo::Gigante;
    }
    if (massa_solar < 25.0 || progresso_pos_sp < 0.70) {
        return EstagioEvolutivo::Supergigante;
    }
    return EstagioEvolutivo::Hipergigante;
}

double sortear_massa(std::mt19937 &prng, const FaixaMassa &faixa) {
    const double expoente = 1.0 - faixa.alfa;
    const double a = std::pow(faixa.min, expoente);
    const double b = std::pow(faixa.max, expoente);
    return std::pow(a + uniforme(prng, 0.0, 1.0) * (b - a), 1.0 / expoente);
}

PropriedadesFisicasEstrela calcular_propriedades_estrela(std::mt19937 &prng, double massa_solar, EstagioEvolutivo estagio) {
    const double l_ms = calcular_luminosidade_ms(massa_solar);
    const double r_ms = calcular_raio_ms(massa_solar);
    const double t_ms = calcular_temperatura_efetiva(l_ms, r_ms);
    const double l_edd = Constantes::LIMITE_EDDINGTON_FATOR * massa_solar;

    PropriedadesFisicasEstrela props{l_ms, r_ms, t_ms};

    switch (estagio) {
        case EstagioEvolutivo::SequenciaPrincipal:
            break;
        case EstagioEvolutivo::Subgigante:
            props.luminosidade = l_ms * uniforme(prng, 1.2, 2.5);
            props.temperatura = t_ms * uniforme(prng, 0.75, 0.95);
            break;
        case EstagioEvolutivo::Gigante:
            props.luminosidade = l_ms * uniforme(prng, 5.0, 50.0);
            props.temperatura = std::max(3400.0, t_ms * uniforme(prng, 0.5, 0.8));
            break;
        case EstagioEvolutivo::Supergigante:
            props.luminosidade = l_ms * uniforme(prng, 3.0, 20.0);
            props.temperatura = log_uniforme(prng, 3500.0, 0.7 * t_ms);
            break;
        case EstagioEvolutivo::Hipergigante:
            props.luminosidade = l_edd * uniforme(prng, 0.3, 1.0);
            props.temperatura = log_uniforme(prng, 3500.0, 25000.0);
            break;
    }

    if (estagio != EstagioEvolutivo::SequenciaPrincipal) {
        props.luminosidade = std::min(props.luminosidade, l_edd);
        props.raio = calcular_raio_stefan_boltzmann(props.luminosidade, props.temperatura);
    }

    return props;
}

char obter_letra_classe_espectral(double temperatura_k) {
    if (temperatura_k >= 30000.0) return 'O';
    if (temperatura_k >= 10000.0) return 'B';
    if (temperatura_k >= 7500.0)  return 'A';
    if (temperatura_k >= 6000.0)  return 'F';
    if (temperatura_k >= 5200.0)  return 'G';
    if (temperatura_k >= 3700.0)  return 'K';
    return 'M';
}

std::string montar_descricao_tipo(int indice_classe, EstagioEvolutivo estagio, double temperatura_k) {
    static constexpr std::array<std::string_view, 7> NOMES_SP = {
        "Anã Vermelha", "Anã Laranja", "Anã Amarela",
        "Anã Amarelo-Branca", "Anã Branca", "Estrela Azul-Branca", "Estrela Azul"
    };

    if (estagio == EstagioEvolutivo::SequenciaPrincipal) {
        if (indice_classe >= 0 && indice_classe < 7) {
            std::string res;
            res.reserve(32);
            res += NOMES_SP[static_cast<std::size_t>(indice_classe)];
            return res;
        }
    }

    const char letra = obter_letra_classe_espectral(temperatura_k);
    std::string_view nome_estagio;
    switch (estagio) {
        case EstagioEvolutivo::Subgigante:   nome_estagio = "Subgigante"; break;
        case EstagioEvolutivo::Gigante:      nome_estagio = "Gigante"; break;
        case EstagioEvolutivo::Supergigante: nome_estagio = "Supergigante"; break;
        case EstagioEvolutivo::Hipergigante: nome_estagio = "Hipergigante"; break;
        default:                             nome_estagio = "Sequência Principal"; break;
    }

    std::string res;
    res.reserve(40);
    res += nome_estagio;
    res += " tipo ";
    res += letra;
    return res;
}

} // namespace FisicaEstelar

