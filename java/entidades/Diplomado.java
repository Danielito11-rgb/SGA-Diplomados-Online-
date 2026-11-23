package entidades;

import java.util.List;

public class Diplomado extends ProgramaAcademico {
    public Diplomado() {
        super("Diplomado");
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
        return promedio >= 14.0;
    }
}