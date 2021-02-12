#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/fs.h>

int ejecutados = 0;
int suspendidos = 0;
int detenidos = 0;
int zombies = 0;
int total = 0;

int contador = 0;

struct task_struct *task;

char *username;

void getEstado(long state)
{
    total++;
    switch (state)
    {

    case 0x0000:
        ejecutados++;
        break;
    case 0x0001:
        suspendidos++;
        break;
    case 0x0402:
        suspendidos++;
        break;
    case 0x0020:
        zombies++;
        break;
    case 0x0004:
        detenidos++;
        break;
    case 0x0100:
        detenidos++;
        break;
    }
}

void printState(struct seq_file *archivo, void *v, long state)
{

    switch (state)
    {

    case 0x0000:
        seq_printf(archivo, "\"estado\":\"Ejecutandose\",");
        break;
    case 0x0001:
        seq_printf(archivo, "\"estado\":\"Suspendido\",");
        break;
    case 0x0402:
        seq_printf(archivo, "\"estado\":\"Suspendido\",");
        break;
    case 0x0020:
        seq_printf(archivo, "\"estado\":\"Zombie\",");
        break;
    case 0x0004:
        seq_printf(archivo, "\"estado\":\"Detenido\",");
        break;
    case 0x0100:
        seq_printf(archivo, "\"estado\":\"Detenido\",");
        break;
    default:
        seq_printf(archivo, "\"estado\":\"%ld\",", state);
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
        printState(archivo, v, tmp->state);
        if (tmp->mm != NULL)
            seq_printf(archivo, "\"ram\":\"%lu MB\",", tmp->mm->total_vm * 4 / 1024);
        else
            seq_printf(archivo, "\"ram\":\"%d MB\",", 0);

        //getEstado(tmp->state);

        seq_printf(archivo, "\"user\":\"%u\"", tmp->cred->uid.val);

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