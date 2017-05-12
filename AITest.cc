#include "Player.hh"

#include <cmath>
#include <assert.h>
#include <set>
#include <list>

using namespace std;

/**
 * Write the name of your player and save this file
 * with a name matching the format "AIyourname.cc".
 */
#define PLAYER_NAME Matrix2v1


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
     

	list<int> busqueda_cami(int ini, int bonus){ //BFS CAMINS MINIMS
		vector<bool> vis(nb_vertices(),false);
		queue< list<int> > cola;
		list<int> aux;
		aux.push_back(ini);
		cola.push(aux);
		while(not cola.empty()){
			list<int> path = cola.front();
			int last_nodo = path.back();
			cola.pop();
			vector<int> vecino_last = vertex(last_nodo).neighbours;
			for(int i = 0; i < (int)vecino_last.size();i++){
				list<int> new_path = path;
				if(not vis[vecino_last[i]] and vecino_last[i] != last_nodo and vertex(vecino_last[i]).wall == -1){
					vis[vecino_last[i]] = true;
					new_path.push_back(vecino_last[i]);
					if(vecino_last[i] == bonus) return new_path;
					cola.push(new_path);
				}
				else if(vertex(vecino_last[i]).wall == -1) vis[vecino_last[i]] = true;
			}
		}
		list<int> fail;
		fail.push_back(-1);
		return fail;
					
			
	}
	
	int calcular_camino_bonus(int ini){
		list<int> recc;
		vector<int> bonus = bonus_vertices();
		for(int i = 0; i < (int)bonus.size(); i++){
			recc = busqueda_cami(ini,bonus[i]);
			int aux = recc.front();
			recc.pop_front();
			if(aux != -1) return recc.front();
		}
		return -1;
	}
		

     bool validar_vertex(int id, int my_bike_id){
		 vector<int> vecinos = vertex(id).neighbours;
		 bool pared = true; //damos por hecho que no sepuede ir pork es un camino sin salida
		 for(int i = 0; i < (int)vecinos.size(); i++){
			 if(vertex(vecinos[i]).bike != -1 and vertex(vecinos[i]).bike != my_bike_id) return false;
			 if(vertex(vecinos[i]).wall == -1 and bike(my_bike_id).vertex != vecinos[i]) pared  = false; //no hay pared
		 }
		 if(pared) return false;
		 return true;
	 }
		 

	set<int> validar_vecinos(vector<int> &vecinos, int my_bike_id){
		set<int> aux;
		for(int i = 0; i < (int)vecinos.size(); i++){ //investigar si en ese vecino vacio cabe la poss de que llege una moto enemiga
			if(validar_vertex(vecinos[i], my_bike_id)){
				aux.insert(vecinos[i]);
			}
		}
		return aux;
	}

	int motos_vivas(){
		int contador = 0;
		int players = nb_players();
		for(int i = 0; i < players; i++){
			vector<int> motos = bikes(i);
			for(int j = 0; j < (int)motos.size(); j++){
				if(bike(motos[j]).alive) contador++;
			}

		}
		return contador;
	}
					
			
			/*IDEAS:
			 * MIRAR SCREENSHOT EN IMAGES, HAY QUE CAMBIAR EL CODIGO PARA EVITAR CRASHES, HABRIA QUE MIRAR SI EL VECINO X TIENE ALGUNA MOTO DE VECINO EXCEPTO TU, SI ES ASI NO QUEREMOS IR!!!
			 * EN EL SCREEN LAS MOTOS NO SE ESQUIVAN PORK NO SON VECINAS*/
	void max_path(int i, vector<int> dist, int &max){
		queue<int> v; //cola bfs de vertices a visitar
		dist[i] = 0;
		v.push(i);
		while(not v.empty()){
			int ref = v.front();
			v.pop(); //estudiamos los adjacentes
			vector<int> vecinos = vertex(ref).neighbours;
			for(int i = 0; i < (int)vecinos.size(); i++){
				if(vertex(vecinos[i]).wall == -1 and dist[vecinos[i]] == -1 and vecinos[i] != ref){
					v.push(vecinos[i]);
					dist[vecinos[i]] = dist[ref] + 1;
					if(dist[vecinos[i]] > max) max = dist[vecinos[i]];

				}
			}
		}
	}	
	int test_bfs(set<int> &vecinos_seguros){ //devuelve el vertex con mas caminos
		set<int>::iterator it;
		int max_i = *vecinos_seguros.begin();
		int max_pathv = 0;
		for(it = vecinos_seguros.begin(); it != vecinos_seguros.end(); it++){
			int max = 0;
			vector<int> dist(nb_vertices(),-1);
			max_path(*it,dist,max);
			if(max > max_pathv){
				max_pathv = max;
				max_i = *it;
			}
			cerr << "TEST BFS: " << endl;
			cerr << "vecino: " << *it << " max_path = " << max << endl;
		}
		return max_i;

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
			vector<int> motos_vecinas;
			
			for (int i = 0; i < (int)neighbours.size(); i++) {
				int id = neighbours[i];
				if (vertex(id).wall == -1) {
					if(id != my_bike.vertex) empty_neighbours.push_back(id);
				}
				if(vertex(id).bike != -1) motos_vecinas.push_back(id);
			}
			
					
			// Create an empty movement
			Movement movement(my_bike.id);
			
			// Set next_vertex to a random empty neighbour, if any, or to a random neighbour otherwise
			if (!empty_neighbours.empty()) { //mirar si tenemos una moto vecina, despues mirar si hay vecinos compartidos
				set<int> vecinos_seguros = validar_vecinos(empty_neighbours,my_bike.id);
				set<int>::iterator it = vecinos_seguros.begin();
				if(not vecinos_seguros.empty()){ //vamos a un pos random valida
					movement.next_vertex = test_bfs(vecinos_seguros);
				}
				else{
					movement.next_vertex = empty_neighbours[ rand() % (int)empty_neighbours.size() ];
					
				} 
				
			} else {
				cerr << "no vecino libre" << endl; //muerte segura, igual se puede evitar con ghost
				if(my_bike.bonus == Ghost){
					vector<int> vecinos_sin_owner;
					for(int i = 0; i < (int)neighbours.size();i++){
						if(neighbours[i] != my_bike.vertex) vecinos_sin_owner.push_back(neighbours[i]);
					}
					movement.next_vertex = vecinos_sin_owner[rand() % (int)vecinos_sin_owner.size()]; 
					movement.use_bonus = true;
				} 
				
				else movement.next_vertex = neighbours[ rand() % (int)neighbours.size() ];
			}
			
			// Use bonus randomly when we have one
			//bonus si ghost->habria que guardarlo para cuando no queden vecinos libres
			//si el bonus es turbo usarlo en el momento
			//si el bonus es puntos dobles esperar a que hayan pocos pilotos para evitar crash? :S
		
			if(my_bike.bonus == Points) movement.use_bonus = true;
			else if(my_bike.bonus == Turbo){
				double aux;
				aux = 0.75*nb_rounds(); //estamos a punto de acabar
				if(round() >= aux or motos_vivas() == 1){
					movement.use_bonus = true;
				}
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