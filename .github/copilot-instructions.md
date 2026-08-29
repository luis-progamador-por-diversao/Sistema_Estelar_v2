# Instruções do projeto

- Projeto C++17 pequeno, sem sistema de build dedicado.
- Arquivos principais: `teste.cpp` contém `main` e geração/saída; `obc_inplementaco.cpp` contém as implementações de domínio; `objetos_celestes.hpp` contém as declarações.
- Compilar a partir da raiz com `g++ -std=c++17 -Wall -Wextra -pedantic teste.cpp obc_inplementaco.cpp -o output\sistema_estelar.exe`.
- Preservar os nomes e comentários em português ao modificar o código, salvo quando a tarefa pedir renomeação.
- Validar alterações compilando novamente com os mesmos flags; executar o binário apenas quando a alteração exigir teste comportamental.
- Evitar `system("cls")` e outros comportamentos específicos do sistema em código novo; manter o escopo das correções focado.
