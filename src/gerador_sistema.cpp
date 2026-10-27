#include "gerador_sistema.hpp"
#include <random>
#include <algorithm>
#include "gerador_aleatorio.hpp"
#include "estabilidade_orbital.hpp"
#include "classificador_wds.hpp"

SistemaEstelar GeradorSistema::gerar(int seed) {
    SistemaEstelar resultado;
    resultado.seed = seed;

    std::mt19937 prng(static_cast<unsigned int>(seed));

    const unsigned int quanti = GeradorAleatorio::sortear_quantidade_estrelas(prng);
    resultado.quantidade_estrelas = quanti;
    resultado.nome = GeradorAleatorio::gerar_nome_sistema(prng);
    resultado.estrelas = GeradorAleatorio::gerar_lote_estrelas(prng, resultado.nome, quanti);

    switch (quanti) {
        case 0:
        case 1:
            break;

        case 2: {
            const double dist = EstabilidadeOrbital::monta_orbitas(
                prng, resultado.estrelas[0], resultado.estrelas[1]);
            resultado.distancia_binaria = dist;
            ClassificadorWDS::classificar_binario(resultado.estrelas[0], resultado.estrelas[1]);
            break;
        }

        case 3: {
            double dist = EstabilidadeOrbital::monta_orbitas(
                prng, resultado.estrelas[0], resultado.estrelas[1]);
            resultado.no_raiz = std::make_unique<SubsistemaNo>(
                resultado.estrelas[0], 0, resultado.estrelas[1], 1, dist);

            dist = EstabilidadeOrbital::monta_orbitas(
                prng, resultado.no_raiz.get(), resultado.estrelas[2]);
            resultado.no_raiz = std::make_unique<SubsistemaNo>(
                std::move(resultado.no_raiz), resultado.estrelas[2], 2, dist);

            ClassificadorWDS::classificar_trinario(resultado.estrelas, resultado.no_raiz.get());
            break;
        }

        default: {
            // 4 até 7 estrelas
            std::vector<PseudoNo> list_pseudo;
            list_pseudo.reserve(quanti - 1);

            std::uniform_int_distribution<int> sorte(0, 1);

            double dist = EstabilidadeOrbital::monta_orbitas(
                prng, resultado.estrelas[0], resultado.estrelas[1]);
            resultado.no_raiz = std::make_unique<SubsistemaNo>(
                resultado.estrelas[0], 0, resultado.estrelas[1], 1, dist);

            list_pseudo.emplace_back(
                resultado.no_raiz.get(),
                std::max(resultado.estrelas[0].get_luminosidade(),
                         resultado.estrelas[1].get_luminosidade()));

            unsigned int i = 2;
            while (i < quanti) {
                const int par_conj = sorte(prng);

                if (par_conj == 1 && (i + 1U <= quanti - 1U)) {
                    const int id1 = static_cast<int>(i);
                    const int id2 = static_cast<int>(i + 1U);

                    dist = EstabilidadeOrbital::monta_orbitas(
                        prng, resultado.estrelas[static_cast<std::size_t>(id1)],
                        resultado.estrelas[static_cast<std::size_t>(id2)]);

                    auto p = std::make_unique<SubsistemaNo>(
                        resultado.estrelas[static_cast<std::size_t>(id1)], id1,
                        resultado.estrelas[static_cast<std::size_t>(id2)], id2, dist);

                    list_pseudo.emplace_back(
                        p.get(),
                        std::max(resultado.estrelas[static_cast<std::size_t>(id1)].get_luminosidade(),
                                 resultado.estrelas[static_cast<std::size_t>(id2)].get_luminosidade()));

                    dist = EstabilidadeOrbital::monta_orbitas(
                        prng, resultado.no_raiz.get(), p.get());

                    resultado.no_raiz = std::make_unique<SubsistemaNo>(
                        std::move(resultado.no_raiz), std::move(p), dist);

                    if (resultado.no_raiz->sub_maior) {
                        resultado.no_raiz->sub_maior->volta = resultado.no_raiz.get();
                    }
                    if (resultado.no_raiz->sub_menor) {
                        resultado.no_raiz->sub_menor->volta = resultado.no_raiz.get();
                    }

                    i += 2;
                } else {
                    const int id = static_cast<int>(i);
                    dist = EstabilidadeOrbital::monta_orbitas(
                        prng, resultado.no_raiz.get(), resultado.estrelas[static_cast<std::size_t>(id)]);

                    resultado.no_raiz = std::make_unique<SubsistemaNo>(
                        std::move(resultado.no_raiz), resultado.estrelas[static_cast<std::size_t>(id)], id, dist);

                    list_pseudo.emplace_back(
                        resultado.no_raiz.get(),
                        resultado.estrelas[static_cast<std::size_t>(id)].get_luminosidade());

                    if (resultado.no_raiz->sub_maior != nullptr) {
                        resultado.no_raiz->sub_maior->volta = resultado.no_raiz.get();
                    } else if (resultado.no_raiz->sub_menor != nullptr) {
                        resultado.no_raiz->sub_menor->volta = resultado.no_raiz.get();
                    }

                    ++i;
                }
            }

            ClassificadorWDS::classificar_multiplo(resultado.estrelas, list_pseudo);
            break;
        }
    }

    return resultado;
}

