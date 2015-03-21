#ifndef _IMAGE_PNG_H_
#define _IMAGE_PNG_H_

/* *
 * Rémi Vernay
 * 21 octobre 2012
 * MAJ : 30 octobre 2012
 */

#include <vector>
#include <string>
#include "base_png.h"

using namespace std;

class ArrayOutOfBoundsException {};

class ImagePNG {
private:
   unsigned width, 
            height;
   vector<unsigned char> *image;
   
   unsigned char channel (const unsigned l, const unsigned c, const unsigned decalage) const;
   unsigned char& channel (const unsigned l, const unsigned c, const unsigned decalage);
   
public:
   ImagePNG();
   ImagePNG(const ImagePNG &ancien);
   virtual ~ImagePNG();
   ImagePNG& operator=(const ImagePNG &droit);
   
   void newImage(const unsigned height, const unsigned width); // cree une image blanche avec de nouvelles dimensions
   
   unsigned getWidth() const; // largeur de l'image
   unsigned getHeight() const; // hauteur de l'image
   
   unsigned char red(const unsigned l, const unsigned c) const; // valeur rouge du pixel de coordonnees (l,c) entre 0 et 255
   unsigned char& red(const unsigned l, const unsigned c); // valeur rouge du pixel de coordonnees (l,c) entre 0 et 255

   unsigned char green(const unsigned l, const unsigned c) const; // valeur verte du pixel de coordonnees (l,c) entre 0 et 255
   unsigned char& green(const unsigned l, const unsigned c); // valeur verte du pixel de coordonnees (l,c) entre 0 et 255

   unsigned char blue(const unsigned l, const unsigned c) const; // valeur bleue du pixel de coordonnees (l,c) entre 0 et 255
   unsigned char& blue(const unsigned l, const unsigned c); // valeur bleue du pixel de coordonnees (l,c) entre 0 et 255

   unsigned char alpha(const unsigned l, const unsigned c) const; // transparence du pixel de coordonnees (l,c) entre 0 et 255
   unsigned char& alpha(const unsigned l, const unsigned c); // transparence du pixel de coordonnees (l,c) entre 0 et 255

   void load(const string &chemin); // charge une image PNG
   void save(const string &chemin) const; // enregistre une image PNG
};

#endif
