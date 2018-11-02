/*
 * demo.h
 *
 *  Created on: Oct 27, 2018
 *      Author: Chris Raff
 */

#ifndef DEMO_H_
#define DEMO_H_

class Demo
{

public:
   int duration;

  Demo(){ duration = 0; }
  virtual ~Demo(){}
  virtual void initialize() = 0;    // "= 0" part makes this method pure virtual, and
                                 // also makes this class abstract.
  virtual void tick() = 0;
};

#endif /* DEMO_H_ */
