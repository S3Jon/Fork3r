# Resultados  
  
Recopilación de resultados  
  
## Programa base  
  
Con la versión presente, el programa se ejecuta correctamente y no causa impacto en el sistema.  
  
## Modificado sin limitador

Simplemente eliminando los limitadores causará que el programa cierre al de poco tiempo debido a la cantidad de llamadas. En la máquina virtual de pruebas, esto ocurre a las 7030 llamadas.

## Modificado y adaptado

Limitando la cantidad de forks, haciendo que envíe infinitas llamadas al host y añadiendo una pausa entre llamadas evita que el programa se cierre pues no se satura tán rápidamente, pero tras unos segundos consigue saturar la máquina virtual. **En este punto, los programas dejaban de correr fluidamente, era imposible ejecutar otra vez el programa y hasta dejó de funcionar correctamente la CLI.**  

Comandos tan simples como *sudo* dejaron de funcionar, y tuve que apagar la máquina desde la GUI.  

>Nota: para esta última versión decidí usar [usleep](https://man7.org/linux/man-pages/man3/usleep.3.html) en vez de [sleep](https://man7.org/linux/man-pages/man1/sleep.1.html) pues pausar 1 segundo entero parecía excesivo conociendo la finalidad de este programa.