
#include <Servo.h>
#include <IRremote.h>
#include <LiquidCrystal.h>

Servo miServo;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
#define LEDV 11
#define LEDN 10
#define VERANO 4010852096
#define PRIMAVERA 3994140416
#define OTONIO 3977428736
#define INVIERNO 3944005376


String obtenerEstacionNombre(int teclaValor);
bool detectarIncendio(String estacion, int temperatura);
void cambiarEstadoLed(int pinLed, int intensidad);

int estacionSeleccionda;
int obtenerEstacionSegunTemp;
char primerFilaLcd[16];
char segundaFilaLcd[16];

void setup()
{
  miServo.attach(9);
  lcd.begin(16,2);
  Serial.begin(9600);
  IrReceiver.begin(8);
  miServo.write(0);
}

void loop()
{		
 	lcd.clear();
  
  	int leerPinTemp = analogRead(A0);
  	int temperaturaLeida = map(leerPinTemp, 20, 358, 0, 100);
  
  /*
  IRreceiver se utiliza para recibir las señales enviadas por un control remoto IR. 
  Las señales recibidas se decodifican para identificar los comandos enviados 
  por el control remoto
  Se utiliza la funcion decode() para verificar si se ha recibido una señal IR.
  */
    if (IrReceiver.decode()) 
    {    
      
      switch(IrReceiver.decodedIRData.decodedRawData)//El valor del boton presionado
      {
        case VERANO:
        estacionSeleccionda = 1;
        break;
        case PRIMAVERA:
        estacionSeleccionda = 2;
        break;
        case OTONIO:
        estacionSeleccionda = 3;
        break;
        case INVIERNO:
        estacionSeleccionda = 4;
        break;
      }
      IrReceiver.resume(); // resume() para habilitar la recepción del siguiente valor IR
    }
  
  	String guardarNombreEstacion = obtenerEstacionNombre(estacionSeleccionda);
 
  	if(detectarIncendio(guardarNombreEstacion, temperaturaLeida))
    {
      	cambiarEstadoLed(LEDV, 0);
      	cambiarEstadoLed(LEDN, 255);
      	lcd.setCursor(0,1);
      	sprintf(segundaFilaLcd, "INCENDIO");
      	lcd.print(segundaFilaLcd);
     	miServo.write(180);
    }
  	else
    {
    	cambiarEstadoLed(LEDN, 0);
      	cambiarEstadoLed(LEDV, 255);
      	miServo.write(0);
    }
  	
    
    if(estacionSeleccionda >= 1 && estacionSeleccionda <= 4)
    {
      lcd.setCursor(0,0);
      lcd.print(guardarNombreEstacion);
      sprintf(primerFilaLcd, "%d-C", temperaturaLeida);
      lcd.setCursor(11,0);
      lcd.print(primerFilaLcd);
    }
    else
    {
        sprintf(primerFilaLcd,"Selec Estacion");
      	sprintf(segundaFilaLcd,"1 - 2 - 3 - 4");
      	lcd.print(primerFilaLcd);
      	lcd.setCursor(0,1);
      	lcd.print(segundaFilaLcd);
    }   
  delay(1000);  
}

String obtenerEstacionNombre(int teclaValor) 
{
  switch(teclaValor) 
  {
    case 1:
      return "Verano";
    case 2:
      return "Primavera";
    case 3:
      return "Otonio";
    case 4:
      return "Invierno";
  }
}


bool detectarIncendio(String estacion, int temperatura)
{
	
  if(estacion == "Verano" && temperatura >= 45)
  {
	return true;
  }
  else
  {
	if(estacion == "Primavera" && temperatura >= 35)
    {
		return true;
    }
    else
    {
		if(estacion == "Otonio" && temperatura >= 25)
        {
			return true;
        }
      	else
      	{
			if(estacion == "Invierno" && temperatura >= 15)
            {
				return true;
            }	
      	}
    }
  }
  return false;
}

void cambiarEstadoLed(int pinLed, int intensidad)
{
	analogWrite(pinLed, intensidad);
}
