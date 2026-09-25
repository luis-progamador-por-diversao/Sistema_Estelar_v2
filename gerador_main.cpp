#include <bits/stdc++.h>
#include "sistema_arvore.hpp"
//||\n\033[0m?
   
//Aleatoriza estrelas e nome.
std::string random_nome(std::mt19937 &mt);
namespace {

const double T_SOL = 5778.0;           // K
const double IDADE_UNIVERSO = 13.0e9;  // anos
const double T_MS_MINIMO = 3.0e6;      // anos (massas altas "saturam")

struct FaixaMassa { double min, max, alfa; };  // p(m) proporcional a m^-alfa

const std::array<FaixaMassa, 7> FAIXAS = {{
    {0.08, 0.45, 1.3},   // M
    {0.45, 0.80, 2.35},  // K
    {0.80, 1.04, 2.35},  // G
    {1.04, 1.40, 2.35},  // F
    {1.40, 2.10, 2.35},  // A
    {2.10, 16.0, 2.35},  // B
    {16.0, 50.0, 2.35}   // O
}};
// pesos de sorteio da classe 
const std::array<double, 7> PESOS = {{60.0, 15.0, 10.0, 7.0, 4.0, 3.0, 1.0}};

enum class Lum { SeqPrincipal, Subgigante, Gigante, Supergigante, Hipergigante };
struct Resultado { double lumi; double raio; double temp; };

double uniforme(std::mt19937 &mt, double a, double b){
    return std::uniform_real_distribution<double>(a, b)(mt);
}
double log_uniforme(std::mt19937 &mt, double a, double b){
    return std::exp(uniforme(mt, std::log(a), std::log(b)));
}
double sorteia_massa(std::mt19937 &mt, const FaixaMassa &f){
    const double e = 1.0 - f.alfa;
    const double a = std::pow(f.min, e), b = std::pow(f.max, e);
    return std::pow(a + uniforme(mt, 0.0, 1.0) * (b - a), 1.0 / e);
}

// Sequência principal (aproximações), massa em M☉
double lum_ms(double m){   // L☉
    if(m < 0.43) return 0.23 * std::pow(m, 2.3);
    if(m < 2.0)  return std::pow(m, 4.0);
    return 1.4 * std::pow(m, 3.5);
}
double raio_ms(double m){  // R☉
    if(m < 1.0) return std::pow(m, 0.9);
    if(m < 2.0) return std::pow(m, 0.8);
    return 1.74 * std::pow(m / 2.0, 0.57);
}
double temp_de(double lumi, double raio){  // K (Stefan-Boltzmann)
    return T_SOL * std::pow(lumi / (raio * raio), 0.25);
}
double raio_de(double lumi, double temp){  // R☉
    const double r = T_SOL / temp;
    return std::sqrt(lumi) * r * r;
}
double tempo_ms(double m){  // anos na sequência principal
    return std::max(1.0e10 * std::pow(m, -2.5), T_MS_MINIMO);
}

Lum define_estagio(double m, double idade){
    const double f = idade / tempo_ms(m);
    if(f < 1.0) return Lum::SeqPrincipal;
    const double x = (f - 1.0) / 0.10;  // progresso pós-SP (0 a 1)
    if(x < 0.20) return Lum::Subgigante;
    if(m < 8.0)  return Lum::Gigante;
    if(m < 25.0 || x < 0.70) return Lum::Supergigante;
    return Lum::Hipergigante;
}

Resultado calcula_estrela(std::mt19937 &mt, double m, Lum est){
    const double L_ms = lum_ms(m), R_ms = raio_ms(m);
    const double T_ms = temp_de(L_ms, R_ms);
    const double L_edd = 3.2e4 * m;  // limite de Eddington (L☉)
    Resultado r{L_ms, R_ms, T_ms};

    switch(est){
        case Lum::SeqPrincipal: break;
        case Lum::Subgigante:
            r.lumi = L_ms * uniforme(mt, 1.2, 2.5);
            r.temp = T_ms * uniforme(mt, 0.75, 0.95);
            break;
        case Lum::Gigante:
            r.lumi = L_ms * uniforme(mt, 5.0, 50.0);
            r.temp = std::max(3400.0, T_ms * uniforme(mt, 0.5, 0.8));
            break;
        case Lum::Supergigante:
            r.lumi = L_ms * uniforme(mt, 3.0, 20.0);
            r.temp = log_uniforme(mt, 3500.0, 0.7 * T_ms);
            break;
        case Lum::Hipergigante:
            r.lumi = L_edd * uniforme(mt, 0.3, 1.0);
            r.temp = log_uniforme(mt, 3500.0, 25000.0);
            break;
    }
    if(est != Lum::SeqPrincipal){
        r.lumi = std::min(r.lumi, L_edd);
        r.raio = raio_de(r.lumi, r.temp);  // T e L mandam; R sai da fórmula
    }
    return r;
}

char letra_por_temp(double T){
    if(T >= 30000) return 'O';
    if(T >= 10000) return 'B';
    if(T >= 7500)  return 'A';
    if(T >= 6000)  return 'F';
    if(T >= 5200)  return 'G';
    if(T >= 3700)  return 'K';
    return 'M';
}
const char* cor(char c){
    switch(c){
        case 'M': return "\033[31m";
        case 'K': return "\033[93m";
        case 'G': return "\033[33m";
        case 'F': return "\033[37m";
        case 'A': return "\033[34m";
        case 'B': return "\033[36m";
        default:  return "\033[95m";
    }
}
std::string monta_tipo(int idx_classe, Lum est, double T){
    static const char LETRAS[] = "MKGFABO";
    static const char* NOMES_SP[7] = {"Ana Vermelha", "Ana Laranja", "Ana Amarela",
        "Ana Amarelo-Branca", "Ana Branca", "Estrela Azul-Branca", "Estrela Azul"};
    if(est == Lum::SeqPrincipal)
        return std::string(cor(LETRAS[idx_classe])) + NOMES_SP[idx_classe] + "\033[0m";

    const char letra = letra_por_temp(T);
    const char* nome = (est == Lum::Subgigante)   ? "Subgigante"
                     : (est == Lum::Gigante)      ? "Gigante"
                     : (est == Lum::Supergigante) ? "Supergigante" : "Hipergigante";
    return std::string(cor(letra)) + nome + " tipo " + letra + "\033[0m";
}

} // namespace
void random_estrelas(std::mt19937 &mt, std::string _nome, std::vector<Estrela> &sistema, const unsigned int &star_size);
unsigned int random_quanti(std::mt19937 & mt);

//Gera proceduralmente as orbitas e hierarquia do sistema.
double random_excentri(std::mt19937 &mt);
double Mardling_Aarseth(const double &m_sub, const double &m_star, const double &e);
double monta_orbitas(std::mt19937 &mt, Estrela &estrela1, Estrela &estrela2);
double monta_orbitas(std::mt19937 &mt, No * sub_conj, Estrela &estrela);
double monta_orbitas(std::mt19937 &mt, No * sub1, No * sub2);

//Classifica as estrelas de acordo com suas massas.
void classifica_binarios(Estrela &estrela_1, Estrela &estrela_2);
void classifica_trinarios(std::vector<Estrela> &sistema, No * raiz);
void classifica_multiplas(std::vector<Estrela> &list, std::vector<Pseudo_no> &ps_list);

//Revela o sistema no terminal.
static std::string periodo_orbital(const Orbita &o1, const Orbita &o2, const double &m1, const double &m2);
static std::string formato_cientifico(double valor);//converte números de notação centifica para string.
static void imprime_orbita(const Orbita &orbit, const std::string & bari);
static void imp_estrela(Estrela &Estrela,  const std::string & bari);
void imprime_sistema(std::vector<Estrela> &list, No * sub);


int main(){
    std::vector<Estrela> sistema;
    
    int seed = 0;
    
    while(true){ 
        char escolha;

        std::cout << "\n\nPrograma para criar sistemas de estrelas:\n\n";
        std::cout << "\nDigite [1] para escolher uma seed.\nDigite [2] para uma seed aleatoria.\n";
        std::cin.get(escolha);std::cin.ignore();

        if(escolha == '1'){
            std::cout << "\nDigite a seed: ";std::cin >> seed;
            
            break;
        }
        if(escolha == '2'){
            std::random_device random;
            seed = random();

            break;
        }
        
        system("cls");
        std::cout << "\n\033[31mNumero imformado e imvalido\033[0m";
        std::cout << "\ntente outra vez:\n";
    }
    
    std::cout << std::endl << "Seed = " << seed;
    std::mt19937 mt(seed);
    
    const unsigned int quanti = random_quanti(mt);
    sistema.reserve(quanti);  
    std::string nome_geral = random_nome(mt);

    random_estrelas(mt, nome_geral, sistema, quanti);
    
    std::unique_ptr<No> no_raiz;

    switch(quanti){
        case 0:
            std::cout << "\nErro: nenhuma estrela foi gerada !!!";
            return -1;
            break;
        case 1:
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n                     Sistema " << nome_geral ;
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n     Estrela  " << sistema[0].get_nome() << ":\n";
            std::cout << "\n Tipo             : " << sistema[0].get_tipo();
            std::cout << "\n Massa            : " << formato_cientifico(sistema[0].get_massa()) << " kg";
            std::cout << "\n Raio             : " << formato_cientifico(sistema[0].get_raio()) << " metros";
            std::cout << "\n Luminosidade     : " << formato_cientifico(sistema[0].get_luminosidade()) << " watts";
            std::cout << "\n Temperatura      : " << sistema[0].get_temperatura() << " graus celsius";
            
            break;
    
        case 2:{
            double dist = monta_orbitas(mt, sistema[0], sistema[1]);                                                                        
            
            classifica_binarios(sistema[0], sistema[1]);
           
            std::cout << "\nDistancia usada nos calculos: " << formato_cientifico(dist);
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n                   Sistema binario " << nome_geral ;
            std::cout << "\n\n=============================================================================";
            std::cout << "\n\nPeriodo orbital do sistema: " 
            << periodo_orbital(sistema[0].get_orbita(), sistema[1].get_orbita(), 
                               sistema[0].get_massa(),  sistema[1].get_massa());

            imp_estrela(sistema[0], "");

            std::cout << "\n\n=============================================================================\n";

            imp_estrela(sistema[1], "");
            
            break;
        }
        case 3:{
            double dist = monta_orbitas(mt, sistema[0], sistema[1]);
            no_raiz = std::make_unique<No>(sistema[0], 0, sistema[1], 1, dist);
            
            dist = monta_orbitas(mt, no_raiz.get(), sistema[2]);
            no_raiz = std::make_unique<No>(no_raiz, sistema[2], 2, dist);
            
            classifica_trinarios(sistema, no_raiz.get());

            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n                  Sistema trinario " << nome_geral ;
    
            imprime_sistema(sistema, no_raiz.get());
            
            break;
        }
        case 4 ... 7:{
            std::vector<Pseudo_no> list_pseudo;
            list_pseudo.reserve(quanti -1);
    
            std::uniform_int_distribution<int> sorte(0,1);
            
            double dist = monta_orbitas(mt, sistema[0], sistema[1]);
            no_raiz = std::make_unique<No>(sistema[0], 0, sistema[1], 1, dist);
            list_pseudo.emplace_back(no_raiz, std::max(sistema[0].get_luminosidade(), sistema[1].get_luminosidade()));
    
            int i = 2, par_conj = 0;
    
            while(i < quanti){
                par_conj = sorte(mt);
    
                if(par_conj == 1 && (i + 1 <= quanti - 1)){
                    int id1 = i;
                    int id2 = i+1;

                    dist = monta_orbitas(mt, sistema[id1], sistema[id2]);
                    auto p = std::make_unique<No>(sistema[id1], id1, sistema[id2], id2, dist);
                    list_pseudo.emplace_back(p, std::max(sistema[id1].get_luminosidade(), sistema[id2].get_luminosidade()));
    
                    dist = monta_orbitas(mt, no_raiz.get(), p.get());
                    no_raiz = std::make_unique<No>(no_raiz, p, dist);
    
                    no_raiz->sub_maior->volta = no_raiz.get();
                    no_raiz->sub_menor->volta = no_raiz.get();
    
                    i += 2;
                }else{
                    dist = monta_orbitas(mt, no_raiz.get(), sistema[i]);
                    no_raiz = std::make_unique<No>(no_raiz, sistema[i], i, dist);
    
                    list_pseudo.emplace_back(no_raiz, sistema[i].get_luminosidade());
    
                    if(no_raiz->sub_maior != nullptr){
                        no_raiz->sub_maior->volta = no_raiz.get();
    
                    }else{
                        no_raiz->sub_menor->volta= no_raiz.get();
                    }
    
                    i++;
                }
            }
    
            classifica_multiplas(sistema, list_pseudo);
            list_pseudo.clear();
    
            std::cout << "\n\n=============================================================================\n";
            std::cout << "\n         Sistema multiplo de "<< quanti << " estrelas: " << nome_geral ;

            imprime_sistema(sistema, no_raiz.get());
            break;
        }
        default:
            std::cout << "\nErro: Foram geradas mais de 7 estrelas !!!";
            return -1;
        break;
    }
    
    std::cout << "\n\n=============================================================================\n";
    sistema.clear();

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

void random_estrelas(std::mt19937 &mt, std::string _nome, std::vector<Estrela> &sistema, const unsigned int &star_size){
    if(star_size == 0) return;
    std::discrete_distribution<int> sorteia_classe(PESOS.begin(), PESOS.end());

    // 1) classes e massas
    std::vector<int> classe(star_size);
    std::vector<double> massa(star_size);
    double m_max = 0.0;
    for(unsigned int i = 0; i < star_size; i++){
        classe[i] = sorteia_classe(mt);
        massa[i]  = sorteia_massa(mt, FAIXAS[classe[i]]);
        m_max = std::max(m_max, massa[i]);
    }

    // 2) idade única do sistema, ligada à estrela mais massiva
    const double t_top = tempo_ms(m_max);
    const double t_ref = std::min(t_top, IDADE_UNIVERSO);
    const double u_max = (t_top > IDADE_UNIVERSO) ? 1.0 : 1.10;
    const double idade = uniforme(mt, 0.05, u_max) * t_ref;

    // 3) L, R, T e rótulo de cada estrela
    for(unsigned int i = 0; i < star_size; i++){
        const Lum est = define_estagio(massa[i], idade);
        const Resultado r = calcula_estrela(mt, massa[i], est);
        sistema.emplace_back(_nome, monta_tipo(classe[i], est, r.temp), 
                             massa[i], r.raio, static_cast<int>(r.temp), r.lumi);
    }
}

//Segunda etapa da geração: randonizando orbitas.
double random_excentri(std::mt19937 &mt){
    std::uniform_int_distribution<int> numeros(1, 70);
    double n = static_cast<double>(numeros(mt));
    return n / 100.0;
}

double Mardling_Aarseth(const double &m_sub, const double &m_star, const double &e){
    const double m = m_star / m_sub;
    const double termo_superior = (1.0 + m) * (1.0 + e);
    const double base = termo_superior / std::sqrt(1.0 - e);
    
    return 2.8 * std::pow(base, 0.4);
}

double monta_orbitas(std::mt19937 &mt, Estrela &estrela1, Estrela &estrela2){

    //double menor_valor = estrela1.get_raio() + estrela2.get_raio();

    double menor_valor = 3 * (estrela1.get_raio() + estrela2.get_raio()); 
    double maior_valor = menor_valor * 50.0;

    double apo_maior = uniforme(mt, menor_valor, maior_valor);
    double exc = random_excentri(mt);

    double m1 = estrela1.get_massa();
    double m2 = estrela2.get_massa();

    if(m1 >= m2){
        double apo1 = apo_maior * (m1 / (m2 + m1));
        double apo2 = apo_maior - apo1;

        Orbita o1(apo1, exc, true), o2(apo2, exc, true);

        estrela2.set_orbita(o1);
        estrela1.set_orbita(o2); 

        return o1.semieixo_maior + o2.semieixo_maior;
    }else{
        double apo1 = apo_maior * (m2 / (m1 + m2));
        double apo2 = apo_maior - apo1;

        Orbita o1(apo1, exc, true), o2(apo2, exc, true);

        estrela1.set_orbita(o1);
        estrela2.set_orbita(o2);
        
        return o1.semieixo_maior + o2.semieixo_maior;
    }
}

double monta_orbitas(std::mt19937 &mt, No * sub, Estrela &estrela){
    
    if(sub == nullptr){
        std::cout << "\nErro: A função monta_orbitas(No + estrela) falhou por ponteiro nulo.";
        return 0.1;
    }

    double excent = random_excentri(mt);
    double peri_minimo = Mardling_Aarseth(sub->massa_sub, estrela.get_massa(), excent);

    double distancia = sub->membro_dist * log_uniforme(mt, peri_minimo, peri_minimo * 10);
   
    double m_subi = sub->massa_sub;
    double m_star = estrela.get_massa();
    
    /*
    double raio_de_Hill = sub->massa_sub * 2.4;
    double excent = random_excentri(mt);
    double distancia = raio_de_Hill / ((1.0 - excent)* std::cbrt((m_subi/(3.0 * m_star))));
    if(m_star < m_subi * 0.3){
        distancia = uniforme(mt, distancia * 2, distancia * 15);
    }
    */
    if(m_subi > m_star){
        double apoastro_1 = distancia * (m_subi /(m_star + m_subi));
        double apoastro_2 = distancia - apoastro_1;

        Orbita o1(apoastro_1, excent, true), o2(apoastro_2, excent, true);

        estrela.set_orbita(o1);
        sub->orbt = o2;

    }else{
        double apoastro_1 = distancia * (m_star /(m_subi + m_star));
        double apoastro_2 = distancia - apoastro_1;

        Orbita o1(apoastro_1, excent, true), o2(apoastro_2, excent, true);

        sub->orbt = o1;
        estrela.set_orbita(o2);
    }

    return distancia;
}

double monta_orbitas(std::mt19937 &mt, No * sub1, No * sub2){

    if(sub1 == nullptr || sub2 == nullptr){
        std::cout << "\nErro: A função monta_orbitas(No + No) falhou po ponteiro nulo";
        return 0.1;
    }

    double menor_dist = 3 * (sub1->membro_dist + sub2->membro_dist);
    double maior_dist = menor_dist * 10.0;

    double dist_entre_sub = uniforme(mt, menor_dist, maior_dist);
    double exc = random_excentri(mt);
    
    double m1 = sub1->massa_sub;
    double m2 = sub2->massa_sub;

    if(m1 >= m2){
        double apo1 = dist_entre_sub * (m1 / (m1 + m2));
        double apo2 = dist_entre_sub - apo1;

        Orbita o1(apo1, exc, true), o2(apo2, exc, true);

        sub2->orbt = o1;
        sub1->orbt = o2;
    }else{
        double apo1 = dist_entre_sub * (m2 / (m1 + m2));
        double apo2 = dist_entre_sub - apo1;

        Orbita o1(apo1, exc, true), o2(apo2, exc, true);

        sub1->orbt = o1;
        sub2->orbt = o2;
    }

    return dist_entre_sub;
}

//Classifica as estrelas de acordo com suas luminosidades.
void classifica_binarios(Estrela &estrela_1, Estrela &estrela_2){
    std::string estrela_nome_1 = estrela_1.get_nome();
    std::string estrela_nome_2 = estrela_2.get_nome();
    estrela_nome_2.push_back(' ');
    estrela_nome_1.push_back(' ');

    if(estrela_1.get_luminosidade() > estrela_2.get_luminosidade()){
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
void classifica_trinarios(std::vector<Estrela> &sistema, No * raiz){
    
    if(sistema.size() != 3 || raiz == nullptr){
        std::cout << "\nA função 'classifica_trinarios' falhou";
        return;
    }
    std::string nome_0 = sistema[0].get_nome();
    std::string nome_1 = sistema[1].get_nome();
    std::string nome_2 = sistema[2].get_nome();
    nome_0.push_back(' ');
    nome_1.push_back(' ');
    nome_2.push_back(' ');
    
    raiz->nome = "ABC";

    if(sistema[0].get_luminosidade() > sistema[1].get_luminosidade() 
    && sistema[0].get_luminosidade() > sistema[2].get_luminosidade()){
        nome_0.push_back('A');
        sistema[0].set_nome(nome_0);

        if(sistema[1].get_luminosidade() > sistema[2].get_luminosidade()){
            nome_1.push_back('B');
            nome_2.push_back('C');
            sistema[1].set_nome(nome_1);
            sistema[2].set_nome(nome_2);

            raiz->sub_maior->nome = "AB";

            return;
        }

        nome_1.push_back('C');
        nome_2.push_back('B');
        sistema[1].set_nome(nome_1);
        sistema[2].set_nome(nome_2);

        raiz->sub_maior->nome = "AC";

        return;
    }

    if(sistema[1].get_luminosidade() > sistema[0].get_luminosidade() 
    && sistema[1].get_luminosidade() > sistema[2].get_luminosidade()){
        nome_1.push_back('A');
        sistema[1].set_nome(nome_1);

        if(sistema[0].get_luminosidade() > sistema[2].get_luminosidade()){
            nome_0.push_back('B');
            nome_2.push_back('C');
            sistema[0].set_nome(nome_0);
            sistema[2].set_nome(nome_2);

            raiz->sub_maior->nome = "AB";

            return;
        }

        nome_0.push_back('C');
        nome_2.push_back('B');
        sistema[0].set_nome(nome_0);
        sistema[2].set_nome(nome_2);
        
        raiz->sub_maior->nome = "AC";

        return;
    }

    if(raiz->sub_maior != nullptr){
        raiz->sub_maior->nome = "BC";
    }else{
        raiz->sub_menor->nome = "BC";
    }

    nome_2.push_back('A');
    sistema[2].set_nome(nome_2);

    if(sistema[1].get_luminosidade() > sistema[0].get_luminosidade()){
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
        int maior_id = 0; 
        int major = -1, menor = -1;

        for(int l = 0; l < list_size; l++){

            if(ps_list[l].utilizado)continue;

            if(maior < ps_list[l].lumi){
                maior = ps_list[l].lumi; 
                maior_id = l;
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

//terceira lei de Kepler Modificada por Newton.
static std::string periodo_orbital(const Orbita &o1, const Orbita &o2, const double &m1, const double &m2){

    double a3 = o1.semieixo_maior + o2.semieixo_maior;
    double t_segundos = 2 * 3.14 * std::sqrt((a3 * a3 * a3)/(6.67e-11 * (m1 + m2)));
    double t_dias = t_segundos / 86400.0;

    std::stringstream t;
    t << "aproximadamente ";
    
    if(t_dias < 30.0){
        t << std::fixed << std::setprecision(2) << t_dias << " dias";
        return t.str();
    }
    if(t_dias < 364.0){
        double t_mes = t_dias / 30;

        t << std::fixed << std::setprecision(2) << t_mes << " meses";
        
        return t.str();
    }
    
    double t_anos = t_dias / 364.0;

    if(t_anos < 1000000.0){
        t << std::fixed << std::setprecision(2) << t_anos << " anos";
    
        return t.str();
    }

    double t_milhao = t_anos / 1000000.0;

    t << std::fixed << std::setprecision(2) << t_milhao << " milhoes de anos";
 
    return t.str();
}
static std::string formato_cientifico(double valor){

    if(valor == 0)return "0";

    int expoente = std::floor(std::log10(std::abs(valor)));
    double mantissa = valor / std::pow(10, expoente);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << mantissa;
    ss << " x 10^" << expoente;

    return ss.str();
}
static void imprime_orbita(const Orbita &orbit, const std::string & bari){
    std::cout << "\n semieixo_maior   : " << formato_cientifico(orbit.semieixo_maior) << " metros";
    std::cout << "\n apoastro         : " << formato_cientifico(orbit.apoastro) << " metros de distancia do baricentro " << bari;
    std::cout << "\n periastro        : " << formato_cientifico(orbit.periastro) << " metros de distancia do baricentro " << bari;
    std::cout << "\n excentricidade   : " << orbit.excentricidade;
}
static void imp_estrela(Estrela &Estrela,  const std::string & bari){

    std::cout << "\n\n     Estrela  " << Estrela.get_nome() << ":";
    std::cout << "\n Classe           : " << Estrela.get_tipo();
    std::cout << "\n Massa            : " << formato_cientifico(Estrela.get_massa()) << " kg";
    std::cout << "\n Raio             : " << formato_cientifico(Estrela.get_raio()) << " metros";
    std::cout << "\n Luminosidade     : " << formato_cientifico(Estrela.get_luminosidade()) << " watts";
    std::cout << "\n Temperatura      : " << Estrela.get_temperatura() << " graus celsius";

    std::cout << "\n\n\033[32mPossui a seguinte orbita:\033[0m\n";

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

        std::cout << "\n\nPeriodo orbital dos menbros: ";
        std::cout << periodo_orbital(list[star_menor].get_orbita(), list[star_maior].get_orbita(), 
                                     list[star_menor].get_massa(),  list[star_maior].get_massa());

        std::cout << "\n\nEstrela mais massiva:";
        imp_estrela(list[star_maior], sub->nome);

        std::cout << "\n\nEstrela menos massiva:";
        imp_estrela(list[star_menor], sub->nome);

        return;
    }
    if(star_menor >= 0 && sub->sub_maior != nullptr){
        std::cout << "\nComposto por uma estrela e um subsistema:";
        std::cout << "\n\nPeriodo orbital dos menbros: ";
        std::cout << periodo_orbital(list[star_menor].get_orbita(), sub->sub_maior->orbt, 
                                     list[star_menor].get_massa(),  sub->sub_maior->massa_sub);
        
        imp_estrela(list[star_menor], sub->nome);

        std::cout << ".\n\nSubsistema " << sub->sub_maior->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";

        imprime_orbita(sub->sub_maior->orbt, sub->nome);
        
        imprime_sistema(list, sub->sub_maior.get());

    }else if(sub->sub_maior != nullptr && sub->sub_menor != nullptr){
        std::cout << "\nComposto por dois subsistemas rivais:";

        std::cout << "\n\nPeriodo orbital dos menbros: ";
        std::cout << periodo_orbital(sub->sub_maior->orbt,      sub->sub_menor->orbt, 
                                     sub->sub_maior->massa_sub, sub->sub_menor->massa_sub);

        std::cout << "\n\nSubsistema " << sub->sub_maior->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";
        imprime_orbita(sub->sub_maior->orbt, sub->nome);

        std::cout << "\n\nSubsistema " << sub->sub_menor->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";
        imprime_orbita(sub->sub_menor->orbt, sub->nome);

        imprime_sistema(list, sub->sub_maior.get());
        imprime_sistema(list, sub->sub_menor.get());
        
    }else if(star_maior >= 0 && sub->sub_menor != nullptr){
        std::cout << "\nComposto por uma estrela e um subsistema:";
        
        std::cout << "\n\nPeriodo orbital dos menbros: ";
        std::cout << periodo_orbital(list[star_maior].get_orbita(), sub->sub_menor->orbt, 
                                     list[star_maior].get_massa(),  sub->sub_menor->massa_sub);

        imp_estrela(list[star_maior], sub->nome);

        std::cout << ".\n\nSubsistema " << sub->sub_menor->nome;
        std::cout << "\n\033[32mPossui a seguinte orbita:\033[0m\n";

        imprime_orbita(sub->sub_menor->orbt, sub->nome);

        imprime_sistema(list, sub->sub_menor.get());
    }
}

