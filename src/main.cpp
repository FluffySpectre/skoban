#include "Skoban.h"

BITMAP *box;
BITMAP *floor;
BITMAP *wall;
BITMAP *buffer;

int x = 10;
int y = 12;

int tempX = 10;
int tempY = 12;

//Anzahl der Boxen die sich noch auf dem Spielfeld befinden
int boxes;

bool GetAllBoxes = false;
bool needsRefresh;

//Hintergrund, 1 = Wolken, 2 = Mauer, 3 = Boden, 4 = Loch
int map[MAP_HEIGHT][MAP_WIDTH];

//Enthält die Objekte der Karte
int objMap[MAP_HEIGHT][MAP_WIDTH] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,100,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,101,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};  

void Setup() {
    set_color_depth(24);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
    
    // Grafiken laden
    buffer = create_bitmap(640, 480);
    box = load_bitmap( "gfx/box.bmp", NULL);
    wall = load_bitmap("gfx/brick.bmp", NULL);
    floor = load_bitmap("gfx/floor.bmp", NULL);
    
    // Karte laden
    LoadMap("maps/map1.dat");
    
    // Anzahl der Boxen festlegen
    boxes = 3;
    
    needsRefresh = true;
}

void LoadMap(const char *path) {
    ifstream filein;
    char temp;
    
	filein.open(path, ifstream::in);
	
    if(filein) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            for(int x = 0; x < MAP_WIDTH; x++) {
                filein >> temp;
                
                map[y][x] = atoi(&temp);               
            }       
        }     
    }
	
	filein.close();
}

void DrawMap() {
    clear_bitmap(buffer);
    
    acquire_screen();
    
    // Karte zeichnen
    for(int i = 0; i <= MAP_HEIGHT; i++){
        for(int t = 0; t <= MAP_WIDTH; t++){
            if(map[i][t] == SKY) rectfill(buffer, t * TILE_WIDTH, i * TILE_HEIGHT, (t + 1) * TILE_WIDTH, (i + 1) * TILE_HEIGHT, makecol(128, 0, 255));
            else if(map[i][t] == BRICK) draw_sprite(buffer, wall, t * TILE_WIDTH, i * TILE_HEIGHT);
            else if(map[i][t] == FLOOR) draw_sprite(buffer, floor, t * TILE_WIDTH, i * TILE_HEIGHT);
            else if(map[i][t] == HOLE) rectfill(buffer, t * TILE_WIDTH, i * TILE_HEIGHT, (t + 1) * TILE_WIDTH, (i + 1) * TILE_HEIGHT, makecol( 0, 0, 0));
        }       
    }
    
    // Objekte zeichnen
    for(int i = 0; i <= MAP_HEIGHT; i++) {
        for(int t = 0; t <= MAP_WIDTH; t++) {
            if(objMap[i][t] == PLAYER) circlefill(buffer, (t * TILE_WIDTH) + 9, (i * TILE_HEIGHT) + 9, 9, makecol(255, 255, 0));
            else if(objMap[i][t] == BOX) draw_sprite(buffer, box, t * TILE_WIDTH, i * TILE_HEIGHT);
        }         
    }   
            
    draw_sprite(screen, buffer, 0, 0);
    release_screen(); 
}

void MovePlayer() {
    // Aktuelle Position des Spielers speichern
    tempX = x;
    tempY = y;
    
    // Tastaturabfrage
    if(key[KEY_UP] && map[y - 1][x] == FLOOR){
        if(objMap[y - 1][x] != BOX){ 
            --y;
        } else if(objMap[y - 1][x] == BOX && map[y - 2][x] == FLOOR || objMap[y - 1][x] == BOX && map[y - 2][x] == HOLE) {
            objMap[y - 2][x] = BOX;
            draw_sprite(buffer, box, x * TILE_WIDTH, (y - 2) * TILE_HEIGHT);
            
            //Befindet sich die Kiste in einem Loch ? 
            if(map[y - 2][x] == HOLE) {
                objMap[y - 2][x] = 0;
                
                --boxes;
            }
            
            --y;
        }
        
        needsRefresh = true;   
                
    } else if(key[KEY_DOWN] && map[y + 1][x] == FLOOR){
        
        if(objMap[y + 1][x] != BOX) { 
                ++y;
        } else if(objMap[y + 1][x] == BOX && map[y + 2][x] == FLOOR || objMap[y + 1][x] == BOX && map[y + 2][x] == HOLE) {
            objMap[y + 2][x] = BOX;
            draw_sprite(buffer, box, x * TILE_WIDTH, (y + 2) * TILE_HEIGHT);
            
            if(map[y + 2][x] == HOLE) {
                objMap[y + 2][x] = 0;
                
                --boxes;
            }
            
            ++y;
        }
        
        needsRefresh = true; 
                
    } else if (key[KEY_RIGHT] && map[y][x + 1] == FLOOR) {
        
        if(objMap[y][x + 1] != BOX){ 
                ++x;
        } else if(objMap[y][x + 1] == BOX && map[y][x + 2] == FLOOR || objMap[y][x + 1] == BOX && map[y][x + 2] == HOLE) {
            objMap[y][x + 2] = BOX;
            draw_sprite(buffer, box, (x + 2) * TILE_WIDTH, y * TILE_HEIGHT);
            
            if(map[y][x + 2] == HOLE) {
                objMap[y][x + 2] = 0;
                
                --boxes;
            }
            
            ++x;
        }
        
        needsRefresh = true; 
                
    } else if (key[KEY_LEFT] && map[y][x - 1] == FLOOR) {
        
        if(objMap[y][x - 1] != BOX){ 
                --x;
        } else if(objMap[y][x - 1] == BOX && map[y][x - 2] == FLOOR || objMap[y][x - 1] == BOX && map[y][x - 2] == HOLE) {
            
            objMap[y][x - 2] = BOX;
            draw_sprite(buffer, box, (x - 2) * TILE_WIDTH, y * TILE_HEIGHT);
            
            if (map[y][x - 2] == HOLE) {
                objMap[y][x - 2] = 0;
                
                --boxes;
            }
            
            --x;
        }
        
        needsRefresh = true; 
    }
    
    objMap[tempY][tempX] = 0;
    objMap[y][x] = PLAYER;   
    
    rest(70);
}   

int main() {
    allegro_init();
    install_keyboard();
    
    Setup();
    
    while(!GetAllBoxes && !key[KEY_ESC]) {
        if(boxes > 0) {
            MovePlayer();
            
            // Falls das Spielfeld sich verändert hat, alles neu zeichnen
            if(needsRefresh) {
                DrawMap();
                
                needsRefresh = false;
            }
        } else {
            textout_ex(screen, font, "Du hast gewonnen !!!", SCREEN_W / 2, SCREEN_H / 2, makecol(0, 0, 255), makecol(255, 0, 0));
        
            GetAllBoxes = true;
        
            rest(3000);      
        }
    }
    
    destroy_bitmap(floor);
    destroy_bitmap(wall);
    destroy_bitmap(box);
    destroy_bitmap(buffer);
    
    return 0;
}
END_OF_MAIN();
