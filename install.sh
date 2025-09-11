#!/bin/bash

# Cria a pasta 3DC se não existir
DIR="/usr/data/printer_data/config/3DC"

if [ -d "$DIR" ]; then
  echo "Directorio '$DIR' ja  existe."
else
  mkdir -p $DIR
  echo "Directorio '$DIR' criado."
fi

# Copia o arquivo principal da macro para a pasta correta
cp Config/camaleaoTURBO.cfg /usr/data/printer_data/config/3DC/Config/camaleaoTURBO.cfg

# Define permissões para garantir execução
chmod 644 /usr/data/printer_data/config/3DC/Config/camaleaoTURBO.cfg
ln -s /usr/data/helper-script/files/klipper-virtual-pins/virtual_pins.py /usr/share/klipper/klippy/extras/virtual_pins.py 
