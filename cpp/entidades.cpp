#include "entidades.hpp"

#include <numeric>
#include <stdexcept>

// --- Persona ---
Persona::Persona(const std::string& cedula, const std::string& nombre, const std::string& correo)
    : cedula_(cedula), nombre_(nombre), correo_(correo) {}

const std::string& Persona::cedula() const {
    return cedula_;
}

const std::string& Persona::nombre() const {
    return nombre_;
}

const std::string& Persona::correo() const {
    return correo_;
}

std::string Persona::comoTexto() const {
    return nombre_ + " (CI: " + cedula_ + ")";
}

// --- Alumno ---
Alumno::Alumno(
    const std::string& cedula,
    const std::string& nombre,
    const std::string& correo,
    ProgramaAcademico* programa
)
    : Persona(cedula, nombre, correo), programa_(programa), notas_() {}

ProgramaAcademico* Alumno::programa() const {
    return programa_;
}

void Alumno::asignarPrograma(ProgramaAcademico* programa) {
    programa_ = programa;
}

const std::vector<double>& Alumno::notas() const {
    return notas_;
}

void Alumno::agregarNota(double nota) {
    if (nota < 0.0 || nota > 20.0) {
        throw std::out_of_range("La nota debe estar entre 0 y 20.");
    }
    notas_.push_back(nota);
}

double Alumno::calcularPromedio() const {
    if (notas_.empty()) {
        return 0.0;
    }
    double suma = std::accumulate(notas_.begin(), notas_.end(), 0.0);
    return suma / static_cast<double>(notas_.size());
}

bool Alumno::estaAprobado() const {
    if (programa_ == nullptr) {
        return false;
    }
    return programa_->evaluarAprobacion(notas_);
}

// --- Profesor ---
Profesor::Profesor(
    const std::string& cedula,
    const std::string& nombre,
    const std::string& correo,
    const std::string& areaEspecialidad
)
    : Persona(cedula, nombre, correo), areaEspecialidad_(areaEspecialidad) {}

const std::string& Profesor::areaEspecialidad() const {
    return areaEspecialidad_;
}

// --- ProgramaAcademico ---
ProgramaAcademico::ProgramaAcademico(const std::string& nombrePrograma)
    : nombrePrograma_(nombrePrograma) {}

const std::string& ProgramaAcademico::nombrePrograma() const {
    return nombrePrograma_;
}

// --- Curso ---
Curso::Curso() : ProgramaAcademico("Curso Profesional") {}

bool Curso::evaluarAprobacion(const std::vector<double>& notas) const {
    if (notas.empty()) {
        return false;
    }
    double suma = std::accumulate(notas.begin(), notas.end(), 0.0);
    double promedio = suma / static_cast<double>(notas.size());
    return promedio >= 10.0;
}

// --- Diplomado ---
Diplomado::Diplomado() : ProgramaAcademico("Diplomado") {}

bool Diplomado::evaluarAprobacion(const std::vector<double>& notas) const {
    if (notas.empty()) {
        return false;
    }
    double suma = std::accumulate(notas.begin(), notas.end(), 0.0);
    double promedio = suma / static_cast<double>(notas.size());
    return promedio >= 14.0;
}

// --- Bootcamp ---
Bootcamp::Bootcamp() : ProgramaAcademico("Bootcamp Intensivo") {}

bool Bootcamp::evaluarAprobacion(const std::vector<double>& notas) const {
    if (notas.empty()) {
        return false;
    }
    double suma = 0.0;
    for (double nota : notas) {
        if (nota < 14.0) {
            return false;
        }
        suma += nota;
    }
    double promedio = suma / static_cast<double>(notas.size());
    return promedio >= 14.0;
}