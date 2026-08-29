#ifndef OBJETOS_CELESTES_HPP_INCLUDED
#define OBJETOS_CELESTES_HPP_INCLUDED
//||\n\033[0m?
#include <string>

double calculo_distancia_pela_esfera_de_Hill(double Raio_de_Hill, float excentricidade, double massa_menor, double Massa_maior);

struct Orbita{
    double apoastro = 0.0;
    double periastro = 0.0;
    float excentricidade = 0.0f;

    Orbita();
    Orbita(const double &apo_ou_peri, const float &excentri, const bool calcular_periastro);
};

Orbita calcula_orbita(double apoastro_ou_periastro, float excentricidade, bool calcular_periastro);

class Estrela{
public:
    Estrela(std::string &Nome, double &Massa, double &raio, int &Temp, char &c);
    
    ~Estrela();

    void set_nome(const std::string &novo_nome);
    void set_orbita(const Orbita &orbt);

    std::string get_nome()const;
    Orbita get_orbita()const;
    double get_massa()const;
    double get_raio()const;
    int get_temperatura()const;
    char get_classe()const;
     
private:
    
    void define_massa(double m);
    void define_raio(double r);
    
    std::string m_nome = "";
    Orbita orbt;
    double m_raio_m = 0.0;
    double m_massa_Kg = 0.0;
    int m_temperatura = 0;
    char m_classe = ' ';
};

/*
// em planejamento....
class Planeta{
public:
    Planeta(double m, double r, char c_t, bool roc);
    ~Planeta(){};
    
    std::string nome;
private:
    double m_massa_Kg = 0.0;
    double m_raio_m = 0.0;
    char m_classe_tamanho = ' ';
    bool m_rochoso = false;
    
    float m_albedo = 0.0f;
    float m_atmosfera_densidade = 0.0f;
    
};

Orbita cria_orbitas_aleatorias(std::mt19937 *motor, float menor_valor, float Maior_valor, bool calcula_periastro){
    std::uniform_int_distribution<int> excent(10, 60);
    std::uniform_real_distribution<float> periastro_ou_apoastro(menor_valor, Maior_valor);
    Orbita orbit;
    
    if(calcula_periastro){
        orbit.excentricidade = excent(*motor) / 100;
        orbit.periastro = periastro_ou_apoastro(*motor);
        
        orbit.apoastro = orbit.periastro * ((1 + orbit.excentricidade)/(1 - orbit.excentricidade));
    }else{
        orbit.excentricidade = excent(*motor) / 100;
        orbit.apoastro = periastro_ou_apoastro(*motor);
        
        orbit.periastro = orbit.apoastro * ((1 - orbit.excentricidade)/(1 + orbit.excentricidade));
    }
    
    return orbit;
}
    
//double Calculo_Massivo_da_esfera_de_Hill(double distancia, float excentricidade, double massa_menor, double Massa_maior);
//double Calculo_da_esfera_de_Hill(double distancia, float excentricidade, double massa_menor, double Massa_maior);

//double Cauculo_forca_gravitacional(double distancia, double massa1, double massa2);
*/

#endif
