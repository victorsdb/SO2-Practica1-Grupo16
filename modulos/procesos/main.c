#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/fs.h>

int procesosEjecucion = 0;
int procesosSuspendidos = 0;
int proccesosDetenidos = 0;
int procesosZombie = 0;
int totalProcesos = 0;

int contador = 0;

struct task_struct *task;

void getEstado(long estado)
{
    totalProcesos = totalProcesos + 1;
    switch (estado)
    {
    case 1: //ejecucion
        procesosEjecucion = procesosEjecucion + 1;
        break;
    case 0: //pendiente
        procesosSuspendidos = procesosSuspendidos + 1;
        break;
    case 260: //Tl
        proccesosDetenidos = proccesosDetenidos + 1;
        break;
    case 128: //zombies
        procesosZombie = procesosZombie + 1;
        break;
    }
}

void recorrer_hijos(struct seq_file *archivo, void *v, struct task_struct *t)
{
    struct list_head *l;
    struct task_struct *tmp;
    int c = 0;
    list_for_each(l, &t->children)
    {
        tmp = list_entry(l, struct task_struct, sibling);

        if (c == 0)
            seq_printf(archivo, ",\"hijos\": [");
        if (c > 0)
            seq_printf(archivo, ",");

        seq_printf(archivo, "{");
        seq_printf(archivo, "\"pid\":\"%d\",", tmp->pid);
        seq_printf(archivo, "\"nombre\":\"%s\",", tmp->comm);
        seq_printf(archivo, "\"estado\":\"%ld\"", tmp->state);

        getEstado(tmp->state);

        recorrer_hijos(archivo, v, tmp);

        seq_printf(archivo, "}");
        getEstado(tmp->state);
        c++;
    }

    if (c > 0)
        seq_printf(archivo, "]");
}

static int escribir_archivo(struct seq_file *archivo, void *v)
{

    seq_printf(archivo, "{");

    seq_printf(archivo, "\"total\":\"%d\",", totalProcesos);
    seq_printf(archivo, "\"ejecutados\":\"%d\",", procesosEjecucion);
    seq_printf(archivo, "\"suspendidos\":\"%d\",", procesosSuspendidos);
    seq_printf(archivo, "\"detenidos\":\"%d\",", proccesosDetenidos);
    seq_printf(archivo, "\"zombies\":\"%d\",", procesosZombie);

    seq_printf(archivo, "\"procesos\": [");

    contador = 0;
    for_each_process(task)
    {
        if (contador > 0)
            seq_printf(archivo, ",");

        seq_printf(archivo, "{");
        seq_printf(archivo, "\"pid\":\"%d\",", task->pid);
        seq_printf(archivo, "\"nombre\":\"%s\",", task->comm);
        seq_printf(archivo, "\"estado\":\"%ld\"", task->state);
  
        getEstado(task->state);

        recorrer_hijos(archivo, v, task);



        seq_printf(archivo, "}");
        contador++;
    }
    seq_printf(archivo, "]");
    seq_printf(archivo, "}");
    return 0;
}

static int al_abrir(struct inode *inode, struct file *file)
{
    return single_open(file, escribir_archivo, NULL);
}

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

void salir(void)
{
    remove_proc_entry("procesos_grupo16", NULL);
    printk(KERN_INFO "Sayonara mundo, somos el grupo 16 y este fue el monitor de procesos.\n");
}

module_init(iniciar);
module_exit(salir);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo de lista de procesos");
MODULE_AUTHOR("Grupo 16");