#include "entidades.hpp"
#include "estructuras.hpp"
#include "gestores.hpp"

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void limpiarEntrada() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string leerLinea(const std::string& mensaje) {
    std::string valor;
    std::cout << mensaje;
    std::getline(std::cin, valor);
    return valor;
}

int leerEntero(const std::string& mensaje) {
    int valor = 0;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) {
            limpiarEntrada();
            return valor;
        }
        std::cout << "Error: Ingrese un valor numerico valido.\n";
        limpiarEntrada();
    }
}

double leerDouble(const std::string& mensaje) {
    double valor = 0.0;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) {
            limpiarEntrada();
            return valor;
        }
        std::cout << "Error: Ingrese un valor numerico valido.\n";
        limpiarEntrada();
    }
}

void mostrarMenu() {
    std::cout << "\n========================================\n"
              << "   SGA - SISTEMA DE GESTION ACADEMICA\n"
              << "========================================\n"
              << "1. Registrar alumno\n"
              << "2. Listar alumnos\n"
              << "3. Agregar nota a alumno\n"
              << "4. Encolar solicitud de certificado\n"
              << "5. Atender siguiente certificado (FIFO)\n"
              << "6. Ver historial de acciones (LIFO)\n"
              << "7. Deshacer ultima nota (LIFO)\n"
              << "8. Generar cola de certificados (TXT)\n"
              << "9. Guardar datos manualmente\n"
              << "0. Salir\n"
              << "========================================\n";
}

} // namespace

int main() {
    GestorSGA gestor("datos");
    gestor.cargarDatos();

    std::vector<std::unique_ptr<ProgramaAcademico>> catalogos;
    catalogos.push_back(std::make_unique<Curso>());
    catalogos.push_back(std::make_unique<Diplomado>());
    catalogos.push_back(std::make_unique<Bootcamp>());

    int opcion = -1;
    do {
        mostrarMenu();
        opcion = leerEntero("Selecciona una opcion: ");

        switch (opcion) {
            case 1: {
                std::string cedula = leerLinea("Cedula: ");
                std::string nombre = leerLinea("Nombre completo: ");
                std::string correo = leerLinea("Correo electronico: ");

                std::cout << "Programa:\n"
                          << "  1. Curso Profesional\n"
                          << "  2. Diplomado\n"
                          << "  3. Bootcamp Intensivo\n"
                          << "  0. Sin Programa\n";
                int opProg = leerEntero("Elige programa: ");

                ProgramaAcademico* progSeleccionado = nullptr;
                if (opProg == 1) {
                    progSeleccionado = catalogos[0].get();
                } else if (opProg == 2) {
                    progSeleccionado = catalogos[1].get();
                } else if (opProg == 3) {
                    progSeleccionado = catalogos[2].get();
                }

                Alumno nuevoAlumno(cedula, nombre, correo, progSeleccionado);
                if (gestor.registrarAlumno(nuevoAlumno)) {
                    std::cout << "Alumno registrado y guardado correctamente.\n";
                } else {
                    std::cout << "Ya existe un alumno registrado con esa cedula.\n";
                }
                break;
            }
            case 2: {
                const auto& lista = gestor.alumnos();
                if (lista.empty()) {
                    std::cout << "No hay alumnos registrados.\n";
                } else {
                    std::cout << "\n--- ALUMNOS REGISTRADOS ---\n";
                    for (const Alumno& a : lista) {
                        std::string nombreProg = (a.programa() != nullptr)
                            ? a.programa()->nombrePrograma()
                            : "Sin Programa";
                        std::string estado = a.estaAprobado() ? "APROBADO" : "NO APROBADO";
                        std::cout << a.comoTexto()
                                  << " | Programa: " << nombreProg
                                  << " | Promedio: " << std::fixed << std::setprecision(2) << a.calcularPromedio()
                                  << " | Estado: " << estado << "\n";
                    }
                }
                break;
            }
            case 3: {
                std::string cedula = leerLinea("Cedula del alumno: ");
                Alumno* alumno = gestor.buscarAlumnoPorCedula(cedula);
                if (alumno == nullptr) {
                    std::cout << "Alumno no encontrado.\n";
                } else {
                    double nota = leerDouble("Nota (0 a 20): ");
                    try {
                        if (gestor.registrarNotaConDeshacer(cedula, nota)) {
                            std::cout << "Nota registrada. Nuevo promedio: "
                                      << std::fixed << std::setprecision(2) << alumno->calcularPromedio() << "\n";
                        }
                    } catch (const std::out_of_range& ex) {
                        std::cout << "Error: " << ex.what() << "\n";
                    }
                }
                break;
            }
            case 4: {
                std::string solicitud = leerLinea("Descripcion de solicitud de certificado (ej. CI - Nombre): ");
                if (!solicitud.empty()) {
                    gestor.colaCertificados().encolar(solicitud);
                    gestor.historial().apilar("Certificado encolado: " + solicitud);
                    std::cout << "Solicitud agregada a la cola.\n";
                } else {
                    std::cout << "La solicitud no puede estar vacia.\n";
                }
                break;
            }
            case 5: {
                std::optional<std::string> atendida = gestor.colaCertificados().desencolar();
                if (atendida.has_value()) {
                    gestor.historial().apilar("Certificado atendido: " + atendida.value());
                    std::cout << "Certificado procesado: " << atendida.value() << "\n";
                } else {
                    std::cout << "La cola de certificados esta vacia.\n";
                }
                break;
            }
            case 6: {
                std::vector<std::string> acciones = gestor.historial().listarElementos();
                if (acciones.empty()) {
                    std::cout << "El historial esta vacio.\n";
                } else {
                    std::cout << "\n--- HISTORIAL DE ACCIONES (ULTIMA A PRIMERA) ---\n";
                    for (const std::string& act : acciones) {
                        std::cout << "- " << act << "\n";
                    }
                }
                break;
            }
            case 7: {
                std::string msg = gestor.deshacerUltimaNota();
                if (msg.empty()) {
                    std::cout << "No hay notas recientes para deshacer.\n";
                } else {
                    std::cout << msg << "\n";
                }
                break;
            }
            case 8: {
                std::filesystem::path ruta = gestor.exportarColaCertificados();
                if (!ruta.empty()) {
                    std::cout << "Archivo generado en: " << std::filesystem::absolute(ruta) << "\n";
                } else {
                    std::cout << "No se pudo generar certificados_pendientes.txt.\n";
                }
                break;
            }
            case 9: {
                gestor.guardarDatos();
                std::cout << "Datos guardados en disco.\n";
                break;
            }
            case 0: {
                gestor.guardarDatos();
                std::cout << "Saliendo del SGA. Hasta luego!\n";
                break;
            }
            default: {
                std::cout << "Opcion no valida.\n";
                break;
            }
        }
    } while (opcion != 0);

    return 0;
}
