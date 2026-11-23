#pragma once

#include <string>
#include <vector>

// Declaración adelantada para romper dependencia circular con ProgramaAcademico
class ProgramaAcademico;

// --- JERARQUÍA DE PERSONAS ---
class Persona {
protected:
    std::string cedula_;
    std::string nombre_;
    std::string correo_;

public:
    Persona(const std::string& cedula, const std::string& nombre, const std::string& correo);
    virtual ~Persona() = default;

    const std::string& cedula() const;
    const std::string& nombre() const;
    const std::string& correo() const;

    virtual std::string comoTexto() const;
};

class Alumno : public Persona {
private:
    ProgramaAcademico* programa_;
    std::vector<double> notas_;

public:
    explicit Alumno(
        const std::string& cedula,
        const std::string& nombre,
        const std::string& correo,
        ProgramaAcademico* programa = nullptr
    );

    ProgramaAcademico* programa() const;
    void asignarPrograma(ProgramaAcademico* programa);

    const std::vector<double>& notas() const;
    void agregarNota(double nota);
    double calcularPromedio() const;
    bool estaAprobado() const;
};

class Profesor : public Persona {
private:
    std::string areaEspecialidad_;

public:
    Profesor(
        const std::string& cedula,
        const std::string& nombre,
        const std::string& correo,
        const std::string& areaEspecialidad
    );

    const std::string& areaEspecialidad() const;
};

// --- JERARQUÍA DE PROGRAMAS ACADÉMICOS (POLIMORFISMO) ---
class ProgramaAcademico {
protected:
    std::string nombrePrograma_;

public:
    explicit ProgramaAcademico(const std::string& nombrePrograma);
    virtual ~ProgramaAcademico() = default;

    const std::string& nombrePrograma() const;
    virtual bool evaluarAprobacion(const std::vector<double>& notas) const = 0;
};

class Curso : public ProgramaAcademico {
public:
    Curso();
    bool evaluarAprobacion(const std::vector<double>& notas) const override;
};

class Diplomado : public ProgramaAcademico {
public:
    Diplomado();
    bool evaluarAprobacion(const std::vector<double>& notas) const override;
};

class Bootcamp : public ProgramaAcademico {
public:
    Bootcamp();
    bool evaluarAprobacion(const std::vector<double>& notas) const override;
};