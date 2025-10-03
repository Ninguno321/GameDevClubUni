#include <raylib.h>
#include <random>
#include <ctime>


//Variables globales
int puntos_j1 = 0;
int puntos_j2 = 0;
Sound sonido_gol;

int ancho_v_min = 1280;
int alto_v_min = 800;

Vector2 vel_bbola = {6.0f,6.0f};

float aleatoriedaD[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.7, 0.8, 0.9}; 

struct Bola{
    // Lo inicializamos para evitar errores
    Vector2 pos = {}; 
    Vector2 vel = {};
    int radio = 0;
    Color color = WHITE;
    int tamano_aleat = sizeof(aleatoriedaD);

    void actualizar(){
        pos.x += vel.x;
        pos.y += vel.y;
        
        if(vel.x > 12){
            color = ORANGE;
            if(vel.x > 18)
                color = RED;
        }
        
        if (pos.x + radio >= GetScreenWidth()){
            puntos_j1++;
            reiniciar();
        }
            
        if (pos.x - radio < 0){
            puntos_j2++;
            reiniciar();
        }
       
        if (pos.y + radio >= GetScreenHeight() or pos.y - radio < 0)
            vel.y *= -1;
        
    }
    
    void reiniciar(){
        PlaySound(sonido_gol);
        pos = {640, 400};
        float deci_random = aleatoriedaD[rand() % tamano_aleat];
        float deci_random2 = aleatoriedaD[rand() % tamano_aleat];
        
        vel = {vel_bbola.x * (rand() % 2 == 0 ? -1.0f : 1.0f) + deci_random,deci_random2 + (rand() % 4 + 4) * (rand() % 2 == 0 ? -1.0f : 1.0f)};
        color = WHITE;
    }
    
    void dibujar(){
        
       DrawCircle(pos.x, pos.y, radio, color);
    }
};

struct Pala {
    Rectangle rec = {};
    int vel = 0;

    void actualizar(){ //Is keypress es para pulsarla una sola vez
        if (IsKeyDown(KEY_W)){
            rec.y -= vel;
        }
        
        if (IsKeyDown(KEY_S)){
            rec.y += vel;
        }
        
        comprobar_colision();
    }
    
    
    void actualizar2(){ //Is keypress es para pulsarla una sola vez
        if (IsKeyDown(KEY_UP)){
            rec.y -= vel;
        }
        
        if (IsKeyDown(KEY_DOWN)){
            rec.y += vel;
        }
        
        comprobar_colision();
    }
    
    void comprobar_colision(){
        //El punto del rectangulo es esquina superior izquierda
        if (rec.y <= 5)
            rec.y = 5;
        else if (rec.y >= GetScreenHeight() - rec.height -5)
            rec.y = GetScreenHeight() - rec.height -5;
    }
    
    
    void dibujar(){
        DrawRectangleRec(rec, WHITE);
    }
    
};


struct Ventana {
    int a = 0;
    int h = 0;
};




/*
 - Definir var
        obj
        
 - Eventos
 - Actualizar la informacion
 - Dibujar 
 
 1 Crear la ventana y un bucle
 2 Dibujar bola, palas y linea (tenemos x y, la y esta invertida (crece hacia abajo))
 3 Mover la bola 
 4 Comprobar las colisiones con los bordes.
 5 Mover pala
 6 Colision bola 
*/


int main(){
    
    //Deeclaracion de variables.
    srand(time(NULL)); 
    Ventana ventana;
    ventana.a = ancho_v_min;
    ventana.h = alto_v_min;
 
    //Para crear ventanas 
    InitWindow(ventana.a, ventana.h, "Pong");
    InitAudioDevice();
    
    SetTargetFPS(60); //Esto limita el juego a 60
    
    //Se mete en el bucle que mria todo el tiempo si se cumple (se puede cambiar pero es el escape)
    Sound sonido_golpeS[2];
    sonido_golpeS[0] = LoadSound("Sounds/pongblipa4.wav");
    sonido_golpeS[1] = LoadSound("Sounds/pongblipa5.wav");
    sonido_gol = LoadSound("Sounds/objective-complete.wav");
    
    Bola bola;
    bola.pos = Vector2 {640, 400};
    bola.vel = Vector2 {6, 6};
    bola.radio = 10;
    
    Pala pala;
    pala.rec = Rectangle{10, GetScreenHeight()/2 - 50, 25, 100};
    pala.vel = 10;
     
    Pala pala2;
    pala2.rec = Rectangle{GetScreenWidth() - 25 -10, GetScreenHeight()/2 - 50, 25, 100};
    pala2.vel = 10;
    
    while(!WindowShouldClose()){
        //EVENTOS
        //ACTUALIZADO
        bola.actualizar();
        pala.actualizar();
        pala2.actualizar2();
        
        if(CheckCollisionCircleRec(bola.pos, bola.radio, pala.rec)){
            bola.pos.x = pala.rec.x + pala.rec.width + bola.radio;
            bola.vel.x *= -1.1f;
            PlaySound(sonido_golpeS[rand() % 2]);
        }
        
        
        if(CheckCollisionCircleRec(bola.pos, bola.radio, pala2.rec)){
            bola.pos.x = pala2.rec.x - pala2.rec.width - bola.radio;
            bola.vel.x *= -1.1f;
            PlaySound(sonido_golpeS[rand() % 2]);
        }
        
        //DIBUJADO
        BeginDrawing();
            ClearBackground(BLACK);
            // x y radio color RGBA (transparencia)
            //DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2 , 10, RED);
            bola.dibujar();
           
           
            DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 10, GRAY);
            //La linea
            DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenWidth(), WHITE);
           
            //Pala izquierda, x, y , ancho, largo, color 
            //DrawRectangle(10, GetScreenHeight()/2 - 50, 25, 100, WHITE);
            pala.dibujar();
            pala2.dibujar();
            
            DrawText(TextFormat("%i", puntos_j1), GetScreenWidth() * 1/3 - 60, 10, 60, WHITE);
            DrawText(TextFormat("%i", puntos_j2), GetScreenWidth() * 2/3, 10, 60, WHITE);
            
            //Pala derecha
            //DrawRectangle(GetScreenWidth() - 35, GetScreenHeight()/2 - 50, 25, 100, WHITE);
            
        EndDrawing();
    }
    
    
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}
























