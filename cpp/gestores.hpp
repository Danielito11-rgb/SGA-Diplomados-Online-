#pragma once

#include "entidades.hpp"
#include "estructuras.hpp"

#include <memory>
#include <string>
#include <vector>

class GestorSGA {
private:
    std::string rutaDatos_;
    std::vector<Alumno> alumnos_;
    std::vector<Profesor> profesores_;
    std::vector<std::unique_ptr<ProgramaAcademico>> programas_;
    PilaHistorial historial_;
    ColaCertificados colaCertificados_;

    void cargarAlumnos();
    void cargarProfesores();
    void guardarAlumnos() const;
    void guardarProfesores() const;
    ProgramaAcademico* instanciarPrograma(const std::string& nombre);

public:
    explicit GestorSGA(const std::string& rutaDatos = "datos");

    GestorSGA(const GestorSGA&) = delete;
    GestorSGA& operator=(const GestorSGA&) = delete;

    void cargarDatos();
    void guardarDatos();

    bool registrarAlumno(const Alumno& alumno);
    Alumno* buscarAlumnoPorCedula(const std::string& cedula);
    const Alumno* buscarAlumnoPorCedula(const std::string& cedula) const;

    const std::vector<Alumno>& alumnos() const;
    const std::vector<Profesor>& profesores() const;
    PilaHistorial& historial();
    const PilaHistorial& historial() const;
    ColaCertificados& colaCertificados();
    const ColaCertificados& colaCertificados() const;
};