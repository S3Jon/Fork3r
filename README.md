Fork3r
======

Autor: S3Jon\
Estado: Proof of Concept (PoC) - Solo para fines educativos

🚀 Introducción
------

Fork3r es un experimento diseñado para explorar la creación y gestión de procesos en sistemas Unix mediante fork(). Este proyecto NO está pensado para entornos de producción y su ejecución sin modificaciones puede tener consecuencias graves.

⚠️ ADVERTENCIA: Con ligeras modificaciones, este código puede causar daños en equipos físicos, pérdida o corrupción de información, y afectar la estabilidad del sistema. El autor no se responsabiliza de su uso indebido.

🎯 Objetivos (Goals)
------

Goal 1: Generar una cadena de procesos, donde cada proceso inicia otro hasta un máximo de 10.

Goal 2: Solo el proceso principal puede lanzar nuevos procesos, los procesos secundarios solo envían señales al main.

Goal 3: Los procesos secundarios pueden generar más forks bajo ciertas condiciones, pero no reservan memoria adicional.

Goal 4: Simular posibles escenarios de abuso de recursos mediante bloqueo de file descriptors y gestión de señales.

🔧 Requisitos
------

Sistema Unix/Linux

Compilador compatible con C (GCC recomendado)

Conocimientos básicos de procesos y señales en sistemas operativos

🛠️ Instalación y Ejecución
------

# Clonar el repositorio
```
git clone https://github.com/S3Jon/Fork3r.git
```
```
cd Fork3r
```

# Compilar el código
```
gcc (archivo_elegido).c -o fork3r
```

# Ejecutar (⚠️ Usar con precaución)
```
./fork3r
```

⚠️ Disclaimer
------

Este proyecto es únicamente una proof of concept con fines educativos. No se recomienda su ejecución en entornos críticos.

El autor no se hace responsable de daños ocasionados a hardware, sistemas operativos o pérdida de información derivada del uso de este código.

📜 Licencia
------

Este proyecto se distribuye bajo la licencia MIT. Puedes modificarlo y compartirlo bajo tu propia responsabilidad.

