package estructuras;

import java.util.ArrayList;
import java.util.List;

public class ColaCertificados {
    private NodoTexto frente;
    private NodoTexto finalCola;
    private int tamano;

    public ColaCertificados() {
        this.frente = null;
        this.finalCola = null;
        this.tamano = 0;
    }

    public void encolar(String solicitud) {
        NodoTexto nuevo = new NodoTexto(solicitud, null);
        if (estaVacia()) {
            frente = nuevo;
            finalCola = nuevo;
        } else {
            finalCola.setSiguiente(nuevo);
            finalCola = nuevo;
        }
        tamano++;
    }

    public String desencolar() {
        if (estaVacia()) {
            return null;
        }
        String valor = frente.getDato();
        frente = frente.getSiguiente();
        if (frente == null) {
            finalCola = null;
        }
        tamano--;
        return valor;
    }

    public boolean estaVacia() {
        return frente == null;
    }

    public int getTamano() {
        return tamano;
    }

    public List<String> listarElementos() {
        List<String> elementos = new ArrayList<>();
        NodoTexto actual = frente;
        while (actual != null) {
            elementos.add(actual.getDato());
            actual = actual.getSiguiente();
        }
        return elementos;
    }
}