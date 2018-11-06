#ifndef CONTROLADORAS_H
#define CONTROLADORAS_H
#include "Interfaces.hpp"
#include "dominios.hpp"
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
  void finaliza();
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
  Cartao_de_Credito *buscar_cartao(string);
  void cadastrarUsuario(string, string, string);
  bool existeUsuario(string);
  void deletarUsuario(string, string);
};

class CtrlIUUsu
{
private:
  Identificador *identificador;
  Senha *senha;
  CtrlServUsu *ctrl;
  void cadastrar();
  void deletar();
  void editar();

public:
  CtrlIUUsu(string, string);
  const static int REGISTRAR = 1;
  const static int DEL_USU = 2;
  const static int EDIT_USU = 3;

  void executa();

  void setCtrlServUsu(CtrlServUsu *);
};

#endif // CONTROLADORAS_
