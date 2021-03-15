#include <stdio.h>                                                       
#include <string.h>                                                      
#include <stdlib.h>
#define DIM 1026
#define PLUTO 300
//void inserisci_VETT(char *str, int curr);
// N_redo aumenta ogni volta che faccio effettivamente un undo e diminuisce quando faccio redo
//N_undo diminuisce quando facci undo e qumenta quando faccio C o D
typedef struct sidelist{
	char *nstr;              //fare dimensione variabile dipendente da strlen
	struct sidelist *next;
}strnode;

typedef struct node{
	int idx1, idx2, totLINES, totLINES_R;
	strnode *side, *currlist;  //testa della sidelist
	char cmd;
	char **stato;
}URnode;


//var GLOBALI-----------------------------------------------------------
int diff=0, dimensione1=10000, totrighe=0, count=0 , first_saveundo=0, dimensione2=50000, pippo=300;//dimensione tiene conto di quanto è grande per il momento il numero di righe allocate
int UR_count=0 , N_redo=0, N_undo=0;// count aumenta quando chiedo redo e diminuisce quando chiedo undo
URnode **Undo_V, **Redo_V; 
strnode *LP, *CP; 
char *temp;
URnode *lastStato;
char **testo;


//var GLOBALI----------------------------------------------------------

int max(int a, int b);
int min(int a, int b);
void saveUndo(int ind1, int ind2, char command, char *str, int totrighe, char *currstr);
void Undo();
void Redo();
void SaveState();


int main(){

	//FILE *stdin=fopen("Altering_History_2_input.txt","r");


	
	LP = NULL;
	CP = NULL;
	int ind1 = 0, ind2 = 0, curr=0, r_flag=0;
	char S_ind1[DIM], S_ind2[DIM]; //penso che 100 basti
	char str[DIM];
	char cmnd[DIM];
	char instruction;
	testo=(char**)malloc(sizeof(char*)*dimensione1);
	Undo_V=malloc(sizeof(URnode*)*dimensione2);
	Redo_V=malloc(sizeof(URnode*)*dimensione2);



	while(1){

		first_saveundo = 0;

		//riconoscimento del comando da eseguire--------------------------------------------------------------------------------------
		fgets (cmnd, 1026 , stdin);

		if(cmnd[0] == 'q'){
			//ricordarsi di fare le FREE
			return 0;
		}

		int i=0,k=0,j=0;
		while(cmnd[i]!=',' && cmnd[i]!='u' && cmnd[i]!='r'){
			S_ind1[i]=cmnd[i];
			i++;
		}
		S_ind1[i]='\0';

		if(cmnd[i]==','){
			k=i+1;

			while(cmnd[k]!='c' && cmnd[k]!='p' && cmnd[k]!='d'){
				S_ind2[j]=cmnd[k];
				k++;
				j++;
		}
		S_ind2[j]='\0';
		}


		if(cmnd[i] == 'u' || cmnd[i] == 'r'){
				ind1=atoi(S_ind1);                      
				instruction=cmnd[i];
		}
		else{
			ind1=atoi(S_ind1);
			ind2=atoi(S_ind2);
			instruction=cmnd[k]; 
			diff=ind2-ind1+1;
		}

		//--------------------------------------------------------------------------------------------------------------------------





		if( instruction == 'c'){
			char c='\0';
			curr=ind1;         
			
	//----------------- parte dell' undo ----------------------------------------//
			if(UR_count < 0)
				Undo();  //qundo faccio gli undo aumento N_redo e diminuisco N_undo
			else
				if(UR_count>0)
					Redo();  // quando faccio redo aumenta N_undo e diminuisce N_redo

	//----------------- parte dell' undo ----------------------------------------//
			
			

			do{

				fgets(str, DIM , stdin);
				
				if(totrighe>=dimensione1-2){
					dimensione1=dimensione1 + 2000;
					testo=(char**)realloc(testo,sizeof(char*)*dimensione1);
				}


				if(curr>totrighe){
					testo[curr]=NULL;			//nella change mi salvo il testo che c'era scritto precedentemente 
					testo[curr]=(char*)malloc(sizeof(char)*(strlen(str)+1)); 
					saveUndo(curr, ind2, instruction, &c, totrighe, testo[curr]);
					strcpy(testo[curr],str);
					totrighe++;
				}
				else{
					// Save State
					temp = testo[curr];
					testo[curr]=NULL;
					testo[curr]=(char*)malloc(sizeof(char)*(strlen(str)+1));
					saveUndo(curr, ind2, instruction, temp, totrighe, testo[curr]);
					strcpy(testo[curr],str);
				}
				
				curr++;
				diff--;
				

			}while(diff>0);

			if(getchar()!='.'){
	    		printf("errore nel change\n");
	        	return 0;
	    	}
	    	else
	    		getchar(); 

	    	if(N_undo%PLUTO == 0 || N_undo==1)
				SaveState();
	
	    	N_redo=0;     // perchè se sono in una change flusho la redo	
		}



		if(instruction == 'p'){
			
	//----------------- parte dell' undo ----------------------------------------//
			if(UR_count < 0)
				Undo();  //qundo faccio gli undo aumento N_redo e diminuisco N_undo
			else
				if(UR_count>0)
					Redo();  // quando faccio redo aumenta N_undo e diminuisce N_redo

	//----------------- parte dell' undo ----------------------------------------//

			curr=ind1;

			while(diff>0 && curr<=totrighe){
				if(curr == 0 && ind2 == 0)
					break;
				printf("%s", testo[curr]);
				diff--;
				curr++;

			} 

			while(diff>0){
				printf(".\n");
				diff--;
			}
		}




		if(instruction == 'd'){
			
	//----------------- parte dell' undo ----------------------------------------//
			if(UR_count < 0)
				Undo();  //qundo faccio gli undo aumento N_redo e diminuisco N_undo
			else
				if(UR_count>0)
					Redo();  // quando faccio redo aumenta N_undo e diminuisce N_redo

	//----------------- parte dell' undo ----------------------------------------//
		
			int a, b;
			char c;
			a = ind2+1;
			b = ind1;


			if(b>totrighe || ind2==0){
				c = 'A';
				saveUndo(ind1, ind2, c, &c, totrighe, &c);
			}

			if(a<=totrighe){
				while(b<=ind2){
					saveUndo(ind1, ind2, instruction, testo[b], totrighe, &c);
					b++;
				}

				b = ind1;

				while(a<=totrighe){
					testo[b] = testo[a];
					a++;
					b++;
				} 

				totrighe = totrighe-diff;
				
				while(diff>0){
						//testo[b][0] = '\0';
						testo[b] = NULL;
						b++;
						diff--;
					}
			}
			else{
				while(b<=totrighe){
					saveUndo(ind1, totrighe, instruction, testo[b], totrighe, &c);
					//testo[b][0] = '\0';
					testo[b] = NULL;
					b++;
				}

				if(ind1<=totrighe)
					totrighe = ind1-1;
			}
			
			if(N_undo%PLUTO == 0 || N_undo==1)
				SaveState();
	
			N_redo=0;     // perchè se sono in una delete flusho la redo	
		       //aumenta il numero di undo disponibili
		}



		if(instruction == 'u'){
			//printf("zio\n");
				UR_count = UR_count - ind1;
				if(UR_count < 0)
				UR_count=max(UR_count , N_undo*(-1));   // se il numero di undo disponibili è minore di quelli richiesti devo fare quelli disponibili al massimo
														// max perchè sono negativi e quindi ragiono al contrario			
			//printf("%d\n", UR_count);
			}


			if(instruction == 'r'){
				//printf("bella\n");
				UR_count = UR_count + ind1;
				if(UR_count > 0)
					UR_count=min(UR_count , N_redo);
			//printf("%d\n", UR_count);
			}
		
		
		//printf("Totrighe: %d\n", totrighe);
	}//while	
	
	return 0;
}//main





int max(int a, int b){

	if(a>b)
		return a;
	return b;
}

int min(int a, int b){
	if(a<b)
		return a;
	return b;
}

			


void saveUndo(int ind1, int ind2, char command, char *str, int totrighe, char *currstr){ // farla come prima ma con totrighe che aiuta

	if(first_saveundo == 0){
		N_undo++; //ho messo N_undo++ nella save undo e ho tolto il +1 ma non va comunque
		if(N_undo>(dimensione2-6)){
			dimensione2=dimensione2+50000;
			Undo_V=realloc(Undo_V, sizeof(URnode*)*dimensione2);
		}

		if(Undo_V[N_undo] == NULL)
			Undo_V[N_undo] = (URnode*)malloc(sizeof(URnode));
		Undo_V[N_undo]->side = NULL;
		Undo_V[N_undo]->currlist = NULL;
		Undo_V[N_undo]->idx1 = ind1;
		Undo_V[N_undo]->idx2 = ind2;
		Undo_V[N_undo]->cmd = command;
		Undo_V[N_undo]->totLINES=totrighe;      // nuovo 
		Undo_V[N_undo]->stato = NULL;
		first_saveundo = 1;

	}

	strnode *l = (strnode*)malloc(sizeof(strnode));
	l->nstr = str;
	if(Undo_V[N_undo]->side == NULL){  // sidelist appena creata (vuota)
		l->next = NULL;
		Undo_V[N_undo]->side = l;
		LP = l;
	}
	else{              //sidelist che già contiene elementi
		LP->next = l;
		LP = l;
		l->next = NULL;
	}


	if(currstr != "A"){
		strnode *z = (strnode*)malloc(sizeof(strnode));
					//l->nstr=malloc(sizeof(char)*(strlen(testo[ind1+j])+1));
		z->nstr = currstr;
					//testo[ind1+j][0] = '\0';                                                   // non sono sicuro
					//printf("???????: %s\n", l->nstr);


					//printf("out if\n");
		if(Undo_V[N_undo]->currlist == NULL){  // sidelist appena creata (vuota)
						//printf("in if\n");
			z->next = NULL;
			Undo_V[N_undo]->currlist = z;
			CP = z;
		}
		else{              //sidelist che già contiene elementi
			CP->next = z;
			CP = z;
			z->next = NULL;
		}
			//printf("UNDO: %s\n", CP->nstr)
	}				

	//printf("---%s\n", LP->nstr);
}









void Undo(){
	int ind1, ind2, diff, j, a, b, totL;
	char instruction;

	
	//printf("%d\n", N_undo);
	if(UR_count*(-1)>=PLUTO){
		//printf("1\n");
		while(1){

			Undo_V[N_undo]->totLINES_R = totrighe;
			

			if(UR_count*(-1)<PLUTO && Undo_V[N_undo]->stato != NULL){
				//testo = Undo_V[N_undo]->stato;
				for(int i=1; i<=totrighe; i++)
					testo[i] = Undo_V[N_undo]->stato[i];
				//dimensione1 = 5000;

				//printf("---------------------------------------\n");
				//printf("totrighe: %d\n", totrighe);
				//for(int i=1;i<=totrighe;i++)
				//	printf("%s",testo[i]);
				//printf("\n\n");
				//for(int i=1;testo[i]!=NULL;i++)
					//printf("%s",testo[i]);
				//printf("---------------------------------------\n");

				
				//UR_count++;
				//printf("%d\n", N_undo);
				//printf("%d\n", UR_count);
				break;
			}

			totrighe = Undo_V[N_undo]->totLINES;

			//printf("2\n");
			Redo_V[N_redo+1] = Undo_V[N_undo];
			Undo_V[N_undo] = NULL;

			UR_count++; //è negativo, lo stiamo diminuendo 
			N_redo++;
			N_undo--;
		}

			
	}
	




	//printf("%d\n", UR_count);
	while(UR_count<0){
		//printf("1\n");
		if(N_undo<0)
			printf("errore\n");
		if(Undo_V[N_undo]==NULL)
			printf("errore\n");

		
		Undo_V[N_undo]->totLINES_R = totrighe;
		

		instruction = Undo_V[N_undo]->cmd;
		ind1 = Undo_V[N_undo]->idx1;
		ind2 = Undo_V[N_undo]->idx2;
		diff = (ind2-ind1)+1;
		totL=Undo_V[N_undo]->totLINES; // il numero di righe che c'erano prima di fare l'istruzione di cui ora sto facendo l'undo



		if(instruction == 'c'){   //undo della change
			int j=0;
			
			LP=Undo_V[N_undo]->side;

			while(diff>0){
				testo[ind1+j] = LP->nstr;
				
				LP = LP->next;
				j++;
				diff--;

			}

		}


		if(instruction == 'd'){  //undo della delete
			a = totrighe;
			b = totL; //totrighe+diff;
			while(a>=ind1){   
				testo[b] = testo[a];           
			
				b--;
				a--;
			}


			a = a+1;
			LP=Undo_V[N_undo]->side;   
			while(diff>0){
				testo[a] = LP->nstr;
				LP = LP->next;
				a++;
				diff--;
			}

		}


		//spostare dal vettore dell'undo a quello del redo il nodo
		totrighe=totL;

		Redo_V[N_redo+1] = Undo_V[N_undo];
		Undo_V[N_undo] = NULL;


		UR_count++;
		N_undo--;
		N_redo++;

		
		
	} // while

	//printf("N_redo: %d\n", N_redo);
	//printf("N_undo: %d\n", N_undo );
	//printf("totrighe end while: %d\n", totrighe );
} //parentesi UNDO








void Redo(){
	//printf("N_redo2: %d\n",N_redo);
	//printf("UR_count: %d\n",UR_count);


	int ind1, ind2, diff, j, a, b, totL_R;
	char instruction;

	if(UR_count>=PLUTO){
		//printf("talla\n");
		while(1){
			//printf("rota\n");

			if(UR_count<PLUTO && Redo_V[N_redo]->stato != NULL){
				//printf("laca\n");
				for(int i=1; i<totrighe+100; i++)
					testo[i] = Redo_V[N_redo]->stato[i];
				//printf("---------------------------------------\n");
				//printf("totrighe: %d\n", totrighe);
				//for(int i=1;i<=totrighe;i++)
					//printf("%s",testo[i]);
				//printf("\n\n");
				//for(int i=1;testo[i]!=NULL;i++)
					//printf("%s",testo[i]);
				//printf("---------------------------------------\n");


				//testo = Redo_V[N_redo]->stato;
				//UR_count;
				break;
			}

			totrighe = Redo_V[N_redo]->totLINES_R;

			Undo_V[N_undo+1] = Redo_V[N_redo];
			Redo_V[N_redo] = NULL;
			UR_count--; //è negativo, lo stiamo diminuendo 
			N_redo--;
			N_undo++;
		}
	}
	//printf("bella\n");
	//printf("%d\n", UR_count);

	//printf("REDO\n");
	//printf("N_undo: %d\n", N_undo);
	//printf("UR_count: %d\n", UR_count);
	//printf("N_redo: %d\n", N_redo);

	while(UR_count>0){

		instruction = Redo_V[N_redo]->cmd;
		ind1 = Redo_V[N_redo]->idx1;
		ind2 = Redo_V[N_redo]->idx2;
		diff = (ind2-ind1)+1;
		totL_R = Redo_V[N_redo]->totLINES_R;


		if(instruction == 'c'){   //redo della change
			int j=0;
			
			LP=Redo_V[N_redo]->currlist;

			while(diff>0){
				//printf("ind1: %d, ind2: %d\n", ind1, ind2);
				//printf("text-> %s\n", LP->nstr);
				//if(Redo_V[N_redo]->side->nstr[0] == '\0')       // forse va fatto Redo_V[N_redo]->side->nstr == NULL;
				//	totrighe++;
				
				testo[ind1+j] = LP->nstr;

				LP = LP->next;
				j++;
				diff--;
			}
		}


		if(instruction == 'd'){
			int a, b;
			char c;
			a = ind2+1;
			b = ind1;

			if(a<=totrighe){
				while(a<=totrighe){
					testo[b] = testo[a];	
					a++;
					b++;
				} 

				totrighe = totrighe-diff;
				
				while(diff>0){
						//testo[b][0] = '\0';
						testo[b] = NULL;
						b++;
						diff--;
					}
			}
			else{//piglia 2 casi, tutto fuori e mezzo
				while(b<=totrighe){
					//testo[b][0] = '\0';
					testo[b] = NULL;
					b++;
				}

				if(ind1<=totrighe)
					totrighe = ind1-1;
			}
		}

		//spostare dal vettore dell'undo a quello del redo il nodo	
		Undo_V[N_undo+1] = Redo_V[N_redo];
		Redo_V[N_redo] = NULL;	

		UR_count--;
		N_redo--;
		N_undo++;

		totrighe = totL_R;
		//printf("%d\n", totrighe); //credo ci sia un problema quando salviamo totLINES_R
		//printf("%d\n",N_redo );
		//printf("rita\n");
		}// while
}// redo




void SaveState(){
	//SaveState-------------------------------------------------------------------------
	//printf("---------------------------------------\n");
	//printf("bella pello stato\n");
	//printf("N_undo: %d\n", N_undo);
		Undo_V[N_undo]->stato = NULL;
		Undo_V[N_undo]->stato = (char**)malloc(sizeof(char*)*(totrighe+5000));
		for(int i=1; testo[i] != NULL; i++)
			Undo_V[N_undo]->stato[i] = testo[i];

				//printf("totrighe: %d\n", totrighe);
		//for(int i=1;i<=totrighe;i++)
			//printf("%s",testo[i]);
			//printf("\n\n");
		//for(int i=1;testo[i]!=NULL;i++)
			//printf("%s",testo[i]);
			//printf("---------------------------------------\n");

		
				
		//SaveState-------------------------------------------------------------------------
}