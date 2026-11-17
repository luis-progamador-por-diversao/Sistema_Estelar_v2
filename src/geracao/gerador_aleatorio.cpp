#include "geracao/gerador_aleatorio.hpp"
#include <cmath>
#include <algorithm>
#include "nucleo/constantes.hpp"
#include "fisica/fisica_estelar.hpp"

namespace GeradorAleatorio {

unsigned int sortear_quantidade_estrelas(std::mt19937 &prng) {
    std::uniform_int_distribution<int> de_um_a_cem(0, 100);
    const int cem = de_um_a_cem(prng);

    if (cem < 75) return 1;
    if (cem < 85) return 2;
    if (cem < 92) return 3;
    if (cem < 95) return 4;
    if (cem < 98) return 5;
    if (cem < 99) return 6;
    return 7;
}

std::string gerar_nome_sistema(std::mt19937 &prng) {
    static constexpr char letras_maiusculas[26] = {
        'A','B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };
    static constexpr char numeros[10] = {'0','1','2','3','4','5','6','7','8','9'};

    std::uniform_int_distribution<int> de_1_a_5(1, 5);
    std::uniform_int_distribution<int> de_1_a_3(1, 3);
    std::uniform_int_distribution<int> letras_sort(0, 25);
    std::uniform_int_distribution<int> num_sort(0, 9);

    std::string nome;
    nome.reserve(24);

    const int quanti_letras = de_1_a_5(prng);
    for (int r = 0; r < quanti_letras; ++r) {
        nome.push_back(letras_maiusculas[letras_sort(prng)]);
    }

    nome.push_back(' ');

    const int grupos_num = de_1_a_3(prng);
    for (int r = 0; r < grupos_num; ++r) {
        const int quanti_digitos = de_1_a_5(prng);
        for (int k = 0; k < quanti_digitos; ++k) {
            nome.push_back(numeros[num_sort(prng)]);
        }
        nome.push_back('-');
    }

    nome.pop_back();
    return nome;
}

double uniforme(std::mt19937 &prng, double min, double max) {
    return std::uniform_real_distribution<double>(min, max)(prng);
}

double log_uniforme(std::mt19937 &prng, double min, double max) {
    return std::exp(uniforme(prng, std::log(min), std::log(max)));
}

std::vector<Estrela> gerar_lote_estrelas(std::mt19937 &prng,
                                        std::string_view nome_geral,
                                        unsigned int quanti, double & _idade) {
    if (quanti == 0) {
        return {};
    }

    std::vector<Estrela> sistema;
    sistema.reserve(quanti);

    std::discrete_distribution<int> sorteia_classe(
        FisicaEstelar::PESOS_CLASSE_IMF.begin(),
        FisicaEstelar::PESOS_CLASSE_IMF.end());

    std::vector<int> classes(quanti);
    std::vector<double> massas(quanti);
    double m_max = 0.0;

    for (unsigned int i = 0; i < quanti; ++i) {
        classes[i] = sorteia_classe(prng);
        massas[i] = FisicaEstelar::sortear_massa(
            prng,
            FisicaEstelar::FAIXAS_MASSA[static_cast<std::size_t>(classes[i])]);
        m_max = std::max(m_max, massas[i]);
    }

    const double t_top = FisicaEstelar::calcular_tempo_sequencia_principal(m_max);
    const double t_ref = std::min(t_top, Constantes::IDADE_UNIVERSO_ANOS);
    const double u_max = (t_top > Constantes::IDADE_UNIVERSO_ANOS) ? 1.0 : 1.10;
    const double idade = uniforme(prng, 0.05, u_max) * t_ref;
    _idade = idade;

    for (unsigned int i = 0; i < quanti; ++i) {
        const auto est = FisicaEstelar::definir_estagio_evolutivo(massas[i], idade);
        const auto props = FisicaEstelar::calcular_propriedades_estrela(prng, massas[i], est);
        const std::string tipo = FisicaEstelar::montar_descricao_tipo(classes[i], est, props.temperatura);
        const char classe_espectral = FisicaEstelar::obter_letra_classe_espectral(static_cast<int>(props.temperatura));

        sistema.emplace_back(std::string(nome_geral), tipo, 
                             massas[i], props.raio, 
                             static_cast<int>(props.temperatura), props.luminosidade,
                             classe_espectral);
    }

    return sistema;
}

} // namespace GeradorAleatorio

