package entidades;

public abstract class Persona {
    private final String cedula;
    private final String nombre;
    private final String correo;

    protected Persona(String cedula, String nombre, String correo) {
        this.cedula = cedula;
        this.nombre = nombre;
        this.correo = correo;
    }

    public String getCedula() {
        return cedula;
    }

    public String getNombre() {
        return nombre;
    }

    public String getCorreo() {
        return correo;
    }

    @Override
    public String toString() {
        return nombre + " (CI: " + cedula + ")";
    }
}