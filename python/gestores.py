import os
from typing import List, Optional
from entidades import Alumno, Profesor, Curso, Diplomado, Bootcamp, ProgramaAcademico
from estructuras import PilaHistorial, ColaCertificados

class GestorSGA:
    """Gestor principal del Sistema"""
    def __init__(self, ruta_datos: str = "datos") -> None:
        self.ruta_datos: str = ruta_datos
        self.alumnos: List[Alumno] = []
        self.profesores: List[Profesor] = []
        self.historial: PilaHistorial = PilaHistorial()
        self.cola_certificados: ColaCertificados = ColaCertificados()
        if not os.path.exists(self.ruta_datos):
            os.makedirs(self.ruta_datos)

    def cargar_datos(self) -> None:
         self._cargar_alumnos()
         self._cargar_profesores()
         self.historial.apilar("Sistema iniciado")
    def _cargar_alumnos(self) -> None:
        ruta_archivo = os.path.join(self.ruta_datos, "alumnos.txt")
        if os.path.exists(ruta_archivo):
            return
        self.alumnos.clear()
        with open(ruta_archivo, "r", encoding="utf-8") as f:
            for linea in f:
                linea = linea.strip()
                if not linea:
                    continue
                partes = linea.split(";")
                if len(partes) >= 4:
                    cedula, nombre, correo, tipo_prog = partes[0], partes[1], partes[2], partes[3]
                    prog = self._insanciar_programa(tipo_prog)
                    alumno = Alumno(cedula, nombre, correo, prog)

                    if len(partes) > 4 and partes[4]:
                        notas_str = partes[4].split(",")
                        for n in notas_str:
                            try:
                                alumno.agregar_nota(float(n))
                            except ValueError:
                                pass
                    self.alumnos.append(alumno)
    def _cargar_profesores(self) -> None:
        ruta_archivo = os.path.join(self.ruta_datos, "profesores.txt")
        if not os.path.exists(ruta_archivo):
            return
        self.profesores.clear()
        with open(ruta_archivo, "r", encoding="utf-8") as f:
            for linea in f:
                linea = linea.strip()
                if not linea:
                    continue
                partes = linea.split(";")
                if len(partes) >= 4:
                    cedula, nombre, correo, especialidad = partes[0], partes[1], partes[2], partes[3]
                    profesor = Profesor(cedula, nombre, correo, especialidad)
                    self.profesores.append(profesor)
    def guardar_datos(self) -> None:
        self._guardar_alumnos()
        self._guardar_profesores()
        self.historial.apilar("Archivos TXT actualizados")

    def _guardar_alumnos(self) -> None:
        ruta_archivo = os.path.join(self.ruta_datos, "alumnos.txt")
        with open(ruta_archivo, "w", encoding="utf-8") as f:
            for a in self.alumnos:
                prog_nombre = a.programa.nombre_programa if a.programa else "Sin Programa"
                notas_str = ",".join(str(n) for n in a.notas)
                f.write(f"{a.cedula};{a.nombre};{a.correo};{prog_nombre};{notas_str}\n")
    def _guardar_profesores(self) -> None:
        ruta_archivo = os.path.join(self.ruta_datos, "profesores.txt")
        with open(ruta_archivo, "w", encoding="utf-8") as f:
            for p in self.profesores:
                f.write(f"{p.cedula};{p.nombre};{p.correo};{p.area_especialidad}\n")
    def _instanciar_programa(self, nombre: str) -> Optional [ProgramaAcademico]:
        nombre_lower = nombre.lower()
        if "curso" in nombre_lower:
            return Curso()
        elif "diplomado" in nombre_lower:
            return Diplomado()
        elif "bootcamp" in nombre_lower:
            return Bootcamp()
        return None
    def registrar_alumno(self, alumno: Alumno) -> bool:
        if self.buscar_alumo_por_cedula(alumno.cedula):
            return False
        self.alumnos.append(alumno)
        self.historial.apilar(f"Alumno registrado: {alumno.nombre} (CI: {alumno.cedula})")
        self.guardar_datos()
        return True
    def buscar_alumno_por_cedula(self, cedula: str) -> Optional[Alumno]:
        for alumno in self.alumnos:
            if alumno.cedula == cedula:
                return alumno
        return None