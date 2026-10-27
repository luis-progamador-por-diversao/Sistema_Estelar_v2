#include "orbita.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

Orbita::Orbita(double apo_ou_peri, double exc, bool calcula_periastro)
    : excentricidade(exc) {
    if (calcula_periastro) {
        apoastro = apo_ou_peri;
        if (exc < 1.0) {
            periastro = apo_ou_peri * ((1.0 - exc) / (1.0 + exc));
        } else {
            periastro = 0.0;
        }
    } else {
        periastro = apo_ou_peri;
        if (exc < 1.0) {
            apoastro = apo_ou_peri * ((1.0 + exc) / (1.0 - exc));
        } else {
            apoastro = apo_ou_peri;
        }
    }
    semieixo_maior = (apoastro + periastro) / 2.0;
}

Orbita::Orbita(double semieixo_maior_m, double exc)
    : semieixo_maior(semieixo_maior_m),
      excentricidade(exc),
      apoastro(semieixo_maior_m * (1.0 + exc)),
      periastro(semieixo_maior_m * (1.0 - exc)) {}

Orbita::Orbita(double semieixo_maior_m, double exc, double inc_graus,
               double arg_peri_graus, double long_no_asc_graus, double anomalia_med_graus)
    : semieixo_maior(semieixo_maior_m),
      excentricidade(exc),
      apoastro(semieixo_maior_m * (1.0 + exc)),
      periastro(semieixo_maior_m * (1.0 - exc)),
      inclinacao(inc_graus),
      argumento_periastro(arg_peri_graus),
      longitude_no_ascendente(long_no_asc_graus),
      anomalia_media(anomalia_med_graus) {}

double Orbita::semieixo_menor() const noexcept {
    if (excentricidade >= 1.0 || semieixo_maior <= 0.0) {
        return 0.0;
    }
    return semieixo_maior * std::sqrt(1.0 - (excentricidade * excentricidade));
}

double Orbita::distancia_focal() const noexcept {
    return semieixo_maior * excentricidade;
}

double Orbita::calcular_periodo_segundos(double semieixo_total_m, double massa_total_kg) {
    if (semieixo_total_m <= 0.0 || massa_total_kg <= 0.0) {
        return 0.0;
    }
    const double a3 = semieixo_total_m * semieixo_total_m * semieixo_total_m;
    const double denominador = Constantes::G * massa_total_kg;
    return 2.0 * Constantes::PI * std::sqrt(a3 / denominador);
}

double Orbita::calcular_periodo_segundos(const Orbita &o1, const Orbita &o2, double m1_kg, double m2_kg) {
    const double a_total = o1.semieixo_maior + o2.semieixo_maior;
    const double m_total = m1_kg + m2_kg;
    return calcular_periodo_segundos(a_total, m_total);
}

std::string Orbita::formatar_periodo(double periodo_segundos) {
    if (periodo_segundos <= 0.0) {
        return "estacionario / indefinido";
    }

    const double t_dias = periodo_segundos / Constantes::SEGUNDOS_POR_DIA;
    std::stringstream ss;
    ss << "aproximadamente " << std::fixed << std::setprecision(2);

    if (t_dias < 30.0) {
        ss << t_dias << " dias";
        return ss.str();
    }

    if (t_dias < Constantes::DIAS_POR_ANO_JULIANO) {
        const double t_mes = t_dias / Constantes::DIAS_POR_MES_MEDIO;
        ss << t_mes << " meses";
        return ss.str();
    }

    const double t_anos = t_dias / Constantes::DIAS_POR_ANO_JULIANO;
    if (t_anos < 1000000.0) {
        ss << t_anos << " anos";
        return ss.str();
    }

    const double t_milhao = t_anos / 1000000.0;
    ss << t_milhao << " milhoes de anos";
    return ss.str();
}

std::string Orbita::calcular_periodo_formatado(const Orbita &o1, const Orbita &o2, double m1_kg, double m2_kg) {
    const double periodo_seg = calcular_periodo_segundos(o1, o2, m1_kg, m2_kg);
    return formatar_periodo(periodo_seg);
}

