#include "interface/visualizador_sistema.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
namespace VisualizadorSistema {

std::string formata_idade(const double idade) {
    std::stringstream ss;
    ss << "Idade estimada: ~";
    
    if (idade <= 1.0e9) {
        
        ss << std::fixed << std::setprecision(2) << idade / 1.0e6 << " milhões de anos.";
        
        return ss.str();
    }

    long long total_anos = static_cast<long long>(idade);
    
    long long bilhoes = total_anos / 1000000000LL;
    long long milhoes = (total_anos % 1000000000LL) / 1000000LL;

    if (bilhoes <= 1) {
        ss << bilhoes << " bilhão";

    } else {
        ss << bilhoes << " bilhões";
    }

    if (milhoes > 0) {
        ss << " e " << milhoes << " milhões";
    }

    ss << " de anos.";
    return ss.str();
}

std::string obter_cor_ansi(char letra_classe) {
    switch (letra_classe) {
        case 'M': return "\033[31m";
        case 'K': return "\033[93m";
        case 'G': return "\033[33m";
        case 'F': return "\033[37m";
        case 'A': return "\033[34m";
        case 'B': return "\033[36m";
        default:  return "\033[95m";
    }
}

std::string formato_cientifico(double valor) {
    if (valor == 0.0) {
        return "0";
    }

    const int expoente = static_cast<int>(std::floor(std::log10(std::abs(valor))));
    const double mantissa = valor / std::pow(10.0, expoente);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << mantissa;
    ss << " x 10^" << expoente;

    return ss.str();
}

void imprimir_orbita(std::ostream &os, const Orbita &orbit, std::string_view baricentro) {

    if(!dynamic_cast<std::ofstream*>(&os)){
        os << "\n\n\033[32mPossui a seguinte órbita:\033[0m\n";
    }else{
        os << "\n\nPossui a seguinte órbita:\n";
    }

    os << "\n semieixo maior   : " << formato_cientifico(orbit.semieixo_maior) << " metros";
    os << "\n apoastro         : " << formato_cientifico(orbit.apoastro) << " metros de distância do baricentro " << baricentro;
    os << "\n periastro        : " << formato_cientifico(orbit.periastro) << " metros de distância do baricentro " << baricentro;
    os << "\n excentricidade   : " << orbit.excentricidade;

    std::stringstream inc;
    inc << std::fixed << std::setprecision(2) << orbit.inclinacao << "°"
        << (orbit.inclinacao > 90.0 ? " (retrógrada)" : " (prógrada)");
    os << "\n inclinação mútua : " << inc.str();
    os << "\n anomalia media   : " << orbit.anomalia_media;

}

void imprimir_estrela(std::ostream &os, const Estrela &estrela, std::string_view baricentro) {

    os << "\n\n     Estrela  " << estrela.get_nome() << ":";
    
    if(!dynamic_cast<std::ofstream*>(&os)){
        os << "\n Classe           : " << obter_cor_ansi(estrela.get_tipo_spectral()) << estrela.get_tipo() << "\033[0m:";
    }else{
        os << "\n Classe           : " << estrela.get_tipo();
    }

    os << "\n Massa            : " << formato_cientifico(estrela.get_massa()) << " kg";
    os << "\n Raio             : " << formato_cientifico(estrela.get_raio()) << " metros";
    os << "\n Luminosidade     : " << formato_cientifico(estrela.get_luminosidade()) << " watts";
    os << "\n Temperatura      : " << estrela.get_temperatura() << " graus celsius";

    imprimir_orbita(os, estrela.get_orbita(), baricentro);
}

void imprimir_sistema(std::ostream &os, const std::vector<Estrela> &lista, const SubsistemaNo *sub) {

    if (sub == nullptr) {
        return;
    }

    os << "\n\n=============================================================================\n";
    os << "\nSubsistema " << sub->nome << ":";

    int star_maior = -1;
    int star_menor = -1;
    sub->get_estrelas_id(star_maior, star_menor);

    if (star_maior >= 0 && star_menor >= 0) {
        os << "\nComposto por duas estrelas:";

        const auto u_menor = static_cast<std::size_t>(star_menor);
        const auto u_maior = static_cast<std::size_t>(star_maior);

        os << "\n\nPeriodo orbital dos membros: ";
        os << Orbita::calcular_periodo_formatado(
            lista[u_menor].get_orbita(), lista[u_maior].get_orbita(),
            lista[u_menor].get_massa(), lista[u_maior].get_massa());

        os << "\n\nEstrela mais massiva:";
        imprimir_estrela(os, lista[u_maior], sub->nome);

        os << "\n\nEstrela menos massiva:";
        imprimir_estrela(os, lista[u_menor], sub->nome);
        return;
    }

    if (star_menor >= 0 && sub->sub_maior != nullptr) {
        const auto u_menor = static_cast<std::size_t>(star_menor);

        os << "\nComposto por uma estrela e um subsistema:";
        os << "\n\nPeriodo órbital dos membros: ";
        os << Orbita::calcular_periodo_formatado(
            lista[u_menor].get_orbita(), sub->sub_maior->orbt,
            lista[u_menor].get_massa(), sub->sub_maior->massa_sub);

        imprimir_estrela(os, lista[u_menor], sub->nome);

        os << ".\n\nSubsistema " << sub->sub_maior->nome;

        imprimir_orbita(os, sub->sub_maior->orbt, sub->nome);
        imprimir_sistema(os, lista, sub->sub_maior.get());
        return;
    }

    if (sub->sub_maior != nullptr && sub->sub_menor != nullptr) {
        os << "\nComposto por dois subsistemas rivais:";
        os << "\n\nPeriodo órbital dos membros: ";
        os << Orbita::calcular_periodo_formatado(
            sub->sub_maior->orbt, sub->sub_menor->orbt,
            sub->sub_maior->massa_sub, sub->sub_menor->massa_sub);

        os << "\n\nSubsistema " << sub->sub_maior->nome;
        imprimir_orbita(os, sub->sub_maior->orbt, sub->nome);

        os << "\n\nSubsistema " << sub->sub_menor->nome;
        imprimir_orbita(os, sub->sub_menor->orbt, sub->nome);

        imprimir_sistema(os, lista, sub->sub_maior.get());
        imprimir_sistema(os, lista, sub->sub_menor.get());
        return;
    }

    if (star_maior >= 0 && sub->sub_menor != nullptr) {
        const auto u_maior = static_cast<std::size_t>(star_maior);

        os << "\nComposto por uma estrela e um subsistema:";
        os << "\n\nPeriodo órbital dos membros: ";
        os << Orbita::calcular_periodo_formatado(
            lista[u_maior].get_orbita(), sub->sub_menor->orbt,
            lista[u_maior].get_massa(), sub->sub_menor->massa_sub);

        imprimir_estrela(os, lista[u_maior], sub->nome);

        os << ".\n\nSubsistema " << sub->sub_menor->nome;

        imprimir_orbita(os, sub->sub_menor->orbt, sub->nome);
        imprimir_sistema(os, lista, sub->sub_menor.get());
    }
}

void relatorio(std::ostream &os,
                        const int Seed,
                        std::string_view nome_geral,
                        const std::vector<Estrela> &sistema,
                        const SubsistemaNo *no_raiz,
                        double idade) {
    const std::size_t quanti = sistema.size();

    switch (quanti) {
        case 0:
            os << "\nErro: nenhuma estrela foi gerada !!!\n";
            break;

        case 1:
            os << "=============================================================================\n";
            os << "\n                     Sistema " << nome_geral;
            os << "\n\n-----------------------------------------------------------------------------\n";
            os << "\n Seed  : " << Seed;
            os << "\n Idade : " << formata_idade(idade);
            os << "\n\n=============================================================================\n";
            os << "\n\n     Estrela  " << sistema[0].get_nome() << ":";

            if(!dynamic_cast<std::ofstream*>(&os)){
                os << "\n Classe           : " << obter_cor_ansi(sistema[0].get_tipo_spectral()) << sistema[0].get_tipo() << "\033[0m:";
            }else{
                os << "\n Classe           : " << sistema[0].get_tipo();
            }

            os << "\n Massa            : " << formato_cientifico(sistema[0].get_massa()) << " kg";
            os << "\n Raio             : " << formato_cientifico(sistema[0].get_raio()) << " metros";
            os << "\n Luminosidade     : " << formato_cientifico(sistema[0].get_luminosidade()) << " watts";
            os << "\n Temperatura      : " << sistema[0].get_temperatura() << " graus celsius\n";
            break;

        case 2:
            os << "=============================================================================\n";
            os << "\n                   Sistema binário " << nome_geral;
            os << "\n\n-----------------------------------------------------------------------------\n";
            os << "\n Seed  : " << Seed;
            os << "\n Idade : " << formata_idade(idade);
            os << "\n Periodo órbital do sistema: "
            << Orbita::calcular_periodo_formatado(
                sistema[0].get_orbita(), sistema[1].get_orbita(),
                sistema[0].get_massa(), sistema[1].get_massa());
            os << "\n\n=============================================================================\n";

            imprimir_estrela(os, sistema[0], "");
            os << "\n\n=============================================================================\n";
            imprimir_estrela(os, sistema[1], "");
            break;

        case 3:
            os << "=============================================================================\n";
            os << "\n                  Sistema trinario " << nome_geral;
            os << "\n\n-----------------------------------------------------------------------------\n";
            os << "\n Seed  : " << Seed;
            os << "\n Idade : " << formata_idade(idade);

            imprimir_sistema(os, sistema, no_raiz);
            break;

        default:
            if (quanti >= 4 && quanti <= 7) {
                os << "=============================================================================\n";
                os << "\n         Sistema múltiplo de " << quanti << " estrelas: " << nome_geral;
                os << "\n\n-----------------------------------------------------------------------------\n";
                os << "\n Seed  : " << Seed;
                os << "\n Idade : " << formata_idade(idade);

                imprimir_sistema(os, sistema, no_raiz);
            } else {
                os << "\nErro: Foram geradas mais de 7 estrelas !!!\n";
            }
            break;
    }

    os << "\n\n=============================================================================\n";
}

} // namespace VisualizadorSistema

