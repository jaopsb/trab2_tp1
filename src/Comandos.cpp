#include "Comandos.h"
#include <iostream>
#include "sqlite3.h"
#include "Controladoras.hpp"

using namespace std;

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

void CmdRegistraUsuario::executar()
{
    CtrlServAut *ctrl = new CtrlServAut();

    Identificador *id = new Identificador();
    id->set_identificador("jopsb");
    Senha *sn = new Senha();
    sn->set_senha("Senha123!");

    Usuario *usu = ctrl->autenticar(*id, *sn);

    cout << "Comando registrar usuario" << endl;
}

CmdBuscarUsuario::CmdBuscarUsuario(Identificador *id, Senha *sn)
{
    identificador = id;
    senha = sn;
}

void CmdBuscarUsuario::executar()
{
    cout << "Entrnado no Comando Buscar Usuario" << endl;
    int rc;
    sqlite3_stmt *stmt;
    ctrl = new CtrlServ();

    string SQL_SELECT_USUARIO = "SELECT * FROM USUARO WHERE ";
    SQL_SELECT_USUARIO += "identificador = '" + identificador->get_identificador() + "'";
    SQL_SELECT_USUARIO += " AND ";
    SQL_SELECT_USUARIO += "senha = '" + senha->get_senha() + "';";

    rc = ctrl->executa(SQL_SELECT_USUARIO);

    if (trata_retorno(rc))
    {
        stmt = ctrl->get_stmt();
        const char *id = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *nome = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        const char *senha = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));

        cout << id << ' ' << nome << ' ' << senha << endl;
    }
}