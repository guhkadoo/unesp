import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.patches import FancyBboxPatch
import numpy as np

# Configurar o matplotlib para usar fontes LaTeX
plt.rcParams.update({
    'font.size': 10,
    'font.family': 'serif',
    'text.usetex': False,  # Desabilitado para compatibilidade
    'figure.figsize': (12, 8)
})

fig, ax = plt.subplots(1, 1, figsize=(14, 10))

# Definir posições dos nós na árvore final
positions = {
    'root': (7, 8),
    'node_71': (4, 6),
    'node_29': (10, 6),
    'A': (2, 4),
    'node_26': (6, 4),
    'node_29_left': (8, 4),
    'node_29_right': (12, 4),
    'C': (4, 2),
    'node_14': (8, 2),
    'B': (6, 2),
    'D': (14, 2),
    'E': (6, 0),
    'F': (10, 0)
}

# Função para desenhar um nó
def draw_node(ax, pos, label, freq, is_leaf=False):
    x, y = pos
    if is_leaf:
        # Nós folha em verde claro
        box = FancyBboxPatch((x-0.4, y-0.3), 0.8, 0.6, 
                           boxstyle="round,pad=0.1", 
                           facecolor='lightgreen', 
                           edgecolor='black',
                           linewidth=1.5)
    else:
        # Nós internos em azul claro
        box = FancyBboxPatch((x-0.4, y-0.3), 0.8, 0.6, 
                           boxstyle="round,pad=0.1", 
                           facecolor='lightblue', 
                           edgecolor='black',
                           linewidth=1.5)
    
    ax.add_patch(box)
    ax.text(x, y, f'{label}\n({freq})', ha='center', va='center', 
            fontweight='bold', fontsize=9)

# Função para desenhar uma aresta
def draw_edge(ax, start_pos, end_pos, label='', offset=0.1):
    x1, y1 = start_pos
    x2, y2 = end_pos
    
    # Ajustar as posições para não sobrepor os nós
    if y1 > y2:  # Aresta de cima para baixo
        y1 -= 0.3
        y2 += 0.3
    
    ax.plot([x1, x2], [y1, y2], 'k-', linewidth=2)
    
    # Adicionar label na aresta
    if label:
        mid_x, mid_y = (x1 + x2) / 2, (y1 + y2) / 2
        # Offset para não sobrepor a linha
        if x1 < x2:  # Aresta indo para direita
            offset_x, offset_y = -0.2, offset
        else:  # Aresta indo para esquerda
            offset_x, offset_y = 0.2, offset
            
        ax.text(mid_x + offset_x, mid_y + offset_y, label, 
                ha='center', va='center', 
                bbox=dict(boxstyle="round,pad=0.2", facecolor='white', alpha=0.8),
                fontweight='bold', fontsize=8)

# Desenhar todos os nós
draw_node(ax, positions['root'], '100', 100)
draw_node(ax, positions['node_71'], '71', 71)
draw_node(ax, positions['node_29'], '29', 29)
draw_node(ax, positions['A'], 'A', 45, is_leaf=True)
draw_node(ax, positions['node_26'], '26', 26)
draw_node(ax, positions['node_29_left'], 'B', 13, is_leaf=True)
draw_node(ax, positions['node_29_right'], 'D', 16, is_leaf=True)
draw_node(ax, positions['C'], 'C', 12, is_leaf=True)
draw_node(ax, positions['node_14'], '14', 14)
draw_node(ax, positions['E'], 'E', 9, is_leaf=True)
draw_node(ax, positions['F'], 'F', 5, is_leaf=True)

# Desenhar todas as arestas com labels
draw_edge(ax, positions['root'], positions['node_71'], '0')
draw_edge(ax, positions['root'], positions['node_29'], '1')
draw_edge(ax, positions['node_71'], positions['A'], '0')
draw_edge(ax, positions['node_71'], positions['node_26'], '1')
draw_edge(ax, positions['node_29'], positions['node_29_left'], '0')
draw_edge(ax, positions['node_29'], positions['node_29_right'], '1')
draw_edge(ax, positions['node_26'], positions['C'], '0')
draw_edge(ax, positions['node_26'], positions['node_14'], '1')
draw_edge(ax, positions['node_14'], positions['E'], '0')
draw_edge(ax, positions['node_14'], positions['F'], '1')

# Configurar o plot
ax.set_xlim(-1, 16)
ax.set_ylim(-1, 9)
ax.set_aspect('equal')
ax.axis('off')

# Adicionar título
plt.title('Árvore de Huffman - Exemplo Prático\n' + 
          'Códigos: A=0, B=101, C=100, D=111, E=1100, F=1101', 
          fontsize=14, fontweight='bold', pad=20)

# Adicionar legenda
legend_elements = [
    patches.Patch(facecolor='lightgreen', edgecolor='black', label='Nós Folha (Símbolos)'),
    patches.Patch(facecolor='lightblue', edgecolor='black', label='Nós Internos')
]
ax.legend(handles=legend_elements, loc='upper right', bbox_to_anchor=(1, 1))

# Adicionar nota explicativa
note_text = ("Nota: Os números entre parênteses representam as frequências.\n"
            "Os labels nas arestas (0 e 1) formam os códigos binários.")
ax.text(8, -0.5, note_text, ha='center', va='top', 
        bbox=dict(boxstyle="round,pad=0.5", facecolor='lightyellow', alpha=0.8),
        fontsize=9, style='italic')

plt.tight_layout()
plt.savefig('arvore_huffman_exemplo.png', dpi=300, bbox_inches='tight')
plt.savefig('arvore_huffman_exemplo.pdf', bbox_inches='tight')
plt.show()

print("Figura da árvore de Huffman salva como:")
print("- arvore_huffman_exemplo.png")
print("- arvore_huffman_exemplo.pdf")
