
# Instalação do 3DC Trocador (Troca de cor para Klipper)

Este projeto permite usar o sistema de troca de cor (como o 3D Chameleon) de forma prática com o Klipper, feito para k1(C,SE,MAX) com atualizações automáticas via Moonraker.

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

## 🔧 Ativando o camaleaoTURBO.cfg
Edite seu arquivo principal de configuração printer.cfg e adicione esta linha no começo:
```
[include 3DC/camaleaoTURBO.cfg]

```
### Depois comente o codigo completo dos sensores [filament_switch_sensor filament_sensor] e [filament_switch_sensor filament_sensor_2]

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
## 🛠️ Configurando gcode_macro.cfg
Depois comente o codigo de carregar o filamento original, e adicione esse: 
Essa parte você deve alterar a temperatura, ela quem será a padrão sempre que carregar o filamento, recomendo deixar em 240 para abs, PLA em 200, ou edite da forma que preferir! 
```
[gcode_macro LOAD_MATERIAL]
gcode:
  {% if printer.extruder.temperature < 220 %}
      M118 Preaquecendo até 220°C
      SET_HEATER_TEMPERATURE HEATER=extruder TARGET=220
  {% endif %}
  LOAD_MATERIAL_CLOSE_FAN2
  G91
  G1 E45 F300
  LOAD_MATERIAL_RESTORE_FAN2
```


## ♻️ Atualizações futuras
Depois de configurado, você poderá atualizar suas macros diretamente pela interface do Moonraker (em "Atualizações" no Fluidd/Mainsail)

✅ Pronto!
Após seguir os passos acima, basta reiniciar o Klipper e seu sistema de troca de cor estará instalado e pronto para uso! Basta terminar de ajustar o Gcode de seu fatiador



# ❓ Como Funciona?
## 🖲️ Seleção de Extrusora
Cada extrusora é selecionada com base no tempo de pressão do botão:
```
Pulso	  Ação
  1º	  Seleciona T0 (filamento 1)
  2º	  Seleciona T1 (filamento 2)
  3º	  Seleciona T2 (filamento 3)
  4º	  Seleciona T3 (filamento 4)
  5º	  Seleciona T4 (filamento 5)
  6º	  Seleciona T5 (filamento 6)
  7º	  Seleciona T6 (filamento 7)
  8º	  Seleciona T7 (filamento 8)
  9º	  Seleciona Carregar/Inicio T0
  10º	  Seleciona Descarregar/Inicio
  11º	  Seleciona Inicio
  12º	  Seleciona Próximo
  13º	  Seleciona Aleatório
  14º	  Seleciona Pulso Extra
  
  ```
## 🤖 Funcionamento Interno
O sistema usa dois sensores de filamento:
Um para detectar a entrada do filamento (sensor da extrusora).
Um para detectar a saída do filamento (sensor traseiro original).

A impressora ativa o pino PA0 para selecionar e injetar o filamento desejado.
Quando o filamento atinge o sensor da extrusora, ele é automaticamente puxado.
A remoção funciona de forma semelhante, moldando a ponta do filamento para facilitar a troca.



## Arquivos do printables e listas de peças: 
[Printables](https://www.printables.com/model/1216331-3dchameleon-k1ck1k1max-arduino-uno-cnc-shield-v3)

# 💖 Apoie este Projeto
Se este projeto te ajudou ou você acredita no que estou construindo, considere apoiar com uma doação. Isso me motiva a continuar melhorando, atualizando e criando novas ferramentas gratuitas para a comunidade 3D!
🔗 Chave Pix (e-mail): [ igordarin34@gmail.com ]

##Créditos: 
[3DChameleon](https://www.3dchameleon.com)
