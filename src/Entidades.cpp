#include "Entidades.hpp"

Usuario::Usuario(string id, string nome, string senha)
{
    set_identificador(id);
    set_nome(nome);
    set_senha(senha);
}

Usuario::Usuario(const char *id, const char *nome, const char *senha)
{
    set_identificador(id);
    set_nome(nome);
    set_senha(senha);
}

Usuario::Usuario(){};

Acomodacao::Acomodacao(string titulo, int capacidade, string cidade, float diaria, string estado, string id, int tipo, string dt_dis_in, string dt_dis_fim)
{
    set_titulo(titulo);
    set_capacidade(capacidade);
    set_cidade(cidade);
    set_diaria(diaria);
    set_estado(estado);
    set_identificador(id);
    set_tipo(tipo);
    set_data_disponibilidade_inicio(dt_dis_in);
    set_data_disponibilidade_fim(dt_dis_fim);
}

Cartao_de_Credito::Cartao_de_Credito(string numero, string data_val)
{
    set_numero(numero);
    set_data_validade(data_val);
}

Conta_corrente::Conta_corrente(int agencia, int banco, string numero)
{
    set_agencia(agencia);
    set_banco(banco);
    set_numero(numero);
}

Reserva::Reserva(string titulo, string data_in, string data_fim, string cidade, string estado, int id_reserva)
{
    set_titulo(titulo);
    set_data_inicio(data_in);
    set_data_fim(data_fim);
    set_cidade(cidade);
    set_estado(estado);
    set_id_reserva(id_reserva);
}

void Reserva::set_titulo(string valor)
{
    titulo.set_nome(valor);
}

void Reserva::set_data_inicio(string valor)
{
    data_inicio.set_data(valor);
}

void Reserva::set_data_fim(string valor)
{
    data_fim.set_data(valor);
}

void Reserva::set_cidade(string valor)
{
    cidade.set_nome(valor);
}

void Reserva::set_estado(string valor)
{
    estado.set_estado(valor);
}

void Reserva::set_id_reserva(int valor)
{
    id_reserva = valor;
}

void Usuario::set_identificador(string valor)
{
    identificador.set_identificador(valor);
}

void Usuario::set_nome(string valor)
{
    nome.set_nome(valor);
}

void Usuario::set_senha(string valor)
{
    senha.set_senha(valor);
}

void Acomodacao::set_titulo(string valor)
{
    titulo = valor;
}

void Acomodacao::set_capacidade(int valor)
{
    capacidade.set_capacidade_acomodacao(valor);
}

void Acomodacao::set_cidade(string valor)
{
    cidade.set_nome(valor);
}

void Acomodacao::set_diaria(float valor)
{
    diaria.set_diaria(valor);
}

void Acomodacao::set_estado(string valor)
{
    estado.set_estado(valor);
}

void Acomodacao::set_identificador(string valor)
{
    identificador.set_identificador(valor);
}

void Acomodacao::set_tipo(int valor)
{
    tipo.set_tipo_acomodacao(valor);
}

void Acomodacao::set_id_acomodacao(int valor)
{
    id_acomodacao = valor;
}

void Acomodacao::set_data_disponibilidade_inicio(string valor)
{
    dt_dis_inicio.set_data(valor);
}

void Acomodacao::set_data_disponibilidade_fim(string valor)
{
    dt_dis_fim.set_data(valor);
}

void Cartao_de_Credito::set_numero(string valor)
{
    numero.set_numero_cartao(valor);
}

void Cartao_de_Credito::set_data_validade(string valor)
{
    data_validade.set_data_validade(valor);
}

void Conta_corrente::set_agencia(int valor)
{
    agencia.set_agencia(valor);
}

void Conta_corrente::set_banco(int valor)
{
    banco.set_banco(valor);
}

void Conta_corrente::set_numero(string valor)
{
    numero.set_numero_conta_corrente(valor);
}
