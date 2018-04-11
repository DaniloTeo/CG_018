# CG_018

# Como usar o git (by Edylson Torikai)
## Instalando
### Windows
[Baixar e instalar no seu pc](https://git-scm.com/downloads)
### Linux
Provavelmente já veio instalado com o Linux. Abra o terminal e tente rodá-lo.
Caso contrário, se estiver usando Ubuntu, ou alguma distribuição que utilize o `apt` apenas rode `sudo apt-get update; sudo apt-get install git -y`

## Configurando o git no seu PC
Abra o terminal (ou o Git Bash no caso do Windows) e configure suas informações pessoais (para que outros identifiquem suas contribuições).
`git config —global user.name="<seu usuário git>"`
`git config —global user.email="<seu email>"`

## Preparando o projeto na sua máquina
Primeiro, você precisará clonar o projeto pro seu pc. Então abra o seu terminal e entre na pasta (usando o comando `cd`) onde deseja colocar a pasta do projeto.
 
Feito isso, rode `git clone <URL do repositório>`. No nosso caso, a URL seria `https://github.com/ET33/petshop.git`, então o comando final a ser rodado é: `git clone https://github.com/ET33/petshop.git`

Por fim, rode um `ls` para se certificar que o projeto foi clonado na sua máquina.

## Fazendo alterações no projeto
Alguns comandos básicos do git incluem:
* `git branch`
 * `git branch <nome_branch>` - Cria uma nova branch com o nome `<nome_branch>`
* `git checkout`
 * `git checkout <nome_branch>` - Vai para a branch `<nome_branch>`
* `git status` - Verifica o estado dos seus arquivos. Aqui serão listados os arquivos modificados, adicionados e deletados
* `git fetch` - Apenas pergunta se houve modificações no repositório remoto (checa se alguém modificou o repositório no GitHub) e atualiza informaçes locais, sem modificar seu código.
* `git add`
 * `git add <caminho_do_arquivo>` - Para você escolher quais arquivos serão enviados no seu próximo _commit_.
 Aqui, é muito comum o uso de `git add .`, onde o `.` significa _diretório atual_, ou seja, adiciona todos os arquivos modificados/adicionados/deletados do seu diretório atual para o commit. (pense no `ls .`)
* `git commit` - Cria um novo commit em sua branch local (cria um checkpoint no projeto). Para facilitar o entendimento, imagine como se você tirasse uma foto do estado atual do projeto, e essa foto serve para você poder voltar para esse estado a qualquer momento.
* `git pull`
 * `git pull <nome_remoto> <nome_branch>` - Atualiza sua branch atual com alguma branch do \*GitHub _(não é bem isso, mas por enquanto pense assim)_. Ao mesmo tempo, faz a junção das mudanças. *\*merge\**
* `git push`
 * `git push <nome_remoto> <nome_branch>` - Atualiza o repositório no GitHub com suas modificações. Um exemplo do comando seria:
 `git push origin master` (NÃO FAÇA ISSO SEM AVISAR ALGUÉM ANTES, PLZ. VAMOS TORNAR O MUNDO MELHOR)


## Progresso (Abaixo será listado as funcionalidades, o nome de quem está trabalhando e se a task está terminada)
Para editar o progresso do projeto, a vossa excelência deverá abrir o arquivo README.MD, vir até aqui e adicionar um x no campo em questão —----->


### Exemplo de demonstração do progresso
- [ ] Classe de usuário (Jon is Working...) —— Alguém já está fazendo está função
- [ ] Classe dos dogs —— Task em aberto ainda, ninguém está cuidando
- [X] Classe sem classe —— Task já está terminada
...
