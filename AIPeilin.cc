#include "Player.hh"
#include <queue>
#include <vector>
#include <string>
/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME redQueen03

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
   const vector<Dir> dirs ={Up, Down, Left, Right};

   typedef vector<bool> VVB;
   typedef vector<VVB>VB;

/*BFS look for BONUS*/
Dir bfs_bonus_nearby(Pos &pos, bool &found){
  VB visited(board_rows(), VVB(board_cols(), false));
  queue<pair<Pos, Dir> > Q;
  int id= cell(pos).id;
  int primers=0;
  Dir d1= Up, d2= Down, d3= Left, d4= Right;
  if(pos_ok(pos+d1) and cell(pos+d1).type!=Building) Q.push(make_pair(pos, d1));
  if(pos_ok(pos+d2) and cell(pos+d2).type!=Building) Q.push(make_pair(pos, d2));
  if(pos_ok(pos+d3) and cell(pos+d3).type!=Building) Q.push(make_pair(pos, d3));
  if(pos_ok(pos+d4) and cell(pos+d2).type!=Building) Q.push(make_pair(pos, d4));

  while(!Q.empty()){
    Pos p= Q.front().first;
    Dir d0= Q.front().second;
    Q.pop();

    if (cell(p).bonus==Money or cell(p).bonus==Food){
      found= true;
      return d0;
    }
    for (Dir d: dirs){
      if (pos_ok(p+d) and cell(p+d).id==-1 and (cell(p+d).type!=Building or cell(p+d).b_owner==me()) and (not visited[(p+d).i][(p+d).j])){
        visited[(p+d).i][(p+d).j]= true;
        if (primers < 4){
          d0= d;
          ++primers;
        }
        Q.push(make_pair(p+d, d0));
      }
    }
  }
  found= false;
  Dir d= Left;
  return d;
}
 
    /* BFS look for weapon BAZOOKA */
Dir bfs_weapon_nearby(Pos &pos, bool &found){
  VB visited(board_rows(), VVB(board_cols(), false));
  queue<pair<Pos, Dir> > Q;
  int id= cell(pos).id;
  int primers=0;
  Dir d1= Up, d2= Down, d3= Left, d4= Right;
  if(pos_ok(pos+d1) and cell(pos+d1).type!=Building) Q.push(make_pair(pos, d1));
  if(pos_ok(pos+d2) and cell(pos+d2).type!=Building) Q.push(make_pair(pos, d2));
  if(pos_ok(pos+d3) and cell(pos+d3).type!=Building) Q.push(make_pair(pos, d3));
  if(pos_ok(pos+d4) and cell(pos+d2).type!=Building) Q.push(make_pair(pos, d4));

  while(!Q.empty()){
    Pos p= Q.front().first;
    Dir d0= Q.front().second;
    Q.pop();

    if (cell(p).weapon==Bazooka){
      found= true;
      return d0;
    }
    for (Dir d: dirs){
      if (pos_ok(p+d) and cell(p+d).id==-1 and (cell(p+d).type!=Building or cell(p+d).b_owner==me()) and (not visited[(p+d).i][(p+d).j])){
        visited[(p+d).i][(p+d).j]= true;
        if (primers < 4){
          d0= d;
          ++primers;
        }
        Q.push(make_pair(p+d, d0));
      }
    }
  }
  found= false;
  Dir d= Left;
  return d;
}

/*BFS look for ENEMIES*/
Dir bfs_enemy_nearby(Pos &pos, bool &found){
  VB visited(board_rows(), VVB(board_cols(), false));
  queue<pair<Pos, Dir> > Q;
  int primers=0;

  for (Dir d : dirs) {
    if(pos_ok(pos+d) and cell(pos+d).type!=Building) Q.push(make_pair(pos, d));
  }

  while(!Q.empty()){
    Pos p= Q.front().first;
    Dir d0= Q.front().second;
    Q.pop();
    int id = cell(p).id;

    if (id!=-1 and citizen(id).player!=me() and (citizen(id).type==Warrior or citizen(id).type==Builder)){
      found= true;
      return d0;
    }
    for (Dir d: dirs){
      if (pos_ok(p+d) and (cell(p+d).type!=Building or cell(p+d).b_owner==me()) and not visited[(p+d).i][(p+d).j]){
        visited[(p+d).i][(p+d).j]= true;
        if (primers < 4){
          d0= d;
          ++primers;
        }
        Q.push(make_pair(p+d, d0));
      }
    }
  }
  found= false;
  Dir d= Left;
  return d;  
}


  /*BFS look for barricade*/
Dir bfs_barricade_nearby(Pos &pos, bool &found){
  VB visited(board_rows(), VVB(board_cols(), false));
  queue<pair<Pos, Dir> > Q;
  int primers=0;

  for (Dir d : dirs) {
    if(pos_ok(pos+d) and cell(pos+d).type!=Building) Q.push(make_pair(pos, d));
  }

  while(!Q.empty()){
    Pos p= Q.front().first;
    Dir d0= Q.front().second;
    Q.pop();
    int id = cell(p).id;

    if (cell(p).b_owner==me()){
      found= true;
      return d0;
    }
    for (Dir d: dirs){
      if (pos_ok(p+d) and cell(p+d).type!=Building and not visited[(p+d).i][(p+d).j]){
        visited[(p+d).i][(p+d).j]= true;
        if (primers < 4){
          d0= d;
          ++primers;
        }
        Q.push(make_pair(p+d, d0));
      }
    }
  }
  found= false;
  Dir d= Left;
  return d;  
}

  /* Build barricades */
int construir(Pos& p, int& id, bool &built){
  for(Dir d: dirs){
    if(pos_ok(p+d) and cell(p+d).is_empty()){
      build(id, d);
      //move(id, d);
      built= true;
      return cell(p+d).resistance;
    }
  }
}

/* Adjacent enemy to barricade */
void run_or_attack_barricade(Pos& p){
  int my_id= cell(p).id;
  /*for (Dir d: dirs){
    if (pos_ok(p+d) and citizen(cell(p+d).id).player!=me()){
      if 
    }
  }*/
  // Derecha
  if ((pos_ok((p.i)+1, p.j)) and (citizen(cell((p.i)+1, p.j).id).player != me())){
    if (citizen(cell((p.i)+1, p.j).id).type == Warrior) move(my_id, Left);
    else if (citizen(cell((p.i)+1, p.j).id).type == Builder) move(my_id, Right);
  }
  // Izquierda
  else if ((pos_ok((p.i)-1, p.j)) and (citizen(cell((p.i)-1, p.j).id).player != me())){
    if (citizen(cell((p.i)-1, p.j).id).type == Warrior) move(my_id, Right);
    else if (citizen(cell((p.i)-1, p.j).id).type == Builder) move(my_id, Left);
  }
  // Arriba
  else if ((pos_ok((p.i), p.j+1)) and (citizen(cell((p.i), p.j+1).id).player != me())){
    if (citizen(cell((p.i), p.j+1).id).type == Warrior) move(my_id, Down);
    else if (citizen(cell((p.i), p.j+1).id).type == Builder) move(my_id, Up);
  }
  // Abajo
  else if ((pos_ok((p.i), p.j-1)) and (citizen(cell((p.i), p.j-1).id).player != me())){
    if (citizen(cell((p.i), p.j-1).id).type == Warrior) move(my_id, Up);
    else if (citizen(cell((p.i), p.j-1).id).type == Builder) move(my_id, Down);
  }
}


  /**
   * Play method, invoked once per each round.
   */

  virtual void play () {

    double st = status(me());
    if (st >= 0.9) return;

    vector<int> b = builders(me());
    vector<int> w = warriors(me());
    vector<Pos> brkde = barricades(me());

    //MODO DIA
    if (is_day()) {
      // Estrategia builder: 
      // Tiene prioridad la comida a partir de cierta vida. En caso contrario matar otros builders. 
      // Piorizan la busqueda de dinero antes que de armas
      // Crear barricadas en espacios random de 2o nivel
      for(int id : b){
        bool found= false;
        Pos p= citizen(id).pos;
        bool built= false;
        Dir dir2= bfs_enemy_nearby(p, found);
        if (brkde.size() < 3){
          if(found){
            int res=0;
            while (res< barricade_max_resistance()/2) res=construir(p, id, built);
          }
        }
        Dir dir= bfs_bonus_nearby(p, found);
        if (found) move(id, dir);
        Dir dir1= bfs_weapon_nearby(p, found);
        if(found) move(id, dir1);
      }
      // Estrategia warrior: 
      //Prioriza busqueda de bazooka.
      //Cada amanecer busca comida hasta límite. 
      //Priorizan la busqueda de armas para deshabilitar a los enemigos
      for (int id : w) {
        bool found= false;
        Pos p= citizen(id).pos;
        Dir dir= bfs_weapon_nearby(p, found);
        if(found) move(id, dir); //hacer que priorice bazooka
        Dir dir1= bfs_bonus_nearby(p, found);
        if (found) move(id, dir1);
        Dir dir2= bfs_enemy_nearby(p, found);
        if(found) move(id, dir2);
        
      }
    }

    //MODO NOCHE
    else if (is_night()) {
      //Estrategia builder: 
      //Buscar la barricada más cercana y quedarse cerca
      //Si alguien se acerca a la barricada huye sentido contrario antes que destruya la barricada
      //Si se acerca enemigo builder ataca hasta que me quede poca vida
      for (int id : b) {
        bool found= false;
        Pos p= citizen(id).pos;
        Dir dir0= bfs_barricade_nearby(p, found);
        if(found and cell(p+dir0).id==-1){
          move(id, dir0);
          //run_or_attack_barricade(p);
        } 
        else {
          Dir dir1= bfs_bonus_nearby(p, found);
          if (found) move(id, dir1);
        }

      }
      //Estrategia warrior: 
      //Jugador con bazooka: Prioriza las víctimas con bazooka 
      //Jugador 
      //Si está en punto crítico se esconde en una barricada. 
      //
      for (int id : w) {
        bool found= false;
        Pos p= citizen(id).pos;
        Dir d= bfs_enemy_nearby(p, found);
        if(found) move(id, d);
      }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
