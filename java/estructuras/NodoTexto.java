package estructuras;

public class NodoTexto {
    private final String dato;
    private NodoTexto siguiente;

    public NodoTexto(String dato, NodoTexto siguiente) {
        this.dato = dato;
        this.siguiente = siguiente;
    }

    public NodoTexto(String dato) {
        this(dato, null);
    }

    public String getDato() {
        return dato;
    }

    public NodoTexto getSiguiente() {
        return siguiente;
    }

    public void setSiguiente(NodoTexto siguiente) {
        this.siguiente = siguiente;
    }
}