
# Instalação do 3DC Trocador (Troca de cor para Klipper)

Este projeto permite usar o sistema de troca de cor (como o 3D Chameleon) de forma prática com o Klipper, com atualizações automáticas via Moonraker.

---

## 📦 Requisitos

- Klipper já instalado e funcionando
- Acesso SSH ou terminal na sua impressora
- Internet ativa na impressora

---

## 🚀 Instalação

Execute os comandos abaixo diretamente no terminal da sua impressora:
```
git clone --depth 1 https://github.com/Igordarin33/3dctrocador.git /usr/data/printer_data/config/3DC
```
---

## 🔧 Ativando o troca_cor.cfg
Edite seu arquivo principal de configuração printer.cfg e adicione esta linha no começo:
```
[include 3DC/troca_cor.cfg]
```
---

## 🛠️ Configurando Moonraker para atualizações automáticas
Edite o arquivo /usr/data/printer_data/config/moonraker.conf e adicione:
```
[update_manager 3dc_macros]
type: git_repo
channel: stable
path: /usr/data/printer_data/config/3DC
origin: https://github.com/Igordarin33/3dctrocador.git
install_script: install.sh
managed_services:
  klipper
  ```
---

## ♻️ Atualizações futuras
Depois de configurado, você poderá atualizar suas macros diretamente pela interface do Moonraker (em "Atualizações" no Fluidd/Mainsail)

✅ Pronto!
Após seguir os passos acima, basta reiniciar o Klipper e seu sistema de troca de cor estará instalado e pronto para uso! Basta terminar de ajustar o Gcode de seu fatiador


##Arquivos do printables e listas de peças: 
[Printables]([https://pages.github.com/](https://www.printables.com/model/1216331-3dchameleon-k1ck1k1max-arduino-uno-cnc-shield-v3))


