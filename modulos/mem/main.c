#include <linux/proc_fs.h>
#include <linux/hugetlb.h>
#include <linux/module.h>

extern unsigned long total_swapcache_pages(void);
#define total_swapcache_pages()			0UL

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

static int al_abrir(struct inode *inode, struct file *file){
    return single_open(file, escribir_archivo, NULL);
}

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

static void salir(void)
{
    remove_proc_entry("mem_grupo16", NULL);
    printk(KERN_INFO "Sayonara mundo, somos el grupo 16 y este fue el monitor de memoria.\n");
}

module_init(iniciar);
module_exit(salir);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo Utilización RAM");
MODULE_AUTHOR("Grupo 16");
