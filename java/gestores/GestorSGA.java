package gestores;

import entidades.Alumno;
import entidades.Bootcamp;
import entidades.Curso;
import entidades.Diplomado;
import entidades.Profesor;
import entidades.ProgramaAcademico;
import estructuras.ColaCertificados;
import estructuras.PilaHistorial;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;

public class GestorSGA {
    private final Path rutaDatos;
    private final List<Alumno> alumnos;
    private final List<Profesor> profesores;
    private final PilaHistorial historial;
    private final ColaCertificados colaCertificados;

    public GestorSGA(String rutaDatos) {
        this.rutaDatos = Paths.get(rutaDatos);
        this.alumnos = new ArrayList<>();
        this.profesores = new ArrayList<>();
        this.historial = new PilaHistorial();
        this.colaCertificados = new ColaCertificados();

        try {
            if (!Files.exists(this.rutaDatos)) {
                Files.createDirectories(this.rutaDatos);
            }
        } catch (IOException e) {
            System.err.println("No se pudo crear la carpeta de datos: " + e.getMessage());
        }
    }

    public GestorSGA() {
        this("datos");
    }

    public void cargarDatos() {
        cargarAlumnos();
        cargarProfesores();
        historial.apilar("Sistema iniciado: Datos cargados desde el almacenamiento.");
    }

    private void cargarAlumnos() {
        Path archivo = rutaDatos.resolve("alumnos.txt");
        if (!Files.exists(archivo)) {
            try {
                Files.createFile(archivo);
            } catch (IOException e) {
                System.err.println("No se pudo crear alumnos.txt: " + e.getMessage());
            }
            return;
        }

        alumnos.clear();
        try (BufferedReader br = Files.newBufferedReader(archivo, StandardCharsets.UTF_8)) {
            String linea;
            while ((linea = br.readLine()) != null) {
                linea = linea.trim();
                if (linea.isEmpty()) {
                    continue;
                }

                String[] partes = linea.split(";", -1);
                if (partes.length >= 4) {
                    String cedula = partes[0].trim();
                    String nombre = partes[1].trim();
                    String correo = partes[2].trim();
                    String tipoProg = partes[3].trim();

                    ProgramaAcademico prog = instanciarPrograma(tipoProg);
                    Alumno alumno = new Alumno(cedula, nombre, correo, prog);

                    if (partes.length > 4 && !partes[4].trim().isEmpty()) {
                        String[] notasStr = partes[4].split(",");
                        for (String nStr : notasStr) {
                            String limpia = nStr.trim();
                            if (limpia.isEmpty()) {
                                continue;
                            }
                            try {
                                double nota = Double.parseDouble(limpia);
                                alumno.agregarNota(nota);
                            } catch (IllegalArgumentException ignored) {
                                // Ignorar notas malformadas o fuera de rango al cargar
                            }
                        }
                    }

                    alumnos.add(alumno);
                }
            }
        } catch (IOException e) {
            System.err.println("Error al leer alumnos.txt: " + e.getMessage());
        }
    }

    private void cargarProfesores() {
        Path archivo = rutaDatos.resolve("profesores.txt");
        if (!Files.exists(archivo)) {
            try {
                Files.createFile(archivo);
            } catch (IOException e) {
                System.err.println("No se pudo crear profesores.txt: " + e.getMessage());
            }
            return;
        }

        profesores.clear();
        try (BufferedReader br = Files.newBufferedReader(archivo, StandardCharsets.UTF_8)) {
            String linea;
            while ((linea = br.readLine()) != null) {
                linea = linea.trim();
                if (linea.isEmpty()) {
                    continue;
                }

                String[] partes = linea.split(";", -1);
                if (partes.length >= 4) {
                    String cedula = partes[0].trim();
                    String nombre = partes[1].trim();
                    String correo = partes[2].trim();
                    String especialidad = partes[3].trim();

                    profesores.add(new Profesor(cedula, nombre, correo, especialidad));
                }
            }
        } catch (IOException e) {
            System.err.println("Error al leer profesores.txt: " + e.getMessage());
        }
    }

    public void guardarDatos() {
        guardarAlumnos();
        guardarProfesores();
        historial.apilar("Persistencia realizada: Archivos TXT actualizados.");
    }

    private void guardarAlumnos() {
        Path archivo = rutaDatos.resolve("alumnos.txt");
        try (BufferedWriter bw = Files.newBufferedWriter(archivo, StandardCharsets.UTF_8)) {
            for (Alumno a : alumnos) {
                String progNombre = (a.getPrograma() != null)
                    ? a.getPrograma().getNombrePrograma()
                    : "Sin Programa";

                List<Double> listaNotas = a.getNotas();
                StringBuilder notasSb = new StringBuilder();
                for (int i = 0; i < listaNotas.size(); i++) {
                    if (i > 0) {
                        notasSb.append(",");
                    }
                    notasSb.append(listaNotas.get(i));
                }

                bw.write(a.getCedula() + ";"
                    + a.getNombre() + ";"
                    + a.getCorreo() + ";"
                    + progNombre + ";"
                    + notasSb);
                bw.newLine();
            }
        } catch (IOException e) {
            System.err.println("Error al guardar alumnos.txt: " + e.getMessage());
        }
    }

    private void guardarProfesores() {
        Path archivo = rutaDatos.resolve("profesores.txt");
        try (BufferedWriter bw = Files.newBufferedWriter(archivo, StandardCharsets.UTF_8)) {
            for (Profesor p : profesores) {
                bw.write(p.getCedula() + ";"
                    + p.getNombre() + ";"
                    + p.getCorreo() + ";"
                    + p.getAreaEspecialidad());
                bw.newLine();
            }
        } catch (IOException e) {
            System.err.println("Error al guardar profesores.txt: " + e.getMessage());
        }
    }

    private ProgramaAcademico instanciarPrograma(String nombre) {
        if (nombre == null) {
            return null;
        }
        String lower = nombre.toLowerCase(Locale.ROOT);
        if (lower.contains("curso")) {
            return new Curso();
        } else if (lower.contains("diplomado")) {
            return new Diplomado();
        } else if (lower.contains("bootcamp")) {
            return new Bootcamp();
        }
        return null;
    }

    public boolean registrarAlumno(Alumno alumno) {
        if (alumno == null || buscarAlumnoPorCedula(alumno.getCedula()) != null) {
            return false;
        }
        alumnos.add(alumno);
        historial.apilar("Alumno registrado: " + alumno.getNombre() + " (CI: " + alumno.getCedula() + ")");
        guardarDatos();
        return true;
    }

    public Alumno buscarAlumnoPorCedula(String cedula) {
        if (cedula == null) {
            return null;
        }
        for (Alumno a : alumnos) {
            if (cedula.equals(a.getCedula())) {
                return a;
            }
        }
        return null;
    }

    public List<Alumno> getAlumnos() {
        return Collections.unmodifiableList(alumnos);
    }

    public List<Profesor> getProfesores() {
        return Collections.unmodifiableList(profesores);
    }

    public PilaHistorial getHistorial() {
        return historial;
    }

    public ColaCertificados getColaCertificados() {
        return colaCertificados;
    }
}