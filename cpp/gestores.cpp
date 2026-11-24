#include "gestores.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

namespace {

std::string recortarEspacios(const std::string& texto) {
    const auto inicio = texto.find_first_not_of(" \t\r\n");
    if (inicio == std::string::npos) {
        return "";
    }
    const auto fin = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fin - inicio + 1);
}

std::string aMinusculas(std::string texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return texto;
}

std::vector<std::string> dividirTexto(const std::string& texto, char delimitador) {
    std::vector<std::string> partes;
    std::stringstream ss(texto);
    std::string item;
    while (std::getline(ss, item, delimitador)) {
        partes.push_back(item);
    }
    return partes;
}

} // namespace

GestorSGA::GestorSGA(const std::string& rutaDatos)
    : rutaDatos_(rutaDatos) {
    fs::create_directories(rutaDatos_);
}

void GestorSGA::cargarDatos() {
    cargarAlumnos();
    cargarProfesores();
    historial_.apilar("Sistema iniciado: Datos cargados desde el almacenamiento.");
}

void GestorSGA::cargarAlumnos() {
    fs::path rutaArchivo = fs::path(rutaDatos_) / "alumnos.txt";

    if (!fs::exists(rutaArchivo)) {
        std::ofstream crear(rutaArchivo);
        return;
    }

    alumnos_.clear();
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        linea = recortarEspacios(linea);
        if (linea.empty()) {
            continue;
        }

        std::vector<std::string> partes = dividirTexto(linea, ';');
        if (partes.size() >= 4) {
            const std::string& cedula = partes[0];
            const std::string& nombre = partes[1];
            const std::string& correo = partes[2];
            const std::string& tipoProg = partes[3];

            ProgramaAcademico* prog = instanciarPrograma(tipoProg);
            Alumno alumno(cedula, nombre, correo, prog);

            if (partes.size() > 4 && !partes[4].empty()) {
                std::vector<std::string> notasStr = dividirTexto(partes[4], ',');
                for (const std::string& nStr : notasStr) {
                    std::string limpia = recortarEspacios(nStr);
                    if (limpia.empty()) {
                        continue;
                    }
                    try {
                        double nota = std::stod(limpia);
                        alumno.agregarNota(nota);
                    } catch (const std::invalid_argument&) {
                    } catch (const std::out_of_range&) {
                    }
                }
            }

            alumnos_.push_back(alumno);
        }
    }
}

void GestorSGA::cargarProfesores() {
    fs::path rutaArchivo = fs::path(rutaDatos_) / "profesores.txt";

    if (!fs::exists(rutaArchivo)) {
        std::ofstream crear(rutaArchivo);
        return;
    }

    profesores_.clear();
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        linea = recortarEspacios(linea);
        if (linea.empty()) {
            continue;
        }

        std::vector<std::string> partes = dividirTexto(linea, ';');
        if (partes.size() >= 4) {
            const std::string& cedula = partes[0];
            const std::string& nombre = partes[1];
            const std::string& correo = partes[2];
            const std::string& especialidad = partes[3];

            profesores_.emplace_back(cedula, nombre, correo, especialidad);
        }
    }
}

void GestorSGA::guardarDatos() {
    guardarAlumnos();
    guardarProfesores();
    historial_.apilar("Persistencia realizada: Archivos TXT actualizados.");
}

void GestorSGA::guardarAlumnos() const {
    fs::path rutaArchivo = fs::path(rutaDatos_) / "alumnos.txt";
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return;
    }

    for (const Alumno& a : alumnos_) {
        std::string progNombre = (a.programa() != nullptr) ? a.programa()->nombrePrograma() : "Sin Programa";
        std::ostringstream notasStream;
        const std::vector<double>& listaNotas = a.notas();
        for (std::size_t i = 0; i < listaNotas.size(); ++i) {
            if (i > 0) {
                notasStream << ",";
            }
            notasStream << listaNotas[i];
        }

        archivo << a.cedula() << ";"
                << a.nombre() << ";"
                << a.correo() << ";"
                << progNombre << ";"
                << notasStream.str() << "\n";
    }
}

void GestorSGA::guardarProfesores() const {
    fs::path rutaArchivo = fs::path(rutaDatos_) / "profesores.txt";
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return;
    }

    for (const Profesor& p : profesores_) {
        archivo << p.cedula() << ";"
                << p.nombre() << ";"
                << p.correo() << ";"
                << p.areaEspecialidad() << "\n";
    }
}

ProgramaAcademico* GestorSGA::instanciarPrograma(const std::string& nombre) {
    std::string lower = aMinusculas(nombre);
    std::unique_ptr<ProgramaAcademico> nuevoPrograma = nullptr;

    if (lower.find("curso") != std::string::npos) {
        nuevoPrograma = std::make_unique<Curso>();
    } else if (lower.find("diplomado") != std::string::npos) {
        nuevoPrograma = std::make_unique<Diplomado>();
    } else if (lower.find("bootcamp") != std::string::npos) {
        nuevoPrograma = std::make_unique<Bootcamp>();
    } else {
        return nullptr;
    }

    ProgramaAcademico* ptr = nuevoPrograma.get();
    programas_.push_back(std::move(nuevoPrograma));
    return ptr;
}

bool GestorSGA::registrarAlumno(const Alumno& alumno) {
    if (buscarAlumnoPorCedula(alumno.cedula()) != nullptr) {
        return false;
    }
    alumnos_.push_back(alumno);
    historial_.apilar("Alumno registrado: " + alumno.nombre() + " (CI: " + alumno.cedula() + ")");
    guardarDatos();
    return true;
}

bool GestorSGA::registrarNotaConDeshacer(const std::string& cedula, double nota) {
    Alumno* alumno = buscarAlumnoPorCedula(cedula);
    if (alumno == nullptr) {
        return false;
    }
    alumno->agregarNota(nota);
    pilaDeshacerNotas_.push_back(RegistroNotaDeshacer{alumno->cedula(), nota});
    historial_.apilar("Nota agregada (" + std::to_string(nota) + ") a CI: " + alumno->cedula());
    guardarDatos();
    return true;
}

std::string GestorSGA::deshacerUltimaNota() {
    if (pilaDeshacerNotas_.empty()) {
        return "";
    }
    RegistroNotaDeshacer ultimo = pilaDeshacerNotas_.back();
    pilaDeshacerNotas_.pop_back();

    for (Alumno& a : alumnos_) {
        if (a.cedula() == ultimo.cedulaAlumno) {
            std::vector<double> actuales = a.notas();
            int idx = -1;
            for (int i = static_cast<int>(actuales.size()) - 1; i >= 0; --i) {
                if (std::fabs(actuales[static_cast<std::size_t>(i)] - ultimo.nota) < 1e-9) {
                    idx = i;
                    break;
                }
            }
            if (idx >= 0) {
                actuales.erase(actuales.begin() + idx);
                Alumno reconstruido(a.cedula(), a.nombre(), a.correo(), a.programa());
                for (double n : actuales) {
                    reconstruido.agregarNota(n);
                }
                a = reconstruido;
            }
            break;
        }
    }

    historial_.apilar("Deshacer: Se revirtio nota de CI: " + ultimo.cedulaAlumno);
    guardarDatos();
    return "Se deshizo la ultima nota del alumno CI: " + ultimo.cedulaAlumno + ".";
}

fs::path GestorSGA::exportarColaCertificados() {
    fs::path rutaArchivo = fs::path("certificados_pendientes.txt");
    std::ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return {};
    }

    std::vector<std::string> elementos = colaCertificados_.listarElementos();
    for (const std::string& item : elementos) {
        archivo << item << "\n";
    }

    historial_.apilar("Cola de certificados exportada a certificados_pendientes.txt.");
    return rutaArchivo;
}

Alumno* GestorSGA::buscarAlumnoPorCedula(const std::string& cedula) {
    for (Alumno& a : alumnos_) {
        if (a.cedula() == cedula) {
            return &a;
        }
    }
    return nullptr;
}

const Alumno* GestorSGA::buscarAlumnoPorCedula(const std::string& cedula) const {
    for (const Alumno& a : alumnos_) {
        if (a.cedula() == cedula) {
            return &a;
        }
    }
    return nullptr;
}

const std::vector<Alumno>& GestorSGA::alumnos() const {
    return alumnos_;
}

const std::vector<Profesor>& GestorSGA::profesores() const {
    return profesores_;
}

PilaHistorial& GestorSGA::historial() {
    return historial_;
}

const PilaHistorial& GestorSGA::historial() const {
    return historial_;
}

ColaCertificados& GestorSGA::colaCertificados() {
    return colaCertificados_;
}

const ColaCertificados& GestorSGA::colaCertificados() const {
    return colaCertificados_;
}