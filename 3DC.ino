// 28 de novembro de 2024

/* Firmware 3DCameleon Mk4.1

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

#1 - 1
#2 - 2
#3 - 3
#4 - 4
#5 - Home e Carregar
#6 - Descarregar Último e Home
#7 - Home
#8 - Próximo Filamento
#9 - Filamento Aleatório
 
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

const unsigned char bitmap_logo[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00110000,B00001100,B00100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111100,B00001110,B00110000,B00011100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B00011111,B01111000,B00011111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B10011111,B11111000,B00011111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B11011111,B11110110,B00011111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B10000000,B00000110,B00011111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B11111100,B01111101,B11111111,B11110010,B00111111,B11110000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B11111111,B00001111,B11111111,B11111110,B00111111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111111,B00111111,B11111111,B11111111,B00111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111110,B00111111,B11111111,B11111111,B01111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111101,B11111111,B11111111,B11111111,B01111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B11111111,B11111110,B11111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01100111,B11101111,B11111111,B11111111,B11111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000111,B10001111,B11110111,B11111111,B11111111,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001111,B10111111,B11111011,B11111111,B11111101,B11111110,B00011111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001111,B10111111,B11111001,B11111111,B11111111,B11111100,B00001111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001111,B01111111,B11111101,B11111111,B11111111,B11111001,B11110111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000111,B11111111,B11111110,B11111111,B11111111,B11111011,B11110011,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000010,B11111111,B11111110,B11111111,B11111111,B11111011,B10111011,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00111100,B11111111,B11111111,B01111111,B11111111,B11111011,B11110011,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111101,B11111111,B11111111,B01111111,B11111111,B11111001,B11110111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111101,B11111111,B11111111,B10111111,B11111101,B11111100,B11101111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00111101,B11111111,B11111111,B10000000,B00000000,B11111111,B11011111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011001,B11111111,B11111111,B10000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001011,B11111111,B11111111,B01100000,B00000000,B01111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000001,B11111111,B11111110,B11111000,B00000000,B00111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011010,B01111111,B11111101,B11111110,B00000000,B00001111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11100111,B11111011,B11111110,B00000000,B00000111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B11111011,B11111100,B01110000,B01111111,B00000000,B00000001,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B00000000,B01111111,B10000000,B00000000,B01111100,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011011,B11111111,B11000000,B11111111,B11111000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001011,B11111111,B11100001,B11111111,B11111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00111011,B11111111,B11100001,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B11100011,B11111111,B11110011,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B11111011,B11111111,B11100000,B00000001,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B01111011,B11111111,B11100000,B00000000,B01111110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000011,B11111111,B11100000,B00000000,B00011110,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000101,B11111111,B11100000,B00000000,B00000010,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00001101,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011111,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00011110,B11111111,B10000000,B00000111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000001,B11111111,B01110000,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000011,B01111111,B11111111,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000011,B11101111,B11111111,B11111111,B11100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000111,B10011111,B11111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00011111,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01011111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01101111,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B01110111,B11111111,B10000000,B00000000,B01100000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000011,B11111111,B11111111,B11111111,B11000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00001110,B11111111,B11111111,B11111111,B10000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00001110,B01111111,B11111111,B11111111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00001100,B11011111,B11111111,B11111100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B11011001,B11111111,B11110000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00011001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000001,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};


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
#define SX1509_OUTPUT 4
SX1509 io;                        // Create an SX1509 object to be used throughout

/* original:
// define números de pinos - 3D Chameleon Board
#define extEnable 0
#define extStep 1
#define extDir 2

#define selEnable A0
#define selStep A1
#define selDir A2

#define trigger A3
#define s_limit A4
#define filament A5
*/

//CNC Shield v3:
// Settings on Shield
// A4988 in base X (ext) 
// A4988 in base Y (sel) 
// X.STEP /DR (ext)
// Y.STEP /DR (sel)
// END STOPS Z+ (clippy)
// CoolEn (trigger)
// DA (s_limit)
// CL (filament)



//Os primeiros três (extEnable, extEtep, extDir) são para o motor do extrusor.
#define extEnable 8
#define extStep 2
#define extDir 5
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

// antes era 16 passo, coloquei 1 de teste, testar com os 2
const int microSteps = 1;
const int speedDelay = 450;

//450 funcionou X no primeiro, e Y no terceiro
// 150 não funcionou x terceiro e y terceiro

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


bool displayEnabled = false;
bool ioEnabled = false;
//int sensorEnabled = 0;

long randomNumber = 0;

void setup()
{

  Wire.begin(); //start i2C  
	Wire.setClock(400000L); //set clock

  // set up IO expander
  if (io.begin(SX1509_ADDRESS) == true)
  {
    io.pinMode(SX1509_FILAMENT_0, INPUT_PULLUP);
    io.pinMode(SX1509_FILAMENT_1, INPUT_PULLUP);
    io.pinMode(SX1509_FILAMENT_2, INPUT_PULLUP);
    io.pinMode(SX1509_FILAMENT_3, INPUT_PULLUP);
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
  oled.println("    3DChameleon    "); //print a welcome message
  oled.println("");
  oled.println("         Pro");
  delay(3000);

  displayText(0, "       Pronto!");
  
  seenCommand = 0;

  // Sets the two pins as Outputs
  pinMode(extEnable, OUTPUT);
  pinMode(extStep, OUTPUT);
  pinMode(extDir, OUTPUT);

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
void updateIOBlock()
{
    if(ioEnabled)
    {
      T0Loaded = io.digitalRead(SX1509_FILAMENT_0);
      T1Loaded = io.digitalRead(SX1509_FILAMENT_1);
      T2Loaded = io.digitalRead(SX1509_FILAMENT_2);
      T3Loaded = io.digitalRead(SX1509_FILAMENT_3);
    }
}

// display command for each button pulse
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
    displayText(25, "  Carregar/Inicio T0");
    break;
  case 7:
    displayText(28, "  Descarregar/Inicio");
    break;
  case 8:
    displayText(50, "      Inicio");
    break;  
  case 9:
    displayText(50, "      Proximo");
    break;
  case 10:
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

  // seleção para comandos
  switch (commandCount)
  {
  case 2: // descarregar filamento, mudar para #0, carregar
    displayText(30, "    T0 Selecionada");
    currentExtruder = 0;
    processMoves();
    displayText(35, "      Idle - T0");
    break;

  case 3: // descarregar filamento, mudar para #1, carregar
    displayText(30, "    T1 Selecionada");
    currentExtruder = 1;
    processMoves();
    displayText(35, "      Idle - T1");
    break;

  case 4: // descarregar filamento, mudar para #3, carregar
    displayText(30, "    T2 Selecionada");
    currentExtruder = 2;
    processMoves();
    displayText(35, "      Idle - T2");
    break;

  case 5: // descarregar filamento, mudar para #4, carregar
    displayText(30, "    T3 Selecionada");
    currentExtruder = 3;
    processMoves();
    displayText(35, "      Idle - T3");
    break;

  case 6: //início e carregar #1
    displayText(40, "  Retornando...");
    homeSelector();
    displayText(15, "  Press.Carregar T0");
    gotoExtruder(0, 0);
    if(loaderMode>0)rotateExtruder(clockwise, loadDistance);
    if(loaderMode>0)gotoExtruder(0, 1);
    currentExtruder = 0;
    lastExtruder = 0;
    displayText(35, "      Idle - T0");
    break;
    
  case 7: // descarregar filamento e reposicionar seletor
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
    } 
    if(loaderMode>0)gotoExtruder((lastExtruder==3?2:lastExtruder+1),lastExtruder);
    if(lastExtruder<2)
    {
      if(loaderMode>0)rotateExtruder(counterclockwise, unloadDistance);
    }
    else
    {
      if(loaderMode>0)rotateExtruder(clockwise, unloadDistance);
    }
    disconnectGillotine();
    displayText(50, "        Idle");
    break;

  case 8:  
    displayText(40, "  Retornando...");
    homeSelector();
    displayText(50, "        Idle");
    break;

  case 9: // move to next available filament
    displayText(30, "   Cortando...");
    connectGillotine();
    cutFilament();
    displayText(30, "Prox.Ferramenta");
    currentExtruder++;
    if(currentExtruder==4)currentExtruder=0;
    processMoves();
    displayText(35, "        Idle");
    break;

  case 10: // mover para um filamento aleatório
    displayText(30, "   Cortando...");
    connectGillotine();
    cutFilament();
    displayText(30, "  Aleatorio");

    // selecione um número aleatório
    randomNumber = random(0,2) + 1;

    // pule adiante muitas ferramentas
    for(long i=0; i<randomNumber; i++)
    {
      currentExtruder++;
      if(currentExtruder==4)currentExtruder=0;
    }
    processMoves();
    displayText(50, "        Idle");
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

  //if(displayEnabled){
    oled.clear();
    oled.println("");
    oled.println("     3DChameleon   "); //imprime uma mensagem de boas-vindas
    oled.println("");
    oled.println("");
    oled.println(str);
    oled.println("");
    oled.println("");
    if(ioEnabled)
    {
      oled.print("     ");
      if(T0Loaded)
      {
        oled.print("0  ");
      }
      else
      {
        oled.print("+  ");
      }
      if(T1Loaded)
      {
        oled.print("1  ");
      }
      else
      {
        oled.print("+  ");
      }
      if(T2Loaded)
      {
        oled.print("2  ");
      }
      else
      {
        oled.print("+  ");
      }
      if(T3Loaded)
      {
        oled.println("3");
      }
      else
      {
        oled.println("+");
      }
    }
    else
    {
      oled.println("      -  -  -  -");
    }
  //  }
}

// o trabalho de verdade está aqui
void processMoves()
{

  // certifique-se de que temos uma extrusora real selecionada
  if(lastExtruder>-1)
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
    } 

    // rolar para o primeiro se estiver no último
    if( loaderMode>0 ) gotoExtruder( ( lastExtruder==3 ? 2 : (lastExtruder+1)), lastExtruder);

    // isso determina em qual direção mover o motor, 0-1: sentido anti-horário, 2-3: sentido horário
    if(lastExtruder<2)
    {
      if(loaderMode>0)rotateExtruder(counterclockwise, unloadDistance);
    }
    else
    {
      if(loaderMode>0)rotateExtruder(clockwise, unloadDistance);
    }
  }
  else
  {
    lastExtruder = 0;
  }
  disconnectGillotine();
  
  // diga para realmente executar esse comando agora
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
  }

  // mesma lógica (mas invertida) para a direção do motor
  if(currentExtruder<2)
  {
    if(loaderMode>0)rotateExtruder(clockwise, loadDistance);
  }
  else
  {
    if(loaderMode>0)rotateExtruder(counterclockwise, loadDistance);
  }

  // se estamos carregando, então carregue agora
  if(loaderMode>0)gotoExtruder(currentExtruder, (currentExtruder==3?2:currentExtruder+1));

  // todos se lembram onde estacionamos!
  lastExtruder = currentExtruder;
}


// esta função simplesmente move do currentCog para o targetCog é a melhor maneira
void gotoExtruder(int currentCog, int targetCog)
{
 
  int newCog = targetCog - currentCog;

  // ok... qual caminho
  int newDirection = counterclockwise;
  if(newCog<0)
  {
   // precisamos ir para o outro lado
    newDirection = clockwise;

    //e já que sabemos que fomos longe demais...vamos fazer o caminho inverso também, passo a passo
    newCog = currentCog - targetCog;
  }

  // se já estamos na engrenagem atual, então não faça nada
  if(newCog > 0)
  {    
    // ferramenta de avanço targetCog times
    for(int i=0; i<newCog; i++)
    {
      rotateSelector(newDirection, (stepsPerRev / 4) * microSteps);
    }
  }
}

// mova o motor da extrusora em uma direção específica por uma distância específica (a menos que seja um "até que o botão não seja pressionado")
void rotateExtruder(bool direction, long moveDistance)
{
  // nota para o Bill: acelere isso para que seja bem rápido!!!
  
  digitalWrite(extEnable, LOW);  // travar o motor
  digitalWrite(extDir, direction); // Permite que o motor se mova em uma direção específica
  const int fastSpeed = speedDelay/2; // tempo duplo

  // isso está obsoleto no momento... talvez volte no futuro
  if(loaderMode==1)
  {

    // Faz 50 pulsos para fazer uma rotação de ciclo completo
    for (long x = 0; x < (moveDistance-1); x++)
    {
      // é assim que pulsamos o motor para fazê-lo dar um passo
      digitalWrite(extStep, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(extStep, LOW);
      delayMicroseconds(fastSpeed);
    }

  }

  if(loaderMode==2)
  {

  // continue esperando até que o botão seja pressionado
   while (digitalRead(trigger) != 0)
    {
      delay(50);
    }
    
   // Mover enquanto o botão estiver pressionado
    while (digitalRead(trigger) == 0)
    {

      // é assim que pulsamos o motor para fazê-lo dar um passo
      digitalWrite(extStep, HIGH);
      delayMicroseconds(fastSpeed);
      digitalWrite(extStep, LOW);
      delayMicroseconds(fastSpeed);
    }
  }
  // ok, terminei de pressionar o botão, então certifique-se de que não estamos energizados (alto é não, baixo é sim)
  digitalWrite(extEnable, HIGH);
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

