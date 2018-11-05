#ifndef CONTROLADORAS_H
#define CONTROLADORAS_H
#include "Interfaces.hpp"
#include "dominios.hpp"
#include "Comandos.h"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

using namespace std;

/*
Classe Base do Banco de dados
Evitando mudancas desnecessarias ao banco
*/
class CtrlServ
{
private:
  static bool banco_criado;
  char *NOME_BD = "hotel.db";
  sqlite3_stmt *stmt;
  sqlite3 *banco;

public:
  bool bd_criado() { return banco_criado; }
  char *get_nome_banco() { return NOME_BD; }
  sqlite3 *get_banco() { return banco; }
  sqlite3_stmt *get_stmt() { return stmt; }
  int executa(string);
  bool init_banco();
};

class CtrlIUAut : public IUAut
{
private:
  IServAut *ctrlServAut;
  Identificador *identificador;
  Senha *senha;
  RetornoLogin retorno;

public:
  CtrlIUAut();
  RetornoLogin autenticar();
  void setCtrlServAut(IServAut *);
};

class CtrlServAut : public IServAut, public CtrlServ
{
private:
  char *NOME_BD = "hotel.db";

  Identificador *identificador;
  Senha *senha;
  sqlite3_stmt *stmt;
  sqlite3 *banco;

  bool init_banco();

public:
  Usuario *autenticar(Identificador &, Senha &);
};

class CtrlServUsu : public CtrlServ
{
private:
public:
  Cartao_de_Credito *buscar_cartao(string *);
  void buscarUsuario();
};

class CtrlIUUsu
{
private:
  Identificador *identificador;
  Senha *senha;
  CtrlServUsu *ctrl;
  ICmd *cmd;
  void editarUsuario();

public:
  CtrlIUUsu(string, string);
  const static int LOGIN = 1;
  const static int REGISTRAR = 2;
  const static int EDIT_USU = 3;
  const static int CRIA_ACOM = 4;
  const static int RESERVA = 5;
  const static int EDIT_ACOM = 6;

  void executa();

  void setCtrlServUsu(CtrlServUsu *);
};

#endif // CONTROLADORAS_
