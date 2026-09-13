from typing import Any, Optional, List

class Nodo:
    """Nodo fundamental para la construccion de estructuras."""

    def __init__(self, dato: Any) -> None:
        self.dato: Any = dato
        self.siguiente: Optional['Nodo'] = None
class PilaHistorial:
    """Estructura de datos LIFO para gestionar el historial de acciones."""

    def __init__(self) -> None:
        self._top: Optional[Nodo] = None
        self._tamano: int = 0

    def apilar(self, accion: str) -> None:
        """Agrega una accion al historial."""
        nuevo_nodo = Nodo(accion)
        nuevo_nodo.siguiente = self._top
        self._top = nuevo_nodo
        self._tamano += 1
    def desapilar(self) -> Optional[str]:
        if self.esta_vacia():
            return None
        dato = self._top.dato
        self._top = self._top.siguiente
        self._tamano -= 1
        return dato
    def esta_vacia(self) -> bool:
        return self._top is None

    def listar_elementos(self) -> List[str]:
        elementos: List[str] = []
        actual = self._top
        while actual:
            elementos.append(actual.dato)
            actual = actual.siguiente
        return elementos

class ColaCertificados:
    def __init__(self) -> None:
        self._frente: Optional[Nodo] = None
        self._final: Optional[Nodo] = None
        self._tamano: int = 0
        def encolar(self, solicitud: Any) -> None:
            nuevo_nodo = Nodo(solicitud)
            if self.esta_vacia():
                self._frente = nuevo_nodo
                self._final = nuevo_nodo
            else:
                self._final.siguiente = nuevo_nodo
                self._final = nuevo_nodo
            self._tamano += 1

    def desencolar(self) -> Optional[Any]:
        if self.esta_vacia():
            return None
        dato = self._frente.dato
        self._frente = self._frente.siguiente
        if self._frente is None:
            self._final = None
        self._tamano -= 1
        return dato
    def esta_vacia(self) -> bool:
        return self._frente is None
    def listar_elementos(self) -> List[Any]:
        elementos: List[Any]= []
        actual = self._frente
        while actual:
            elementos.append(actual.dato)
            actual = actual.siguiente
        return elementos