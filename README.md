
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

## 🔧 Ativando o troca_cor.cfg
Edite seu arquivo principal de configuração printer.cfg e adicione esta linha no começo:
```
[include 3DC/troca_cor.cfg]
```
### Depois comente o codigo dos sensores [filament_switch_sensor filament_sensor] e [filament_switch_sensor filament_sensor_2]
Em seguida adicione os novos: 
```
[filament_switch_sensor filament_sensor]
pause_on_runout: true
switch_pin: PC15
event_delay: 1.5
pause_delay: 0.25
insert_gcode:
#  {% if printer.extruder.temperature < 140 %}
#    M118 "Preaquecendo até 140°C"
#    SET_HEATER_TEMPERATURE HEATER=extruder TARGET=140
#  {% endif %}

  {% if "x" not in printer.toolhead.homed_axes or
        "y" not in printer.toolhead.homed_axes or
        "z" not in printer.toolhead.homed_axes %}
    M118 "Homing necessário - executando G28"
    G28
  {% endif %}

  M118 "Filamento removido ou escasso"
  G0 X228.94 Y-0.49 F15000
  G4 P1000 #Tempo para sai do hub
  SET_PIN PIN=troca_cor VALUE=0
runout_gcode:
  M118 "Filamento inserido"
  

#sensor da extrusora
[filament_switch_sensor filament_sensor_2]
pause_on_runout: false
switch_pin: !nozzle_mcu:PA10
event_delay: 1.5
pause_delay: 0.25
insert_gcode:
  {% if printer.extruder.temperature < 140 %}
    M118 "Preaquecendo até 140°C"
    SET_HEATER_TEMPERATURE HEATER=extruder TARGET=140
  {% endif %}

  {% if "x" not in printer.toolhead.homed_axes or
        "y" not in printer.toolhead.homed_axes or
        "z" not in printer.toolhead.homed_axes %}
    M118 "Homing necessário - executando G28"
    G28
  {% endif %}
  G1 E15
  SET_PIN PIN=troca_cor VALUE=0  #Solta o botão
  CHAMELEON_PURGE_POSITION
  LOAD_MATERIAL
  CORTARFDESCARGA
#SET_HEATER_TEMPERATURE HEATER=extruder TARGET=0
runout_gcode:
  M118 "filament runout"
  {% if printer.extruder.can_extrude|lower == 'true' %}
    G91
    G1 E-30 F180
    G1 E-50 F2000
    G90
    SET_HEATER_TEMPERATURE HEATER=extruder TARGET=0
  {% endif %}
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
  5º	  Início e carga de T0 (use no início do G-code)
  6º	  Descarrega o filamento atual (use no G-code final)
  7º	  Apenas início (sem movimentar filamento)
  8º	  Avança para o próximo filamento
  9º	  Seleciona filamento aleatório
  ```
## 🤖 Funcionamento Interno
O sistema usa dois sensores de filamento:
Um para detectar a entrada do filamento (sensor traseiro).
Outro na extrusora.

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
