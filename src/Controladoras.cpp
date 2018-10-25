#include <iostream>
#include <sqlite3.h>
#include <vector>
#include "dominios.hpp"
#include "Controladoras.hpp"

/************* SCRIPTS CRIACAO DO BANCO ************/
char *SQL_STMT_CREATE_USUARIO = "CREATE TABLE IF NOT EXISTS 'USUARIO' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT,`LOGIN` TEXT NOT NULL, `NOME` TEXT NOT NULL, `SENHA` TEXT NOT NULL ); ";
char *SQL_STMT_CREATE_ACOMODACAO = "CREATE TABLE IF NOT EXISTS `ACOMODACAO` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `tipo` INTEGER NOT NULL, `capacidade` INTEGER NOT NULL, `cidade` TEXT NOT NULL, `estado` INTEGER NOT NULL, `diaria` REAL NOT NULL );";
char *SQL_STMT_CREATE_CARTAO = "CREATE TABLE IF NOT EXISTS 'CARTAO' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT,`NUMERO` TEXT NOT NULL, `DT_VALIDADE` TEXT NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_CONTACORRENTE = "CREATE TABLE IF NOT EXISTS 'CONTACORRENTE' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT, `NUMERO` TEXT NOT NULL, `AGENDA` INTEGER NOT NULL, `BANCO` INTEGER NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_RESERVA = "CREATE TABLE IF NOT EXISTS `RESERVA` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `id_usuario` INTEGER NOT NULL, `id_acomodacao` INTEGER NOT NULL, `data_incio` TEXT NOT NULL, `data_fim` TEXT NOT NULL );";
/************ SCRIPTS CRIACAO DO BANCO *************/
/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

CtrlIUAut::CtrlIUAut()
{
  identificador = new Identificador();
  senha = new Senha();
}

void CtrlIUAut::setCtrlServAut(IServAut *iServAut)
{
  ctrlServAut = iServAut;
}

bool CtrlIUAut::autenticar()
{
  string id, snh;
  char resp;
  bool resultado = true;
  bool fim = false;

  while (!fim)
  {
    try
    {
      cout << "LOGIN" << endl;
      cout << "identificador:";
      cin >> id;
      identificador->set_identificador(id);
      cout << "Senha:";
      cin >> snh;
      senha->set_senha(snh);

      system("cls"); //windows
      fim = true;

      resultado = ctrlServAut->autenticar(*identificador, *senha);
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        resultado = false;
        fim = true;
      }
    }
  }

  return resultado;
}

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

/*************CONTROLADORA DE SERVICO - AUTENTICACAO********/
bool CtrlServAut::init_banco()
{
  char *errmsg;
  bool resultado = true;
  vector<char *> sql;

  sql.push_back(SQL_STMT_CREATE_USUARIO);
  sql.push_back(SQL_STMT_CREATE_ACOMODACAO);
  sql.push_back(SQL_STMT_CREATE_CARTAO);
  sql.push_back(SQL_STMT_CREATE_CONTACORRENTE);
  sql.push_back(SQL_STMT_CREATE_RESERVA);

  //inicia o banco
  if (sqlite3_open(NOME_BD, &banco) != SQLITE_OK)
  {
    throw sqlite3_errmsg(banco);
  }

  try
  {
    //inicia o banco de dados com as tabelas
    for (int i = 0; i < sql.size(); i++)
    {
      sqlite3_prepare_v2(banco, sql.at(i), -2, &stmt, NULL);

      if (sqlite3_step(stmt) == SQLITE_ERROR)
        throw sqlite3_errmsg(banco);
    }
  }
  catch (char *msg)
  {
    cout << msg << endl;
    sqlite3_free(msg);
    resultado = false;
  }

  sqlite3_free(errmsg);
  sqlite3_close(banco);

  return resultado;
}

bool CtrlServAut::autenticar(Identificador &id, Senha &senha)
{
  bool resultado = false;

  cout << id.get_identificador() << '-' << senha.get_senha() << endl;

  if (CtrlServAut::init_banco())
  {
    cout << "banco inicializado" << endl;

    if (id.get_identificador() == "jopsb" && senha.get_senha() == "Senha123!")
    {
      resultado = true;
    }
  }
  else
  {
    cout << "erro banco" << endl;
  }

  return resultado;
}
/*************CONTROLADORA DE SERVICO - AUTENTICACAO********/
/*************CONTROLADORA DE INTEFACE DE USUARIO - MENU ******/
void CtrlIUMenu::menu()
{
  int resp = 0;

  identificador = new Identificador();
  senha = new Senha();

  while (resp != 5)
  {


    cout << "Sistema de Hospedagem 2.0" << endl;
    cout << "login     - " << LOGIN << endl;
    cout << "Registrar - " << REGISTRAR << endl;
    cout << "Sair      - 5" << endl;
    cout << "Selecione uma opcao";

    cin >> resp;
    system("cls"); //windows

    switch (resp)
    {
    case LOGIN:
      cout << "login" << endl;
      break;
    case REGISTRAR:
      cout << "registrar" << endl;
      break;
    }
  }
}

void CtrlIUMenu::login()
(
 bool fim = false;
 string id,snh;

 cout << "login"<< endl;
 cout << "identificador:";
 cin >> id;

)
/*************CONTROLADORA DE INTEFACE DE USUARIO - MENU ******/
