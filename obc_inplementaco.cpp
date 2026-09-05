#include <cmath>
#include "objetos_celestes.hpp"
//||\n\033[0m?
double calculo_distancia_pela_esfera_de_Hill(double Raio_de_Hill, double excentricidade, double massa_menor, double Massa_maior){

    double distancia = Raio_de_Hill / ((1.0 - excentricidade) * std::cbrt((massa_menor/(3.0 * Massa_maior))));
    return distancia;
}

Orbita::Orbita(){}
Orbita::Orbita(const double &apo_ou_peri,const double &excentri,const bool calcular_periastro){
    
    this->excentricidade = excentri;

    if(calcular_periastro){
        this->periastro = apo_ou_peri * ((1 - excentri) / (1 + excentri));
        this->apoastro = apo_ou_peri;
        return;
    }

    this->apoastro = apo_ou_peri * ((1 + excentri) / (1 - excentri));
    this->periastro = apo_ou_peri;
}

Orbita calcula_orbita(double apoastro_ou_periastro, double excentricidade, bool calcula_periastro){
    Orbita orbita;

    if(calcula_periastro){
        //calcula o periastro
        orbita.periastro = apoastro_ou_periastro * ((1 - excentricidade)/(1 + excentricidade));
        orbita.excentricidade = excentricidade;
        orbita.apoastro = apoastro_ou_periastro;

    }else{
        //calcula o apoastro
        orbita.apoastro = apoastro_ou_periastro * ((1 + excentricidade)/(1 - excentricidade));
        orbita.excentricidade = excentricidade;
        orbita.periastro = apoastro_ou_periastro;
    }

    return orbita;
}


Estrela::Estrela(std::string &Nome, double &Massa, double &raio, int &Temp, char &c){
    this->m_nome = Nome;
    this->define_massa(Massa);
    this->define_raio(raio);
    this->m_temperatura = Temp;
    this->m_classe = c;
}

Estrela::~Estrela(){}

void Estrela::set_nome(const std::string &novo_nome){
    this->m_nome = novo_nome;
}
void Estrela::set_orbita(const Orbita &orbt){
    this->orbt = orbt;
}


std::string Estrela::get_nome()const{
    return this->m_nome;
}
Orbita Estrela::get_orbita()const{
    return this->orbt;
}
double Estrela::get_massa()const{
    return this->m_massa_Kg;
}
double Estrela::get_raio()const{
    return this->m_raio_m;
}
int Estrela::get_temperatura()const{
    return this->m_temperatura;
}
char Estrela::get_classe()const{
    return this->m_classe;
}


void Estrela::define_massa(double m){
    this->m_massa_Kg = m * 1.989e30;
}
void Estrela::define_raio(double r){
    this->m_raio_m = r * 6.95e8;
}



/*
double Cauculo_Massivo_da_esfera_de_Hill(double distancia, double excentricidade, double massa_menor, double Massa_maior){
    //testa se as variaveis fazem sentido
    if((distancia <= 0.0 || massa_menor <= 0.0) || (Massa_maior <= 0.0 || (excentricidade <= 0.0f && excentricidade > 0.9f))){
        throw std::domain_error("Erro: valores informados nao fazem sentido");
    }

    double raio_de_Hill = distancia * ((1 - excentricidade) * (std::cbrt((massa_menor/(3 * (Massa_maior + massa_menor))))));

    return raio_de_Hill;
}
double Cauculo_da_esfera_de_Hill(double distancia, double excentricidade, double massa_menor, double Massa_maior){
    //testa se as variaveis fazem sentido
    if((distancia <= 0.0 || massa_menor <= 0.0) || (Massa_maior <= 0.0 || (excentricidade <= 0.0f && excentricidade > 0.9f))){
        throw std::domain_error("Erro: valores informados nao fazem sentido");
    }

    if(excentricidade < 0.20){
        double raio_de_Hill = distancia * (std::cbrt((massa_menor/(3 * Massa_maior))));
    
        return raio_de_Hill;
    }
    
    double raio_de_Hill = distancia * ((1 - excentricidade) * (std::cbrt((massa_menor/(3 * Massa_maior)))));

    return raio_de_Hill;
}

double Cauculo_forca_gravitacional(double distancia, double massa1, double massa2){

    if(distancia < 0.0)throw std::domain_error("Erro: nao existe distancia negativa");
    if(massa1 <= 0.0)throw std::domain_error("Erro: valor de massa1 e invalido");
    if(massa2 <= 0.0)throw std::domain_error("Erro: valor de massa2 e invalido");

    double forca_gravitacional = 0.0;

    forca_gravitacional = (massa1 * massa2 * 6.67e-11) / (std::pow(distancia, 2));

    return forca_gravitacional;
};
*/
/*
Planeta::Planeta(double m, double r, char c_t, bool roc){
    this->m_massa_Kg = m;
    this->m_raio_m = r;
    this->m_classe_tamanho = c_t;
    this->m_rochoso = roc;
}
*/