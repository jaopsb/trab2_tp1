#include <iostream>
#include <Testes.h>

using namespace std;

int main()
{
  Teste_Dominios teste_d;
  Teste_Entidades teste_e;

  cout << "TESTES DOS DOMINIOS" << endl;
  teste_d.run();

  cout << "TESTE DAS ENTIDADES" << endl;
  teste_e.run();

  return 0;
}
