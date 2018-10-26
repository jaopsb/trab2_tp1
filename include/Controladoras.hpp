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

  Identificador *identificador;
  Senha *senha;
  Nome *nome;

  IServMenu *ctrlServMenu;

  void login();
  void registrar();

public:
  void menu();
  void setCtrlServMenu(IServMenu *);
};

/*
Classe Base do Banco de dados
Evitando mudancas desnecessarias ao banco
*/
class CtrlServ
{
private:
  static bool banco_criado;
  char *NOME_BD = "hotel.db";
  sqlite3 *banco;

public:
bool bd_criado(){return banco_criado;}
  char *get_nome_banco() { return NOME_BD; }
  sqlite3 *get_banco() { return banco; }
  bool init_banco();
};

class CtrlServLogin : public IServLogin, CtrlServ
{
private:
  char *NOME_BD = "hotel.db";

  Identificador *identificador;
  Senha *senha;
  sqlite3_stmt *stmt;
  sqlite3 *banco;

  void init_banco();
  void buscarUsuario();

public:
  CtrlServLogin();
  Usuario login(Identificador &, Senha &);
};

class CtrlServMenu : public IServMenu, CtrlServ
{
private:
  Identificador *identificador;
  Senha *senha;

public:
  CtrlServMenu();
  void registrar(Nome &, Identificador &, Senha &);
};

#endif // CONTROLADORAS_
