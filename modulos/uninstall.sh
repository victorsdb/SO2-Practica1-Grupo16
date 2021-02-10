dmesg --clear
if rmmod cpu_grupo16
then echo "\e[7;92m      Se removio el modulo cpu_grupo16 correctamente.\e[0;0m"
else echo "\e[7;91m      ERROR: No se logro remover el modulo cpu_grupo16.\e[0;0m"
fi

if rmmod mem_grupo16
then echo "\e[7;92m      Se removio el modulo mem_grupo16 correctamente.\e[0;0m"
else echo "\e[7;91m      ERROR: No se logro remover el modulo mem_grupo16.\e[0;0m"
fi

if  rmmod procesos_grupo16
then echo "\e[7;92m      Se removio el modulo procesos_grupo16 correctamente.\e[0;0m"
else echo "\e[7;91m      ERROR: No se logro remover el modulo procesos_grupo16.\e[0;0m"
fi
dmesg