package entidades;

import java.util.List;

public abstract class ProgramaAcademico {
    private final String nombrePrograma;

    protected ProgramaAcademico(String nombrePrograma) {
        this.nombrePrograma = nombrePrograma;
    }

    public String getNombrePrograma() {
        return nombrePrograma;
    }

    public abstract boolean evaluarAprobacion(List<Double> notas);
}