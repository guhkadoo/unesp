# Projeto e Implementação de Sistemas 

Esta pasta contém o Latex do TCC desenvolvido durante a matéria de Projeto e Implementação de Sistemas.

## Latex no Linux
``` bash
sudo apt install texlive
sudo apt install texlive-full
pdflatex file.tex && bibtex file && pdflatex file.tex && latexmk -pdf -c file.tex
```

## Estrutura do Projeto 

O arquivo **monografia.tex** se refere ao Trabalho de Conclusão de Curso já completo.

Por sua vez, o arquivo **proposta.tex** se refere à proposta de Projeto a ser desenvolvida.
``` bash
├── chapters_monografia
│   ├── analise_comparativa.tex
│   ├── conclusao.tex
│   ├── fundamentacao_teorica.tex
│   ├── implementacao.tex
│   ├── introducao.tex
│   └── referencias.bib
├── chapters_proposta
│   ├── cronograma.tex
│   ├── introducao.tex
│   ├── justificativa.tex
│   ├── metodo.tex
│   ├── objetivos.tex
│   ├── problema.tex
│   └── referencias.bib
├── figs
│   ├── entropia-moeda.png 
│   └── ficha_catalografica.pdf 
├── monografia.tex
└── proposta.tex
```
