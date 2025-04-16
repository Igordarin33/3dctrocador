#!/bin/bash

# Cria a pasta 3DC se não existir
mkdir -p /usr/data/printer_data/config/3DC

# Copia o arquivo principal da macro para a pasta correta
cp troca_cor.cfg /usr/data/printer_data/config/3DC/troca_cor.cfg

# Define permissões para garantir execução
chmod 644 /usr/data/printer_data/config/3DC/troca_cor.cfg
