package entidades;

import java.util.List;

public class Curso extends ProgramaAcademico {
    public Curso() {
        super("Curso Profesional");
    }

    @Override
    public boolean evaluarAprobacion(List<Double> notas) {
        if (notas == null || notas.isEmpty()) {
            return false;
        }
        double suma = 0.0;
        for (double nota : notas) {
            suma += nota;
        }
        double promedio = suma / notas.size();
        return promedio >= 10.0;
    }
}