# PGCCHIB - Processamento Gráfico (EM ANDAMENTO)

Este repositório é baseado na estrutura do projeto PGCCHIB - Processamento Gráfico, fornecido pela disciplina de Processamento Gráfico. 

![image](https://github.com/user-attachments/assets/b27f999e-ff81-4394-9637-3ade365d57ed)

## 🛠️ Como compilar e rodar o projeto (PowerShell - Windows)
Para facilitar a compilação e execução, fornecemos um script .bat que automatiza todo o processo.

## Instale o vcpkg caso ainda não tenha:
- Clone o repositório vcpkg
- Execute o bootstrap para preparar o vcpkg

## Configure a variável de ambiente VCPKG_ROOT apontando para a pasta raiz do seu vcpkg. 
No prompt do Windows (cmd), execute:
```
setx VCPKG_ROOT "C:\caminho\para\vcpkg"
```
> Importante: Após definir a variável, feche e abra o terminal para que a variável seja reconhecida.

## Execute o script build_and_run.bat que está neste repositório, na raiz do projeto. Ele vai:
- Apagar a pasta build se existir
- Criar a pasta build
- Configurar o CMake usando o toolchain do vcpkg via a variável VCPKG_ROOT
- Construir o projeto
- Executar o programa

## Caso queira compilar manualmente
- cd ~seudiretório\V1_ProcessamentoGrafico\
- Remove-Item -Recurse -Force .\build\
- mkdir build
- cd build
- cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/Users/raian/OneDrive/Desktop/vcpkg/scripts/buildsystems/vcpkg.cmake" -A x64
- cmake --build .
- .\build\Debug\MultiTexturedSprites.exe
- 
Siga as instruções detalhadas em [GettingStarted.md](GettingStarted.md) para configurar e compilar o projeto.

## ⚠️ **IMPORTANTE: Baixar a GLAD Manualmente**
Para que o projeto funcione corretamente, é necessário **baixar a GLAD manualmente** utilizando o **GLAD Generator**.

### 🔗 **Acesse o web service do GLAD**:
👉 [GLAD Generator](https://glad.dav1d.de/)

### ⚙️ **Configuração necessária:**
- **API:** OpenGL  
- **Version:** 3.3+ (ou superior compatível com sua máquina)  
- **Profile:** Core  
- **Language:** C/C++  

### 📥 **Baixe e extraia os arquivos:**
Após a geração, extraia os arquivos baixados e coloque-os nos diretórios correspondentes:
- Copie **glad.h** para `include/glad/`
- Copie **khrplatform.h** para `include/glad/KHR/`
- Copie **glad.c** para `common/`

🚨 **Sem esses arquivos, a compilação falhará!** É necessário colocá-los nos diretórios corretos conforme a orientação acima.

---



