package entidades;

public class Profesor extends Persona {
    private final String areaEspecialidad;

    public Profesor(String cedula, String nombre, String correo, String areaEspecialidad) {
        super(cedula, nombre, correo);
        this.areaEspecialidad = areaEspecialidad;
    }

    public String getAreaEspecialidad() {
        return areaEspecialidad;
    }
}