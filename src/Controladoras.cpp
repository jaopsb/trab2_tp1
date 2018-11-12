#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <cstring>
#include <typeinfo>
#include "dominios.hpp"
#include "Entidades.hpp"
#include "Controladoras.hpp"

/************* SCRIPTS CRIACAO DO BANCO ************/
char *SQL_STMT_CREATE_USUARIO = "CREATE TABLE IF NOT EXISTS `USUARIO` ( `ID` INTEGER, `IDENTIFICADOR` TEXT NOT NULL UNIQUE, `NOME` TEXT NOT NULL, `SENHA` TEXT NOT NULL, PRIMARY KEY(`IDENTIFICADOR`,`ID`));";
char *SQL_STMT_CREATE_ACOMODACAO = "CREATE TABLE IF NOT EXISTS `ACOMODACAO` (`id`	INTEGER,`tipo`	INTEGER NOT NULL,`capacidade`	INTEGER NOT NULL,`cidade`	TEXT NOT NULL,`estado`	INTEGER NOT NULL,`diaria`	REAL NOT NULL,`dono`	TEXT,PRIMARY KEY(`dono`));";
char *SQL_STMT_CREATE_CARTAO = "CREATE TABLE IF NOT EXISTS 'CARTAO' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT,`NUMERO` TEXT NOT NULL, `DT_VALIDADE` TEXT NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_CONTACORRENTE = "CREATE TABLE IF NOT EXISTS 'CONTACORRENTE' ( `ID` INTEGER PRIMARY KEY AUTOINCREMENT, `NUMERO` TEXT NOT NULL, `AGENDA` INTEGER NOT NULL, `BANCO` INTEGER NOT NULL, `ID_USUARIO` INTEGER NOT NULL );";
char *SQL_STMT_CREATE_RESERVA = "CREATE TABLE IF NOT EXISTS `RESERVA` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `id_usuario` INTEGER NOT NULL, `id_acomodacao` INTEGER NOT NULL, `data_incio` TEXT NOT NULL, `data_fim` TEXT NOT NULL );";
/************ SCRIPTS CRIACAO DO BANCO *************/

/***************FUNCOES AUXILIARES************/
bool trata_retorno(int rc)
{
  switch (rc)
  {
  case SQLITE_DONE:
  case SQLITE_ROW:
  case SQLITE_OK:
    return true;
    break;
  case SQLITE_CONSTRAINT:
    throw invalid_argument("Identificador ja utilizado!");
    break;
  case SQLITE_ERROR:
    throw invalid_argument("Erro no servi�o!");
    break;

  default:

    throw invalid_argument(to_string(rc));

    break;
  }
}

string formata_senha(string senha)
{
  string nSenha;
  for (int i = 0; i < senha.length(); i++)
  {
    nSenha += '*';
  }
  return nSenha;
}

/***************FUNCOES AUXILIARES************/

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

RetornoLogin CtrlIUAut::autenticar()
{
  string id, snh;
  char resp;
  bool resultado = true;
  bool fim = false;

  while (!fim)
  {
    try
    {
      cout << "identificador:";
      cin >> id;
      identificador->set_identificador(id);
      cout << "Senha:";
      cin >> snh;
      senha->set_senha(snh);
      system("cls"); //windows

      //TODO retornar o usuario
      Usuario *usu = ctrlServAut->autenticar(*identificador, *senha);
      retorno.set_usuario(usu);
      fim = true;
    }
    catch (const exception &ia)
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

  retorno.set_resultado(resultado);

  return retorno;
}

/*************CONTROLADORA DE INTERFACE DE USUARIO - AUTENTICACAO********/

/************* CONTROLADORA DE SERVICOS *************/

int CtrlServ::executa(string sql)
{
  int rc;
  sqlite3 *banco = CtrlServ::get_banco();

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  sqlite3_prepare_v2(banco, sql.c_str(), -2, &stmt, NULL);

  rc = sqlite3_step(stmt);

  sqlite3_close(banco);
  return rc;
}

void CtrlServ::finaliza()
{
  sqlite3_finalize(stmt);
  sqlite3_close(banco);
}

/************* CONTROLADORA DE SERVICOS *************/

/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/
void CtrlIUUsu::setCtrlServUsu(CtrlServUsu *serv)
{
  ctrl = serv;
}

void CtrlIUUsu::executa()
{
  ctrl = new CtrlServUsu();

  int opt = 0;
  while (opt != 5)
  {
    system("cls");
    cout << "Painel Usuario - " << identificador->get_identificador() << endl;
    cout << "Cadastrar Usuario         - " << CtrlIUUsu::REGISTRAR << endl;
    cout << "Remover Usuario           - " << CtrlIUUsu::DEL_USU << endl;
    cout << "Editar dados              - " << CtrlIUUsu::EDIT_USU << endl;
    cout << "Cadastrar Conta corrente  - " << CtrlIUUsu::REG_CONTAC << endl;
    cout << "Sair                      - 5" << endl;
    cout << "Selecione a opcao: ";
    cin >> opt;

    switch (opt)
    {
    case CtrlIUUsu::DEL_USU:
      CtrlIUUsu::deletar();
      break;
    case CtrlIUUsu::REGISTRAR:
      CtrlIUUsu::cadastrar();
      break;
    case CtrlIUUsu::EDIT_USU:
      CtrlIUUsu::editar();
      break;
    case CtrlIUUsu::REG_CONTAC:
      CtrlIUUsu::cadastrarCC();
      break;
    default:
      break;
    }
  }
}

CtrlIUUsu::CtrlIUUsu(string id, string sn)
{
  identificador = new Identificador();
  identificador->set_identificador(id);
  senha = new Senha();
  senha->set_senha(sn);
}

void CtrlIUUsu::cadastrarCC()
{
  bool fim = false;
  char resp;
  string nconta;
  int nagencia, nbanco;
  Conta_corrente *conta;
  Numero_Conta_Corrente num_conta;
  Agencia agencia;
  Banco banco;

  while (!fim)
  {
    try
    {
      cout << "Cadastrar Conta Corrente" << endl;
      cout << "Numero da conta:";
      cin >> nconta;
      num_conta.set_numero_conta_corrente(nconta);

      cout << "Numero da agencia:";
      cin >> nagencia;
      agencia.set_agencia(nagencia);

      cout << "Numero banco:";
      cin >> nbanco;
      banco.set_banco(nbanco);

      system("cls");

      cout << "Confirmar Cadastro?" << endl;
      cout << "Numero da conta: " << num_conta.get_numero_conta_corrente() << endl
           << "Numero da agencia: " << agencia.get_agencia() << endl
           << "Numero do banco: " << banco.get_banco() << endl;
      cout << "(s/S/n/N):";

      do
      {
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
      if (resp == 's' || resp == 'S')
      {
        cout << "chamando Controladora para cadastrar conta corrente";
        fim = true;
      }
    }
    catch (const exception &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::editar()
{
  int alteracoes = 0;
  char resp;
  bool fim = false;
  Usuario *u;

  Nome nome;
  Identificador iden;
  Senha sen;

  string novoNome, novoId, novaSenha;

  while (!fim)
  {
    try
    {
      u = ctrl->buscarUsuario(identificador->get_identificador());

      cout << "Editar Dados" << endl
           << "Para alterar os dados, escreva-o nos determinados campos,para pula-lo, escreva espaco e em seguida enter" << endl;
      cout << "Nome (" << u->get_nome() << "):";

      getline(cin, novoNome);
      getline(cin, novoNome);

      if (novoNome.compare(" ") != 0)
      {
        nome.set_nome(novoNome);
        alteracoes++;
      }

      cout << "Senha:";
      getline(cin, novaSenha);

      if (novaSenha.compare(" ") != 0)
      {
        sen.set_senha(novaSenha);
        alteracoes++;
      }

      if (alteracoes > 0)
      {
        cout << "Confirmar alteracoes?" << endl;
        if (!nome.get_nome().empty())
        {
          cout << "Nome :" << nome.get_nome() << endl;
        }
        if (!sen.get_senha().empty())
        {
          cout << "Senha :" << formata_senha(sen.get_senha()) << endl;
        }
        cout << "(s/S/n/N):";
        do
        {
          cin >> resp;
        } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');
        if (resp == 's' || resp == 'S')
        {
          ctrl->editarUsuario(u->get_identificador(), nome.get_nome(), sen.get_senha());
          cout << "Dados editados com sucesso pressione enter para voltar ao menu" << endl;
          getchar();
          getchar();
        }
      }
      fim = true;
    }
    catch (const invalid_argument &ia)
    {
      cout << "ERRO - " << ia.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}

void CtrlIUUsu::cadastrar()
{
  Identificador id;
  Senha sn;
  Nome n;
  char resp;
  string nome, senha, identificador;
  bool fim = false;
  bool resultado = true;
  while (!fim)
  {
    try
    {
      cout << "Nome:";
      getline(cin, nome);
      getline(cin, nome); //apagando enter
      n.set_nome(nome);
      cout << "Identificador:";
      cin >> identificador;
      id.set_identificador(identificador);
      cout << "Senha:";
      cin >> senha;
      sn.set_senha(senha);

      do
      {
        cout << "Confirmar cadastro? (s/S/n/N)" << endl
             << "Nome: " << n.get_nome() << "\t"
             << "identificador:" << id.get_identificador() << endl
             << "Senha : " << formata_senha(sn.get_senha()) << endl;
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->cadastrarUsuario(identificador, nome, senha);
        cout << "Usuario cadastrado, pressione enter para retornar ao menu" << endl;
        getchar();
        getchar();
      }
      fim = true;
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
}

void CtrlIUUsu::deletar()
{
  bool fim = false;
  char resp;
  Identificador id;
  string id_del;

  while (!fim)
  {
    try
    {
      cout << "identificador do Usuario a ser deletado:";
      cin >> id_del;
      id.set_identificador(id_del);

      do
      {
        cout << "Confimar remover o usuario - " << id_del << "?" << endl
             << "(s/S/n/N):" << endl;
        cin >> resp;
      } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

      if (resp == 's' || resp == 'S')
      {
        ctrl->deletarUsuario(identificador->get_identificador(), id_del);

        cout << "Usuario deletado, pressione enter para retornar ao menu" << endl;
        getchar();
        getchar();
      }

      fim = true;
    }
    catch (const exception &ex)
    {
      cout << "ERRO - " << ex.what() << endl;
      cout << "Deseja tentar de novo? (s/S|n/N)" << endl;
      cin >> resp;
      if (resp == 'N' || resp == 'n')
      {
        fim = true;
      }
    }
  }
}
/************* CONTROLADORA DE INTERFACE DE USUARIO - USUARIO ***********/

/************* CONTROLADORA DE SERVICOS DE USUARIO - *************/
Usuario *CtrlServUsu::buscarUsuario(string id)
{
  int rc;
  Usuario *u;
  sqlite3_stmt *stmt;

  if (!CtrlServUsu::existeUsuario(id))
    throw runtime_error("Usuario nao encontrado");

  string SQL_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE ";
  SQL_SELECT_USUARIO += "identificador = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_USUARIO);

  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    string id((char *)sqlite3_column_text(stmt, 1));
    string nome((char *)sqlite3_column_text(stmt, 2));
    string senha((char *)sqlite3_column_text(stmt, 3));

    u = new Usuario(id, nome, senha);
  }
  CtrlServ::finaliza();
  return u;
}

void CtrlServUsu::editarUsuario(string identificador, string nome, string senha)
{
  int rc;
  cout << 'alterando usuario';

  string SQL_EDIT_USUARIO = "UPDATE Usuario set ";

  if (!nome.empty())
  {
    SQL_EDIT_USUARIO += "nome = '" + nome + "' ";
  }

  if (!senha.empty())
  {
    SQL_EDIT_USUARIO += "senha = '" + senha + "' ";
  }

  SQL_EDIT_USUARIO += "where identificador = '" + identificador + "';";

  rc = CtrlServ::executa(SQL_EDIT_USUARIO);
  trata_retorno(rc);
}

void CtrlServUsu::deletarUsuario(string id_usu, string id_del)
{
  int rc;
  sqlite3_stmt *stmt;

  if (id_usu.compare(id_del) == 0)
    throw runtime_error("O Usuario nao pode deletar sua propria conta");

  if (!CtrlServUsu::existeUsuario(id_del))
    throw runtime_error("O Usuario nao existe!");

  string SQL_DEL_USUARIO = "DELETE FROM Usuario WHERE ";
  SQL_DEL_USUARIO += "identificador = '" + id_del + "'";

  rc = CtrlServ::executa(SQL_DEL_USUARIO);
  trata_retorno(rc);
}

bool CtrlServUsu::existeUsuario(string id)
{
  int rc;
  bool resultado = false;
  sqlite3_stmt *stmt;
  const char *zero("0"); //valor padrao para validacao

  //comando de selecao que conta quantos usuarios existem com o identificador dado
  //deve ser retornado 0 ou 1
  string SQL_SELECT_USUARIO = "SELECT COUNT(*) FROM USUARIO WHERE ";
  SQL_SELECT_USUARIO += "identificador = '" + id + "';";

  rc = CtrlServ::executa(SQL_SELECT_USUARIO);
  if (trata_retorno(rc))
  {
    stmt = CtrlServ::get_stmt();
    //convertendo void* para cosnt char* da resposta do banco
    const char *num = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));

    if (strcmp(num, zero) == 0)
    {
      resultado = false;
    }
    else
    {
      resultado = true;
    }
  }
  CtrlServ::finaliza();
  return resultado;
}

void CtrlServUsu::cadastrarUsuario(string id, string nome, string senha)
{
  int rc;
  sqlite3_stmt *stmt;

  string SQL_INSERT_USUARIO = "INSERT INTO USUARIO (identificador,nome,senha) VALUES (";
  SQL_INSERT_USUARIO += "'" + id + "',";
  SQL_INSERT_USUARIO += "'" + nome + "',";
  SQL_INSERT_USUARIO += "'" + senha + "');";

  if (CtrlServUsu::existeUsuario(id))
    throw invalid_argument("Usuario ja Cadastrado!");

  rc = CtrlServ::executa(SQL_INSERT_USUARIO);

  trata_retorno(rc);
  CtrlServ::finaliza();
}
/************* CONTROLADORA DE SERVICOS DE USUARIO - *************/

/************* CONTROLADORA DE SERVICOS DE AUTENTICACAO - USUARIO - *************/
Usuario *CtrlServAut::autenticar(Identificador &id, Senha &senha)
{
  int rc;
  Usuario *usu;
  sqlite3 *banco = CtrlServ::get_banco();

  vector<vector<string>> result;

  if (!CtrlServ::bd_criado())
    CtrlServ::init_banco();

  rc = sqlite3_open(CtrlServ::get_nome_banco(), &banco);

  string SQL_STMT_SELECT_USUARIO = "SELECT * FROM USUARIO WHERE ";
  SQL_STMT_SELECT_USUARIO += "identificador = '" + id.get_identificador() + "'";
  SQL_STMT_SELECT_USUARIO += " AND ";
  SQL_STMT_SELECT_USUARIO += "senha = '" + senha.get_senha() + "';";

  sqlite3_prepare_v2(banco, SQL_STMT_SELECT_USUARIO.c_str(), -2, &stmt, NULL);
  sqlite3_reset(stmt);

  rc = sqlite3_step(stmt);

  if (trata_retorno(rc) && sqlite3_data_count(stmt) > 0)
  {

    string id_str(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
    string nome_str(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
    string senha_str(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));

    usu = new Usuario(id_str, nome_str, senha_str);
  }
  else
  {
    throw runtime_error("Usuario nao cadastrado");
  }

  sqlite3_finalize(stmt);
  sqlite3_close(banco);
  return usu;
}
/************* CONTROLADORA DE SERVICOS DE AUTENTICACAO - USUARIO - *************/

/****CONTROLADORA DE SERVICOS BASE ********/

bool CtrlServ::banco_criado = false;

bool CtrlServ::init_banco()
{
  bool resultado = true;
  sqlite3_stmt *stmt;
  vector<char *> sql;

  if (!CtrlServ::banco_criado)
  {

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

    sqlite3_close(banco);
    banco_criado = true;
  } //banco ja criado
  return resultado;
}
