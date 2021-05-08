// #include <vector>

// void initialiserEmplacements()
// {
//   Grille laGrille = Grille(20);
//   int idF = 0;
//   for (int y = 9; y < 13; y++)
//     for (int x = 9; x < 13; x++)
//     {
//       Coord c = Coord(x, y);

//       if (x >= 10 && x <= 12 && y >= 10 && y <= 12)
//       {
//         Place pNid = Place(c);
//         pNid.poseNid();
//         laGrille.rangePlace(pNid);
//       }
//       else
//       {
//         Place pF = Place(c);
//         pF.poseFourmi(Fourmi(c, idF));
//         laGrille.rangePlace(pF);
//       }
//     }
// }
