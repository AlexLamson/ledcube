/*
 * DataSnake.h
 *
 *  Created on: Nov 2, 2018
 *      Author: raffc
 */

#ifndef DATASNAKE_H_
#define DATASNAKE_H_

#include "Demo.h"
#include "Arduino.h"

class DataSnake: public Demo {
private:
  int frame;
public:
  DataSnake();
  void initialize();
  void tick();
};

#endif /* DATASNAKE_H_ */
