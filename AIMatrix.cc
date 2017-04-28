#include "Player.hh"

#include <cmath>
#include <assert.h>
#include <set>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Matrix


struct PLAYER_NAME : public Player {

    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }


    /**
     * Attributes for your player can be defined here.
     */     
    //vector<int> my_awesome_vector_of_integers;


    /**
     * Play method.
     * 
     * This method will be invoked once per each round.
     * You have to read the board and place your actions
     * for this round.
     */
  /*   bool is_mybike(vector<int> motos_vecinas, int &i){
		int i = 0;
		for(int j = 0; j < (int)motos_vecinas.size(); j++){
			if(bikes(motos_vecinas[j]).player == me()) {i = j; return true;}
		}
		return false;
	 
	 
	 }*/
     //pre a contiene vecinos vacios de n
     //post b contiene vecinos vacios no amenazados por otras motos
    void bfs_dist2(vector<int> &a, set<int> &b){
		for(int i = 0; i < (int)a.size(); i++){
			vector<int> vecinos2 = vertex(a[i]).neighbours; //vecinos2 contiene 
			int j = 0;
			while(j < (int)vecinos2.size()){
				if(vertex(vecinos2[j]).bike == -1 and vecinos2[j] != a[i]) b.insert(a[i]);
				j++;
			}
		}
	}
	set<int> evitar_choque(vector<int>&a, vector<int>&loc_motos){
		set<int> aux;
		for(int i = 0; i < (int)loc_motos.size(); i++){
			for(int j = 0; j < (int)a.size(); j++){ //encontrar coincidencias entre vecinos
					if(a[j] != loc_motos[i] and not is_neighbour(a[j], loc_motos[i])){
						 cerr << a[j] << ' ' << loc_motos[i] << "no son vecinos"; 
						 aux.insert(a[j]); //seguro que no crasheo, pork no son vecinos
				}
			}
		}

		return aux;
	}
				
			
			
			
			
			/*IDEAS:
			 * MIRAR SCREENSHOT EN IMAGES, HAY QUE CAMBIAR EL CODIGO PARA EVITAR CRASHES, HABRIA QUE MIRAR SI EL VECINO X TIENE ALGUNA MOTO DE VECINO EXCEPTO TU, SI ES ASI NO QUEREMOS IR!!!
			 * EN EL SCREEN LAS MOTOS NO SE ESQUIVAN PORK NO SON VECINAS*/
			
		
		
    void play () {
		
        vector<int> my_bikes = bikes(me());
        for (int i = 0; i < (int)my_bikes.size(); ++i) {
			
			const Bike& my_bike = bike(my_bikes[i]);
			
			// Do something only if this bike is alive
			if (!my_bike.alive) {
				continue;
			}
			
			//Only turbo bikes can move at odd rounds
			if (round() % 2 && my_bike.turbo_duration <= 0) {
				continue;
			}
			
			// Find all empty neighbours
			vector<int> neighbours = vertex(my_bike.vertex).neighbours;
			vector<int> empty_neighbours;
			vector<int> motos_vecinas;
			
			for (int i = 0; i < (int)neighbours.size(); i++) {
				int id = neighbours[i];
				if (vertex(id).wall == -1) {
					empty_neighbours.push_back(id);
				}
				if(vertex(id).bike != -1) motos_vecinas.push_back(id);
			}
				
			
			// Create an empty movement
			Movement movement(my_bike.id);
			// Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
			if (!empty_neighbours.empty()) {
				if(motos_vecinas.empty()){ //si no hay motos vecinas
					set<int> empty_safe;
					bfs_dist2(empty_neighbours, empty_safe); //miramos vecinos seguros para evitar choques entre pilotos
					if(not empty_safe.empty()){
						cerr << "soy la moto " << my_bike.id << endl;
						cerr << "estoy en: " << my_bike.vertex << endl;
						cerr << "puedo ir a: ";
						set<int>::iterator it;
						for(it = empty_safe.begin(); it != empty_safe.end(); it++) cerr << *it << ' ';
						cerr << endl;
						//assert(0);
						movement.next_vertex = *empty_safe.begin();
					}
					else{
						cerr << "no vecino seguro" << endl;
						 movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
					 }
				 }else{
					 
					 cerr << "cuidado possible crash!!! con "  << endl;
					 set<int> safe_loc;
					 safe_loc = evitar_choque(empty_neighbours, motos_vecinas);
					 cerr << "no me estrellaré si voy a  "; 
					 set<int>::iterator it;
					 for(it = safe_loc.begin(); it != safe_loc.end(); it++) cerr << *it << ' ';
					 if(not safe_loc.empty()){
						  int ir_a = rand() % (int)safe_loc.size();
						  int contador = 0;
				
						  for(it = safe_loc.begin(); it != safe_loc.end(); it++){
							  if(contador == ir_a)  movement.next_vertex = *it;
							  else contador++;
						  }
					  }
					 else{ //evitar crashes entre mis pilotos a toda costa
						 movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
					 }
				 }
				
			} else {
				cerr << "no vecino libre" << endl;
				//estaria guay poder prever possibles choques entre otros pilotos y evitar que se quede encerrado
				movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
			}
			
			// Use bonus randomly when we have one
			//bonus si ghost->habria que guardarlo para cuando no queden vecinos libres
			//si el bonus es turbo usarlo en el momento
			//si el bonus es puntos dobles esperar a que hayan pocos pilotos para evitar crash? :S
			if (my_bike.bonus != None && rand()%5 > 3) {
				movement.use_bonus = true;
			}
			
			// Command the movement
            command(movement);
			
        }
        
    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

