#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "sistema_arvore.hpp"
//||\n\033[0m?
   
//Aleatoriza estrelas e nome.
std::string random_nome(std::mt19937 &mt);
void random_estrelas(std::mt19937 &mt,std::string _nome, std::vector<Estrela> &sistema, const unsigned int &star_size);
unsigned int random_quanti(std::mt19937 & mt);

//Gera proceduralmente as orbitas e hierarquia do sistema.
double random_excentri(std::mt19937 &mt);
double random_dist(std::mt19937 &mt, double menor_dist_pos, double maior_dist_pos);
double monta_orbitas(std::mt19937 &mt, Estrela &estrela1, Estrela &estrela2);
double monta_orbitas(std::mt19937 &mt, No * sub_conj, Estrela &estrela);
double monta_orbitas(std::mt19937 &mt, No * sub1, No * sub2);

//Classifica as estrelas de acordo com suas massas.
void classifica_binarios(Estrela &estrela_1, Estrela &estrela_2);
void classifica_trinarios(std::vector<Estrela> &sistema);
void classifica_multiplas(std::vector<Estrela> &list, std::vector<Pseudo_no> &ps_list);

//Revela o sistema no terminal.
std::string formata_cientifico(double valor);//converte números de notação centifica para string.
void imprime_orbita(const Orbita &orbit, const std::string & bari);
void imp_estrela(Estrela &Estrela,  const std::string & bari);
void imprime_sistema(std::vector<Estrela> &list, No * sub);


int main(){
    std::vector<Estrela> sistema_estelar;
    
    int seed = 0, escolhido = 0;
    char escolha;
    
    while(true){ 
        std::cout << "\n\nPrograma para criar sistemas de estrelas:\n\n";
        
        std::cout << "\nDigite [1] para escolher uma seed.\nDigite [2] para uma seed aleatoria.\n";
        std::cin.get(escolha);std::cin.ignore();
        if(escolha == '1' || escolha == '2'){
            if(escolha == '1'){
                escolhido = 1;
            }else if(escolha == '2'){
                escolhido = 2;
            }
            break;
        }else{
            system("cls");
            std::cout << "\n\033[31mNumero imformado e imvalido\033[0m";
            std::cout << "\ntente outra vez:\n";
        }
    }
    if(escolhido == 1){
        std::cout << "\nDigite a seed: ";std::cin >> seed; 
    }else{
        std::random_device random;
        seed = random(); 
    }
    std::cout << std::endl << "Seed = " << seed;

    std::mt19937 mt(seed);
    
    const unsigned int quanti = random_quanti(mt);
    sistema_estelar.reserve(quanti);  
    std::string nome_geral = random_nome(mt);

    random_estrelas(mt, nome_geral, sistema_estelar, quanti);
   
    switch(quanti){
        case 0:
            std::cout << "\nErro: nenhuma estrela foi gerada !!!";
            return -1;
            break;
        case 1:
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n                     Sistema " << nome_geral ;
            std::cout << "\n\n=============================================================================\n";

            std::cout << "\n     Estrela  " << sistema_estelar[0].get_nome() << ":\n";
            std::cout << " " << std::left << std::setw(16) << "Tipo"        << ": " << sistema_estelar[0].get_tipo() << "\n";
            std::cout << " " << std::left << std::setw(16) << "Massa"       << ": " << formata_cientifico(sistema_estelar[0].get_massa()) << " kg;\n";
            std::cout << " " << std::left << std::setw(16) << "Raio"        << ": " << formata_cientifico(sistema_estelar[0].get_raio()) << " metros;\n";
            std::cout << " " << std::left << std::setw(16) << "Temperatura" << ": " << sistema_estelar[0].get_temperatura() << " graus celsius;\n";
            
            break;
    
        case 2:{
            classifica_binarios(sistema_estelar[0], sistema_estelar[1]);
           
            double dist = monta_orbitas(mt, sistema_estelar[0], sistema_estelar[1]);                                                                        
           
            std::cout << "\nDistancia usada nos calculos: " << dist;
    
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n                   Sistema binario " << nome_geral ;
            std::cout << "\n\n=============================================================================\n";

            imp_estrela(sistema_estelar[0], "");
            std::cout << "\n\n=============================================================================\n";
            imp_estrela(sistema_estelar[1], "");
            
            break;
        }
        case 3:{
            classifica_trinarios(sistema_estelar);
    
            double dist = monta_orbitas(mt, sistema_estelar[0], sistema_estelar[1]);
            auto n = std::make_unique<No>(sistema_estelar[0], 0, sistema_estelar[1], 1, dist);
    
            dist = monta_orbitas(mt, n.get(), sistema_estelar[2]);
            n = std::make_unique<No>(n, sistema_estelar[2], 2, dist);

            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n                  Sistema trinario" << nome_geral ;
    
            imprime_sistema(sistema_estelar, n.get());
            
            break;
        }
        case 4 ... 7:{
            std::vector<Pseudo_no> list_pseudo;
            list_pseudo.reserve(quanti -1);
    
            std::uniform_int_distribution<int> sorte(0,1);
            
            double dist = monta_orbitas(mt, sistema_estelar[0], sistema_estelar[1]);
            auto no_raiz = std::make_unique<No>(sistema_estelar[0], 0, sistema_estelar[1], 1, dist);
            list_pseudo.emplace_back(no_raiz, std::max(sistema_estelar[0].get_massa(), sistema_estelar[1].get_massa()));
    
            int i = 2, par_conj = 0;
    
            while(i < quanti){
                par_conj = sorte(mt);
    
                if(par_conj == 1 && (i + 1 <= quanti - 1)){
                    int id1 = i;
                    int id2 = i+1;

                    dist = monta_orbitas(mt, sistema_estelar[id1], sistema_estelar[id2]);
                    auto p = std::make_unique<No>(sistema_estelar[id1], id1, sistema_estelar[id2], id2, dist);
                    list_pseudo.emplace_back(p, std::max(sistema_estelar[id1].get_massa(), sistema_estelar[id2].get_massa()));
    
                    dist = monta_orbitas(mt, no_raiz.get(), p.get());
                    no_raiz = std::make_unique<No>(no_raiz, p, dist);
    
                    no_raiz->sub_maior->volta = no_raiz.get();
                    no_raiz->sub_menor->volta = no_raiz.get();
    
                    i += 2;
                }else{
                    dist = monta_orbitas(mt, no_raiz.get(), sistema_estelar[i]);
                    no_raiz = std::make_unique<No>(no_raiz, sistema_estelar[i], i, dist);
    
                    list_pseudo.emplace_back(no_raiz, sistema_estelar[i].get_massa());
    
                    if(no_raiz->sub_maior != nullptr){
                        no_raiz->sub_maior->volta = no_raiz.get();
    
                    }else{
                        no_raiz->sub_menor->volta= no_raiz.get();
                    }
    
                    i++;
                }
            }
    
            classifica_multiplas(sistema_estelar, list_pseudo);
            list_pseudo.clear();
    
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n         Sistema multiplo de "<< quanti << " estrelas " << nome_geral ;

            imprime_sistema(sistema_estelar, no_raiz.get());
            break;
        }
        default:
            std::cout << "\nErro: Foram geradas mais de 7 estrelas !!!";
            return -1;
        break;
    }
    std::cout << "\n\n=============================================================================\n";
    sistema_estelar.clear();

    std::cout << "\n\n\033[32mPrograma finalizado\033[0m";
    return 0;
}

//Primeiras etapas da geração.
unsigned int random_quanti(std::mt19937 & mt){
    std::uniform_int_distribution<int> de_um_a_cem(0, 100);
    int cem = de_um_a_cem(mt);
    unsigned int t = 0;
    if(cem < 75){
        t = 1;
    }else if(cem >= 75 && cem < 85){
        t = 2;
    }else if(cem >= 85 && cem < 92){
        t = 3;
    }else if(cem >= 92 && cem < 95){
        t = 4;
    }else if(cem >= 95 && cem < 98){
        t = 5;
    }else if(cem >= 98 && cem < 99){
        t = 6;
    }else{
        t = 7;
    }
    return t;
};
std::string random_nome(std::mt19937 &mt){

    char letras_maiusculas[26]{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char num[10]{'0','1','2','3','4','5','6','7','8','9'};
    std::uniform_int_distribution<int> de_1_a_5(1, 5);
    std::uniform_int_distribution<int> de_1_a_3(1, 3);
    std::uniform_int_distribution<int> letras_sort(0, 25);
    std::uniform_int_distribution<int> num_sort(0, 9);
    std::string Nome = "";
    int r = 0, R = 0, numP = 0, letraP = 0, quanti = 0, quanti2 = 0;

    quanti = de_1_a_5(mt);
    for(r = 0; r < quanti; r++){
        letraP = letras_sort(mt);    
        Nome.push_back(letras_maiusculas[letraP]);
    }

    Nome.push_back(' ');
    quanti2 = de_1_a_3(mt);
    for(r = 0; r < quanti2; r++){
        quanti = de_1_a_5(mt);
        for(R = 0; R < quanti; R++){
            numP = num_sort(mt);
            Nome.push_back(num[numP]);         
        } 
        Nome.push_back('-');
    }
    Nome.pop_back();
    return Nome;
}
void random_estrelas(std::mt19937 &mt,std::string _nome, std::vector<Estrela> &sistema, const unsigned int &star_size){ 
    std::uniform_int_distribution<int> de_um_a_cem(0, 100);
    std::string _tipo = "";
    double _raio = 0.0;
    double _massa = 0.0;
    double _lumi = 0.0;
    int tipo = 0, _temp = 0;

    for(int i = 0; i < star_size; i++){
        tipo = de_um_a_cem(mt);

        if(tipo < 50){// M - Anã Vermelha
            std::uniform_real_distribution<double> massa(0.08, 0.45);
            _massa = massa(mt);
            _raio = _massa;
            _lumi = std::pow(_massa, 2.3); 
            double temp_calc = 5778.0 * std::pow(_lumi / std::pow(_raio, 2), 0.25);
            _temp = static_cast<int>(temp_calc);
            
            // Clamping de segurança para manter dentro dos limites da classe M
            if(_temp < 1700) _temp = 1700;
            if(_temp > 3200) _temp = 3200;

            _tipo = "\033[31mAna Vermelha\033[0m";
    
        }else if(tipo >= 50 && tipo < 70){// K - Anã Laranja
            std::uniform_real_distribution<double> massa(0.45, 0.8);
            _massa = massa(mt);
            
            _raio = std::pow(_massa, 0.8);
            _lumi = std::pow(_massa, 4.0);
            
            double temp_calc = 5778.0 * std::pow(_lumi / std::pow(_raio, 2), 0.25);
            _temp = static_cast<int>(temp_calc);
            
            if(_temp < 3600) _temp = 3600;
            if(_temp > 5000) _temp = 5000;
            
            _tipo = "\033[93mAna Laranja\033[0m";
    
        }else if(tipo >= 70 && tipo < 80){// G - Anã Amarela
            std::uniform_real_distribution<double> massa(0.8, 1.04);
            _massa = massa(mt);
            
            _raio = _massa; 
            _lumi = std::pow(_massa, 4.0);
            
            double temp_calc = 5778.0 * std::pow(_lumi / std::pow(_raio, 2), 0.25);
            _temp = static_cast<int>(temp_calc);
            
            if(_temp < 5000) _temp = 5000;
            if(_temp > 5800) _temp = 5800;
            
            _tipo = "\033[33mAna Amarela\033[0m";
    
        }else if(tipo >= 80 && tipo < 89){// F - Subgigante
            std::uniform_real_distribution<double> massa(1.04, 1.4);
            _massa = massa(mt);
            
            std::uniform_real_distribution<double> fator_expansao(1.15, 2.5);
            _raio = std::pow(_massa, 0.7) * fator_expansao(mt);
            _lumi = std::pow(_massa, 4.3);
            
            double temp_calc = 5778.0 * std::pow(_lumi / std::pow(_raio, 2), 0.25);
            _temp = static_cast<int>(temp_calc);
            
            if(_temp < 5800) _temp = 5800;
            if(_temp > 7300) _temp = 7300;
            
            _tipo = "\033[37mSubgigante\033[0m";
    
        }else if(tipo >= 89 && tipo < 95){// A - Tipo 'A' Gigante
            std::uniform_real_distribution<double> massa(1.4, 2.1); 
            _massa = massa(mt);
            
            std::uniform_real_distribution<double> fator_gigante(5.0, 15.0);
            _raio = fator_gigante(mt);
            
            std::uniform_int_distribution<int> temp(7300, 9700);
            _temp = temp(mt); 
            
            _tipo = "\033[34mTipo 'A' Gigante\033[0m";
    
        }else if(tipo >= 95 && tipo < 98){// B - Supergigante
            std::uniform_real_distribution<double> massa(2.0, 16.0);
            _massa = massa(mt);

            std::uniform_real_distribution<double> raio(20.0, 150.0);
            _raio = raio(mt);

            std::uniform_int_distribution<int> temp(9700, 29700);
            _temp = temp(mt); 

            _tipo = "\033[36mSupergigante\033[0m";
    
        }else{// O - Hipergigante
            std::uniform_real_distribution<double> massa(16.0, 50.0);
            _massa = massa(mt);
            
            std::uniform_real_distribution<double> raio(150.0, 800.0);
            _raio = raio(mt);
            
            std::uniform_int_distribution<int> temp(29700, 50000);
            _temp = temp(mt); 
            
            _tipo = "\033[95mHipergigante\033[0m";
        }
        
        sistema.emplace_back(_nome, _tipo, _massa, _raio, _temp);
    }
}

//Segunda etapa da geração: randonizando orbitas.
double random_excentri(std::mt19937 &mt){
    std::uniform_int_distribution<int> numeros(10, 70);
    double n = static_cast<double>(numeros(mt));
    return n / 100.0f;
}
double random_dist(std::mt19937 &mt, double menor_dist_pos, double maior_dist_pos){
    std::uniform_real_distribution<double> dist(menor_dist_pos, maior_dist_pos);
    return dist(mt);
}
double monta_orbitas(std::mt19937 &mt, No * sub_conj, Estrela &estrela){
    
    if(sub_conj == nullptr)return 0.1;

    double raio_de_Hill = sub_conj->membro_dist * 2.4;
    double excent = random_excentri(mt);
    
    double m_subi = sub_conj->massa_sub;
    double m_star = estrela.get_massa();
    
    double distancia = raio_de_Hill / ((1.0 - excent) * std::cbrt((m_subi /(3.0 * m_star))));

    if(m_star < m_subi * 0.3){// evita que uma estrela menos massiva passe entre o par binario
        distancia = random_dist(mt, distancia * 2.0, distancia * 15.0);

    }

    if(m_subi > m_star){
        double apoastro_1 = distancia * (m_subi /(m_star + m_subi));
        double apoastro_2 = distancia - apoastro_1;

        estrela.set_orbita(calcula_orbita(apoastro_1, excent, true));
        sub_conj->orbt = calcula_orbita(apoastro_2, excent, true);

    }else{
        double apoastro_1 = distancia * (m_star /(m_subi + m_star));
        double apoastro_2 = distancia - apoastro_1;

        sub_conj->orbt = calcula_orbita(apoastro_1, excent, true);
        estrela.set_orbita(calcula_orbita(apoastro_2, excent, true));
    }

    return distancia;
}
double monta_orbitas(std::mt19937 &mt, Estrela &estrela1, Estrela &estrela2){// essa foi desafiadora

    double menor_valor = estrela1.get_raio() + estrela2.get_raio(); 
    double maior_valor = menor_valor * 90.0;

    double dist_entre_par = random_dist(mt, menor_valor, maior_valor);
    double exc = random_excentri(mt);

    double m1 = estrela1.get_massa();
    double m2 = estrela2.get_massa();

    if(m1 >= m2){
        double apo1 = dist_entre_par * (m1 / (m2 + m1));
        double apo2 = dist_entre_par - apo1;

        estrela2.set_orbita(calcula_orbita(apo1, exc, true));
        estrela1.set_orbita(calcula_orbita(apo2, exc, true));
    }else{
        double apo1 = dist_entre_par * (m2 / (m1 + m2));
        double apo2 = dist_entre_par - apo1;

        estrela1.set_orbita(calcula_orbita(apo1, exc, true));
        estrela2.set_orbita(calcula_orbita(apo2, exc, true));
    }
    
    return dist_entre_par;
}
double monta_orbitas(std::mt19937 &mt, No * sub1, No * sub2){

    if(sub1 == nullptr || sub2 == nullptr){
        std::cout << "\nErro: A função monta_orbitas falhou.";
        return 0.1;
    }

    double menor_dist = (sub1->membro_dist + sub2->membro_dist);
    double maior_dist = menor_dist * 10.0;

    double dist_entre_sub = random_dist(mt, menor_dist, maior_dist);
    double exc = random_excentri(mt);
    
    double m1 = sub1->massa_sub;
    double m2 = sub2->massa_sub;

    if(m1 >= m2){
        double apo1 = dist_entre_sub * (m1 / (m1 + m2));
        double apo2 = dist_entre_sub - apo1;

        sub2->orbt = calcula_orbita(apo1, exc, true);
        sub1->orbt = calcula_orbita(apo2, exc, true);
    }else{
        double apo1 = dist_entre_sub * (m2 / (m1 + m2));
        double apo2 = dist_entre_sub - apo1;

        sub1->orbt = calcula_orbita(apo1, exc, true);
        sub2->orbt = calcula_orbita(apo2, exc, true);
    }

    return dist_entre_sub;
}

//Classifica as estrelas de acordo com suas massas.
void classifica_binarios(Estrela &estrela_1, Estrela &estrela_2){
    std::string estrela_nome_1 = estrela_1.get_nome();
    std::string estrela_nome_2 = estrela_2.get_nome();
    estrela_nome_2.push_back(' ');
    estrela_nome_1.push_back(' ');

    if(estrela_1.get_massa() > estrela_2.get_massa()){
        estrela_nome_1.push_back('A');
        estrela_nome_2.push_back('B');

        estrela_1.set_nome(estrela_nome_1);
        estrela_2.set_nome(estrela_nome_2);
        return;
    }

    estrela_nome_1.push_back('B');
    estrela_nome_2.push_back('A');

    estrela_1.set_nome(estrela_nome_1);
    estrela_2.set_nome(estrela_nome_2);
        
    return;
}
void classifica_trinarios(std::vector<Estrela> &sistema){
    
    if(sistema.size() != 3)return;

    std::string nome_0 = sistema[0].get_nome();
    std::string nome_1 = sistema[1].get_nome();
    std::string nome_2 = sistema[2].get_nome();
    nome_0.push_back(' ');
    nome_1.push_back(' ');
    nome_2.push_back(' ');

    if(sistema[0].get_massa() > sistema[1].get_massa() && sistema[0].get_massa() > sistema[2].get_massa()){
        nome_0.push_back('A');
        sistema[0].set_nome(nome_0);

        if(sistema[1].get_massa() > sistema[2].get_massa()){
            nome_1.push_back('B');
            nome_2.push_back('C');
            sistema[1].set_nome(nome_1);
            sistema[2].set_nome(nome_2);

            return;
        }

        nome_1.push_back('C');
        nome_2.push_back('B');
        sistema[1].set_nome(nome_1);
        sistema[2].set_nome(nome_2);

        return;
    }

    if(sistema[1].get_massa() > sistema[0].get_massa() && sistema[1].get_massa() > sistema[2].get_massa()){
        nome_1.push_back('A');
        sistema[1].set_nome(nome_1);

        if(sistema[0].get_massa() > sistema[2].get_massa()){
            nome_0.push_back('B');
            nome_2.push_back('C');
            sistema[0].set_nome(nome_0);
            sistema[2].set_nome(nome_2);

            return;
        }

        nome_0.push_back('C');
        nome_2.push_back('B');
        sistema[0].set_nome(nome_0);
        sistema[2].set_nome(nome_2);

        return;
    }

    nome_2.push_back('A');
    sistema[2].set_nome(nome_2);

    if(sistema[1].get_massa() > sistema[0].get_massa()){
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
void classifica_multiplas(std::vector<Estrela> &list, std::vector<Pseudo_no> &ps_list){
    //responsavel por clasificar o sistema conforme o Washington double star catalog.

    char letras[7]{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    int letra_atual = 0;
    No * proximo = nullptr;
    const int list_size = ps_list.size();
    
    for(int i = 0; i < list_size; i++){
        double maior = 0.0;
        unsigned int maior_id = 0; 
        int major = -1, menor = -1;

        for(int l = 0; l < list_size; l++){

            if(!ps_list[l].utilizado){

                if(maior < ps_list[l].mass){
                    maior = ps_list[l].mass; 
                    maior_id = l;
                }
            }
        }

        ps_list[maior_id].utilizado = true;
        proximo = ps_list[maior_id].no_original;

        if(proximo == nullptr){
            std::cout << "Funcao de classificacao falhou";
            return;
        }

        proximo->get_estrelas_id(major, menor);
  
        if(major >= 0 && menor >= 0){
            std::string n = list[major].get_nome();
            list[major].set_nome(n + " " + letras[letra_atual] + "a");
            n = list[menor].get_nome();
            list[menor].set_nome(n + " " + letras[letra_atual] + "b");
           
        }else if(major >= 0){
            std::string n = list[major].get_nome();
            list[major].set_nome(n + " " + letras[letra_atual]);
           
        }else if(menor >= 0){
            std::string n = list[menor].get_nome();
            list[menor].set_nome(n + " " + letras[letra_atual]);

        }
        
        while(proximo != nullptr){
            proximo->nome.push_back(letras[letra_atual]);
            proximo = proximo->volta;
        }
        letra_atual++;
    }
}

//funções responsaveis pela impressão.
std::string formata_cientifico(double valor){

    if(valor == 0)return "0";

    int expoente = std::floor(std::log10(std::abs(valor)));
    double mantissa = valor / std::pow(10, expoente);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << mantissa;
    ss << " x 10^" << expoente;

    return ss.str();
}
void imprime_orbita(const Orbita &orbit, const std::string & bari){
    std::cout << " " << std::left << std::setw(16) << "apoastro"       << ": " << formata_cientifico(orbit.apoastro) << " metros de distancia do baricentro " << bari << ";\n";
    std::cout << " " << std::left << std::setw(16) << "periastro"      << ": " << formata_cientifico(orbit.periastro) << " metros de distancia do baricentro " << bari << ";\n";
    std::cout << " " << std::left << std::setw(16) << "excentricidade" << ": " << orbit.excentricidade;
}
void imp_estrela(Estrela &Estrela,  const std::string & bari){

    std::cout << "\n\n";
    std::cout << "     Estrela  " << Estrela.get_nome() << ":\n";
    std::cout << " " << std::left << std::setw(16) << "Tipo"        << ": " << Estrela.get_tipo() << "\n";
    std::cout << " " << std::left << std::setw(16) << "Massa"       << ": " << formata_cientifico(Estrela.get_massa()) << " kg;\n";
    std::cout << " " << std::left << std::setw(16) << "Raio"        << ": " << formata_cientifico(Estrela.get_raio()) << " metros;\n";
    std::cout << " " << std::left << std::setw(16) << "Temperatura" << ": " << Estrela.get_temperatura() << " graus celsius;\n";

    std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";

    imprime_orbita(Estrela.get_orbita(), bari);
}
void imprime_sistema(std::vector<Estrela> &list, No * sub){

    if(sub == nullptr)return;

    std::cout << "\n\n=============================================================================\n";
    std::cout << "\nSubsistema " << sub->nome << ":";

    int star_maior = 0, star_menor = 0;
    sub->get_estrelas_id(star_maior, star_menor);

    if(star_maior >= 0 && star_menor >= 0){
        std::cout << "\nComposto por duas estrelas:";
        std::cout << "\n\nEstrela mais massiva:";
        imp_estrela(list[star_maior], sub->nome);

        std::cout << "\n\nEstrela menos massiva:";
        imp_estrela(list[star_menor], sub->nome);

        return;
    }
    if(star_menor >= 0 && sub->sub_maior != nullptr){
        std::cout << "\nComposto por uma estrela e um subsistema:";
        
        imp_estrela(list[star_menor], sub->nome);

        std::cout << ".\n\nSubsistema " << sub->sub_maior->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";

        imprime_orbita(sub->sub_maior->orbt, sub->nome);
        
        imprime_sistema(list, sub->sub_maior.get());

    }else if(sub->sub_maior != nullptr && sub->sub_menor != nullptr){
        std::cout << "\nComposto por dois subsistemas rivais:";

        std::cout << "\n\nSubsistema " << sub->sub_maior->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";
        imprime_orbita(sub->sub_maior->orbt, sub->nome);

        std::cout << "\n\nSubsistema " << sub->sub_menor->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";
        imprime_orbita(sub->sub_menor->orbt, sub->nome);

        imprime_sistema(list, sub->sub_maior.get());
        imprime_sistema(list, sub->sub_menor.get());
        
    }else if(star_maior >= 0 && sub->sub_menor != nullptr){
        std::cout << "\ncomposto por uma estrela e um subsistema:";
        
        imp_estrela(list[star_maior], sub->nome);

        std::cout << ".\n\nSubsistema " << sub->sub_menor->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";

        imprime_orbita(sub->sub_menor->orbt, sub->nome);

        imprime_sistema(list, sub->sub_menor.get());
    }
}

