# Motor Core Backend — Sistema de Gestión Académica (SGA-DO)

## 📌 Descripción del Proyecto
El **SGA-DO** es el motor backend modular desarrollado para resolver la crisis operativa de DiplomadosOnline, reemplazando el control manual en hojas de cálculo por una arquitectura robusta orientada a objetos (POO), manejo de persistencia plana y estructuras de datos dinámicas (Pilas y Colas).

## 📑 Reglas de Negocio (Criterios de Aprobación)
* **Curso:** Aprueba con un promedio final de notas $\ge 10.0$ puntos.
* **Diplomado:** Aprueba con un promedio final de notas $\ge 14.0$ puntos.
* **Bootcamp:** Evaluación polimórfica individual. Exige que **ninguna nota parcial sea menor a 14.0 puntos**, sin importar el promedio general.

## 📁 Estructura del Repositorio
```text
├── docs/          # Análisis de Requerimientos (Fase 1) y Diagrama UML (Fase 2)
├── python/        # Implementación del MVP en Python y archivos planos (.txt) (Fase 4)
├── java/          # Migración del motor a Java (Fase 5)
└── cpp/           # Migración del motor a C++ (Fase 6)
