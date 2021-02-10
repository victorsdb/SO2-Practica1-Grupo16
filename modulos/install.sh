sh uninstall.sh
dmesg --clear

if       cd cpu && make all  && insmod cpu_grupo16.ko  && make clean &&
cd .. && cd mem  && make all  && insmod mem_grupo16.ko  && make clean &&
cd .. && cd procesos  && make all && insmod procesos_grupo16.ko && make clean
then echo "\e[7;92m      Se instaloron los módulos correctamente.\e[0;0m"
else echo "\e[7;91m      ERROR: No se logro instalar todos los modulos.\e[0;0m"
fi

dmesg