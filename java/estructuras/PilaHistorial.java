package estructuras;

import java.util.ArrayList;
import java.util.List;

public class PilaHistorial {
    private NodoTexto top;
    private int tamano;

    public PilaHistorial() {
        this.top = null;
        this.tamano = 0;
    }

    public void apilar(String accion) {
        NodoTexto nuevo = new NodoTexto(accion, top);
        top = nuevo;
        tamano++;
    }

    public String desapilar() {
        if (estaVacia()) {
            return null;
        }
        String valor = top.getDato();
        top = top.getSiguiente();
        tamano--;
        return valor;
    }

    public boolean estaVacia() {
        return top == null;
    }

    public int getTamano() {
        return tamano;
    }

    public List<String> listarElementos() {
        List<String> elementos = new ArrayList<>();
        NodoTexto actual = top;
        while (actual != null) {
            elementos.add(actual.getDato());
            actual = actual.getSiguiente();
        }
        return elementos;
    }
}