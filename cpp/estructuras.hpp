#pragma once

#include <optional>
#include <string>
#include <vector>

struct NodoTexto {
    std::string dato;
    NodoTexto* siguiente;

    explicit NodoTexto(const std::string& valor, NodoTexto* sig = nullptr);
};

class PilaHistorial {
private:
    NodoTexto* top_;
    std::size_t tamano_;

public:
    PilaHistorial();
    ~PilaHistorial();

    PilaHistorial(const PilaHistorial&) = delete;
    PilaHistorial& operator=(const PilaHistorial&) = delete;

    void apilar(const std::string& accion);
    std::optional<std::string> desapilar();
    bool estaVacia() const;
    std::size_t tamano() const;
    std::vector<std::string> listarElementos() const;
};

class ColaCertificados {
private:
    NodoTexto* frente_;
    NodoTexto* final_;
    std::size_t tamano_;

public:
    ColaCertificados();
    ~ColaCertificados();

    ColaCertificados(const ColaCertificados&) = delete;
    ColaCertificados& operator=(const ColaCertificados&) = delete;

    void encolar(const std::string& solicitud);
    std::optional<std::string> desencolar();
    bool estaVacia() const;
    std::size_t tamano() const;
    std::vector<std::string> listarElementos() const;
};