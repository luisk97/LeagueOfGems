#include <allegro5/allegro.h>
#include <ctime>
#include "Game.h"

Game::Game() {
    playersList = new LinkedList<Player *>;
    enemyList = new LinkedList<Enemy *>;
    player = new Player("../resources/hetalia.png");
    tiles = al_load_bitmap("../resources/medievaltiles.png");
    level = 1;

    createMap();

    dij = Dijkstra(map);// A esta clase nadamas se le debe pazar la matriz del mapa al iniciarla
}

void Game::update(int x, int y) {
    Vertice next = dij.obtenerSiguienteVertice();//obtiene el siguiente nodo al que debe avanzar
    int nextX,nextY;
    nextX = (next.posicionXtiles)*50;
    nextY = (next.posicionYtiles)*50;
    if(nextX != -50){
        player->update(nextX, nextY);
    }else {
        player->update(x, y);
    }
}

void Game::updateCenter(int x, int y){
    int xPlayer,yPlayer;
    xPlayer = player->getX()/50;//posicion actual del jugador
    yPlayer = player->getY()/50;
    dij.definirPesos(x,y);//define los pesos a partir del nodo que se clickeo
    dij.definirRutaOptima(xPlayer,yPlayer);//define la ruta desde el la posicion inicial del personaje

}

void Game::draw() {
    drawMap();
    player->draw();

    for (int i = 0; i < enemyList->length(); ++i) {
        enemyList->get(i)->draw();
    }
}

void Game::createPlayers() {

}

void Game::drawMap() {
    for (int i = 0; i < 27; ++i) {
        for (int j = 0; j < 21; ++j) {

            if (map[j][i] == 1){
                al_draw_bitmap_region(tiles, 50 * 1, 50 * 4, 50, 50,
                                      50 * i, 50 * j, 0);
            }
            else{
                al_draw_bitmap_region(tiles, 50 * 1, 50 * 2, 50, 50,
                                      50 * i, 50 * j, 0);
            }
        }
    }
}

void Game::createMap() {
    std::srand(std::time(0));
    int random, enemys = 4;

    for (int dj = 0; dj < 21; dj++) {
        for (int di = 0; di < 27; di++) {
            random = std::rand() % 100;

            if(random > 90 && enemys != 0){
                map[dj][di] = 3;
                enemyList->add(new Enemy(di * 50, dj * 50, "../resources/enemy1.png"));
                enemys -= 1;
            }
            else if (random > 81){
                map[dj][di] = 1;
            }
            else{
                map[dj][di] = 0;
            }
        }
    }
}

Game::~Game(){
    delete player;
    delete playersList;
    delete enemyList;
    al_destroy_bitmap(tiles);
}