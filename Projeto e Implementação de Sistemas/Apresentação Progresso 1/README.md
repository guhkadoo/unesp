# Projeto e Implementação de Sistemas 

Esta pasta contém o Latex da Apresentação da Proposta desenvolvido durante a matéria de Projeto e Implementação de Sistemas.

## Latex no Linux
``` bash
sudo apt install texlive
sudo apt install texlive-full
pdflatex main.tex && biber main && pdflatex main.tex && latexmk -pdf -c main.tex
```

## Estrutura do Projeto 

O arquivo **main.tex** se refere à Apresentação da Proposta em formato beamer.

``` bash
├── imagens
│   ├── bmp_header.png
│   ├── huffman_tree.png
│   ├── unesp.png
│   └── wav_header.png
├── main.bib
├── main.pdf
└── main.tex
```
