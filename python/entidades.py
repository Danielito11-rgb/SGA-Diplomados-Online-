from abc import ABC, abstractmethod
from typing import List

# --- JERARQUIA DE PERSONAS ---
class Persona(ABC):
    """Clase base abstracta para representar a una persona."""

    def __init__(self, cedula: str, nombre: str, correo: str) -> None:
        self._cedula = cedula
        self._nombre = nombre
        self._correo = correo
    # Encapsulamiento mediante getters
    @property
    def cedula(self) -> str:
        return self._cedula

    @property
    def nombre(self) -> str:
        return self._nombre

    @property
    def correo(self) -> str:
        return self._correo
    def __str__(self) -> str:
        return f"{self.nombre} (CI: {self._cedula})"

class Alumno(Persona):
        """Clase que representa a un estudiante encuestado/inscrito."""

        def __init__(self, cedula: str, nombre: str, correo: str, programa: 'ProgramaAcademico' = None) -> None:
            super().__init__(cedula, nombre, correo)
            self.programa: 'ProgramaAcademico' = programa
            self.notas: List[float] = []

        def agregar_nota(self, nota: float) -> None:
            """Agrega una calificacion en un rango de 0 a 20."""
            if 0.0 <= nota <= 20.0:
                self.notas.append(nota)
            else:
                raise ValueError("La nota debe estar entre 0 y 20.")
        def calcular_promedio(self) -> float:
            if not self.notas:
                return 0.0
            return sum(self.notas) / len(self.notas)
        def esta_aprobado(self) -> bool:
            if not self.programa:
                return False
            return self.programa.evaluar_aprobacion(self.notas)

class Profesor(Persona):
    """Clase qe representa al personal docente.""" 
    def __init__(self, cedula: str, nombre: str, correo: str, area_especialidad: str) -> None:
        super().__init__(cedula, nombre, correo)
        self.area_especialidad: str = area_especialidad

# --- JERARQUIA DE PROGRAMAS ACADEMICOS (POLIMORFISMO) ---

class ProgramaAcademico(ABC):
    """Clase que define el contrato para la evaluacion"""
    def __init__(self, nombre_programa: str) -> None:
        self.nombre_programa: str = nombre_programa

    @abstractmethod
    def evaluar_aprobacion(self, notas: List[float]) -> bool:
        """Metodo abstracto que cada programa debe implementar segun sus reglas"""
        pass

class Curso(ProgramaAcademico):
    def __init__(self) -> None:
        super().__init__("Curso Profesional")
    def evaluar_aprobacion(self, notas: List[float]) -> bool:
        if not notas:
            return False
        promedio = sum(notas) / len(notas)
        return promedio >= 10.0

class Diplomado(ProgramaAcademico):
    def __init__(self) -> None:
        super().__init__("Diplomado")
    def evaluar_aprobacion(self, notas: List[float]) -> bool:
        if not notas:
            return False
        promedio = sum(notas) / len(notas)
        return promedio >= 14.0

class Bootcamp(ProgramaAcademico):
    def __init__(self) -> None:
        super().__init__("Bootcamp Intensivo")

    def evaluar_aprobacion(self, notas: List[float]) -> bool:
        if not notas:
            return False
        # Regla esricta: Aprueba si el proedio es >= 14 y ninguna Nota es menor a 14
        promedio = sum(notas) / len(notas)
        ninguna_reprobada = all(nota >= 14.0 for nota in notas)
        return promedio >= 14.0 and ninguna_reprobada
    