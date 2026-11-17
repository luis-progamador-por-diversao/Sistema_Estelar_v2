#include "geracao/classificador_wds.hpp"
#include <iostream>
#include <array>

namespace ClassificadorWDS {

void classificar_binario(Estrela &estrela1, Estrela &estrela2) {
    std::string nome1 = estrela1.get_nome();
    std::string nome2 = estrela2.get_nome();
    nome1.push_back(' ');
    nome2.push_back(' ');

    if (estrela1.get_luminosidade() > estrela2.get_luminosidade()) {
        nome1.push_back('A');
        nome2.push_back('B');
    } else {
        nome1.push_back('B');
        nome2.push_back('A');
    }

    estrela1.set_nome(nome1);
    estrela2.set_nome(nome2);
}

void classificar_trinario(std::vector<Estrela> &sistema, SubsistemaNo *raiz) {
    if (sistema.size() != 3 || raiz == nullptr) {
        std::cerr << "\nA funcao 'classificar_trinario' falhou: tamanho invalido ou raiz nula.\n";
        return;
    }

    std::string nome_0 = sistema[0].get_nome();
    std::string nome_1 = sistema[1].get_nome();
    std::string nome_2 = sistema[2].get_nome();
    nome_0.push_back(' ');
    nome_1.push_back(' ');
    nome_2.push_back(' ');

    raiz->nome = "ABC";

    const double l0 = sistema[0].get_luminosidade();
    const double l1 = sistema[1].get_luminosidade();
    const double l2 = sistema[2].get_luminosidade();

    if (l0 > l1 && l0 > l2) {
        nome_0.push_back('A');
        sistema[0].set_nome(nome_0);

        if (l1 > l2) {
            nome_1.push_back('B');
            nome_2.push_back('C');
            sistema[1].set_nome(nome_1);
            sistema[2].set_nome(nome_2);

            if (raiz->sub_maior) {
                raiz->sub_maior->nome = "AB";
            } else if (raiz->sub_menor) {
                raiz->sub_menor->nome = "AB";
            }
            return;
        }

        nome_1.push_back('C');
        nome_2.push_back('B');
        sistema[1].set_nome(nome_1);
        sistema[2].set_nome(nome_2);

        if (raiz->sub_maior) {
            raiz->sub_maior->nome = "AC";
        } else if (raiz->sub_menor) {
            raiz->sub_menor->nome = "AC";
        }
        return;
    }

    if (l1 > l0 && l1 > l2) {
        nome_1.push_back('A');
        sistema[1].set_nome(nome_1);

        if (l0 > l2) {
            nome_0.push_back('B');
            nome_2.push_back('C');
            sistema[0].set_nome(nome_0);
            sistema[2].set_nome(nome_2);

            if (raiz->sub_maior) {
                raiz->sub_maior->nome = "AB";
            } else if (raiz->sub_menor) {
                raiz->sub_menor->nome = "AB";
            }
            return;
        }

        nome_0.push_back('C');
        nome_2.push_back('B');
        sistema[0].set_nome(nome_0);
        sistema[2].set_nome(nome_2);

        if (raiz->sub_maior) {
            raiz->sub_maior->nome = "AC";
        } else if (raiz->sub_menor) {
            raiz->sub_menor->nome = "AC";
        }
        return;
    }

    if (raiz->sub_maior != nullptr) {
        raiz->sub_maior->nome = "BC";
    } else if (raiz->sub_menor != nullptr) {
        raiz->sub_menor->nome = "BC";
    }

    nome_2.push_back('A');
    sistema[2].set_nome(nome_2);

    if (l1 > l0) {
        nome_1.push_back('B');
        nome_0.push_back('C');
        sistema[1].set_nome(nome_1);
        sistema[0].set_nome(nome_0);
        return;
    }

    nome_1.push_back('C');
    nome_0.push_back('B');
    sistema[1].set_nome(nome_1);
    sistema[0].set_nome(nome_0);
}

void classificar_multiplo(std::vector<Estrela> &estrelas, std::vector<PseudoNo> &pseudo_lista) {
    static constexpr std::array<char, 7> letras{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    std::size_t letra_atual = 0;
    const std::size_t list_size = pseudo_lista.size();

    for (std::size_t i = 0; i < list_size; ++i) {
        double maior = -1.0;
        std::size_t maior_id = 0;

        for (std::size_t l = 0; l < list_size; ++l) {
            if (pseudo_lista[l].utilizado) continue;

            if (maior < pseudo_lista[l].lumi) {
                maior = pseudo_lista[l].lumi;
                maior_id = l;
            }
        }

        pseudo_lista[maior_id].utilizado = true;
        SubsistemaNo *proximo = pseudo_lista[maior_id].no_original;

        if (proximo == nullptr) {
            std::cerr << "\nErro: Classificacao WDS falhou por ponteiro nulo em pseudo_no.\n";
            return;
        }

        int major = -1;
        int menor = -1;
        proximo->get_estrelas_id(major, menor);

        const char letra = (letra_atual < letras.size()) ? letras[letra_atual] : '?';

        if (major >= 0 && menor >= 0) {
            const auto u_major = static_cast<std::size_t>(major);
            const auto u_menor = static_cast<std::size_t>(menor);

            if (u_major < estrelas.size()) {
                estrelas[u_major].set_nome(estrelas[u_major].get_nome() + " " + letra + "a");
            }
            if (u_menor < estrelas.size()) {
                estrelas[u_menor].set_nome(estrelas[u_menor].get_nome() + " " + letra + "b");
            }
        } else if (major >= 0) {
            const auto u_major = static_cast<std::size_t>(major);
            if (u_major < estrelas.size()) {
                estrelas[u_major].set_nome(estrelas[u_major].get_nome() + " " + letra);
            }
        } else if (menor >= 0) {
            const auto u_menor = static_cast<std::size_t>(menor);
            if (u_menor < estrelas.size()) {
                estrelas[u_menor].set_nome(estrelas[u_menor].get_nome() + " " + letra);
            }
        }

        while (proximo != nullptr) {
            proximo->nome.push_back(letra);
            proximo = proximo->volta;
        }

        ++letra_atual;
    }
}

void classificar(std::vector<Estrela> &estrelas, SubsistemaNo *raiz, std::vector<PseudoNo> &pseudo_lista) {
    if (estrelas.size() == 2) {
        classificar_binario(estrelas[0], estrelas[1]);
    } else if (estrelas.size() == 3) {
        classificar_trinario(estrelas, raiz);
    } else if (estrelas.size() >= 4) {
        classificar_multiplo(estrelas, pseudo_lista);
    }
}

} // namespace ClassificadorWDS

