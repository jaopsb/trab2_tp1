#ifndef CONTROLADORAS_H
#define CONTROLADORAS_H
#include "Interfaces.hpp"
#include "dominios.hpp"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

using namespace std;

class CtrlIUAut : public IUAut
{
private:
  const static int LOGIN = 1;
  const static int REGISTRAR = 2;

  IServAut *ctrlServAut;
  Identificador *identificador;
  Senha *senha;

  void login();
  void registar();
  void menu();

public:
  CtrlIUAut();
  bool autenticar();
  void setCtrlServAut(IServAut *);
};

class CtrlServAut : public IServAut
{
private:
  char *NOME_BD = "hotel.db";

  Identificador *identificador;
  Senha *senha;
  sqlite3_stmt *stmt;
  sqlite3 *banco;

  bool init_banco();

public:
  bool autenticar(Identificador &, Senha &);
};

class CtrlIUMenu
{
private:
    const static int LOGIN = 1;
    const static int REGISTRAR = 2;
    Identificador* identificador;
    Senha* senha;
    void login();
    void registrar();

public:
    void menu();
};

#endif // CONTROLADORAS_H
