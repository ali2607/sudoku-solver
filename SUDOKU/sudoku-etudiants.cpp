#include <iostream>
#include <vector>
#include <string>
#include <fstream> // pour les lectures-écritures dans les fichiers

using namespace std;

typedef struct {
  string nom;
  vector<vector<int>> grille;
} Grille;

typedef struct {
  size_t lig;
  size_t col;
} Indices;


// insérer ici les fonctions que vous appelez avant que leurs definitions apparaissent dans le fichier

/*void affiche_vect(vector<int> a)
{
    for(size_t i=0;i< a.size(); i+=1)           EN CAS DE BESOIN
    {
        cout<<a.at(i)<<" ";
    }
}*/
void imprime(const Grille &g); // definie en fin de fichier.


/*
*indique si la grille ne contient que des chiffres et est de la  bonne forme (taille 9x9)
*@param g, la grille de sudoku
*Return true ssi la grille à la bonne forme et ne contient que des valeurs de 0 à 9
*/
bool bienFormee(const Grille &g) {
  bool res = true;
  if(g.grille.size()==9)
  {
      for(size_t i=0;i<g.grille.size();i+=1)
      {
          if(g.grille.at(i).size()!=9)
          {
              res=false;
          }
          for(size_t j=0;j<g.grille.at(i).size();j+=1)
          {
              if(g.grille.at(i).at(j)>9 || g.grille.at(i).at(j)<0)
              {
                  res=false;
              }
          }

      }
  }
  else
  {
      res=false;
  }
  return res;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//      PARTIE "VALIDE"
/*
*indique si une colonne donnée d'un sudoku est valide
*@param g la grille de sudoku, j l'indice de la colonne
*return true si la colonne est valide et faux sinon
*/
bool colonnevalide(const Grille &g,size_t j)
{
    bool res= true;
    for(size_t i=0;i<9;i+=1)
    {
      for(size_t k=i+1;k<9;k+=1)
      {
            if((g.grille.at(i).at(j)!=0)&&(g.grille.at(i).at(j)==g.grille.at(k).at(j)))
            {
                res=false;
            }
      }

    }
    return res;
}
/*
*indique si une ligne donnée d'un sudoku est valide
*@param g la grille de sudoku, i l'indice de la ligne
*return true si la ligne est valide et faux sinon
*/
bool lignevalide(const Grille &g,size_t i)
{
    bool res = true;
    for(size_t j=0;j<9;j+=1)
    {
        for(size_t k=j+1;k<9;k+=1)
        {
            if((g.grille.at(i).at(j)!=0)&&(g.grille.at(i).at(j)==g.grille.at(i).at(k)))
            {
                res=false;
            }
        }
    }
    return res;
}
/*
*indique si une région donnée d'un sudoku est valide
*@param g la grille de sudoku, colonne et ligne, les indices de la première case d'une région
*return true si la région est valide et faux sinon
*/
bool regionvalide(const Grille &g,int colonne,int ligne)
{
    //on transforme une région en vector
    vector<int> region (0);
    for(int i=colonne; i<colonne+3;i+=1)
    {
        for(int j=ligne;j<ligne+3;j+=1)
        {
            region.push_back(g.grille.at(j).at(i));
        }
    }

     bool res = true;
     // on réapplique l'algorithme de lignevalide sur la région transformée en vector
    for(int i=0;i<9;i+=1)
    {
        for(int k=i+1;k<9;k+=1)
        {
            if(region.at(i)!=0 && region.at(i)==region.at(k))
            {
                res=false;
            }
        }
    }
    return res;
}




/* Verifie si la grille est valide : chaque valeur de 1 à 9 apparait au plus une fois dans chaque ligne, colonne et région
 *@param g la grille de sudoku
 *return true si la grille est valide et faux sinon
 */
bool valide(const Grille &g) {
    bool res = true;
    for(int i=0;i<9;i+=1)
    {
        if(!lignevalide(g,i))
        {
            res=false;
        }
        for(int j=0;j<9;j+=1)
        {
            if(!colonnevalide(g,j))
            {
                res=false;
            }
            if(i%3==0 && j%3==0)
            {
                if(!regionvalide(g,i,j))
                {
                    res=false;
                }
            }

        }
    }
    if(!bienFormee(g))
    {
        res=false;
    }
  return res;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------
//      PARTIE "POSSIBLES"


/*
*indique les valeurs possibles pour une case d'indice donné
*@param g, la grille de sudoku
*@param ind, le couple d'indice de la case (ligne et colonne)
*return le vecteur des valeurs disponibles pour le couple d'indices ind,compte-tenu des valeurs placées sur sa ligne, colonne et sa région
*/
vector<int> possibles(const Grille &g, Indices ind) {
  Grille temp = g;
  vector<int> res;
  if (temp.grille.at(ind.lig).at(ind.col)==0)
  {
      for(int i =1; i<10;i+=1 )
      {
        temp.grille.at(ind.lig).at(ind.col)=i;
        if(valide(temp))
        {
            res.push_back(i);
        }
      }
  }
  return res;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//      PARTIE "FORCE"

/* reçoit en argument le vecteur des valeurs possibles pour la case
 * d'indices ij. Parcourt les valeurs en cherchant si pour l'une d'entre
 * elles il existe un seul emplacement valide dans la région.
 * Dans ce cas, stocke la valeur dans la case et renvoie true.
 * Renvoie false si pour chaque valeur possible il existe au moins deux
 * cases où on pourrait potentiellement la ranger. Dans ce cas ne
 * modifie pas la grille.
 */
bool force(Grille &g, Indices ij, vector<int> vpossibles) {
  bool res = false;
  // on essaye de se placer au début de la région appartenant à la case entrée en paramètre
  size_t lig_init;
  size_t col_init;
  //pour les lignes
  if(ij.lig<3)
  {
      lig_init=0;
  }
  else if(ij.lig>=3 && ij.lig<6)
  {
      lig_init=3;
  }
  else if(ij.lig>=6 && ij.lig<=8)
  {
      lig_init=6;
  }
  //pour les colonnes
    if(ij.col<3)
  {
      col_init=0;
  }
  else if(ij.col>=3 && ij.col<6)
  {
      col_init=3;
  }
  else if(ij.col>=6 && ij.col<=8)
  {
      col_init=6;
  }
  //nous pouvons donc commencer à iterer sur toute la région concernée
  vector<int> poss;
  for(size_t i = lig_init;i<lig_init+3;i+=1)
  {
      for(size_t j = col_init;j< col_init+3;j+=1)
      {
          if(i==ij.lig && j==ij.col)
          {
              continue;
          }
          else if(g.grille.at(i).at(j)==0)
          {
              for(size_t k =0;k< vpossibles.size();k+=1)
              {
                  poss=possibles(g,{i,j});
                  for(size_t l=0;l<poss.size();l+=1)
                  {

                      if(vpossibles.at(k)== poss.at(l))
                      {
                          vpossibles.at(k)=-1;
                      }
                  }
              }

          }
      }
  }
  //on vérifie s'il reste une valeur valable dans le vecteur vpossible
  for(size_t i=0;i<vpossibles.size();i+=1)
  {
      if(vpossibles.at(i)!=-1)
      {
          g.grille.at(ij.lig).at(ij.col)=vpossibles.at(i);
          res=true;
      }
  }
  return res;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//      PARTIE "USERSUGGEST"

/*
* demande à l'utilisateur de faire une suggestion pour modifier la grille
*@param g, la grille de sudoku
*renvoie true si la demande à pu aboutir et false sinon
*/
bool userSuggest(Grille &g) {
  bool res = false;
  int i;
  int j;
  int val;
  cout<<"entrez l'indice de la ligne (entre 0 et 8) : ";
  cin>>i;
  cout<<endl;
  while(i<0 || i>=9)
  {
      cout<<"ERREUR entrez une valeur entre 0 et 8 : ";
      cin>>i;
      cout<<endl;
  }
  cout<<"entrez l'indice de la colonne (entre 0 et 8) : ";
  cin>>j;
  cout<<endl;
    while(j<0 || j>=9)
  {
      cout<<"ERREUR entrez une valeur entre 0 et 8 : ";
      cin>>j;
      cout<<endl;
  }
  if(g.grille.at(i).at(j)!=0)
  {
      cout<<"ERREUR case deja rempli"<<endl;
      return false;
  }
  cout<<" entrez la valeur de la case : ";
  cin>>val;
  g.grille.at(i).at(j)=val;
  if(valide(g))
  {
      res=true;
  }
  else
  {
     g.grille.at(i).at(j)=0;
     cout<<"ERREUR, valeur non valide"<<endl;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//      PARTIE "JOUE" ET SAUVEGARDE DE PARTIE

/*
*verifie l'avancement d'un sudoku c'est à dire si la grille est complète ou si elle est insolvable
*@param g la grille de sudoku et Gprec la grille avant les modifications d'un user suggest
*return true si le sudoku est complet et false sinon
*/
bool avancee_resolution(Grille &g,Grille Gprec)
{
      bool complet = true;
      bool insolv=false;
      for(size_t i=0;i<9;i+=1)
      {
          for(size_t j=0;j<9;j+=1)
          {
              if(g.grille.at(i).at(j)==0)
              {
                  if(possibles(g,{i,j}).size()==0)
                  {
                      insolv=true;
                      imprime(g);
                      cout<<" grille insolvable a la case("<<i+1<<","<<j+1<<"),retour a la grille precedente "<<endl;
                  }
                  complet=false;
              }

          }
      }
      if(insolv)
      {
          g=Gprec;
      }
      return complet;

}
/*
* sauvegarde une grille dans un fichier .txt
*/
void sauvegarde(Grille &g)
{
      ofstream masauvegarde_ecriture("sauvegarde.txt",ios::app);
      masauvegarde_ecriture<<g.nom<<" ";
      for(int i=0;i<9;i+=1)
      {
          for(int j=0;j<9;j+=1)
          {
              if(g.grille.at(i).at(j)!=0)
              {
                  masauvegarde_ecriture<<i<<j<<g.grille.at(i).at(j)<<" ";
              }
          }
      }
      masauvegarde_ecriture<<endl;
}

/*
* utilise les algorithmes de "possible" , "force" et "user suggest" pour resoudre une grille
*/
bool joue(Grille &g) {
    //ON VEUT SAVOIR SI L'UTILISATEUR VEUT SAUVEGARDER
    bool save;
    char rep;
    do
    {
        cout<<"voulez vous sauvegarder la partie ?( o ou n ) : ";
        cin>> rep;
        if(rep!= 'o' && rep != 'n')
        {
            cout<<"erreur, entrez soit o pour oui soit n pour non"<<endl;
        }
    }while(rep!= 'o' && rep != 'n');
    if(rep == 'o')
    {
        save=true;
    }
    else
    {
        save=false;
    }
  bool res = false;
  // ON COMMENCE LE TRAITEMENT DE LA GRILLE
  cout << "Debut du traitement de la grille " << g.nom << endl;
  imprime(g);
  int tour=0;
  Indices ind;
  vector<int> poss;
  bool complet=false;
  Grille Gprec;
  while(!complet)
  {
      //VERIFICATION DE L'AVANCEMENT DE LA RESOLUTION
      complet=avancee_resolution(g,Gprec); // on l'a mis au début plutot qu'à la fin de la boucle pour pouvoir traiter le cas ou on charge un sudoku résolu
      //APPLICATION DE L'ALGORITHME
      int cpt_possible=0;
      int cpt_force=0;
      tour+=1;
      cout<<"Tour numero : "<<tour<<endl;
      imprime(g);
      // ETAPE "POSSIBLE"
      for(int i=0;i<9;i+=1)
      {
          for(int j=0;j<9;j+=1)
          {
                  ind.lig=i;
                  ind.col=j;
                  poss=possibles(g,ind);
                  if(poss.size()==1)
                  {
                      g.grille.at(i).at(j)=poss.at(0);
                      cout<<"Case ("<<i+1<<", "<<j+1<<") <- "<<poss.at(0)<<endl;
                      cpt_possible+=1;
                  }
          }
      }
      if(cpt_possible>0)
      {
          cout<<"Case remplies automatiquement: "<<cpt_possible<<endl;
      }
      //ETAPE "FORCE"
      else if (cpt_possible == 0)
      {
        for(int i=0;i<9;i+=1)
        {
          for(int j=0;j<9;j+=1)
          {
                  ind.lig=i;
                  ind.col=j;
                  poss=possibles(g,ind);
                  bool a =force(g,ind,poss);
                  if(a)
                  {
                      cout<<"Force: la case ("<<i+1<<", "<<j+1<<") <- "<< g.grille.at(i).at(j)<<endl;
                      cpt_force+=1;
                  }

          }
        }
      }
      //ETAPE "USER SUGGEST"
      if(cpt_force==0 && cpt_possible==0 && complet == false)
      {
          if(save)
          {
              cout<<"appuyer sur x si vous voulez arreter la partie : ";
              cin>>rep;
              cout<<endl;
              if(rep == 'x')
              {
                  break;
              }
          }
          bool fonctionne;
          do
          {
              Gprec = g;
              fonctionne=userSuggest(g);
          }while(!fonctionne);
      }
  }
  //SAUVEGARDE
  if(save)
  {
      sauvegarde(g);
  }
  imprime(g);
  cout<<endl<<"grille "<<g.nom<<" finale"<<endl;
  res=true;
  return res;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//      PARTIE CHARGE DE PARTIE ET MAIN

/*
*charge une partie dont les données sont déjà sauvegardées dans le fichier .txt
*/
bool charge_partie(Grille &gcharge)
{
    ifstream masauvegarde_lecture("sauvegarde.txt");
    vector<string> t;
    string nomgrille;
    cout<<"entrez le nom de la grille : ";
    cin>> nomgrille;
    string ligne;
    gcharge={nomgrille,{ { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                            { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
                            }};
    while(getline(masauvegarde_lecture,ligne))
    {
        t.push_back(ligne);   //ON STOCK LE FICHIER DANS UN VECTOR POUR POUVOIR PLUS FACILEMENT MANIPULER LE VECTOR
    }
    // ON VERIFIE SI LE NOM ENTRE CORRESPOND BIEN A UN NOM D'UNE GRILLE SAUVEGARDE
    bool memenom=true;
    int emplacement=0;
    for(int i=t.size()-1;i>=0;i-=1)
    {
        memenom=true;
        for(size_t j=0;j<nomgrille.size();j+=1)
        {
            if(nomgrille.at(j)!=t.at(i).at(j))
            {
                memenom= false;
            }
        }
        if(memenom)
        {
            memenom=false;
            if(t.at(i).at(nomgrille.size())==' ')
            {
                memenom=true;
                emplacement=i;
                break;
            }
        }
    }
    // ON CHARGE LA GRILLE SI ILS ONT LE MEME NOM SINON ON ARRETE LE PROGRAMME
    if(memenom)
    {
        int l,c,val;
        for(size_t i=nomgrille.size()+1;i<t.at(emplacement).size();i+=4)
        {
                l=t.at(emplacement).at(i)-'0';
                c=t.at(emplacement).at(i+1)-'0';
                val=t.at(emplacement).at(i+2)-'0';
                gcharge.grille.at(l).at(c)=val;
        }
        return true;

    }
    else
    {
        cout<<"aucun nom de la sorte trouve"<<endl;
        return false;
    }

}

/* quelques grilles prédéfinies */
int main() {
  // Les grilles G2 à G3 peuvent se résoudre sans aide de l'utilisateur

  Grille g2 = { "G2",
		{ { 5, 3, 0, 0, 7, 0, 0, 0, 0 },
		  { 6, 0, 0, 1, 9, 5, 0, 0, 0 },
		  { 0, 9, 8, 0, 0, 0, 0, 6, 0 },
		  { 8, 0, 0, 0, 6, 0, 0, 0, 3 },
		  { 4, 0, 0, 8, 0, 3, 0, 0, 1 },
		  { 7, 0, 0, 0, 2, 0, 0, 0, 6 },
		  { 0, 6, 0, 0, 0, 0, 2, 8, 0 },
		  { 0, 0, 0, 4, 1, 9, 0, 0, 5 },
		  { 0, 0, 0, 0, 8, 0, 0, 7, 9 }
		}};

  Grille g3 = { "G3",
		{ { 0, 2, 7, 0, 6, 5, 0, 4, 9 },
		  { 3, 5, 0, 0, 0, 9, 0, 0, 8 },
		  { 8, 9, 0, 0, 0, 0, 0, 0, 0 },
		  { 2, 0, 0, 0, 9, 0, 0, 0, 0 },
		  { 0, 0, 0, 6, 8, 7, 0, 0, 0 },
		  { 0, 0, 0, 0, 3, 0, 0, 0, 5 },
		  { 0, 0, 0, 0, 0, 0, 0, 6, 3 },
		  { 7, 0, 0, 2, 0, 0, 0, 8, 4 },
		  { 4, 8, 0, 9, 7, 0, 1, 5, 0 }
		}};
  Grille g4 = { "G4",
		{ { 8, 0, 0, 0, 0, 0, 1, 0, 0 },
		  { 0, 3, 4, 0, 2, 0, 6, 0, 0 },
		  { 0, 9, 0, 4, 0, 0, 0, 0, 2 },
		  { 5, 1, 0, 0, 4, 2, 9, 6, 0 },
		  { 0, 0, 0, 0, 6, 0, 0, 0, 0 },
		  { 0, 6, 7, 1, 5, 0, 0, 2, 3 },
		  { 9, 0, 0, 0, 0, 4, 0, 3, 0 },
		  { 0, 0, 2, 0, 1, 0, 7, 4, 0 },
		  { 0, 0, 8, 0, 0, 0, 0, 0, 1 }
		}};
  // cette grille admet deux solutions différentes mais demande une aide
  // de l'utilisateur si on se limite aux deux autres stratégies définies
  Grille g5 = { "G5",
		{ { 0, 0, 8, 0, 0, 0, 2, 0, 0 },
		  { 0, 0, 4, 3, 0, 0, 0, 0, 1 },
		  { 0, 5, 0, 0, 0, 7, 0, 8, 9 },
		  { 0, 0, 5, 7, 4, 0, 0, 0, 2 },
		  { 0, 0, 0, 1, 0, 3, 0, 0, 0 },
		  { 4, 0, 0, 0, 2, 0, 6, 0, 0 },
		  { 2, 6, 0, 5, 0, 0, 0, 7, 0 },
		  { 5, 0, 0, 0, 0, 2, 9, 0, 0 },
		  { 0, 0, 1, 0, 0, 0, 5, 0, 0 }
		}};
//ON VEUT SAVOIR SI L'UTILISATEUR VEUT CHARGER UNE PARTIE
char rep;
do
{
    cout<<"voulez vous charger une partie ?( o ou n ) : ";
    cin>> rep;
    if(rep!= 'o' && rep != 'n')
    {
        cout<<"erreur, entrez soit o pour oui soit n pour non"<<endl;
    }
}while(rep!= 'o' && rep != 'n');
//ON CHARGE ET JOUE LA PARTIE DEMANDEE SI L'UTILISATEUR VEUT CHARGER UNE PARTIE , SINON ON JOUE PAR DEFAUT UNE GRILLE PRECISEE DANS LE CODE
if(rep== 'o')
{
    Grille gcharge;
    if(charge_partie(gcharge))
    {
        joue(gcharge);
    }
    else
    {
        return -1;
    }
}
else
{
//joue(g2);
//joue(g3);
//joue(g4);
joue(g5);
}
  return 0;
}

		// Fonctions fournies, rien à modifier à priori.


/* Imprime une ligne de séparation formée du caractère passé en argument */
void imprimeL(char c) {
  cout << ' ';
  for(size_t i = 0; i < 9; i += 1) { cout << c << c << c << c; }
  /* pour prendre en compte les espaces ajoutés prèsb des '+' */
  cout << c << c << c << c << c << c;
  cout << endl;
}

/* suppose la grille bien formee: 9 lignes de 9 colonnes de chiffres de 0 à 9 */
void imprime(const Grille &g) {
  for(size_t i = 0; i < 9; i += 1) {
    imprimeL(i % 3 == 0 ? '+' : '-'); // ligne d'en-tete
    for(size_t j = 0; j < 9; j += 1) {
      char c = '0' + g.grille.at(i).at(j);
      cout << (j % 3 == 0 ? " + " : " " ) << ' ' << c << ' ';
    }
    cout << "+" << endl;
  }
  imprimeL('+'); // ligne finale du bas
  cout << endl;
}
