#include "estructuras.hpp"

// --- NodoTexto ---
NodoTexto::NodoTexto(const std::string& valor, NodoTexto* sig)
    : dato(valor), siguiente(sig) {}

// --- PilaHistorial (LIFO) ---
PilaHistorial::PilaHistorial() : top_(nullptr), tamano_(0) {}

PilaHistorial::~PilaHistorial() {
    while (!estaVacia()) {
        desapilar();
    }
}

void PilaHistorial::apilar(const std::string& accion) {
    NodoTexto* nuevo = new NodoTexto(accion, top_);
    top_ = nuevo;
    ++tamano_;
}

std::optional<std::string> PilaHistorial::desapilar() {
    if (estaVacia()) {
        return std::nullopt;
    }
    NodoTexto* temp = top_;
    std::string valor = temp->dato;
    top_ = top_->siguiente;
    delete temp;
    --tamano_;
    return valor;
}

bool PilaHistorial::estaVacia() const {
    return top_ == nullptr;
}

std::size_t PilaHistorial::tamano() const {
    return tamano_;
}

std::vector<std::string> PilaHistorial::listarElementos() const {
    std::vector<std::string> elementos;
    elementos.reserve(tamano_);
    NodoTexto* actual = top_;
    while (actual != nullptr) {
        elementos.push_back(actual->dato);
        actual = actual->siguiente;
    }
    return elementos;
}

// --- ColaCertificados (FIFO) ---
ColaCertificados::ColaCertificados() : frente_(nullptr), final_(nullptr), tamano_(0) {}

ColaCertificados::~ColaCertificados() {
    while (!estaVacia()) {
        desencolar();
    }
}

void ColaCertificados::encolar(const std::string& solicitud) {
    NodoTexto* nuevo = new NodoTexto(solicitud, nullptr);
    if (estaVacia()) {
        frente_ = nuevo;
        final_ = nuevo;
    } else {
        final_->siguiente = nuevo;
        final_ = nuevo;
    }
    ++tamano_;
}

std::optional<std::string> ColaCertificados::desencolar() {
    if (estaVacia()) {
        return std::nullopt;
    }
    NodoTexto* temp = frente_;
    std::string valor = temp->dato;
    frente_ = frente_->siguiente;
    if (frente_ == nullptr) {
        final_ = nullptr;
    }
    delete temp;
    --tamano_;
    return valor;
}

bool ColaCertificados::estaVacia() const {
    return frente_ == nullptr;
}

std::size_t ColaCertificados::tamano() const {
    return tamano_;
}

std::vector<std::string> ColaCertificados::listarElementos() const {
    std::vector<std::string> elementos;
    elementos.reserve(tamano_);
    NodoTexto* actual = frente_;
    while (actual != nullptr) {
        elementos.push_back(actual->dato);
        actual = actual->siguiente;
    }
    return elementos;
}