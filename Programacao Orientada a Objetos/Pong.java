import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public class Pong{
    public static void main(String[] args){
        new Frame();
    }
}

class PongPanel extends JPanel{
    Random rand = new Random();
    int j1x = 5, j2x = 585, j1y = 250, j2y = 250;
    int bx = 300, by = 300;
    int vx = rand.nextInt(1, 3), vy = rand.nextInt(1, 3);
    int numPontosJ1 = 0, numPontosJ2 = 0;
    JLabel pontosJ1, pontosJ2;

    PongPanel(){
        setBackground(Color.BLACK);
        setPreferredSize(new Dimension(600, 600));
        pontosJ1 = new JLabel("1");
        new Thread(new AtualizarPos()).start();
    }

    public void paintComponent(Graphics g){
        super.paintComponent(g);
        g.setColor(Color.LIGHT_GRAY);
        g.fillRect(300, 0, 3, this.getHeight());
        g.fillOval(bx, by, 30, 30);
        g.fillRect(j1x, j1y, 10, 100);
        g.fillRect(j2x, j2y, 10, 100);
    }

    class AtualizarPos implements Runnable{
        public void run(){
            while(true){
                buscaExcecao();
                bx += vx;
                by += vy;
                try{
                    Thread.sleep(10);
                }catch(Exception e){}
                repaint();
            }
        }

        void buscaExcecao(){
            if(bx+30 >= j2x && by <= j2y+100 && by+30 >= j2y){
                vx = (-1)*(rand.nextInt(2)+2);
                bx -= (bx+30 - j2x); 
            }
            else if(bx <= j1x+10 && by <= j1y+100 && by+30 >= j1y){
                vx = (rand.nextInt(2)+2);
                bx += (j1x+10 - bx);
            }
            if(bx >= 600){
                System.exit(0);
            }
            else if(bx+30 <= 0){
                System.exit(0);
            }
            if(by <= 0){
                vy = (rand.nextInt(2)+2);
            }
            else if(by+30 >= 600){
                vy = (-1)*(rand.nextInt(2)+2);
            }
        }
    }
}

class Frame extends JFrame{
    boolean j1UpArrow, j1DownArrow, j2UpArrow, j2DownArrow;
    PongPanel pong;
    Frame(){
        super("Pong");
        pong = new PongPanel();
        add(pong);
        this.addKeyListener(new PlayersMovement());
        new Thread(new PlayersMovement()).start();
        setVisible(true); pack();
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setResizable(false);
    }

    class PlayersMovement extends KeyAdapter implements Runnable{
        public void keyPressed(KeyEvent e){
            if(e.getKeyCode() == KeyEvent.VK_UP){
                j2UpArrow = true;
            }
            if(e.getKeyCode() == KeyEvent.VK_DOWN){
                j2DownArrow = true;
            }
            if(e.getKeyCode() == KeyEvent.VK_W){
                j1UpArrow = true;
            }
            if(e.getKeyCode() == KeyEvent.VK_S){
                j1DownArrow = true;
            }
        }
        
        public void keyReleased(KeyEvent e){
            if(e.getKeyCode() == KeyEvent.VK_UP){
                j2UpArrow = false;
            }
            if(e.getKeyCode() == KeyEvent.VK_DOWN){
                j2DownArrow = false;
            }
            if(e.getKeyCode() == KeyEvent.VK_W){
                j1UpArrow = false;
            }
            if(e.getKeyCode() == KeyEvent.VK_S){
                j1DownArrow = false;
            }
        }
        
        public void run(){
            while(true){
                if(j1UpArrow == true && j1DownArrow == false){
                    pong.j1y -= 4; 
                } else if(j1DownArrow == true && j1UpArrow == false){
                    pong.j1y += 4;
                }
                if(j2UpArrow == true && j2DownArrow == false){
                    pong.j2y -= 4;
                } else if(j2DownArrow == true && j2UpArrow == false){
                    pong.j2y += 4;
                }
                try{
                    Thread.sleep(15);
                }catch(Exception e){}
            }
        }
    }
}

