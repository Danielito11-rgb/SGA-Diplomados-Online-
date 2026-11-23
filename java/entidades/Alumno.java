package entidades;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Alumno extends Persona {
    private ProgramaAcademico programa;
    private final List<Double> notas;

    public Alumno(String cedula, String nombre, String correo, ProgramaAcademico programa) {
        super(cedula, nombre, correo);
        this.programa = programa;
        this.notas = new ArrayList<>();
    }

    public Alumno(String cedula, String nombre, String correo) {
        this(cedula, nombre, correo, null);
    }

    public ProgramaAcademico getPrograma() {
        return programa;
    }

    public void setPrograma(ProgramaAcademico programa) {
        this.programa = programa;
    }

    public List<Double> getNotas() {
        return Collections.unmodifiableList(notas);
    }

    public void agregarNota(double nota) {
        if (nota >= 0.0 && nota <= 20.0) {
            notas.add(nota);
        } else {
            throw new IllegalArgumentException("La nota debe estar entre 0 y 20.");
        }
    }

    public double calcularPromedio() {
        if (notas.isEmpty()) {
            return 0.0;
        }
        double suma = 0.0;
        for (double nota : notas) {
            suma += nota;
        }
        return suma / notas.size();
    }

    public boolean estaAprobado() {
        if (programa == null) {
            return false;
        }
        return programa.evaluarAprobacion(notas);
    }
}