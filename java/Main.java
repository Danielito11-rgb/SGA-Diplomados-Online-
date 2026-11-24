import entidades.Alumno;
import entidades.Bootcamp;
import entidades.Curso;
import entidades.Diplomado;
import entidades.ProgramaAcademico;
import gestores.GestorSGA;

import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.util.List;
import java.util.Locale;
import java.util.Scanner;

public class Main {
    private static int leerEntero(Scanner scanner, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String entrada = scanner.nextLine().trim();
            try {
                return Integer.parseInt(entrada);
            } catch (NumberFormatException e) {
                System.out.println("Error: Ingrese un valor numérico válido.");
            }
        }
    }

    private static double leerNota(Scanner scanner, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String entrada = scanner.nextLine().trim().replace(',', '.');
            try {
                return Double.parseDouble(entrada);
            } catch (NumberFormatException e) {
                System.out.println("Error: Ingrese un valor numérico válido.");
            }
        }
    }

    private static ProgramaAcademico elegirPrograma(Scanner scanner) {
        while (true) {
            System.out.println("\nSeleccione el programa académico:");
            System.out.println("1. Curso Profesional (aprueba con promedio >= 10.0)");
            System.out.println("2. Diplomado (aprueba con promedio >= 14.0)");
            System.out.println("3. Bootcamp Intensivo (aprueba con promedio >= 14.0 y cada nota >= 14.0)");
            int op = leerEntero(scanner, "Opción: ");
            switch (op) {
                case 1:
                    return new Curso();
                case 2:
                    return new Diplomado();
                case 3:
                    return new Bootcamp();
                default:
                    System.out.println("Opción no válida. Elija entre 1 y 3.");
            }
        }
    }

    private static void registrarAlumnoMenu(Scanner scanner, GestorSGA gestor) {
        System.out.println("\n--- Registrar Alumno ---");
        System.out.print("Cédula: ");
        String cedula = scanner.nextLine().trim();
        if (cedula.isEmpty()) {
            System.out.println("La cédula no puede estar vacía.");
            return;
        }
        if (gestor.buscarAlumnoPorCedula(cedula) != null) {
            System.out.println("Ya existe un alumno registrado con la cédula " + cedula + ".");
            return;
        }

        System.out.print("Nombre completo: ");
        String nombre = scanner.nextLine().trim();
        if (nombre.isEmpty()) {
            System.out.println("El nombre no puede estar vacío.");
            return;
        }

        System.out.print("Correo electrónico: ");
        String correo = scanner.nextLine().trim();

        ProgramaAcademico programa = elegirPrograma(scanner);
        Alumno alumno = new Alumno(cedula, nombre, correo, programa);

        if (gestor.registrarAlumno(alumno)) {
            System.out.println("Alumno registrado exitosamente en " + programa.getNombrePrograma() + ".");
        } else {
            System.out.println("No se pudo registrar el alumno.");
        }
    }

    private static void consultarAlumnoMenu(Scanner scanner, GestorSGA gestor) {
        System.out.println("\n--- Consultar Alumno ---");
        System.out.print("Ingrese la cédula del alumno: ");
        String cedula = scanner.nextLine().trim();
        Alumno alumno = gestor.buscarAlumnoPorCedula(cedula);

        if (alumno == null) {
            System.out.println("No se encontró ningún alumno con la cédula " + cedula + ".");
            return;
        }

        String nombreProg = (alumno.getPrograma() != null)
            ? alumno.getPrograma().getNombrePrograma()
            : "Sin Programa";
        String estado = alumno.estaAprobado() ? "APROBADO" : "NO APROBADO";

        System.out.println("Nombre   : " + alumno.getNombre());
        System.out.println("Cédula   : " + alumno.getCedula());
        System.out.println("Correo   : " + alumno.getCorreo());
        System.out.println("Programa : " + nombreProg);
        System.out.println("Notas    : " + alumno.getNotas());
        System.out.printf(Locale.ROOT, "Promedio : %.2f%n", alumno.calcularPromedio());
        System.out.println("Estado   : " + estado);
    }

    private static void agregarNotaMenu(Scanner scanner, GestorSGA gestor) {
        System.out.println("\n--- Agregar Nota a Alumno ---");
        System.out.print("Ingrese la cédula del alumno: ");
        String cedula = scanner.nextLine().trim();
        Alumno alumno = gestor.buscarAlumnoPorCedula(cedula);

        if (alumno == null) {
            System.out.println("No se encontró ningún alumno con la cédula " + cedula + ".");
            return;
        }

        double nota = leerNota(scanner, "Ingrese la nota (0.0 a 20.0): ");
        try {
            if (gestor.registrarNotaConDeshacer(cedula, nota)) {
                System.out.printf(Locale.ROOT, "Nota registrada. Nuevo promedio: %.2f%n", alumno.calcularPromedio());
            }
        } catch (IllegalArgumentException e) {
            System.out.println("Error al agregar nota: " + e.getMessage());
        }
    }

    private static void encolarCertificadoMenu(Scanner scanner, GestorSGA gestor) {
        System.out.println("\n--- Solicitar Certificado (Cola FIFO) ---");
        System.out.print("Ingrese la cédula del alumno: ");
        String cedula = scanner.nextLine().trim();
        Alumno alumno = gestor.buscarAlumnoPorCedula(cedula);

        if (alumno == null) {
            System.out.println("No se encontró ningún alumno con la cédula " + cedula + ".");
            return;
        }

        if (!alumno.estaAprobado()) {
            System.out.println("Aviso: El alumno figura como NO APROBADO actualmente.");
        }

        String solicitud = alumno.getNombre() + " (CI: " + alumno.getCedula() + ")";
        gestor.getColaCertificados().encolar(solicitud);
        gestor.getHistorial().apilar("Certificado encolado para: " + solicitud);
        System.out.println("Solicitud agregada a la cola de certificados.");
    }

    private static void atenderCertificadoMenu(GestorSGA gestor) {
        System.out.println("\n--- Atender Siguiente Certificado (FIFO) ---");
        String atendido = gestor.getColaCertificados().desencolar();
        if (atendido == null) {
            System.out.println("No hay solicitudes pendientes en la cola de certificados.");
        } else {
            gestor.getHistorial().apilar("Certificado atendido/emitido para: " + atendido);
            System.out.println("Certificado emitido para: " + atendido);
        }
    }

    private static void verHistorialMenu(GestorSGA gestor) {
        System.out.println("\n--- Historial de Acciones (Pila LIFO) ---");
        List<String> acciones = gestor.getHistorial().listarElementos();
        if (acciones.isEmpty()) {
            System.out.println("El historial está vacío.");
            return;
        }
        int idx = 1;
        for (String accion : acciones) {
            System.out.println(idx++ + ". " + accion);
        }
    }

    private static void deshacerMenu(GestorSGA gestor) {
        System.out.println("\n--- Deshacer Última Nota (LIFO) ---");
        String resultado = gestor.deshacerUltimaNota();
        if (resultado == null) {
            System.out.println("No hay notas recientes para deshacer.");
        } else {
            System.out.println(resultado);
        }
    }

    private static void generarColaCertificadosMenu(GestorSGA gestor) {
        System.out.println("\n--- Generar Cola de Certificados ---");
        Path rutaExportada = gestor.exportarColaCertificados();
        if (rutaExportada != null) {
            System.out.println("Archivo generado en: " + rutaExportada.toAbsolutePath());
        } else {
            System.out.println("No se pudo generar certificados_pendientes.txt.");
        }
    }

    private static void reporteGeneralMenu(GestorSGA gestor) {
        System.out.println("\n--- Reporte General de Alumnos ---");
        List<Alumno> lista = gestor.getAlumnos();
        if (lista.isEmpty()) {
            System.out.println("No hay alumnos registrados.");
            return;
        }
        for (Alumno a : lista) {
            String prog = (a.getPrograma() != null) ? a.getPrograma().getNombrePrograma() : "Sin Programa";
            String estado = a.estaAprobado() ? "APROBADO" : "NO APROBADO";
            System.out.printf(Locale.ROOT, "- %s (%s) | %s | Prom: %.2f | %s%n",
                a.getNombre(), a.getCedula(), prog, a.calcularPromedio(), estado);
        }
    }

    public static void main(String[] args) {
        GestorSGA gestor = new GestorSGA("datos");
        gestor.cargarDatos();

        try (Scanner scanner = new Scanner(System.in, StandardCharsets.UTF_8)) {
            boolean ejecutando = true;
            while (ejecutando) {
                System.out.println("\n==========================================");
                System.out.println("  SISTEMA DE GESTION ACADEMICA (SGA - JAVA)");
                System.out.println("==========================================");
                System.out.println("1. Registrar alumno");
                System.out.println("2. Consultar alumno (promedio y estado)");
                System.out.println("3. Agregar nota a alumno");
                System.out.println("4. Encolar solicitud de certificado (FIFO)");
                System.out.println("5. Atender siguiente certificado (FIFO)");
                System.out.println("6. Ver historial de acciones (LIFO)");
                System.out.println("7. Deshacer última nota (LIFO)");
                System.out.println("8. Generar cola de certificados (TXT)");
                System.out.println("9. Reporte general");
                System.out.println("0. Guardar y salir");
                System.out.println("==========================================");

                int opcion = leerEntero(scanner, "Seleccione una opción: ");
                switch (opcion) {
                    case 1:
                        registrarAlumnoMenu(scanner, gestor);
                        break;
                    case 2:
                        consultarAlumnoMenu(scanner, gestor);
                        break;
                    case 3:
                        agregarNotaMenu(scanner, gestor);
                        break;
                    case 4:
                        encolarCertificadoMenu(scanner, gestor);
                        break;
                    case 5:
                        atenderCertificadoMenu(gestor);
                        break;
                    case 6:
                        verHistorialMenu(gestor);
                        break;
                    case 7:
                        deshacerMenu(gestor);
                        break;
                    case 8:
                        generarColaCertificadosMenu(gestor);
                        break;
                    case 9:
                        reporteGeneralMenu(gestor);
                        break;
                    case 0:
                        gestor.guardarDatos();
                        System.out.println("Datos guardados. ¡Hasta luego!");
                        ejecutando = false;
                        break;
                    default:
                        System.out.println("Opción no válida. Intente nuevamente.");
                }
            }
        }
    }
}