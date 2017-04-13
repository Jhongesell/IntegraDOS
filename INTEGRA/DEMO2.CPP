#include "C:\INTEGRA\SYS\INCLUDE\DEF_USER.HPP"

extern "C" {
  #include <math.h>
}


extern double inv(const double x);
extern double sig(const double x);


void Definicion_usuario::Inicializa(void)
{

  Proyecto = "DEMO.D";

  Numero_sistemas = 4;


  Nombre_sistema[0] = "OSCILADOR";
  Nombre_sistema[1] = "PENDULO SIN FRICCION";
  Nombre_sistema[2] = "PENDULO CON FRICCION";
  Nombre_sistema[3] = "FITZHUGH-NAGUMO";

  Nombre_ecuacion[0][0]="x'=y";
  Nombre_ecuacion[0][1]="y'=-a*x";
  Nombre_ecuacion[1][0]="x'=y";
  Nombre_ecuacion[1][1]="y'=-c*sin(x)";
  Nombre_ecuacion[2][0]="x'=y";
  Nombre_ecuacion[2][1]="y'=-k*y-c*sin(x)";
  Nombre_ecuacion[3][0]="x'=-x*(x-1)*(x-a)-y+I";
  Nombre_ecuacion[3][1]="y'=b*(x-g*y)";

  Numero_ecuaciones[0] = 2;
  Numero_parametros[0] = 1;
  Nombre_parametro[0][0]="a";

  Numero_ecuaciones[1] = 2;
  Numero_parametros[1] = 1;
  Nombre_parametro[1][0]="c";

  Numero_ecuaciones[2] = 2;
  Numero_parametros[2] = 2;
  Nombre_parametro[2][0]="k";
  Nombre_parametro[2][1]="c";

  Numero_ecuaciones[3] = 2;
  Numero_parametros[3] = 4;
  Nombre_parametro[3][0]="a";
  Nombre_parametro[3][1]="I";
  Nombre_parametro[3][2]="b";
  Nombre_parametro[3][3]="g";



  funcion[0][0] = &Definicion_usuario::Fn0000;
  funcion[0][1] = &Definicion_usuario::Fn0001;
  funcion[1][0] = &Definicion_usuario::Fn0100;
  funcion[1][1] = &Definicion_usuario::Fn0101;
  funcion[2][0] = &Definicion_usuario::Fn0200;
  funcion[2][1] = &Definicion_usuario::Fn0201;
  funcion[3][0] = &Definicion_usuario::Fn0300;
  funcion[3][1] = &Definicion_usuario::Fn0301;


  Numero_funciones_auxiliares[0] = 0;
  Numero_funciones_auxiliares[1] = 0;
  Numero_funciones_auxiliares[2] = 0;
  Numero_funciones_auxiliares[3] = 0;


  Parametros(99,99);
}


void Definicion_usuario::Parametros(const unsigned int i, const unsigned int tp)
{
  if(tp == 0 || tp == 99) {
     if(i == 0 || i == 99) _P[0][0][0]=1;
     if(i == 1 || i == 99) _P[0][1][0]=1;
     if(i == 2 || i == 99) _P[0][2][0]=0.5;
     if(i == 2 || i == 99) _P[0][2][1]=1.0;
     if(i == 3 || i == 99) _P[0][3][0]=0.14;
     if(i == 3 || i == 99) _P[0][3][1]=0.09;
     if(i == 3 || i == 99) _P[0][3][2]=0.0054;
     if(i == 3 || i == 99) _P[0][3][3]=3.0;
  }
  if(tp == 1 || tp == 99) {
     if(i == 0 || i == 99) _P[1][0][0]=1;
     if(i == 1 || i == 99) _P[1][1][0]=1;
     if(i == 2 || i == 99) _P[1][2][0]=0.5;
     if(i == 2 || i == 99) _P[1][2][1]=1.0;
     if(i == 3 || i == 99) _P[1][3][0]=0.14;
     if(i == 3 || i == 99) _P[1][3][1]=0.09;
     if(i == 3 || i == 99) _P[1][3][2]=0.0054;
     if(i == 3 || i == 99) _P[1][3][3]=3.0;
  }
}


void Definicion_usuario::Configuracion(void)
{
  Config[0][0][0]=-10.000000;
  Config[0][0][1]=-10.000000;
  Config[0][0][2]=-10.000000;
  Config[0][0][3]=10.000000;
  Config[0][0][4]=10.000000;
  Config[0][0][5]=10.000000;
  Config[0][1][0]=-10.000000;
  Config[0][1][1]=-10.000000;
  Config[0][1][2]=-10.000000;
  Config[0][1][3]=10.000000;
  Config[0][1][4]=10.000000;
  Config[0][1][5]=10.000000;
  Config[0][2][0]=-10.000000;
  Config[0][2][1]=-10.000000;
  Config[0][2][2]=-10.000000;
  Config[0][2][3]=10.000000;
  Config[0][2][4]=10.000000;
  Config[0][2][5]=10.000000;
  Config[0][3][0]=-1.000000;
  Config[0][3][1]=-0.300000;
  Config[0][3][2]=-10.000000;
  Config[0][3][3]=1.000000;
  Config[0][3][4]=0.300000;
  Config[0][3][5]=10.000000;

  Config[1][0][0]=-100.000000;
  Config[1][0][1]=-10.000000;
  Config[1][0][2]=-10.000000;
  Config[1][0][3]=100.000000;
  Config[1][0][4]=10.000000;
  Config[1][0][5]=10.000000;
  Config[1][1][0]=-10.000000;
  Config[1][1][1]=-10.000000;
  Config[1][1][2]=-10.000000;
  Config[1][1][3]=10.000000;
  Config[1][1][4]=10.000000;
  Config[1][1][5]=10.000000;
  Config[1][2][0]=-10.000000;
  Config[1][2][1]=-10.000000;
  Config[1][2][2]=-10.000000;
  Config[1][2][3]=10.000000;
  Config[1][2][4]=10.000000;
  Config[1][2][5]=10.000000;
  Config[1][3][0]=-1.000000;
  Config[1][3][1]=-0.300000;
  Config[1][3][2]=-10.000000;
  Config[1][3][3]=1.000000;
  Config[1][3][4]=0.300000;
  Config[1][3][5]=10.000000;


  for(int i = 0; i < Numero_sistemas ; i++) {
       // Escenarios
       Escenario[0][i].x = 1, Escenario[0][i].y = 2;
       if(Numero_ecuaciones[i] >= 3) Escenario[0][i].z = 3;
          else Escenario[0][i].z = 0;

       Escenario[1][i].x = Numero_ecuaciones[i]+1, Escenario[1][i].y = 1;
       Escenario[1][i].z = 0;
  }
}

double Definicion_usuario::Fn0000(void)
{
  return ( _X[1] );
}

double Definicion_usuario::Fn0001(void)
{
  return ( -_P[P_a][0][0]*_X[0] );
}

double Definicion_usuario::Fn0100(void)
{
  return ( _X[1] );
}

double Definicion_usuario::Fn0101(void)
{
  return ( -_P[P_a][1][0]*sin(_X[0]) );
}

double Definicion_usuario::Fn0200(void)
{
  return ( _X[1] );
}

double Definicion_usuario::Fn0201(void)
{
  return ( -_P[P_a][2][0]*_X[1]-_P[P_a][2][1]*sin(_X[0]) );
}

double Definicion_usuario::Fn0300(void)
{
  return ( -_X[0]*(_X[0]-1)*(_X[0]-_P[P_a][3][0])-_X[1]+_P[P_a][3][1] );
}

double Definicion_usuario::Fn0301(void)
{
  return ( _P[P_a][3][2]*(_X[0]-_P[P_a][3][3]*_X[1]) );
}
