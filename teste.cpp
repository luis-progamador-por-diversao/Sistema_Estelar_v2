#include <iostream>
#include <random>
#include <vector>
#include <stdlib.h>
#include "sistema_arvore.hpp"
//||\n\033[0m?
   
//Aleatoriza estrelas e nome.
std::string nome(std::mt19937 *mt);
void estrelas_aleatorias(std::mt19937 *motor_aleatorio, std::vector<Estrela> &sistema);

//Revela o sistema no terminal.
void imprime_orbita(const Orbita orbit);
void imprime_estrela(Estrela &Estrela);

//Gera proceduralmente as orbitas e hierarquia do sistema.
float excentri(std::mt19937 *motor);
double distancia_aleatoria(std::mt19937 *motor, float menor_dist_pos, float maior_dist_pos);
double monta_orbitas_de_binarios(std::mt19937 *motor, Estrela &estrela1, Estrela &estrela2);
double monta_orbitas_para_pares_binarios(std::mt19937 *motor, No &par_binario, Estrela &estrela);

//Classifica as estrelas de acordo com suas massas.
void nomeia_pares_binarios(Estrela &estrela_1, Estrela &estrela_2, bool binario = true);
void classifica_trinarios(std::vector<Estrela> &sistema);
void classifica_estrelas(No &no_raiz, const int quanti);

int main(){
    std::vector<Estrela> sistema_estelar;
    std::vector<No> galhos;
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
   
    estrelas_aleatorias(&mt, sistema_estelar);
   
    switch (quanti){
    case 1:
       imprime_estrela(sistema_estelar[0]);
       break;

    case 2:
       nomeia_pares_binarios(sistema_estelar[0], sistema_estelar[1], true);
       
       monta_orbitas_de_binarios(&mt, sistema_estelar[0], sistema_estelar[1]);                                                                        
       
       imprime_estrela(sistema_estelar[0]);
       imprime_estrela(sistema_estelar[1]);
       
       break;
    
    case 3:
        classifica_trinarios(sistema_estelar);

        for(int R = 0; R < quanti; R++){imprime_estrela(sistema_estelar[R]);}

        break;
    default:
        const int conju_quanti = quanti - 1;
        
        galhos.reserve(conju_quanti);
        double dist = monta_orbitas_de_binarios(&mt, sistema_estelar[0], sistema_estelar[1]);
        galhos.emplace_back(sistema_estelar[0], sistema_estelar[1], dist);
        galhos[0].modificado = true;

        std::uniform_int_distribution<int> _0_ou_1(0,1);
        int pares = 1;

        for(int i = 2, p = 0; i < quanti; i++, p++){
            pares = _0_ou_1(mt);

            if(pares == 0){
                if(i+1 <= quanti -1){
                    dist = monta_orbitas_de_binarios(&mt, sistema_estelar[i], sistema_estelar[i+1]);
                    galhos.emplace_back(sistema_estelar[i], sistema_estelar[i++], dist);

                }else{
                    pares = 1;
                }
            }
            if(pares == 1){
                dist = monta_orbitas_para_pares_binarios(&mt, galhos[p], sistema_estelar[i]);
                galhos.emplace_back(galhos[p], sistema_estelar[i], dist);
            }
        }

        if(galhos.capacity() != galhos.size()){
            unsigned int no_id = 0;

            while(galhos.size() < galhos.capacity()){
                int i = 0;

                while(i < galhos.size()){

                    if(!galhos[i].modificado && (galhos[i].subsistema_maior == nullptr && galhos[i].subsistema_menor == nullptr)){
                        no_id = i-1;
                        galhos[i].modificado = true;
                        break;
                    }
                    i++;
                }
                galhos.emplace_back(galhos.back(), galhos[no_id], 0.0);
            }
        }
        galhos[0].modificado = false;
        //classifica_estrelas(galhos.back(), sistema_estelar.size());

        for(int R = 0; R < quanti; R++){imprime_estrela(sistema_estelar[R]);}
        galhos.clear();

        break;
    }

    /*
    std::cout << "\n\nConferindo as posicoes do galhos de tamanho: " << galhos.size();
    for(int v = 0; v < galhos.size(); v++){
        std::cout << "\n\nconju[" << v << "].posicao1 = " << galhos[v].posicao_estrela1;
        std::cout << "\nconju[" << v << "].posicao2 = " << galhos[v].posicao_estrela2;
    }
    */
    sistema_estelar.clear();

    std::cout << "\n\n\033[32mProgama finalizado\033[0m";
    return 0;
}


std::string nome(std::mt19937 *mt){

    char letras_maiusculas[26]{'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    char num[10]{'0','1','2','3','4','5','6','7','8','9'};
    std::uniform_int_distribution<int> de_1_a_5(1, 5);
    std::uniform_int_distribution<int> de_1_a_3(1, 3);
    std::uniform_int_distribution<int> letras_sort(0, 25);
    std::uniform_int_distribution<int> num_sort(0, 9);
    std::string Nome = "";
    int r = 0, R = 0, numP = 0, letraP = 0, quanti = 0, quanti2 = 0;

    quanti = de_1_a_5(*mt);
    for(r = 0; r < quanti; r++){
        letraP = letras_sort(*mt);    
        Nome.push_back(letras_maiusculas[letraP]);
    }

    Nome.push_back(' ');
    quanti2 = de_1_a_3(*mt);
    for(r = 0; r < quanti2; r++){
        quanti = de_1_a_5(*mt);
        for(R = 0; R < quanti; R++){
            numP = num_sort(*mt);
            Nome.push_back(num[numP]);         
        } 
        Nome.push_back('-');
    }
    Nome.pop_back();
    return Nome;
}
void estrelas_aleatorias(std::mt19937 *motor_aleatorio, std::vector<Estrela> &sistema){ 
    std::uniform_int_distribution<int> de_um_a_cem(0, 100);
    std::string _nome = nome(motor_aleatorio);
    double _raio = 0.0;
    double _massa = 0.0;
    //float _lumi = 0.0f;
    int tipo = 0, _temp = 0;
    const int capacidade = sistema.capacity();
    char _classe = ' ';

    for(int i = 0; i < capacidade; i++){
        tipo = de_um_a_cem(*motor_aleatorio);

        if(tipo < 50){
            std::uniform_real_distribution<float> raio(0.1f, 0.7f);
            std::uniform_real_distribution<float> massa(0.0175f, 0.5f);
            std::uniform_int_distribution<int> temp(1700, 3200);
            //std::uniform_real_distribution<float> luz(0.075f, 0.1f);
            //_lumi = luz(*motor_aleatorio);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'M';
    
        }else if(tipo >= 50 && tipo < 70){// k
            std::uniform_real_distribution<float> raio(0.7f, 0.96f);
            std::uniform_real_distribution<float> massa(0.5f, 0.8f);
            //std::uniform_real_distribution<float> luz(0.1f, 0.6f);
            std::uniform_int_distribution<int> temp(3600, 5000);
            //_lumi = luz(*motor_aleatorio);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'K';
    
        }else if(tipo >= 70 && tipo < 80){// G
            std::uniform_real_distribution<float> raio(0.84f, 1.15f);
            std::uniform_real_distribution<float> massa(0.8f, 1.2f);
            //std::uniform_real_distribution<float> luz(0.5f, 2.0f);
            std::uniform_int_distribution<int> temp(5000, 5800);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            //_lumi = luz(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'G';
    
        }else if(tipo >= 80 && tipo < 89){// F
            std::uniform_real_distribution<float> raio(1.15f, 10.0f);
            std::uniform_real_distribution<float> massa(1.2f, 1.7f);
            //std::uniform_real_distribution<float> luz(1.5f, 5.0f);
            std::uniform_int_distribution<int> temp(5800, 7300);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            //_lumi = luz(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'F';
    
        }else if(tipo >= 89 && tipo < 95){// A
            std::uniform_real_distribution<float> raio(10.0f, 100.0f);
            std::uniform_real_distribution<float> massa(1.7f, 2.1f); 
            //std::uniform_real_distribution<float> luz(5.0f, 50.0f);
            std::uniform_int_distribution<int> temp(7300, 9700);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            //_lumi = luz(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'A';
    
        }else if(tipo >= 95 && tipo < 98){// B
            std::uniform_real_distribution<float> raio(100.0f, 1000.0f);
            std::uniform_real_distribution<float> massa(2.0f, 16.0f);
            //std::uniform_int_distribution<int> luz(1000, 100000);
            std::uniform_int_distribution<int> temp(9700, 29700);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            //_lumi = luz(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'B';
    
        }else{// O
            std::uniform_int_distribution<int> raio(1000, 2500);
            std::uniform_real_distribution<double> massa(16.0, 50.0);
            //std::uniform_int_distribution<int> luz(100000, 1000000);
            std::uniform_int_distribution<int> temp(29700, 100000);
            _raio = raio(*motor_aleatorio);
            _massa = massa(*motor_aleatorio);
            //_lumi = luz(*motor_aleatorio);
            _temp = temp(*motor_aleatorio); 
            _classe = 'O';
        }
        sistema.emplace_back(_nome, _massa, _raio, _temp, _classe);
    }
}


void imprime_estrela(Estrela &Estrela){
    std::cout << "\n\n     Estrela  " << Estrela.get_nome();
    std::cout << "\nClasse:  tipo " << Estrela.get_classe();
    std::cout << "\nmassa: " << Estrela.get_massa() << " kg";
    std::cout << "\nraio: " << Estrela.get_raio() << " metros";
    std::cout << "\ntemperatura: " << Estrela.get_temperatura() << " graus celcios";
    std::cout << "\nPossui a seguinte orbita:";
    imprime_orbita(Estrela.get_orbita());
}
void imprime_orbita(const Orbita orbit){
    std::cout << "\nApoastro: " << orbit.apoastro << " metros";
    std::cout << "\nPeriastro: " << orbit.periastro << " metros";
    std::cout << "\nexcentricidade: " << orbit.excentricidade;
}

//Em desenvolvimento...
void imprime_sistema(const std::vector<No> &sub, std::vector<Estrela> &estrelas){

}

float excentri(std::mt19937 *motor){
    std::uniform_int_distribution<int> numeros(10, 70);
    float n = numeros(*motor);
    return n / 100;
}
double distancia_aleatoria(std::mt19937 *motor, float menor_dist_pos, float maior_dist_pos){
    std::uniform_real_distribution<float> dist(menor_dist_pos, maior_dist_pos);
    return dist(*motor);
}
double monta_orbitas_para_pares_binarios(std::mt19937 *motor, No &par_binario, Estrela &estrela){
    
    double raio_de_Hill = par_binario.membro_dist * 2.4;
    float excent = excentri(motor);
    
    double distancia = calculo_distancia_pela_esfera_de_Hill(raio_de_Hill, excent, par_binario.massa_subsistema, estrela.get_massa());

    if(estrela.get_massa() < par_binario.massa_subsistema * 0.3){// evita que uma estrela menos massiva passe entre o par binario
        distancia = distancia_aleatoria(motor, distancia * 2, distancia * 15);

    }

    if(par_binario.massa_subsistema > estrela.get_massa()){
        double maior_valor = par_binario.massa_subsistema;
        double menor_valor = estrela.get_massa();

        double apoastro_1 = distancia * (maior_valor /(maior_valor + menor_valor));
        double apoastro_2 = distancia - apoastro_1;

        estrela.set_orbita(calcula_orbita(apoastro_1, excent, true));
        par_binario.orbt = calcula_orbita(apoastro_2, excent, true);
    }else{
        double maior_valor = par_binario.massa_subsistema;
        double menor_valor = estrela.get_massa();

        double apoastro_1 = distancia * (maior_valor /(maior_valor + menor_valor));
        double apoastro_2 = distancia - apoastro_1;

        par_binario.orbt = calcula_orbita(apoastro_1, excent, true);
        estrela.set_orbita(calcula_orbita(apoastro_2, excent, true));
    }

    return distancia;
}
double monta_orbitas_de_binarios(std::mt19937 *motor, Estrela &estrela1, Estrela &estrela2){// essa foi desafiadora

    double menor_valor = estrela1.get_raio() + estrela2.get_raio(); 
    double maior_valor = menor_valor * 90;
    double dist_media_entre_pares = distancia_aleatoria(motor, menor_valor, maior_valor);
    float exc = excentri(motor);

    if(estrela1.get_massa() > estrela2.get_massa()){
        maior_valor = estrela1.get_massa();
        menor_valor = estrela2.get_massa();

        double apoastro_1 =  dist_media_entre_pares * (maior_valor /(maior_valor + menor_valor));
        double apoastro_2 = dist_media_entre_pares - apoastro_1;

        estrela2.set_orbita(calcula_orbita(apoastro_1, exc, true));
        estrela1.set_orbita(calcula_orbita(apoastro_2, exc, true));
            
    }else{
        maior_valor = estrela2.get_massa();
        menor_valor = estrela1.get_massa();

        double apoastro_1 = dist_media_entre_pares * (maior_valor /(maior_valor + menor_valor)) ;
        double apoastro_2 = dist_media_entre_pares - apoastro_1;

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
//Em desenvolvimento...
void classifica_estrelas(std::vector<No> &grup, const int quanti){
    
    //Esta função só pode trabalhar acima de 3 estrelas.
    if(quanti < 3)return;

    char letras[7]{'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    double mass_m = 0.0;
    unsigned int letra_id = 0;
    unsigned int no_id = 0;
    int nomeadas = 0;
    
    auto nomeia_bi = [&](Estrela &star_maior, Estrela &star_menor){
        std::string n = star_maior.get_nome();
        n.push_back(' ');
        n.push_back(letras[letra_id]);
        n.push_back('a');
        star_maior.set_nome(n);
        
        n = star_menor.get_nome();
        n.push_back(' ');
        n.push_back(letras[letra_id]);
        n.push_back('b');
        star_menor.set_nome(n);
        
        letra_id++;
        nomeadas += 2;
    };

    auto nomeia_soli = [&](Estrela &star){
        std::string n = star.get_nome();
        n.push_back(' ');
        n.push_back(letras[letra_id]);
        star.set_nome(n);

        letra_id++;
        nomeadas++;
    };
    
    while(nomeadas < quanti){
        int situacao = 0;

        for(int i = 0; i < grup.size(); i++){
            situacao = 0;

            if(!grup[i].modificado){
                double massa = 0.0;

                if(grup[i].estrela_maior != nullptr && grup[i].estrela_menor != nullptr){
                    massa = grup[i].massa_subsistema;
                    situacao = 1;

                }else if(grup[i].estrela_maior != nullptr){
                    massa = grup[i].estrela_maior->get_massa();
                    situacao = 2;

                }else if(grup[i].estrela_menor != nullptr){
                    massa = grup[i].estrela_menor->get_massa();
                    situacao = 3;
                }

                if(mass_m < massa){
                    mass_m = massa;
                    no_id = i;
                }
            }
        }

        switch (situacao)
        {
        case 1:
            nomeia_bi(*grup[no_id].estrela_maior, *grup[no_id].estrela_menor);
            break;
        case 2:
            nomeia_soli(*grup[no_id].estrela_maior);
            break;
        case 3:
            nomeia_soli(*grup[no_id].estrela_menor);
            break;
        default:
            break;
        }
    }
}

