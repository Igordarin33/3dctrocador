// 28 de novembro de 2024

/* Firmware 3DCameleon Mk4.1 >camaleão turbo 8x

Copyright 2024 William J. Steele


Edited and modified by IGOR Henrique Darin

A permissão é concedida, gratuitamente, a qualquer pessoa que obtenha uma cópia deste software e arquivos de documentação
associados (o “Software”), para lidar com o Software sem restrições, incluindo, sem limitação,
os direitos de usar, copiar, modificar, mesclar, publicar, distribuir, sublicenciar e/ou vender cópias do Software,
e permitir que as pessoas a quem o Software é fornecido o façam, sujeito às seguintes condições:

O aviso de direitos autorais acima e este aviso de permissão devem ser incluídos em todas as cópias ou partes substanciais
do Software.

O SOFTWARE É FORNECIDO "NO ESTADO EM QUE SE ENCONTRA", SEM GARANTIA DE QUALQUER TIPO, EXPRESSA OU IMPLÍCITA, INCLUINDO, MAS NÃO SE LIMITANDO
ÀS GARANTIAS DE COMERCIALIZAÇÃO, ADEQUAÇÃO A UM DETERMINADO FIM E NÃO VIOLAÇÃO. NO CASO,
OS AUTORES OU TITULARES DOS DIREITOS AUTORAIS SERÃO RESPONSÁVEIS POR QUALQUER REIVINDICAÇÃO, DANOS OU OUTRA RESPONSABILIDADE, SEJA EM UMA AÇÃO DE
CONTRATO, ATO ILÍCITO OU DE OUTRA FORMA, DECORRENTE DE, DE OU EM CONEXÃO COM O SOFTWARE OU O USO OU OUTRAS
NEGAÇÕES NO SOFTWARE.

Comandos de Pressionamento de Botão Único (contagem de pulsos do seletor)

    1: "Extrusora T0",
    2: "Extrusora T1",
    3: "Extrusora T2",
    4: "Extrusora T3",
    5: "Extrusora T4",
    6: "Extrusora T5",
    7: "Extrusora T6",
    8: "Extrusora T7",
    9: "Carregar/Inicio T0",
    10: "Descarregar/Inicio",
    11: "Inicio",
    12: "Próximo",
    13: "Aleatório",
    14: "Pulso Extra"
 
*/

#include <SSD1306Ascii.h> //i2C OLED
#include <SSD1306AsciiWire.h> //i2C OLED
#include <SparkFunSX1509.h> // sparkfun i/o expansion board - used for additional filament sensors as well as communications with secondary boards
#include <Wire.h>
#include <SPI.h>
#include <Servo.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from 128x64 pasted image
 */
#pragma once


// Declaração para um display SSD1306 conectado a I2C (pinos SDA, SCL)
#define OLED_RESET  -1 // Pino de reinicialização # (ou -1 se estiver compartilhando o pino de reinicialização do Arduino)
#define OLED_I2C_ADDRESS 0x3C
SSD1306AsciiWire oled;

// define a expansão sparkfun io
const byte SX1509_ADDRESS = 0x3E; // Endereço I2C SX1509
#define SX1509_FILAMENT_0 0
#define SX1509_FILAMENT_1 1
#define SX1509_FILAMENT_2 2
#define SX1509_FILAMENT_3 3
#define SX1509_FILAMENT_4 4
#define SX1509_FILAMENT_5 5
#define SX1509_FILAMENT_6 6
#define SX1509_FILAMENT_7 7
#define SX1509_OUTPUT 8
SX1509 io;                        // Create an SX1509 object to be used throughout


//Os primeiros três (extEnable, extEtep, extDir) são para o motor do extrusor.
#define extEnable 8
#define extStep 2
#define extDir 5
//Os outros três (ext2Enable, ext2Etep, ext2Dir) são para o motor do segundo extrusor.
#define ext2Enable 8
#define ext2Step 4
#define ext2Dir 7
//Os próximos três (selEnable, selStep, selDir) são para o motor do seletor de filamento.
#define selEnable 8
#define selStep 3
#define selDir 6
//Os últimos três (trigger, s_limit, filament) são sensores para monitorar o estado do filamento.
#define trigger A3
#define s_limit A4
#define filament A5
const int counterclockwise = HIGH;
const int clockwise = !counterclockwise;
const int stepsPerRev = 200;
const int microSteps = 16;
const int speedDelay = 100;
const int defaultBackoff = 10;
Servo filamentCutter;  // cria objeto servo para controlar um servo
int cutterPos = 0;    // variável para armazenar a posição do servo
bool reverseServo = true;
int currentExtruder = -1;
int nextExtruder = 0;
int lastExtruder = -1;
int tempExtruder = -1;
int seenCommand = 0;
int prevCommand = 0;
int loaderMode = 2;  //(0= acionamento direto, 1=carregador/descarregador, 2=carregador/descarregador com prensa cortar o filamento)
long triggerTime = 300;
long pulseTime = (triggerTime / 2);
long distance = 10;
long unloadDistance = stepsPerRev * microSteps * distance;  // isto é 10 rotações - cerca de 10"
long loadDistance   = unloadDistance * 1.1;           // isto é 11 rotações - cerca de 11"
int address = 0;
byte value;
long idleCount = 0;
bool logoActive = false;
bool T0Loaded = false;
bool T1Loaded = false;
bool T2Loaded = false;
bool T3Loaded = false;
bool T4Loaded = false;
bool T5Loaded = false;
bool T6Loaded = false;
bool T7Loaded = false;
bool displayEnabled = false;
bool ioEnabled = false;
//int sensorEnabled = 0;
long randomNumber = 0;

void setup()
{
  Wire.begin(); // Inicia o I2C
  Wire.setClock(400000L); // Define clock do I2C

  // Inicializa o expansor IO
  if (io.begin(SX1509_ADDRESS) == true)
  {
    // Configura os 8 pinos de sensores como entrada com pull-up
    for (int i = SX1509_FILAMENT_0; i <= SX1509_FILAMENT_7; i++) {
      io.pinMode(i, INPUT_PULLUP);
    }

    // Configura o pino de saída como OUTPUT
    io.pinMode(SX1509_OUTPUT, OUTPUT);

    ioEnabled = true;
  }
  // enable OLED display
  oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS);

  // wait for it to start up
  delay(50);

  // welcome screen
  oled.setFont(Adafruit5x7);
  oled.clear(); //clear display
	oled.println("");
  oled.println("       Bem vindo"); //print a welcome message  
  oled.println("");
  oled.println("   Camaleao Turbo  "); //print a welcome message
  oled.println("");
  oled.println("         8x ");
  delay(3000);

  displayText(0, "       Pronto!");
  
  seenCommand = 0;

    // Os 3 pino de saida:
  pinMode(extEnable, OUTPUT);
  pinMode(extStep, OUTPUT);
  pinMode(extDir, OUTPUT);

  pinMode(extEnable, OUTPUT);
  pinMode(ext2Step, OUTPUT);
  pinMode(ext2Dir, OUTPUT);

  pinMode(selEnable, OUTPUT);
  pinMode(selStep, OUTPUT);
  pinMode(selDir, OUTPUT);

  // set up the button
  pinMode(trigger, INPUT_PULLUP);  // selector

  // a little override here... we're using the two inputs as I2C instead
  pinMode(s_limit, OUTPUT);    
  pinMode(filament, OUTPUT); 

  // lock the selector by energizing it
  digitalWrite(selEnable, HIGH);

  // make sure filament isn't blocked by gillotine
  connectGillotine();
  cutFilament();
  disconnectGillotine();
  
  prevCommand = 0;

}

int lastLoop = 0;

void loop()
{
  static uint32_t lastTime = 0;

  seenCommand = 0;
  idleCount++;

  // process button press
  if (digitalRead(trigger) == 0)
  {
    idleCount = 0;
    logoActive = false;
    unsigned long nextPulse;
    unsigned long pulseCount = 0;
    unsigned long commandCount = 0;

    // keep counting (and pulsing) until button is released
    while (digitalRead(trigger) == 0)
    {
      if(pulseCount<pulseTime)
      {
        pulseCount++;
        displayCommand(pulseCount);
        if(pulseCount>1) vibrateMotor();
      }
      delay(400);  // each pulse is 400+ milliseconds apart 
    }
    processCommand(pulseCount); // ok... execute whatever command was caught (by pulse count)
    pulseCount = 0;
  }

  // atualiza o bloco IO, duh! Não, sério, pega o estado da expansão gpio sparkfun
  updateIOBlock();

  // cada loop adiciona um atraso de 50 ms, que é adicionado DEPOIS que o comando é processado antes que o próximo possa começar
  delay(50);
}

// read the sparkfun SX1509 io
void updateIOBlock() {
  if(ioEnabled) {
    T0Loaded = io.digitalRead(SX1509_FILAMENT_0);
    T1Loaded = io.digitalRead(SX1509_FILAMENT_1);
    T2Loaded = io.digitalRead(SX1509_FILAMENT_2);
    T3Loaded = io.digitalRead(SX1509_FILAMENT_3);
    T4Loaded = io.digitalRead(SX1509_FILAMENT_4);
    T5Loaded = io.digitalRead(SX1509_FILAMENT_5);
    T6Loaded = io.digitalRead(SX1509_FILAMENT_6);
    T7Loaded = io.digitalRead(SX1509_FILAMENT_7);
  }
}



// exibição do display conforme os pulsos
void displayCommand(long commandCount)
{
  switch(commandCount)
  {
    case 2:
      displayText(25, "     Extrusora T0");
      break;
    case 3:
      displayText(25, "     Extrusora T1");
      break;
    case 4:
      displayText(25, "     Extrusora T2");
      break;
    case 5:
      displayText(25, "     Extrusora T3");
      break;
    case 6:
      displayText(25, "     Extrusora T4");
      break;
    case 7:
      displayText(25, "     Extrusora T5");
      break;
    case 8:
      displayText(25, "     Extrusora T6");
      break;
    case 9:
      displayText(25, "     Extrusora T7");
      break;
    case 10:
      displayText(25, "  Carregar/Inicio T0");
      break;
    case 11:
      displayText(28, "  Descarregar/Inicio");
      break;
    case 12:
      displayText(50, "      Inicio");
      break;  
    case 13:
      displayText(50, "      Proximo");
      break;
    case 14:
      displayText(40, "     Aleatorio");
      break;
    default:
      displayText(30, "     Sem comando");
      break;
  }
}


// execute o comando de contagem de pulsos
void processCommand(long commandCount)
{
  switch (commandCount)
  {
  case 2: // Extrusora T0
    displayText(30, "    T0 Selecionada");
    currentExtruder = 0;
    processMoves();
    displayText(35, "      Idle - T0");
    break;

  case 3: // Extrusora T1
    displayText(30, "    T1 Selecionada");
    currentExtruder = 1;
    processMoves();
    displayText(35, "      Idle - T1");
    break;

  case 4: // Extrusora T2
    displayText(30, "    T2 Selecionada");
    currentExtruder = 2;
    processMoves();
    displayText(35, "      Idle - T2");
    break;

  case 5: // Extrusora T3
    displayText(30, "    T3 Selecionada");
    currentExtruder = 3;
    processMoves();
    displayText(35, "      Idle - T3");
    break;

  case 6: // Extrusora T4
    displayText(30, "    T4 Selecionada");
    currentExtruder = 4;
    processMoves();
    displayText(35, "      Idle - T4");
    break;

  case 7: // Extrusora T5
    displayText(30, "    T5 Selecionada");
    currentExtruder = 5;
    processMoves();
    displayText(35, "      Idle - T5");
    break;

  case 8: // Extrusora T6
    displayText(30, "    T6 Selecionada");
    currentExtruder = 6;
    processMoves();
    displayText(35, "      Idle - T6");
    break;

  case 9: // Extrusora T7
    displayText(30, "    T7 Selecionada");
    currentExtruder = 7;
    processMoves();
    displayText(35, "      Idle - T7");
    break;

  case 10: // Carregar/Inicio T0
    displayText(40, "  Retornando...");
    homeSelector();
    displayText(15, "  Press.Carregar T0");
    gotoExtruder(0, 0);
    if(loaderMode > 0) rotateExtruder(clockwise, loadDistance);
    if(loaderMode > 0) gotoExtruder(0, 1);
    currentExtruder = 0;
    lastExtruder = 0;
    displayText(35, "      Idle - T0");
    break;

  case 11: // Descarregar/Inicio
    displayText(30, "   Cortando...");
    connectGillotine();
    cutFilament();
    switch(lastExtruder)
    {
      case 0:
        displayText(10, " Press.Descarregar T0");
        break;
      case 1:
        displayText(10, " Press.Descarregar T1");
        break;
      case 2:
        displayText(10, " Press.Descarregar T2");
        break;
      case 3:
        displayText(10, " Press.Descarregar T3");
        break;
      case 4:
        displayText(10, " Press.Descarregar T4");
        break;
      case 5:
        displayText(10, " Press.Descarregar T5");
        break;
      case 6:
        displayText(10, " Press.Descarregar T6");
        break;
      case 7:
        displayText(10, " Press.Descarregar T7");
        break;
    }
    if(loaderMode > 0) gotoExtruder((lastExtruder == 7 ? 6 : lastExtruder + 1), lastExtruder);
    if(lastExtruder < 4)
    {
      if(loaderMode > 0) rotateExtruder(counterclockwise, unloadDistance);
    }
    else
    {
      if(loaderMode > 0) rotateExtruder(clockwise, unloadDistance);
    }
    disconnectGillotine();
    displayText(50, "        Idle");
    break;

  case 12: // Inicio
    displayText(40, "  Retornando...");
    homeSelector();
    displayText(50, "      Inicio");
    break;

  case 13: // Proximo
    displayText(30, "   Cortando...");
    connectGillotine();
    cutFilament();
    displayText(30, "Prox.Ferramenta");
    currentExtruder++;
    if(currentExtruder == 8) currentExtruder = 0;
    processMoves();
    displayText(50, "      Proximo");
    break;

  case 14: // Aleatorio
    displayText(30, "   Cortando...");
    connectGillotine();
    cutFilament();
    displayText(30, "  Aleatorio");

    randomNumber = random(0, 8) + 1; // 1 a 4

    for(long i = 0; i < randomNumber; i++)
    {
      currentExtruder++;
      if(currentExtruder == 8) currentExtruder = 0;
    }
    processMoves();
    displayText(50, "     Aleatorio");
    break;

  default:
    displayText(47, "       Clear");
    delay(200);
    displayText(50, "        Idle");
    break;
  }
}


// apenas a rotina para atualizar o OLED
void displayText(int offset, String str)
{
  oled.clear();
  oled.println("");
  oled.println("   Camaleao Turbo  "); // imprime uma mensagem de boas-vindas
  
  oled.println("");
  oled.println("");
  oled.println(str);
  oled.println("");
  oled.println("");
  
  if(ioEnabled)
  {
    oled.print("     ");
    oled.print(T0Loaded ? "0 " : "+");
    oled.print(T1Loaded ? "1" : "+");
    oled.print(T2Loaded ? "2" : "+");
    oled.println(T3Loaded ? "3" : "+");

    oled.print("     ");
    oled.print(T4Loaded ? "4  " : "+  ");
    oled.print(T5Loaded ? "5  " : "+  ");
    oled.print(T6Loaded ? "6  " : "+  ");
    oled.println(T7Loaded ? "7" : "+");
  }
  else
  {
    oled.println("-  -  -  -  -  -  -  -");
    oled.println("-  -  -  -  -  -  -  -");
  }
}


// o trabalho de verdade está aqui
void processMoves()
{
  // certifique-se de que temos uma extrusora real selecionada
  if(lastExtruder > -1)
  {
    // se sim, precisamos cortar o filamento
    displayText(30, "   Cortando...");
    connectGillotine();
    cutFilament();
    // ok... então espere o 2º botão ser pressionado para descarregá-lo
    switch(lastExtruder)
    {
      case 0:
        displayText(10, "Press.Descarregar T0");
        break;
      case 1:
        displayText(10, "Press.Descarregar T1");
        break;
      case 2:
        displayText(10, "Press.Descarregar T2");
        break;
      case 3:
        displayText(10, "Press.Descarregar T3");
        break;
      case 4:
        displayText(10, "Press.Descarregar T4");
        break;
      case 5:
        displayText(10, "Press.Descarregar T5");
        break;
      case 6:
        displayText(10, "Press.Descarregar T6");
        break;
      case 7:
        displayText(10, "Press.Descarregar T7");
        break;
    } 
    // rolar para o primeiro se estiver no último
    if(loaderMode > 0) gotoExtruder((lastExtruder == 7 ? 6 : (lastExtruder + 1)), lastExtruder);
    // isso determina em qual direção mover o motor, 0-1-4-5: sentido anti-horário, 2-3-6-7: sentido horário
    if(lastExtruder == 0 || lastExtruder == 1 || lastExtruder == 4 || lastExtruder == 5)
    {
      if(loaderMode > 0)
      {
        if(lastExtruder < 4) rotateExtruder(counterclockwise, unloadDistance);  // Motor 1 (T0~T3)
        else rotateExtruder2(counterclockwise, unloadDistance);                 // Motor 2 (T4~T7)
      }
    }
    else
    {
      if(loaderMode > 0)
      {
        if(lastExtruder < 4) rotateExtruder(clockwise, unloadDistance);  // Motor 1
        else rotateExtruder2(clockwise, unloadDistance);                 // Motor 2
      }
    }
  }
  else
  {
    lastExtruder = 0;
  }
  disconnectGillotine();
  gotoExtruder(lastExtruder, currentExtruder);
  // ok... filamento descarregado, hora de carregar o novo... então diga ao usuário
  switch(currentExtruder)
  {
    case 0:
      displayText(15, "  Press.Carregar T0");
      break;
    case 1:
      displayText(15, "  Press.Carregar T1");
      break;
    case 2:
      displayText(15, "  Press.Carregar T2");
      break;
    case 3:
      displayText(15, "  Press.Carregar T3");
      break;
    case 4:
      displayText(15, "  Press.Carregar T4");
      break;
    case 5:
      displayText(15, "  Press.Carregar T5");
      break;
    case 6:
      displayText(15, "  Press.Carregar T6");
      break;
    case 7:
      displayText(15, "  Press.Carregar T7");
      break;
  }
  // mesma lógica (mas invertida) para a direção do motor
  if(currentExtruder == 0 || currentExtruder == 1 || currentExtruder == 4 || currentExtruder == 5)
  {
    if(loaderMode > 0)
    {
      if(currentExtruder < 4) rotateExtruder(clockwise, loadDistance);  // Motor 1
      else rotateExtruder2(clockwise, loadDistance);                   // Motor 2
    }
  }
  else
  {
    if(loaderMode > 0)
    {
      if(currentExtruder < 4) rotateExtruder(counterclockwise, loadDistance);  // Motor 1
      else rotateExtruder2(counterclockwise, loadDistance);                   // Motor 2
    }
  }
  // se estamos carregando, então carregue agora
  if(loaderMode > 0) gotoExtruder(currentExtruder, (currentExtruder == 7 ? 6 : (currentExtruder + 1)));
  // todos se lembram onde estacionamos!
  lastExtruder = currentExtruder;
}

// esta função simplesmente move do currentCog para o targetCog é a melhor maneira
void gotoExtruder(int currentCog, int targetCog)
{
  // extrator lógico da posição no seletor (0 a 3)
  int currentPos = currentCog % 4;
  int targetPos = targetCog % 4;

  int newCog = targetPos - currentPos;

  // direção padrão
  int newDirection = counterclockwise;

  if(newCog < 0)
  {
    newDirection = clockwise;
    newCog = -newCog;  // inverter o sinal
  }

  // se já estamos na posição certa, não mova o seletor
  if(newCog > 0)
  {    
    // mover seletor para a nova posição
    for(int i = 0; i < newCog; i++)
    {
      rotateSelector(newDirection, (stepsPerRev / 4) * microSteps);
    }
  }
}


// mova o motor da extrusora em uma direção específica por uma distância específica (a menos que seja um "até que o botão não seja pressionado")
void rotateExtruder(bool direction, long moveDistance)
{
  digitalWrite(extEnable, LOW);  // travar o motor
  digitalWrite(extDir, direction); // Permite que o motor se mova em uma direção específica
  const int fastSpeed = speedDelay/2; // tempo duplo
  if(loaderMode==1)
  {
    for (long x = 0; x < (moveDistance-1); x++)
    {
      digitalWrite(extStep, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(extStep, LOW);
      delayMicroseconds(fastSpeed);
    }
  }
  if(loaderMode==2)
  {
   while (digitalRead(trigger) != 0)
    {
      delay(50);
    }
    while (digitalRead(trigger) == 0)
    {
      digitalWrite(extStep, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(extStep, LOW);
      delayMicroseconds(fastSpeed);
    }
  }
  digitalWrite(extEnable, HIGH);
}

void rotateExtruder2(bool direction, long moveDistance)
{
  digitalWrite(ext2Enable, LOW);  // travar o motor 2
  digitalWrite(ext2Dir, direction); // definir direção motor 2
  const int fastSpeed = speedDelay/2; // tempo duplo
  if(loaderMode==1)
  {
    for (long x = 0; x < (moveDistance-1); x++)
    {
      digitalWrite(ext2Step, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(ext2Step, LOW);
      delayMicroseconds(fastSpeed);
    }
  }
  if(loaderMode==2)
  {
    while (digitalRead(trigger) != 0)
    {
      delay(50);
    }
    while (digitalRead(trigger) == 0)
    {
      digitalWrite(ext2Step, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(ext2Step, LOW);
      delayMicroseconds(fastSpeed);
    }
  }
  digitalWrite(ext2Enable, HIGH);  // desenergizar motor 2
}


// semelhante à extrusora, mas apenas avançando 50 (de 200) por vez
void rotateSelector(bool direction, int moveDistance)
{

  // já que estamos nisso... podemos tornar isso mais rápido usando a mágica que você inventou acima?
  
  digitalWrite(selEnable, LOW); // bloquear o seletor
  digitalWrite(selDir, direction); // Permite que o motor se mova em uma direção específica

    // Faz 50 pulsos para fazer uma rotação de ciclo completo
    for (int x = 0; x < (moveDistance-1); x++)
    {
      digitalWrite(selStep, HIGH);
      delayMicroseconds(speedDelay);
      digitalWrite(selStep, LOW);
      delayMicroseconds(speedDelay);
    }
}

// isso alterna o servo entre duas posições
void cutFilament() {
  digitalWrite(selEnable, LOW); // desabilita o stepper para que tenhamos energia!
  if(reverseServo==false)
  {
    openGillotine();
    closeGillotine();
  }
  else
  {
    closeGillotine();
    openGillotine();
  }
  digitalWrite(selEnable, HIGH);
}

// habilita o servo
void connectGillotine()
{
  filamentCutter.attach(11);
}

// desabilita o servo - para que ele não vibre quando não estiver em uso
void disconnectGillotine()
{
  filamentCutter.detach();
}

// ciclo servo de 135 e 180
void openGillotine()
{
    for (int pos = 135; pos <= 180; pos += 1) { // vai de 0 graus a 180 graus
    // em passos de 1 grau
    filamentCutter.write(pos);              // diz ao servo para ir para a posição na variável 'pos'
    delayMicroseconds(25000);                       // espera 15ms para o servo atingir a posição
  }
  //filamentCutter.write(3.5);       // diz ao servo para ir para a posição na variável 'pos'
  delay(50);                       // espera 15ms para o servo atingir a posição
}

// reverse cycle servo from 180 back to 135
void closeGillotine()
{
  for (int pos = 180; pos >= 135; pos -= 1) { // vai de 180 graus a 0 graus
    filamentCutter.write(pos);              // diz ao servo para ir para a posição na variável 'pos'
    delayMicroseconds(25000);                       // espera 15ms para o servo atingir a posição
  }
  delay(50);                       // espera 15ms para o servo atingir a posição
}

// gire o seletor no sentido horário muito longe de 4, então ele irá ranger no batente
void homeSelector()
{
  // gire no sentido anti-horário até parar bruscamente
  rotateSelector(clockwise, stepsPerRev * microSteps);

  // mova apenas ligeiramente para a extrusora 1 (isso recua um pouco da parada brusca)
  rotateSelector(counterclockwise, defaultBackoff * microSteps);

 currentExtruder = 0;
 lastExtruder = -2;

}

// buzz buzz buzz
void vibrateMotor()
{
  // oscila o seletor 1 vez
  rotateSelector(clockwise, 2 * 16);
  rotateSelector(!clockwise, 2 * 16);
}

