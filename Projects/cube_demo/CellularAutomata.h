/*
 * CellularAutomata.h
 *
 *  Created on: Jul 20, 2019
 *      Author: raffc
 */

#ifndef CELLULAR_AUTOMATA_H_
#define CELLULAR_AUTOMATA_H_

#include "Demo.h"
#include "Arduino.h"

class CellularAutomata: public Demo {
private:
  bool survival[27];
  bool birth[27];
  byte maxCellStates;
  byte cells[512]; // does this take too much memory?

  bool rhs; // true when the "previous" cell value is on the right 4 bits of the byte

public:
  CellularAutomata();
  void initialize();
  void tick();
};

#endif /* CELLULAR_AUTOMATA_H_ */
