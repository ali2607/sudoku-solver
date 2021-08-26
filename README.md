# sudoku-solver
un solveur de sudoku fait par moi même en C++ pour un projet de mon école.

le programme permet de résoudre seul un sudoku si cela est possible, sinon il demande à l'utilisateur de faire un choix pour pouvoir avancer tout en repérant plusieurs étapes à l'avance si un choix fait par l'utilisateur résultera à un blocage du sudoku.

Cependant cette fonctionnalité de repérage est très limitée car je n'ai pas rajouté de récursivité. Ainsi, le programme peut tout de même nous mener à une impasse si l'un de nos choix nous mène à un sudoku avec un deuxième choix à faire mais qui mène dans tous les cas à une impasse.

J'ai aussi ajouté une fonctionnalité de sauvegarde/charge de partie pour pouvoir continuer son sudoku un autre jour.
