#include "network.h"
#include "random.h"
#include "iostream"
using namespace std;


/*
  This is a network of nodes with bidirectional links: if <b>(a, b)</b> is a link then <b>(b, a)</b> is also a link.
	//if link between node a and b exists then node b to a also excists
	//<b> is to put in bold in the doxygen file
	// links: relie indices éléments
  Each node supports a *double* value, and the list of nodes is in fact defined by the list of their values.
 */


// Resizes the list of nodes (\ref values) and also resets all values.
//After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
 
    void Network::resize(const size_t &n){
		links.clear();
		values.resize(n);//rajouter des noeuds ou en enlever
		RNG.normal(values,0,1);
	}
	
	
/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted (true if both nodes exist and the link did not exist yet)
 */
    bool Network::add_link(const size_t& indiceA, const size_t& indiceB){
		
		//a ne peut pas faire lien avec a
		if(indiceA == indiceB){return false;}
		
		//indiceA et indiceB doivent être dans tableau  //pas plus de lien que de valeurs
		if(indiceA >= size() or indiceB >= size()){return false;}
		
		//lien est fait seulement si pas déjà présent
		auto range = links.equal_range(indiceA);
		for (auto it=range.first; it!=range.second; ++it){
			if(it->second == indiceB){ return false;} 
		}
		
		
		//lien bidirectionnel: à partir de a 
		links.insert(pair<const size_t&,const size_t&>(indiceA,indiceB)); 
		links.insert(pair<const size_t&,const size_t&>(indiceB,indiceA)); 
		return true;
	}
	
	
/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
   @param[out] number of links created.
 */
 

   size_t Network::random_connect(const double& mean_deg){
	 links.clear();
	  
	 for(size_t i(0); i<size() ; ++i){                                         
		 int nb_liens(RNG.poisson(mean_deg)); 
		 while(nb_liens>=size()-1){ 
			 nb_liens=RNG.poisson(mean_deg);
		}
		
		int nb(0);  
	                       
			 while(nb<nb_liens){   
				int indice=RNG.uniform_double(0, size()-1);       
				if(add_link(i, indice)){
				 ++nb;
				}
			}
		 
	 }
	 return links.size()/2;
 }
		
		
/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
    size_t Network::set_values(const std::vector<double>& new_values){
		values.clear();
		links.clear(); 
		
		if(new_values.empty()){
			return 0;
		}
		
		for(auto i: new_values){
			values.push_back(i);
		}
		
		return size();
	}
    
/*! Number of nodes */
    size_t Network::size() const{
		return values.size();
	}
    
/*! Degree (number of links) of node no *n* */
    size_t Network::degree(const size_t &_n) const{
		return links.count(_n);
	}
    
/*! Value of node no *n* */
    double Network::value(const size_t &_n) const{
		if(_n>= size()){
			return 0;
		}
		return values[_n];
		
	}
/*! All node values in descending order */
    std::vector<double> Network::sorted_values() const{
		vector<double> ordered;
		
		for(auto i: values){
			ordered.push_back(i);
		}
		
		sort(ordered.begin(), ordered.end(), greater<double>());
		return ordered;
	}
		
/*! All neighbors (linked) nodes of node no *n* */
    std::vector<size_t> Network::neighbors(const size_t& indice) const{
		vector<size_t> neighbors;
		if(indice >= size()){
			return neighbors;
		}
		
		auto range = links.equal_range(indice);
		
		for (auto it=range.first; it!=range.second; ++it){
			neighbors.push_back(it->second);
		}
		return neighbors;
	}



