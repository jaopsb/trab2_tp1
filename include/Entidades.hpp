#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <dominios.hpp>

class Usuario
{
public:
  Usuario(string, string, string);
  Usuario(const char *, const char *, const char *);
  Usuario();
  void set_nome(string);
  string get_nome() { return nome.get_nome(); };

  void set_identificador(string);
  string get_identificador() { return identificador.get_identificador(); };

  void set_senha(string);
  string get_senha() { return senha.get_senha(); };

private:
  Nome nome;
  Identificador identificador; //PK
  Senha senha;
};

class Acomodacao
{
public:
  Acomodacao(){};
  Acomodacao(string, int, string, float, string, string, int);

  void set_titulo(string);
  string get_titulo() { return titulo; };

  void set_capacidade(int);
  int get_capacidade() { return capacidade.get_capacidade_acomodacao(); };

  void set_cidade(string);
  string get_cidade() { return cidade.get_nome(); };

  void set_diaria(float);
  float get_diaria() { return diaria.get_diaria(); };

  void set_estado(string);
  string get_estado() { return estado.get_estado(); };

  void set_identificador(string);
  string get_identificador() { return identificador.get_identificador(); };

  void set_tipo(int);
  int get_tipo() { return tipo.get_tipo_acomodacao(); };

  void set_id_acomodacao(int);
  int get_id_acomodacao() { return id_acomodacao; };

private:
  Identificador identificador;
  Tipo_Acomodacao tipo;
  Capacidade_Acomodacao capacidade;
  Nome cidade;
  Estado estado;
  Diaria diaria;
  string titulo;
  int id_acomodacao;
};

class Cartao_de_Credito
{
public:
  Cartao_de_Credito(string, string);
  void set_numero(string);
  string get_numero() { return numero.get_numero_cartao(); };

  void set_data_validade(string);
  string get_data_validade() { return data_validade.get_data_validade(); }

private:
  Numero_Cartao numero;
  Data_Validade data_validade;
};

class Conta_corrente
{
public:
  Conta_corrente(int, int, string);
  void set_agencia(int);
  int get_agencia() { return agencia.get_agencia(); };

  void set_banco(int);
  int get_banco() { return banco.get_banco(); };

  void set_numero(string);
  string get_numero() { return numero.get_numero_conta_corrente(); };

private:
  Numero_Conta_Corrente numero;
  Agencia agencia;
  Banco banco;
};

class RetornoBase
{
private:
  bool resultado;

public:
  bool get_resultado() { return resultado; };
  void set_resultado(bool r)
  {
    this->resultado = r;
  };
};

class RetornoLogin : public RetornoBase
{
private:
  Usuario *usu;

public:
  Usuario *get_usuario() { return usu; };

  void set_usuario(Usuario *u)
  {
    this->usu = u;
  };
};

#endif // ENTIDADES_H
