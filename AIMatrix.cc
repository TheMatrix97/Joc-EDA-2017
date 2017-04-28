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
	bool moto_vecina(vector<int>&a, set<int>&loc_motos){ //si hay motos vecinas, hay que evitar los vecinos compartidos
		for(int i = 0; i < (int)a.size(); i++){
			if(vertex(a[i]).bike != -1) loc_motos.insert();
		}
	}
		
				
			
			
			
			
			
		 
		
		
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
			for (int i = 0; i < (int)neighbours.size(); i++) {
				int id = neighbours[i];
				if (vertex(id).wall == -1) {
					empty_neighbours.push_back(id);
				}
			}
				
			
			// Create an empty movement
			Movement movement(my_bike.id);
			// Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
			
			if (!empty_neighbours.empty()) {
				
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

