import sys
from entidades import Alumno, Profesor, Curso, Diplomado, Bootcamp
from gestores import GestorSGA


def mostrar_menu() -> None:
    print("\n" + "=" * 50)
    print("      SISTEMA DE GESTIÓN ACADÉMICA (SGA)      ")
    print("=" * 50)
    print("1. Registrar nuevo Alumno")
    print("2. Registrar nuevo Profesor")
    print("3. Asignar nota a Alumno")
    print("4. Consultar estado / aprobación de Alumno")
    print("5. Solicitar Certificado (Encolar)")
    print("6. Procesar siguiente Certificado (Desencolar)")
    print("7. Ver Historial de Acciones (Pila LIFO)")
    print("8. Listar Alumnos y Profesores registrados")
    print("9. Guardar y Salir")
    print("=" * 50)


def main() -> None:
    gestor = GestorSGA()
    gestor.cargar_datos()

    while True:
        mostrar_menu()
        opcion = input("Seleccione una opción (1-9): ").strip()

        if opcion == "1":
            print("\n--- REGISTRAR ALUMNO ---")
            cedula = input("Cédula/CI: ").strip()
            nombre = input("Nombre completo: ").strip()
            correo = input("Correo electrónico: ").strip()
            print("Seleccione el Programa Académico:")
            print(" 1. Curso Profesional (Aprobación >= 10)")
            print(" 2. Diplomado Superior (Aprobación >= 14)")
            print(" 3. Bootcamp Intensivo (Aprobación >= 14 y sin reprobar)")
            prog_opcion = input("Opción (1-3): ").strip()

            programa = None
            if prog_opcion == "1":
                programa = Curso()
            elif prog_opcion == "2":
                programa = Diplomado()
            elif prog_opcion == "3":
                programa = Bootcamp()
            else:
                print("Opción de programa no válida. Registro cancelado.")
                continue

            alumno = Alumno(cedula, nombre, correo, programa)
            if gestor.registrar_alumno(alumno):
                print(f" Alumno '{nombre}' registrado exitosamente.")
            else:
                print("Error: Ya existe un alumno con esa cédula.")

        elif opcion == "2":
            print("\n--- REGISTRAR PROFESOR ---")
            cedula = input("Cédula/CI: ").strip()
            nombre = input("Nombre completo: ").strip()
            correo = input("Correo electrónico: ").strip()
            especialidad = input("Área de especialidad: ").strip()

            profesor = Profesor(cedula, nombre, correo, especialidad)
            gestor.profesores.append(profesor)
            gestor.historial.apilar(f"Profesor registrado: {nombre}")
            gestor.guardar_datos()
            print(f"Profesor '{nombre}' registrado exitosamente.")

        elif opcion == "3":
            print("\n--- ASIGNAR NOTA A ALUMNO ---")
            cedula = input("Ingrese la cédula del alumno: ").strip()
            alumno = gestor.buscar_alumno_por_cedula(cedula)
            if alumno:
                try:
                    nota = float(input("Ingrese la calificación (0-20): "))
                    alumno.agregar_nota(nota)
                    gestor.historial.apilar(f"Nota {nota} asignada a CI: {cedula}")
                    gestor.guardar_datos()
                    print(f"Nota {nota} asignada correctamente a {alumno.nombre}.")
                except ValueError as e:
                    print(f"Error: {e}")
            else:
                print(" Alumno no encontrado.")

        elif opcion == "4":
            print("\n--- CONSULTAR ESTADO DE ALUMNO ---")
            cedula = input("Ingrese la cédula del alumno: ").strip()
            alumno = gestor.buscar_alumno_por_cedula(cedula)
            if alumno:
                promedio = alumno.calcular_promedio()
                aprobado = alumno.esta_aprobado()
                estado_str = "APROBADO 🎉" if aprobado else "REPROBADO / EN CURSO ⚠️"
                prog_str = alumno.programa.nombre_programa if alumno.programa else "Sin programa"
                
                print(f"\nAlumno: {alumno.nombre} (CI: {alumno.cedula})")
                print(f"Programa: {prog_str}")
                print(f"Notas: {alumno.notas}")
                print(f"Promedio: {promedio:.2f}")
                print(f"Estado Final: {estado_str}")
            else:
                print(" Alumno no encontrado.")

        elif opcion == "5":
            print("\n--- SOLICITAR CERTIFICADO ---")
            cedula = input("Ingrese la cédula del alumno: ").strip()
            alumno = gestor.buscar_alumno_por_cedula(cedula)
            if alumno:
                if alumno.esta_aprobado():
                    solicitud = f"Certificado de {alumno.nombre} ({alumno.programa.nombre_programa})"
                    gestor.cola_certificados.encolar(solicitud)
                    gestor.historial.apilar(f"Certificado encolado para CI: {cedula}")
                    print(f" Solicitud agregada a la cola: {solicitud}")
                else:
                    print("El alumno aún no cumple los requisitos para certificarse.")
            else:
                print("Alumno no encontrado.")

        elif opcion == "6":
            print("\n--- PROCESAR SIGUIENTE CERTIFICADO ---")
            cert = gestor.cola_certificados.desencolar()
            if cert:
                gestor.historial.apilar(f"Certificado procesado: {cert}")
                print(f" PROCESADO Y EMITIDO: {cert}")
            else:
                print("ℹ No hay certificados pendientes en la cola.")

        elif opcion == "7":
            print("\n--- HISTORIAL DE ACCIONES (PILA LIFO) ---")
            acciones = gestor.historial.listar_elementos()
            if acciones:
                for idx, acc in enumerate(acciones, 1):
                    print(f" {idx}. {acc}")
            else:
                print("ℹ El historial está vacío.")

        elif opcion == "8":
            print("\n--- LISTADO GENERAL DE REGISTROS ---")
            print("\n[ ALUMNOS ]")
            if gestor.alumnos:
                for a in gestor.alumnos:
                    prog_str = a.programa.nombre_programa if a.programa else "Sin Programa"
                    print(f" - {a} | Programa: {prog_str} | Promedio: {a.calcular_promedio():.2f}")
            else:
                print(" No hay alumnos registrados.")

            print("\n[ PROFESORES ]")
            if gestor.profesores:
                for p in gestor.profesores:
                    print(f" - {p} | Especialidad: {p.area_especialidad}")
            else:
                print(" No hay profesores registrados.")

        elif opcion == "9":
            gestor.guardar_datos()
            print("\n Datos guardados correctamente. ¡Hasta luego!")
            sys.exit(0)

        else:
            print(" Opción no válida. Por favor, intente de nuevo.")


if __name__ == "__main__":
    main()