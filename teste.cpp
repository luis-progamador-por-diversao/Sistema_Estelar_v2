#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include "sistema_arvore.hpp"
//||\n\033[0m?
   
//Aleatoriza estrelas e nome.
std::string nome(std::mt19937 &mt);
void estrelas_aleatorias(std::mt19937 &mt, std::vector<Estrela> &sistema);

//Gera proceduralmente as orbitas e hierarquia do sistema.
float excentri(std::mt19937 &mt);
float distancia_aleatoria(std::mt19937 &mt, float menor_dist_pos, float maior_dist_pos);
float monta_orbitas(std::mt19937 &mt, Estrela &estrela1, Estrela &estrela2);
float monta_orbitas(std::mt19937 &mt, No * sub_conj, Estrela &estrela);

//Classifica as estrelas de acordo com suas massas.
void nomeia_pares_binarios(Estrela &estrela_1, Estrela &estrela_2, bool binario = true);
void classifica_trinarios(std::vector<Estrela> &sistema);
void classifica_estrelas(std::vector<Estrela> &list, std::vector<Pseudo_no> &ps_list);

//Revela o sistema no terminal.
void imprime_orbita(const Orbita &orbit, const std::string & bari);
void imprime_estrela(Estrela &Estrela,  const std::string & bari);
void imprime_sistema(std::vector<Estrela> &list, No * sub, int &poss);

int main(){//em desenvolvimento...
    std::vector<Estrela> sistema_estelar;
    std::vector<Pseudo_no> list_pseudo;
    int seed = 0, escolhido = 0;
    char escolha;
    
    while(true){ 
        std::cout << "\n\nProgama para criar sistemas de estrelas:\n\n";
        
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
    std::cout << "\nGerando Estrelas...\n";

    std::mt19937 mt(seed);
    /*
    std::uniform_int_distribution<int> de_um_a_cem(0, 100);
    int cem = de_um_a_cem(mt);
    int quanti = 0;
    if(cem < 75){
        quanti = 1;
    }else if(cem >= 75 && cem < 85){
        quanti = 2;
    }else if(cem >= 85 && cem < 92){
        quanti = 3;
    }else if(cem >= 92 && cem < 95){
        quanti = 4;
    }else if(cem >= 95 && cem < 98){
        quanti = 5;
    }else if(cem >= 98 && cem < 99){
        quanti = 6;
    }else{
        quanti = 7;
    }
    */
    const int quanti = 4;
    sistema_estelar.reserve(quanti);  
   
    estrelas_aleatorias(mt, sistema_estelar);
   
    switch(quanti){
    case 1:
       imprime_estrela(sistema_estelar[0], "");
       break;

    case 2:
       nomeia_pares_binarios(sistema_estelar[0], sistema_estelar[1], true);
       
       monta_orbitas(mt, sistema_estelar[0], sistema_estelar[1]);                                                                        
       
       imprime_estrela(sistema_estelar[0], "");
       imprime_estrela(sistema_estelar[1], "");
       
       break;
    
    case 3:
        classifica_trinarios(sistema_estelar);

        //for(int R = 0; R < quanti; R++){imprime_estrela(sistema_estelar[R]);}

        break;
    default:
        list_pseudo.reserve(quanti -1);

        std::uniform_int_distribution<int> sorte(0,1);
        
        float dist = monta_orbitas(mt, sistema_estelar[0], sistema_estelar[1]);
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

                no_raiz = std::make_unique<No>(no_raiz, p);

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

        classifica_estrelas(sistema_estelar, list_pseudo);
        list_pseudo.clear();

        i = 0;
        imprime_sistema(sistema_estelar, no_raiz.get(), i);

        break;
    }

    sistema_estelar.clear();

    std::cout << "\n\n\033[32mProgama finalizado\033[0m";
    return 0;
}


std::string nome(std::mt19937 &mt){

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
void estrelas_aleatorias(std::mt19937 &mt, std::vector<Estrela> &sistema){ 
    std::uniform_int_distribution<int> de_um_a_cem(0, 100);
    std::string _nome = nome(mt);
    std::string _tipo = "";
    float _raio = 0.0;
    float _massa = 0.0;
    //float _lumi = 0.0f;
    int tipo = 0, _temp = 0;
    const int capacidade = sistema.capacity();

    for(int i = 0; i < capacidade; i++){
        tipo = de_um_a_cem(mt);

        if(tipo < 50){//M
            std::uniform_real_distribution<float> raio(0.1f, 0.7f);
            std::uniform_real_distribution<float> massa(0.0175f, 0.5f);
            std::uniform_int_distribution<int> temp(1700, 3200);
            //std::uniform_real_distribution<float> luz(0.075f, 0.1f);
            //_lumi = luz(*mt);
            _raio = raio(mt);
            _massa = massa(mt);
            _temp = temp(mt); 
            _tipo = "\033[31mAna Vermelha\033[0m";
    
        }else if(tipo >= 50 && tipo < 70){// k
            std::uniform_real_distribution<float> raio(0.7f, 0.96f);
            std::uniform_real_distribution<float> massa(0.5f, 0.8f);
            //std::uniform_real_distribution<float> luz(0.1f, 0.6f);
            std::uniform_int_distribution<int> temp(3600, 5000);
            //_lumi = luz(*mt);
            _raio = raio(mt);
            _massa = massa(mt);
            _temp = temp(mt); 
            _tipo = "\033[93mAna Laranja\033[0m";
    
        }else if(tipo >= 70 && tipo < 80){// G
            std::uniform_real_distribution<float> raio(0.84f, 1.15f);
            std::uniform_real_distribution<float> massa(0.8f, 1.2f);
            //std::uniform_real_distribution<float> luz(0.5f, 2.0f);
            std::uniform_int_distribution<int> temp(5000, 5800);
            _raio = raio(mt);
            _massa = massa(mt);
            //_lumi = luz(*mt);
            _temp = temp(mt); 
            _tipo = "\033[33mAna Amarela\033[0m";
    
        }else if(tipo >= 80 && tipo < 89){// F
            std::uniform_real_distribution<float> raio(1.15f, 10.0f);
            std::uniform_real_distribution<float> massa(1.2f, 1.7f);
            //std::uniform_real_distribution<float> luz(1.5f, 5.0f);
            std::uniform_int_distribution<int> temp(5800, 7300);
            _raio = raio(mt);
            _massa = massa(mt);
            //_lumi = luz(*mt);
            _temp = temp(mt); 
            _tipo = "\033[37mSubgigante\033[0m";
    
        }else if(tipo >= 89 && tipo < 95){// A
            std::uniform_real_distribution<float> raio(10.0f, 100.0f);
            std::uniform_real_distribution<float> massa(1.7f, 2.1f); 
            //std::uniform_real_distribution<float> luz(5.0f, 50.0f);
            std::uniform_int_distribution<int> temp(7300, 9700);
            _raio = raio(mt);
            _massa = massa(mt);
            //_lumi = luz(*mt);
            _temp = temp(mt); 
            _tipo = "\033[34mTipo 'A' Gigante\033[0m";
    
        }else if(tipo >= 95 && tipo < 98){// B6
            std::uniform_real_distribution<float> raio(100.0f, 1000.0f);
            std::uniform_real_distribution<float> massa(2.0f, 16.0f);
            //std::uniform_int_distribution<int> luz(1000, 100000);
            std::uniform_int_distribution<int> temp(9700, 29700);
            _raio = raio(mt);
            _massa = massa(mt);
            //_lumi = luz(*mt);
            _temp = temp(mt); 
            _tipo = "\033[36mSupergigante\033[0m";
    
        }else{// O
            std::uniform_real_distribution<float> raio(1000.0f, 2500.0f);
            std::uniform_real_distribution<float> massa(16.0f, 50.0f);
            //std::uniform_int_distribution<int> luz(100000, 1000000);
            std::uniform_int_distribution<int> temp(29700, 100000);
            _raio = raio(mt);
            _massa = massa(mt);
            //_lumi = luz(*mt);
            _temp = temp(mt); 
            _tipo = "\033[95mHipergigante\033[0m";
        }
        sistema.emplace_back(_nome, _tipo, _massa, _raio, _temp);
    }
}


float excentri(std::mt19937 &mt){
    std::uniform_int_distribution<int> numeros(10, 70);
    float n = static_cast<float>(numeros(mt));
    return n / 100.0f;
}
float distancia_aleatoria(std::mt19937 &mt, float menor_dist_pos, float maior_dist_pos){
    std::uniform_real_distribution<float> dist(menor_dist_pos, maior_dist_pos);
    return dist(mt);
}
float monta_orbitas(std::mt19937 &mt, No * sub_conj, Estrela &estrela){
    
    if(sub_conj == nullptr)return 0.0f;

    float raio_de_Hill = sub_conj->membro_dist * 2.4f;
    float excent = excentri(mt);
    
    float distancia = calculo_distancia_pela_esfera_de_Hill(raio_de_Hill, excent, sub_conj->massa_sub, estrela.get_massa());

    if(estrela.get_massa() < sub_conj->massa_sub * 0.3f){// evita que uma estrela menos massiva passe entre o par binario
        distancia = distancia_aleatoria(mt, distancia * 2.0f, distancia * 15.0f);

    }

    if(sub_conj->massa_sub > estrela.get_massa()){
        float maior_valor = sub_conj->massa_sub;
        float menor_valor = estrela.get_massa();

        float apoastro_1 = distancia * (maior_valor /(maior_valor + menor_valor));
        float apoastro_2 = distancia - apoastro_1;

        estrela.set_orbita(calcula_orbita(apoastro_1, excent, true));
        sub_conj->orbt = calcula_orbita(apoastro_2, excent, true);
    }else{
        float maior_valor = sub_conj->massa_sub;
        float menor_valor = estrela.get_massa();

        float apoastro_1 = distancia * (maior_valor /(maior_valor + menor_valor));
        float apoastro_2 = distancia - apoastro_1;

        sub_conj->orbt = calcula_orbita(apoastro_1, excent, true);
        estrela.set_orbita(calcula_orbita(apoastro_2, excent, true));
    }

    return distancia;
}
float monta_orbitas(std::mt19937 &mt, Estrela &estrela1, Estrela &estrela2){// essa foi desafiadora

    float menor_valor = estrela1.get_raio() + estrela2.get_raio(); 
    float maior_valor = menor_valor * 90.0f;
    float dist_media_entre_pares = distancia_aleatoria(mt, menor_valor, maior_valor);
    float exc = excentri(mt);

    if(estrela1.get_massa() > estrela2.get_massa()){
        maior_valor = estrela1.get_massa();
        menor_valor = estrela2.get_massa();

        float apoastro_1 =  dist_media_entre_pares * (maior_valor /(maior_valor + menor_valor));
        float apoastro_2 = dist_media_entre_pares - apoastro_1;

        estrela2.set_orbita(calcula_orbita(apoastro_1, exc, true));
        estrela1.set_orbita(calcula_orbita(apoastro_2, exc, true));
            
    }else{
        maior_valor = estrela2.get_massa();
        menor_valor = estrela1.get_massa();

        float apoastro_1 = dist_media_entre_pares * (maior_valor /(maior_valor + menor_valor)) ;
        float apoastro_2 = dist_media_entre_pares - apoastro_1;

        estrela1.set_orbita(calcula_orbita(apoastro_1, exc, true));
        estrela2.set_orbita(calcula_orbita(apoastro_2, exc, true));
    }  
    
    return dist_media_entre_pares;
}


void nomeia_pares_binarios(Estrela &estrela_1, Estrela &estrela_2, bool binario){
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

//||\n\033[0m?
//responsavel por clasificar o sistema conforme o Washington float star catalog.
void classifica_estrelas(std::vector<Estrela> &list, std::vector<Pseudo_no> &ps_list){
    char letras[7]{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    int letra_atual = 0;
    No * proximo = nullptr;
    const int list_size = ps_list.size();
    
    for(int i = 0; i < list_size; i++){
        float maior = 0.0;
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
void imprime_orbita(const Orbita &orbit, const std::string & bari){
    std::cout << "\nApoastro: " << orbit.apoastro << " metros de distancia do baricento " << bari;
    std::cout << "\n\nPeriastro: " << orbit.periastro << " metros de distancia do baricento " << bari;
    std::cout << "\nexcentricidade: " << orbit.excentricidade;
}
void imprime_estrela(Estrela &Estrela,  const std::string & bari){
    std::cout << "\n\n     Estrela  " << Estrela.get_nome();
    std::cout << "\nTipo: " << Estrela.get_tipo();
    std::cout << "\nMassa: " << Estrela.get_massa() << " kg";
    std::cout << "\nRaio: " << Estrela.get_raio() << " metros";
    std::cout << "\nTemperatura: " << Estrela.get_temperatura() << " graus celcios";
    std::cout << "\nPossui a seguinte orbita:\n";

    imprime_orbita(Estrela.get_orbita(), bari);
}
void imprime_sistema(std::vector<Estrela> &list, No * sub, int &poss){

    if(sub == nullptr)return;

    int meu_poss = poss;
    std::cout << "\n\n===========================================\n";
    std::cout << "\nSubsistema " << sub->nome << ":";
    poss++;

    int star_maior = 0, star_menor = 0;
    sub->get_estrelas_id(star_maior, star_menor);

    if(star_maior >= 0 && star_menor >= 0){
        std::cout << "\nEsse subsistema possui duas estrelas:\n";
        std::cout << "\nEstrela mais massiva:";
        imprime_estrela(list[star_maior], sub->nome);

        std::cout << "\n\nEstrela menos massiva:";
        imprime_estrela(list[star_menor], sub->nome);

        return;
    }
    if(star_menor >= 0 && sub->sub_maior != nullptr){
        std::cout << "\nPossui uma estrela orbitando o subsistema " << sub->sub_maior->nome << '\n';
        imprime_estrela(list[star_menor], sub->nome);

        std::cout << "\n";
        imprime_sistema(list, sub->sub_maior.get(), poss);

    }else if(sub->sub_maior != nullptr && sub->sub_menor != nullptr){
        std::cout << "\nEste subsistema nao possui nenhuma estrela\n";

        imprime_sistema(list, sub->sub_maior.get(), poss);

        imprime_sistema(list, sub->sub_menor.get(), poss);
        
    }else if(star_maior >= 0 && sub->sub_menor != nullptr){
        std::cout << "\nPossui uma estrela orbitando o subsistema " << sub->sub_menor->nome << '\n';
   
        imprime_estrela(list[star_maior], sub->nome);

        imprime_sistema(list, sub->sub_menor.get(), poss);
    }
}

