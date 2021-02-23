# Manual Técnico

En este manual se incluye la descripción de los modulos creados para los procesos y memoria RAM que se esta controlando desde la aplicación web.

## Instalación de los módulos

Para la instalación de cada uno de los modulos ingresamos  el siguiente comando dentro de las carpetas modulos/mem y modulos/procesos y el mismo se encargara de instalarlos, ya que en este se incluye el código necesario para la instalación. 

```
    sudo sh install.sh
```
## Creación de instalador
Dentro del archivo se utilizaron los siguientes comandos, los cuales se explicación mas a detalle posteriormente:

```s
make all
rmmod mem_grupo16
insmod mem_grupo16.ko
```
 ## Makefile
Se utilizó el siguiente código en un archivo Makefile:

```Makefile
obj-m += mem_grupo16.o
mem_grupo16-objs += main.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

luego de esto se corrio el comando **make all** para la creación de los ejecutables, y el comando **make clean** para borrarlos.

 ## Instalación Modulos
Para la instalación de los módulos se utilizo el siguiente comando para cargar los modulos al kernel:

```s
sudo insmod <<nombre_modulo>>.ko
```

 ## Desinstalación de Modulos
Se utilizo el siguiente comando para eliminar los modulos del kernel:
```s
sudo rmmod <<nombre_modulo>>.ko
```
 ## Buffer
 El siguiente comando se utilizo para visualizar el buffer:
Se utilizo el siguiente comando para eliminar los modulos al kernel:
```s
Dmesg
```
 ## Vaciar los mensajes del buffer
 Este comando se utilizo para visualizar el buffer:
Se utilizo el siguiente comando para eliminar los modulos al kernel:
```s
Dmesg -c
```


# Módulo de memoria RAM

Para la monitorear los datos de memoria RAM se utilizarón los siguientes métodos, dentro de un archivo en el lenguaje C.

* Se agregaron los siguientes includes para tener acceso a la carpeta proc.

```c
#include <linux/proc_fs.h>
#include <linux/hugetlb.h>
#include <linux/module.h>
```

* El flujo de las llamadas de los métodos utilizados en el módulo es el siguiente:

* Iniciamos el módulo: 

```c
module_init(iniciar);
```

* Se ejecuta el método **iniciar**, el cual ingresa al directorio proc.

```c
static struct file_operations operaciones = {
    .open = al_abrir,
    .read = seq_read
};

static int iniciar(void) 
{
    struct proc_dir_entry *entry;
    
    entry = proc_create("mem_grupo16", 0, NULL, &operaciones);
    
    if(!entry) 
        return -1; 
    else 
        printk(KERN_INFO "Hola mundo, somos el grupo 16 y este es el monitor de memoria.\n");
        
    return 0;
}
```

* Con el método **al_abrir()** se realiza la llamada al método **escribir_archivo()** para poder obtener la información que necesitamos monitorear. En este caso obtenemos la memoria total, la memoria usada y se calcula un porcentaje de uso a traves de estos valores obtenidos del struct sysinfo.

```c
static int escribir_archivo(struct seq_file *archivo, void *v)
{
    struct sysinfo i;
    long total_memory, free_memory, cached, usage_memory;

    si_meminfo(&i);

	cached = global_node_page_state(NR_FILE_PAGES) - total_swapcache_pages() - i.bufferram;
	
    if (cached < 0) 
        cached = 0;

    total_memory = i.totalram * 4;
    free_memory = (i.freeram + i.bufferram + cached) * 4;
    usage_memory = total_memory - free_memory;
    
    seq_printf(archivo, "{");
    seq_printf(archivo, "\"mem_tot\": %lu,",total_memory / 1024 );
    seq_printf(archivo, "\"mem_usada\": %lu,", usage_memory / 1024 );
    seq_printf(archivo, "\"porcentaje\": %lu", usage_memory * 10000 / total_memory);
    seq_printf(archivo, "}");
	return 0;
}
```

# Módulo de procesos

Para la monitorear los procesos se utilizarón los siguientes métodos, dentro de un archivo en el lenguaje C.

* Se agregaron los siguientes includes para tener acceso a la carpeta proc.

```c
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/fs.h>
```

El flujo de las llamadas de los métodos utilizados en el módulo es el siguiente:

* Iniciamos el módulo: 

```c
module_init(iniciar);
```

* * Se ejecuta el método **iniciar**, el cual ingresa al directorio proc.

```c
static struct file_operations operaciones =
    {
        .open = al_abrir,
        .read = seq_read};

int iniciar(void)
{
    struct proc_dir_entry *entry;

    entry = proc_create("procesos_grupo16", 0, NULL, &operaciones);

    if (!entry)
        return -1;
    else
        printk(KERN_INFO "Hola mundo, somos el grupo 16 y este es el monitor de procesos.\n");

    return 0;
}


```

* Con el método **al_abrir()** se realiza la llamada al método **escribir_archivo()** para poder obtener la información que necesitamos monitorear. En este caso obtenemos los procesos de cada tipo: ejecutados, suspendidos, detenidos, zombies y el total de procesos. Como algunos de los procesos cuentas con procesos hijos se utiliza un método foreach para poder realizar el recorrido

```c

static int escribir_archivo(struct seq_file *archivo, void *v)
{

    ejecutados = 0;
    suspendidos = 0;
    detenidos = 0;
    zombies = 0;
    total = 0;
    contador = 0;

    seq_printf(archivo, "{");

    seq_printf(archivo, "\"procesos\": [");

    for_each_process(task)
    {
        if (contador > 0)
            seq_printf(archivo, ",");

        seq_printf(archivo, "{");
        seq_printf(archivo, "\"pid\":\"%d\",", task->pid);
        seq_printf(archivo, "\"nombre\":\"%s\",", task->comm);
        //seq_printf(archivo, "\"estado\":\"%ld\",", task->state);
        printState(archivo, v, task->state);
        if (task->mm != NULL)
            seq_printf(archivo, "\"ram\":\"%lu MB\",", task->mm->total_vm * 4 / 1024);
        else
            seq_printf(archivo, "\"ram\":\"%d MB\",", 0);

        getEstado(task->state);

        seq_printf(archivo, "\"user\":\"%u\"", task->cred->uid.val);

        recorrer_hijos(archivo, v, task);

        seq_printf(archivo, "}");
        contador++;
    }
    seq_printf(archivo, "],");

    seq_printf(archivo, "\"total\":\"%d\",", total);
    seq_printf(archivo, "\"ejecutados\":\"%d\",", ejecutados);
    seq_printf(archivo, "\"suspendidos\":\"%d\",", suspendidos);
    seq_printf(archivo, "\"detenidos\":\"%d\",", detenidos);
    seq_printf(archivo, "\"zombies\":\"%d\"", zombies);

    seq_printf(archivo, "}");
    return 0;
}
```

* Se utilizarón algunos métodos extras para identificar los tipos de procesos y así poder obtener los conteos finales dependiendo del estado de cada uno.